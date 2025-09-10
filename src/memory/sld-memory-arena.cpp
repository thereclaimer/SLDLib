#pragma once

#include "sld-memory-internal.cpp"

namespace sld {

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API bool
    arena_validate(
        arena_t* arena) {

        bool is_valid = (arena != NULL);
        if (is_valid) {
            is_valid &= stack_validate       (arena->stack);
            is_valid &= reservation_validate (arena->reservation);

            arena->last_error.val = (is_valid)
                ? memory_error_e_success
                : memory_error_e_invalid_arena;
        };
        return(is_valid);
    }

    SLD_API arena_t*
    arena_commit(
        reservation_t* reservation) {

        static arena_list_t& arena_list = global_stack_get_arena_list();
        arena_t* arena = NULL;
        const bool is_valid = reservation_validate(reservation);
        if (!is_valid) return(arena);

        // attempt to re-commit a decommitted or released
        // arena 
        bool is_recommit = false;
        arena = reservation_remove_next_decommitted_arena(reservation);
        if (arena == NULL) {
            arena = arena_list_remove_next_released(arena_list);
        } else {
            is_recommit = true;
        }       

        // if none are available, allocate a new one
        if (arena == NULL) {
            static const u32 push_size = sizeof(arena_t);
            arena = (arena_t*)global_stack_push_bytes(push_size);
        }

        // we should have one at this point
        if (arena == NULL) {
            return(arena);
        }

        // if this isn't a re-commit, we need to calculate the 
        // new address
        //
        // otherwise, we will reuse the address in the stack
        const u64 commit_offset = reservation_size_committed(reservation);
        void*     commit_start  = (is_recommit)
            ? (void*)arena->stack.start
            : (void*)(reservation->start + commit_offset);

        // attempt to commit memory
        const void* commit_result = os_memory_commit(commit_start, reservation->size.arena);
        if (commit_result != commit_start) {

            // if that failed, put the arena back on the stack as released
            arena_list_insert_released(arena_list, arena);
            arena->last_error.val = memory_error_e_os_failed_to_commit;
            return(arena);
        }

        // initialize the arena and add to the reservation
        arena->stack.start    = (addr)commit_result;
        arena->stack.size     = reservation->size.arena;
        arena->stack.position = 0;
        arena->stack.save     = 0;
        arena->reservation    = reservation;
        arena->last_error.val = memory_error_e_success;
        reservation_insert_committed_arena(reservation, arena);
        return(arena);
    }

    SLD_API bool
    arena_decommit(
        arena_t* arena) {

        // validate the arena
        const bool is_valid = arena_validate(arena);
        if (!is_valid) {
            arena->last_error.val = memory_error_e_invalid_arena;
            return(is_valid);
        }

        // attempt to decommit the memory
        const bool is_decommitted = os_memory_decommit(
            (void*)arena->stack.start,
            arena->stack.size
        );
        if (!is_decommitted) {
            arena->last_error.val = memory_error_e_os_failed_to_decommit;
            return(is_decommitted);
        }

        // add the arena to the reservation's decommitted list
        reservation_remove_committed_arena(arena->reservation, arena);
        arena->last_error.val = memory_error_e_success;
        return(is_decommitted);
    }

    SLD_API byte*
    arena_push_bytes(
        arena_t*  arena,
        const u64 size,
        const u64 alignment) {

        byte* bytes = NULL;

        // validate the arena
        const bool is_valid = arena_validate(arena);
        if (!is_valid) return(bytes);

        // align the size
        const u64 size_aligned = size_is_pow_2(alignment)
            ? size_align_pow_2 (size, alignment)
            : size_align       (size, alignment);

        // do the push
        bytes = (byte*)stack_push(arena->stack, size_aligned);
        if (bytes == NULL) {
            arena->last_error.val = memory_error_e_arena_not_enough_memory;
            return(bytes);
        }

        arena->last_error.val = memory_error_e_success;
        return(bytes);
    }

    SLD_API bool
    arena_pull_bytes(
        arena_t*  arena,
        const u64 size,
        const u64 alignment) {

        // validate the arena
        const bool is_valid = arena_validate(arena);
        if (!is_valid) return(is_valid);

        // align the size
        const u64 size_aligned = size_is_pow_2(alignment)
            ? size_align_pow_2 (size, alignment)
            : size_align       (size, alignment);

        // do the pull
        const bool is_pulled = stack_pull(arena->stack, size_aligned);
        arena->last_error.val = (is_pulled)
            ? memory_error_e_success 
            : memory_error_e_arena_not_enough_memory;
        return(is_pulled);
    }

    SLD_API u64
    arena_size_total(
        arena_t* arena) {

        const u64 size_total = arena_validate(arena)
            ? arena->stack.size
            : 0;

        return(size_total);
    }

    SLD_API u64
    arena_size_free(
        arena_t* arena) {

        const u64 size_free = arena_validate(arena)
            ? (arena->stack.size - arena->stack.position)
            : 0;

        return(size_free);
    }

    SLD_API u64
    arena_size_used(
        arena_t* arena) {

        const u64 size_used = arena_validate(arena)
            ? (arena->stack.position)
            : 0;

        return(size_used);
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_INTERNAL arena_t*
    arena_list_remove_next_released(
        arena_list_t& arena_list) {
        
        arena_t* arena = arena_list.released;
        if (arena != NULL) {
            arena_t* next = arena->next;
            next->prev  = NULL;
            arena->prev = NULL;   
            arena->next = NULL;
            arena_list.released = next;
        }
        return(arena);
    }

    SLD_INTERNAL void
    arena_list_insert_released(
        arena_list_t& arena_list,
        arena_t*      arena) {

        arena_t* next = arena_list.released;

        if (next) next->prev = arena;
        arena->next          = next;
        arena_list.released  = arena;
    }

    SLD_INTERNAL void
    arena_list_insert_all_released(
        arena_list_t&  arena_list,
        reservation_t* reservation) {

        // the released arena list will start with committed arenas
        arena_t* released = reservation->arena_list.committed;

        // add the decommited arenas to the tail of the released list
        if (released != NULL) {

            // get the last committed arena
            arena_t* last_released = NULL;
            for (
                arena_t* last_released = reservation->arena_list.committed;
                (last_released != NULL) && (last_released->next != NULL);
                last_released = last_released->next
            );

            arena_t* first_decommitted = reservation->arena_list.decommitted; 
            last_released->next = first_decommitted;
            if (first_decommitted) {
                first_decommitted->prev = last_released;
            }
        }
        else {
            released = reservation->arena_list.decommitted;
        }

        // add the released arenas back to the stack
        arena_t* next = arena_list.released;
        if (next) next->prev = released;
        released->next       = next;
        arena_list.released  = released;

        // null the reservation arenas
        reservation->arena_list.committed   = NULL;
        reservation->arena_list.decommitted = NULL;
    }

};
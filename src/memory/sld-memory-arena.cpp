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
            is_valid &= stack_validate       ((stack_t*)arena);
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

        const bool is_valid = reservation_validate(reservation);
        if (!is_valid) return(NULL);

        // find the next free arena
        const u32 arena_count_max = (reservation->size.reserved / reservation->size.arena);
        void*     commit_start    = NULL;
        for (
            u32 arena_index = 0;
            arena_index < arena_count_max;
            ++arena_index) {

            const u32  arena_offset  = (arena_index * reservation->size.arena);
            void*      arena_tmp     = (void*)(reservation->start + arena_offset); 
            const bool arena_is_free = os_memory_is_reserved(arena_tmp);
            
            if (arena_is_free) {
                commit_start = arena_tmp;
                break;
            } 
        }

        // attempt to commit memory
        arena_t* arena = (arena_t*)os_memory_commit(commit_start, reservation->size.arena);
        if ((void*)arena == commit_start ) {

            constexpr u32 arena_struct_size = sizeof(arena_t);

            // initialize the arena
            arena->start          = (addr)arena             + arena_struct_size;
            arena->size           = reservation->size.arena - arena_struct_size;
            arena->position       = 0;
            arena->save           = 0;
            arena->reservation    = reservation;
            arena->last_error.val = memory_error_e_success;

            // add the arena to the reservation
            arena_t* next = reservation->arenas;
            if (next) next->prev = arena;
            arena->next = next;
            reservation->arenas = arena;
        }
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

        reservation_t* reservation = arena->reservation;
        const bool     is_first    = (arena == reservation->arenas); 
        arena_t*       next        = arena->next;
        arena_t*       prev        = arena->prev;

        // attempt to decommit the memory
        const bool is_decommitted = os_memory_decommit(
            (void*)arena,
            arena->size
        );
        if (!is_decommitted) {
            arena->last_error.val = memory_error_e_os_failed_to_decommit;
            return(is_decommitted);
        }

        // remove the arena from the list
        if (next)     next->prev          = prev;
        if (prev)     prev->next          = next;
        if (is_first) reservation->arenas = next;
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
        bytes = (byte*)stack_push(arena, size_aligned);
        if (bytes == NULL) {
            arena->last_error.val = memory_error_e_arena_not_enough_memory;
            return(bytes);
        }

        arena->last_error.val = memory_error_e_success;
        return(bytes);
    }

    SLD_API block_allocator_t*
    arena_push_block_allocator(
        arena_t*  arena,
        const u32 size,
        const u32 granularity) {

        // calculate the push size
        constexpr u32 size_struct       = sizeof(block_allocator_t);
        const     u32 size_gran_aligned = size_round_up_pow2(granularity);
        const     u32 size_mem_aligned  = size_align_pow_2  (size, granularity);
        const     u32 size_push         = size_struct + size_mem_aligned;

        // initialize allocator
        const void*        memory    = arena_push_bytes     (arena,  size_push, size_gran_aligned);
        block_allocator_t* allocator = block_allocator_init (memory, size_push, size_gran_aligned);
        return(allocator);
    }

    SLD_API stack_allocator_t*
    arena_push_stack_allocator(
        arena_t*  arena,
        const u32 size,
        const u32 granularity) {

        // calculate the push size
        constexpr u32 size_struct       = sizeof(stack_allocator_t);
        const     u32 size_gran_aligned = size_round_up_pow2(granularity);
        const     u32 size_mem_aligned  = size_align_pow_2  (size, granularity);
        const     u32 size_push         = size_struct + size_mem_aligned;

        // initialize allocator
        const void*        memory    = arena_push_bytes     (arena,  size_push, size_gran_aligned);
        stack_allocator_t* allocator = stack_allocator_init (memory, size_push, size_gran_aligned);
        return(allocator);        
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
        const bool is_pulled = stack_pull(arena, size_aligned);
        arena->last_error.val = (is_pulled)
            ? memory_error_e_success 
            : memory_error_e_arena_not_enough_memory;
        return(is_pulled);
    }

    SLD_API bool
    arena_reset(
        arena_t* arena) {

        const bool is_valid = arena_validate(arena);
        if (!is_valid) return(is_valid);

        const bool did_reset = stack_reset(arena);
        return(did_reset);
    }

    SLD_API bool
    arena_roll_back(
        arena_t* arena) {

        const bool is_valid = arena_validate(arena);
        if (!is_valid) return(is_valid);

        const bool did_reset = stack_reset_to_save(arena);
        return(did_reset);        
    }

    SLD_API bool
    arena_save_position(
        arena_t* arena) {

        const bool is_valid = arena_validate(arena);
        if (!is_valid) return(is_valid);

        const bool did_save = stack_save(arena);
        return(did_save);        
    }

    SLD_API u64
    arena_size_total(
        arena_t* arena) {

        const u64 size_total = arena_validate(arena)
            ? arena->size
            : 0;

        return(size_total);
    }

    SLD_API u64
    arena_size_free(
        arena_t* arena) {

        const u64 size_free = arena_validate(arena)
            ? (arena->size - arena->position)
            : 0;

        return(size_free);
    }

    SLD_API u64
    arena_size_used(
        arena_t* arena) {

        const u64 size_used = arena_validate(arena)
            ? (arena->position)
            : 0;

        return(size_used);
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_FUNC arena_t*
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

    SLD_FUNC void
    arena_list_insert_released(
        arena_list_t& arena_list,
        arena_t*      arena) {

        arena_t* next = arena_list.released;

        if (next) next->prev = arena;
        arena->next          = next;
        arena_list.released  = arena;
    }
};
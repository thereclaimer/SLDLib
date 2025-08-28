#pragma once

#include "sld-memory-internal.cpp"

namespace sld {

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API const memory_arena_handle_t
    memory_arena_commit(
        const memory_reservation_handle_t reservation_handle) {

        memory_arena_handle_t         arena_handle = { SLD_MEMORY_INVALID_HANDLE };
        static memory_error_t& error        = memory_last_error_instance();

        // validate the reservation
        memory_reservation_t* reservation = memory_stack_get_reservation(reservation_handle);
        if (!memory_reservation_validate_internal(reservation)) {
            error.val = memory_error_e_invalid_reservation;
            return(arena_handle);
        }

        // check if there is space in the reservation
        const bool can_commit = (reservation->size.committed < reservation->size.total);
        if (!can_commit) {
            error.val = memory_error_e_reservation_out_of_memory;
            return(arena_handle);
        }

        // attempt to recycle an arena from the list
        memory_arena_list_t& arena_list = memory_arena_list_instance ();
        memory_arena_t*      arena      = memory_arena_list_recycle  (arena_list, reservation);
        
        // if we can't recycle one, we need to allocate a new one
        // if that doesn't work, return invalid
        if (arena == NULL) {
            arena = memory_stack_push_arena();
            if (arena == NULL) {
                error.val = memory_error_e_stack_not_enough_memory;
                return(arena_handle); 
            } 
        }

        // initialize the arena
        const bool is_init = memory_arena_init(arena, reservation, error);
        if (!is_init) {

            // if that failed, add the arena to the decommitted list
            const bool remove_reservation = true;
            memory_arena_list_add_decommitted(
                arena_list,
                arena,
                remove_reservation);
        }

        // return the handle
        arena_handle = memory_stack_get_arena_handle(arena);
        error.val    = memory_error_e_success;
        return(arena_handle);
    }

    SLD_API bool
    memory_arena_decommit(
        const memory_arena_handle_t arena_handle) {

        // validate the arena
        static memory_error_t& error    = memory_last_error_instance();
        memory_arena_t*            arena    = memory_stack_get_arena         (arena_handle);
        const bool                 is_valid = memory_arena_validate_internal (arena);
        if (!is_valid) {
            error.val = memory_error_e_invalid_arena;
            return(is_valid);
        }

        // deinitialize the arena
        const bool is_deinit = memory_arena_deinit(arena, error);
        if (is_deinit) {

            // add the arena to the decommitted list
            const bool           keep_reservation = true;
            memory_arena_list_t& arena_list       = memory_arena_list_instance();
            memory_arena_list_add_decommitted(
                arena_list,
                arena,
                keep_reservation
            );
        } 

        return(is_deinit);
    }

    SLD_API bool
    memory_arena_validate(
        const memory_arena_handle_t arena_handle) {

        const memory_arena_t* arena    = memory_stack_get_arena         (arena_handle);
        const bool            is_valid = memory_arena_validate_internal (arena);
        return(is_valid);
    }

    SLD_API bool
    memory_arena_save(
        const memory_arena_handle_t arena_handle) {
        
        static memory_error_t& error    = memory_last_error_instance();
        memory_arena_t*            arena    = memory_stack_get_arena         (arena_handle);
        const bool                 is_valid = memory_arena_validate_internal (arena);
        
        if (!is_valid) {
            error.val = memory_error_e_invalid_arena;
            return(is_valid);
        }

        arena->save = arena->position;
        error.val   = memory_error_e_success;
        return(is_valid);
    }

    SLD_API bool
    memory_arena_reset(
        const memory_arena_handle_t arena_handle) {

        static memory_error_t& error    = memory_last_error_instance();
        memory_arena_t*            arena    = memory_stack_get_arena (arena_handle);
        const bool                 is_valid = memory_arena_validate_internal  (arena);
        
        if (!is_valid) {
            error.val = memory_error_e_invalid_arena;
            return(is_valid);
        }

        arena->position = 0;
        error.val       = memory_error_e_success;
        return(is_valid);
    }

    SLD_API bool
    memory_arena_reset_to_save(
        const memory_arena_handle_t arena_handle) {

        static memory_error_t& error    = memory_last_error_instance();
        memory_arena_t*            arena    = memory_stack_get_arena (arena_handle);
        const bool                 is_valid = memory_arena_validate_internal  (arena);
        
        if (!is_valid) {
            error.val = memory_error_e_invalid_arena;
            return(is_valid);
        }

        arena->position = arena->save;
        error.val       = memory_error_e_success;
        return(is_valid);        
    }

    SLD_API const u64
    memory_arena_space_remaining(
        const memory_arena_handle_t arena_handle) {

        static memory_error_t& error    = memory_last_error_instance();
        const memory_arena_t*      arena    = memory_stack_get_arena (arena_handle);
        const bool                 is_valid = memory_arena_validate_internal  (arena);
        
        u64 space_remaining = 0;

        if (!is_valid) {
            error.val = memory_error_e_invalid_arena;
            return(space_remaining);
        }

        const memory_reservation_t* reservation = arena->reservation;
        space_remaining = reservation->size - arena->position; 
        error.val       = memory_error_e_success;
        return(space_remaining);   
    }

    SLD_API byte*
    memory_arena_push_bytes(
        const memory_arena_handle_t arena_handle,
        const u64              size) {

        static memory_error_t& error = memory_last_error_instance();
        byte* bytes                      = NULL;

        // validate the arena
        memory_arena_t* arena    = memory_stack_get_arena (arena_handle);
        const bool      is_valid = memory_arena_validate_internal  (arena);
        if (!is_valid) {
            error.val = memory_error_e_invalid_arena;
            return(bytes);
        }

        // make sure we can push
        const memory_reservation_t* reservation        = arena->reservation;
        const u64                   arena_new_position = arena->position + size;
        const bool                  can_push           = (arena_new_position <= reservation->size.arena);
        if (!can_push) {
            error.val = memory_error_e_arena_not_enough_memory;
            return(bytes);
        }

        // do the push
        byte* bytes     = (byte*)(arena->start + arena->position);
        arena->position = arena_new_position;
        error.val       = memory_error_e_success;
        return(bytes);
    }
    
    SLD_API bool
    memory_arena_pull_bytes(
        const memory_arena_handle_t arena_handle,
        const u64              size) {

        bool result = false;

        // validate the arena
        static memory_error_t& error    = memory_last_error_instance();
        memory_arena_t*            arena    = memory_stack_get_arena (arena_handle);
        const bool                 is_valid = memory_arena_validate_internal  (arena);
        if (!is_valid) {
            error.val = memory_error_e_invalid_arena;
            return(result);
        } 

        // make sure we can pull
        const bool can_pull = size <= arena->position;
        if (!can_pull) {
            error.val = memory_error_e_arena_not_enough_memory;            
            return(result);
        } 

        // do the pull
        arena->position -= size;
        error.val        = memory_error_e_success;            
        result           = true;
        return(result);
    }
    
    SLD_API bool
    memory_arena_can_push_bytes(
        const memory_arena_handle_t arena_handle,
        const u64               size) {

        // validate the arena
        static memory_error_t& error    = memory_last_error_instance();
        memory_arena_t*            arena    = memory_stack_get_arena         (arena_handle);
        const bool                 is_valid = memory_arena_validate_internal (arena);
        if (!is_valid) {
            error.val = memory_error_e_invalid_arena;
            return(NULL);
        } 

        // make sure we can push
        const memory_reservation_t* reservation        = arena->reservation;
        const u64                   arena_new_position = arena->position + size;
        const bool                  can_push           = (arena_new_position <= reservation->size.arena);
        error.val                                      = memory_error_e_success;
        return(can_push);
    }
    
    SLD_API bool
    memory_arena_can_pull_bytes(
        const memory_arena_handle_t arena_handle,
        const u64               size) {

        // validate the arena
        static memory_error_t& error    = memory_last_error_instance();
        memory_arena_t*            arena    = memory_stack_get_arena (arena_handle);
        const bool                 is_valid = memory_arena_validate_internal  (arena);
        if (!is_valid) {
            error.val = memory_error_e_invalid_arena;
            return(is_valid);
        }

        // make sure we can pull
        const bool can_pull = size <= arena->position;
        error.val           = memory_error_e_success;
        return(can_pull);
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_INLINE bool
    memory_arena_validate_internal(
        const memory_arena_t* arena) {

        bool is_valid = (arena != NULL);
        if (is_valid) {

            const memory_reservation_t* reservation = arena->reservation;

            is_valid &= (arena->reservation  != NULL);
            is_valid &= (arena->next         != arena);
            is_valid &= (arena->prev         != arena);
            is_valid &= (arena->start        != 0);
            is_valid &= (arena->save         <= arena->position);
            is_valid &= (reservation != NULL ?  arena->position <= reservation->size : false);
        }
        return(is_valid);
    }

    SLD_INLINE memory_arena_list_t&
    memory_arena_list_instance(
        void) {

        static memory_arena_list_t arena_list = {
            NULL,
            NULL
        };

        return(arena_list);
    }

    SLD_INLINE memory_arena_t*
    memory_arena_list_recycle(
        memory_arena_list_t&        arena_list,
        const memory_reservation_t* reservation) {

        memory_arena_t* first_decommitted = NULL; // decommitted, matching reservation 
        memory_arena_t* first_unused      = NULL; // totally unused, no reservation

        for (
            memory_arena_t* current = arena_list.decommitted;
            current != NULL;
            current = current->next) {

            // ideally, we want an arena already inside this reservation
            if (current->reservation != NULL && current->start != 0) {
                first_decommitted = current;
                break; 
            }

            // otherwise, just store the unused arena
            // if there are no available arenas in this reservation,
            // we will return this one
            else if (current->reservation == NULL && first_unused == NULL) {
                first_unused = current;
            }
        }

        // this is what we will return
        // if we don't have anything, it will just be null
        memory_arena_t* recycled_arena = (first_decommitted != NULL)
            ? first_decommitted
            : first_unused;

        // if we have an arena to recycle,
        // update the list
        if (recycled_arena) {

            memory_arena_t* next = recycled_arena->next;
            memory_arena_t* prev = recycled_arena->prev;

            if (next != NULL) {
                next->prev = prev;
            }
            if (prev != NULL) {
                prev->next = next;
            } 

            recycled_arena->next = NULL;
            recycled_arena->prev = NULL;
        }
        return(recycled_arena);
    }

    SLD_INLINE void
    memory_arena_list_add_committed(
        memory_arena_list_t& arena_list,
        memory_arena_t*      arena) {

        if (arena_list.committed) {
            arena_list.committed->prev = arena;
        }
        if (arena) {
            arena->prev = NULL;
            arena->next = arena_list.committed;
        }
        arena_list.committed = arena;
    }

    SLD_INLINE void
    memory_arena_list_add_decommitted(
        memory_arena_list_t& arena_list,
        memory_arena_t*      arena,
        const bool           keep_reservation) {

        if (!keep_reservation) {
            arena->reservation = NULL;
            arena->start       = 0;
        }
        arena->save     = 0;
        arena->position = 0;

        if (arena_list.decommitted) {
            arena_list.decommitted->prev = arena;
        }
        if (arena) {
            arena->prev = NULL;
            arena->next = arena_list.decommitted;
        }
        arena_list.decommitted = arena;
    }

    SLD_INLINE bool
    memory_arena_init(
        memory_arena_t*       arena,
        memory_reservation_t* reservation,
        memory_error_t&   last_error) {

        // if we have an existing reservation but it doesn't match,
        // something went wrong
        const bool has_reservation = (arena->reservation != NULL);
        const bool is_match        = (arena->reservation == reservation);
        const bool is_valid        = (!has_reservation || is_match);
        if (!is_valid) {
            last_error.val = memory_error_e_critical;
            return(is_valid);
        }

        // if this is a new arena,
        // set the reservation and calculate the start
        if (!has_reservation) {
            arena->reservation = reservation;
            arena->start       = reservation->size.committed + reservation->size.arena;
        }

        // commit memory
        const void* commit_result = os_memory_commit(arena->start, reservation->size.arena);
        const bool  is_committed  = (commit_start != commit_result); 
        if (!is_committed) {
            last_error.val = memory_error_e_os_failed_to_commit;
            return(is_committed);
        }

        // update the reservation and the arena            
        reservation->size.committed += commit_size;
        arena->position = 0;
        arena->save     = 0;
        last_error.val  = memory_error_e_success;
        return(is_committed);
    }

    SLD_INLINE bool
    memory_arena_deinit(
        memory_arena_t*     arena,
        memory_error_t& last_error) {

        memory_reservation_t* reservation = arena->reservation; 
        const bool is_decommitted = os_memory_decommit(
            arena->start,
            reservation->size.arena
        );

        if (!is_decommitted) {
            last_error.val = memory_error_e_os_failed_to_decommit;
            return(is_decommitted);
        }

        reservation->size.total -= reservation->size.arena;
        arena->position          = 0;
        arena->save              = 0;
        last_error.val           = memory_error_e_success;
        return(is_decommitted);
    }
};
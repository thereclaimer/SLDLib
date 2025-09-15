#pragma once

#include "sld-memory-internal.cpp"

namespace sld {

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API bool
    reservation_validate(
        reservation_t* reservation) {

        bool is_valid = (reservation != NULL);
        if (!is_valid) {
            is_valid &= (reservation->start         != NULL);
            is_valid &= (reservation->size.reserved != 0);
            is_valid &= (reservation->size.arena    != 0);

            reservation->last_error.val = (is_valid) 
                ? memory_error_e_success
                : memory_error_e_invalid_reservation;
        }
        return(is_valid);
    }


    SLD_API reservation_t*
    reservation_acquire(
        const u64 size_min_reservation,
        const u64 size_min_arena) {

        static reservation_list_t& reservation_list = global_stack_get_reservation_list();

        // check if there are any released arenas to recycle 
        // if not, push a new one on the stack
        reservation_t* reservation = reservation_list_remove_next_released(reservation_list);
        if (reservation == NULL) {
            static const u32 push_size = sizeof(reservation_t);
            reservation = (reservation_t*)global_stack_push_bytes(push_size);
        }

        // we should have one by now
        if (!reservation) return(reservation);

        // align and commit memory
        const u64 size_reservation  =       os_memory_align_to_granularity (size_min_reservation);  
        const u64 size_arena        =       os_memory_align_to_page        (size_min_arena);  
        addr      reservation_start = (addr)os_memory_reserve              (NULL,size_reservation);
        if (reservation_start == NULL) {
            reservation->last_error.val = memory_error_e_os_failed_to_reserve;
            reservation_list_insert_released(reservation_list, reservation);
            return(reservation);
        }

        // initialize the reservation 
        // and add it to the list
        reservation->start          = reservation_start;
        reservation->size.reserved  = size_reservation;
        reservation->size.arena     = size_arena; 
        reservation->last_error.val = memory_error_e_success;
        reservation_list_insert_reserved(reservation_list, reservation);

        return(reservation);
    }
    
    SLD_API bool
    reservation_release(
        reservation_t* reservation) {

        const bool is_valid = reservation_validate(reservation);
        if (!is_valid) return(is_valid);

        // attempt to update the reseved list
        reservation_list_t& r_list     = global_stack_get_reservation_list();
        const bool          is_removed = reservation_list_remove_reserved(r_list, reservation);
        if (!is_removed) {
            reservation->last_error.val = memory_error_e_invalid_reservation;
            return(is_removed);
        }

        // attempt to release the os memory 
        const bool is_released = os_memory_release(
            (void*)reservation->start,
                   reservation->size.reserved);
        
        if (!is_released) {
            reservation->last_error.val = memory_error_e_os_failed_to_release;
            return(is_released);
        }

        // add the reservation to the released list
        reservation->last_error.val = memory_error_e_success;
        reservation_list_insert_released(r_list, reservation);

        // add committed and decommitted arenas in this
        // reservation to the released list
        static arena_list_t& arena_list = global_stack_get_arena_list();
        arena_list_insert_all_released(arena_list, reservation);        

        return(is_released);
    }

    SLD_API bool
    reservation_reset(
        reservation_t* reservation) {

        const bool is_valid = reservation_validate(reservation);
        if (is_valid) {

            bool is_decommitted = true;

            arena_t* tmp;
            for (
                arena_t* arena = reservation->arena_list.committed;
                arena != NULL;
                arena = tmp) {

                is_decommitted &= arena_decommit(arena);
                if (is_decommitted) {

                    tmp = arena->next;
                    reservation_insert_decommitted_arena(reservation, arena);
                }
            }

            reservation->last_error.val = (is_decommitted)
                ? memory_error_e_critical
                : memory_error_e_success;
        } 

        const bool result = is_valid && (reservation->last_error.val == memory_error_e_success);

        return(result);            
    }

    SLD_API u64
    reservation_size_committed(
        reservation_t* reservation) {

        u64 size_committed = 0;
        const bool is_valid = reservation_validate(reservation);
        
        if (is_valid) {

            reservation->last_error.val = memory_error_e_success;
            const u64 arena_size = reservation->size.arena; 
            for (
                arena_t* arena = reservation->arena_list.committed;
                arena != NULL;
                arena = arena->next) {
                
                size_committed += arena_size;
            }
        } 
        
        return(size_committed);
    }

    SLD_API u64
    reservation_size_decommitted(
        reservation_t* reservation) {

        u64 size_decommitted = 0;
        u64 size_committed   = 0;
        const bool is_valid = reservation_validate(reservation);
        
        if (is_valid) {

            reservation->last_error.val = memory_error_e_success;
            const u64 arena_size = reservation->size.arena; 
            for (
                arena_t* arena = reservation->arena_list.committed;
                arena != NULL;
                arena = arena->next) {
                
                size_committed += arena_size;
            }

            size_decommitted = (reservation->size.reserved - size_committed);
        } 

        return(size_decommitted);
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_FUNC arena_t*
    reservation_remove_next_decommitted_arena(
        reservation_t* reservation) {
        
        arena_t* arena = reservation->arena_list.decommitted;
        if (arena != NULL) {
            arena_t* next = arena->next;
            next->prev = NULL;
            reservation->arena_list.decommitted = next;            
            arena->next = NULL;
            arena->prev = NULL;
        }
        return(arena);
    }

    SLD_FUNC void
    reservation_remove_committed_arena(
        reservation_t* reservation,
        arena_t*       arena) {

        if (reservation->arena_list.committed == arena) {
            reservation->arena_list.committed = arena->next;
        }

        if (arena->prev != NULL) arena->prev = arena->next;
        if (arena->next != NULL) arena->next = arena->prev;
    }

    SLD_FUNC void
    reservation_insert_committed_arena(
        reservation_t* reservation,
        arena_t*       arena) {

        arena_t* next = reservation->arena_list.committed;
        if (next != NULL) next->prev = arena;
        arena->prev = NULL;
        reservation->arena_list.committed = arena;        
    }

    SLD_FUNC void
    reservation_insert_decommitted_arena(
        reservation_t* reservation,
        arena_t*       arena) {

        arena_t* next = reservation->arena_list.decommitted;
        if (next != NULL) next->prev = arena;
        arena->next = next;
        reservation->arena_list.decommitted = arena;
    }

    SLD_FUNC reservation_t*
    reservation_list_remove_next_released(
        reservation_list_t& reservation_list) {

        reservation_t* reservation = reservation_list.released;
        if (reservation != NULL) {

            reservation_t* next = reservation->next;
            if (next != NULL) {
                next->prev = NULL;
            }
            reservation_list.released = next;
            reservation->next = NULL;
            reservation->prev = NULL;
        }
        return(reservation);
    }

    SLD_FUNC bool
    reservation_list_remove_reserved(
        reservation_list_t& reservation_list,
        reservation_t*      reservation) {

        // check that the reservation is in the reserved list
        bool is_valid = false;
        for (
            reservation_t* r = reservation_list.reserved;
            (r != NULL) && (is_valid == false);
            r = r->next) {

            is_valid = (r == reservation);
        }

        if (is_valid) {

            // get the adjacent reservations
            reservation_t* next = reservation->next;
            reservation_t* prev = reservation->prev;

            // update the adjacent reservations
            // if we are removing the first one in the list,
            // we need to make sure we update it
            if      (next)                                     next->prev = prev;
            if      (prev)                                     prev->next = next;
            else if (reservation == reservation_list.reserved) reservation_list.reserved = reservation->next;

            reservation->next = NULL;
            reservation->prev = NULL;
        }

        return(is_valid);
    }

    SLD_FUNC void
    reservation_list_insert_released(
        reservation_list_t& reservation_list,
        reservation_t*      reservation) {

        reservation_t* next = reservation_list.released;
        
        if (next) next->prev = reservation;

        reservation->next         = next;
        reservation_list.released = reservation;
    }

    SLD_FUNC void
    reservation_list_insert_reserved(
        reservation_list_t& reservation_list,
        reservation_t*      reservation) {

        reservation_t* next = reservation_list.reserved;
        
        if (next) next->prev = reservation;

        reservation->next         = next;
        reservation_list.reserved = reservation;
    }
};
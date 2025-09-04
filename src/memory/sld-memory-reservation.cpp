#pragma once

#include "sld-memory.hpp"
#include "sld-os.hpp"
#include ""

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    reservation_t* reservation_list_remove_next_released (reservation_list_t& reservation_list);
    bool           reservation_list_remove_reserved      (reservation_list_t& reservation_list, reservation_t** reservation);
    void           reservation_list_insert_released      (reservation_list_t& reservation_list, reservation_t** reservation);
    void           reservation_list_insert_reserved      (reservation_list_t& reservation_list, reservation_t** reservation);

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_FUNC reservation_list_t& 
    reservation_list_instance(
        void) {

        static reservation_list_t list = {
            NULL, // reserved
            NULL  // released
        };

        return(list);
    }

    SLD_FUNC bool
    reservation_validate(
        reservation_t* reservation) {

        bool is_valid = (reservation != NULL);
        if (!is_valid) {
            is_valid &= (reservation->start         != NULL);
            is_valid &= (reservation->size.reserved != 0);
            is_valid &= (reservation->size.arena    != 0);

            reservation->last_error = (is_valid) 
                ? memory_error_e_success
                : memory_error_e_invalid_reservation;
        }
        return(is_valid);
    }


    SLD_FUNC reservation_t*
    reservation_acquire(
        const u64   size_min_reservation = 0,
        const u64   size_min_arena       = 0) {

        static reservation_list_t& reservation_list = reservation_list_instance();

        // check if there are any released arenas to recycle 
        // if not, push a new one on the stack
        reservation_t* reservation = reservation_list_remove_next_released(reservation_list);
        if (reservation == NULL) {
            static const push_size = sizeof(reservation_t);
            reservation = (reservation_t*)global_stack_push_bytes(push_size);
        }

        // we should have one by now
        if (!reservation) return(reservation);

        // align and commit memory
        const u64 size_reservation  = os_memory_align_to_granularity (size_min_reservation);  
        const u64 size_arena        = os_memory_align_to_page        (size_min_arena);  
        addr      reservation_start = os_memory_reserve              (NULL,reservation_size_actual);
        if (reservation_start == NULL) {
            reservation->last_error = memory_error_e_os_failed_to_reserve;
            reservation_list_insert_released(reservation_list, &reservation);
            return(false);
        }

        // initialize the reservation 
        // and add it to the list
        reservation->start         = reservation_start;
        reservation->size.reserved = reservation_size_actual;
        reservation->size.arena    = arena_size_actual; 
        reservation->last_error    = memory_error_e_success;
        reservation_list_insert_reserved(reservation_list, &reservation);

        return(reservation);
    }
    
    SLD_FUNC bool
    reservation_release(
        reservation_t* reservation) {

        const bool is_valid = reservation_validate(reservation);
        if (!is_valid) return(is_valid);

        // attempt to update the reseved list
        reservation_list_t& list = reservation_list_instance();
        const bool is_removed    = reservation_list_remove_reserved(list, &reservation);
        if (!is_removed) {
            reservation->last_error = memory_error_e_invalid_reservation;
            return(is_removed);
        }

        // attempt to release the os memory 
        const bool is_released = os_memory_release(
            reservation->start,
            reservation->size.reserved);
        
        if (!is_released) {
            reservation->last_error = memory_error_e_os_failed_to_release;
            return(is_released);
        }
        reservation->last_error = memory_error_e_success;

        // we don't need to manually decommit the arenas,
        // just add them all to the released list
        memory_arena_t* arenas_committed   = reservation->arena_list.committed;
        memory_arena_t* arenas_decommitted = reservation->arena_list.decommitted;

        if (arenas_decommitted != NULL) {
            arenas_decommitted->prev = arenas_committed;
        }
        if (arenas_committed != NULL) {
            arenas_committed->next   = arenas_decommitted;
            arenas_decommitted->prev = NULL;            
        }
        reservation->arena_list.decommitted = arenas_committed;
        return(is_released);
    }

    SLD_FUNC bool
    reservation_reset(
        reservation_t* reservation) {

        const bool is_valid = reservation_validate(reservation);
        if (is_valid) {

            bool is_decommitted = true;
            for (
                memory_arena_t* arena = reservation->arena_list.committed;
                arena = arena->next;
                arena != NULL) {

                is_decommitted &= memory_arena_decommit(arena);
            }

            reservation->last_error = (is_decommitted)
                ? memory_error_e_critical
                : memory_error_e_success;
        } 

        const bool result = is_valid && (reservation->last_error == memory_error_e_success) 
        return(result);            
    }

    SLD_FUNC u64
    reservation_size_committed(
        reservation_t* reservation) {

        u64 size_committed = 0;
        const bool is_valid = reservation_validate(reservation);
        
        if (is_valid) {

            reservation->last_error = memory_error_e_success;

            for (
                memory_arena_t* arena = reservation->arena_list.committed;
                arena = arena->next;
                arena != NULL) {

                size_committed += reservation->size.arena; 
            } 
        } 
        
        return(size_committed);
    }

    SLD_FUNC u64
    reservation_size_decommitted(
        reservation_t* reservation) {

        const u64 size_committed   = reservation_size_committed(reservation);
        const u64 size_decommitted = (reservation->last_error == memory_error_e_success)
            ? (reservation->size.total - size_committed)
            : 0;

        return(size_decommitted);
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_INLINE reservation_t*
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

    SLD_INLINE bool
    reservation_list_remove_reserved(
        reservation_list_t& reservation_list,
        reservation_t**     reservation) {

        // check that the reservation is in the reserved list
        // and its memory has been released
        bool is_valid = false;
        reservation_t* tmp = *reservation;
        for (
            reservation_t* r = reservation_list.reserved;
            (r != NULL) && (is_valid == false);
            r = r->next) {

            is_valid = (r == tmp);
        }

        if (is_valid) {

            // get the adjacent reservations
            reservation_t* next = tmp->next;
            reservation_t* prev = tmp->prev;

            // update the adjacent reservations
            // if we are removing the first one in the list,
            // we need to make sure we update it
            if      (next)                             next->prev = prev;
            if      (prev)                             prev->next = next;
            else if (tmp == reservation_list.reserved) reservation_list.reserved = tmp->next;

            tmp->next = NULL;
            tmp->prev = NULL;
        }

        return(is_valid);
    }

    SLD_INLINE void
    reservation_list_insert_released(
        reservation_list_t& reservation_list,
        reservation_t**     reservation) {

        reservation_t* tmp  = *reservation;    
        reservation_t* next = reservation_list.released;
        
        if (next) next->prev = tmp;

        tmp->next                 = next;
        reservation_list.released = tmp;
    }

    SLD_INLINE void
    reservation_list_insert_reserved(
        reservation_list_t& reservation_list,
        reservation_t**     reservation) {

        reservation_t* tmp  = *reservation;    
        reservation_t* next = reservation_list.reserved;
        
        if (next) next->prev = tmp;

        tmp->next                 = next;
        reservation_list.reserved = tmp;
    }
};
#pragma once

#include "sld-memory.hpp"
#include "sld-os.hpp"

namespace sld {

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

        reservation_t* reservation = global_stack_push_reservation();
        if (!reservation) return(reservation);

        const u64 size_reservation  = os_memory_align_to_granularity (size_min_reservation);  
        const u64 size_arena        = os_memory_align_to_page        (size_min_arena);  
        addr      reservation_start = os_memory_reserve              (NULL,reservation_size_actual);

        if (reservation_start == NULL) {
            reservation->last_error = memory_error_e_os_failed_to_reserve;
            return(false);
        }

        reservation->start                  = reservation_start;
        reservation->size.reserved          = reservation_size_actual;
        reservation->size.arena             = arena_size_actual; 
        reservation->arena_list.committed   = NULL;
        reservation->arena_list.decommitted = NULL;
        reservation->last_error             = memory_error_e_success;

        return(reservation);
    }
    
    SLD_FUNC bool
    reservation_release(
        reservation_t* reservation) {

        const bool is_valid = reservation_validate(reservation);
        if (!is_valid) return(is_valid);

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
        // just add them all to the decomitted list
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
};
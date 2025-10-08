#pragma once

#include "sld-memory.hpp"

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
            is_valid &= (reservation->size.arena_memory    != 0);

            reservation->last_error.val = (is_valid) 
                ? memory_error_e_success
                : memory_error_e_invalid_reservation;
        }
        return(is_valid);
    }


    SLD_API bool
    reservation_acquire(
        reservation_t* reservation,
        const u64      size_min_reservation,
        const u64      size_min_arena,
        const u64      size_arena_header) {
        
        bool can_reserve = true;
        can_reserve &= (reservation          != NULL);
        can_reserve &= (size_min_reservation != 0);
        can_reserve &= (size_min_arena       != 0);        
        if (!can_reserve) return(false);

        // align and reserve memory
        const u64 size_header       = sld::size_round_up_pow2        (size_arena_header); 
        const u64 size_reservation  = os_memory_align_to_granularity (size_min_reservation);  
        const u64 size_arena        = os_memory_align_to_page        (size_min_arena + size_arena_header);  
        addr      reservation_start = (addr)os_memory_reserve        (NULL,size_reservation);
        if (reservation_start == 0) return(false);

        // initialize the reservation 
        // and add it to the list
        reservation->start             = reservation_start;
        reservation->size.reserved     = size_reservation;
        reservation->size.arena_memory = size_arena; 
        reservation->size.arena_header = size_header; 
        reservation->last_error.val    = memory_error_e_success;
        reservation->arenas            = NULL;
        return(reservation);
    }
    
    SLD_API bool
    reservation_release(
        reservation_t* reservation) {

        const bool is_valid = reservation_validate(reservation);
        if (!is_valid) return(is_valid);

        // attempt to release the os memory 
        const bool is_released = os_memory_release(
            (void*)reservation->start,
                   reservation->size.reserved);
        
        if (!is_released) {
            reservation->last_error.val = memory_error_e_os_failed_to_release;
            return(is_released);
        }

        reservation->arenas         = NULL;
        reservation->size.reserved  = 0;
        reservation->size.arena_memory     = 0;
        reservation->last_error.val = memory_error_e_success;
        return(is_released);
    }

    SLD_API bool
    reservation_reset(
        reservation_t* reservation) {

        const bool is_valid = reservation_validate(reservation);
        if (is_valid) {

            bool is_decommitted = true;

            for (
                arena_t* arena = reservation->arenas;
                arena != NULL;
                arena = arena->next) {

                is_decommitted &= arena_decommit(arena);
            }

            reservation->last_error.val = (is_decommitted)
                ? memory_error_e_critical
                : memory_error_e_success;
        } 

        reservation->arenas = NULL;

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
            const u64 arena_size = reservation->size.arena_memory; 
            for (
                arena_t* arena = reservation->arenas;
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

            const u32 size_committed = reservation_size_committed(reservation);
            size_decommitted         = (reservation->size.reserved - size_committed);
        } 

        return(size_decommitted);
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------
};
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


    SLD_API bool
    reservation_acquire(
        reservation_t* reservation,
        const u64      size_min_reservation,
        const u64      size_min_arena) {
        
        bool can_reserve = true;
        can_reserve &= (reservation          != NULL);
        can_reserve &= (size_min_reservation != 0);
        can_reserve &= (size_min_arena       != 0);        
        if (!can_reserve) return(false);

        // align and reserve memory
        const u64 size_reservation  =       os_memory_align_to_granularity (size_min_reservation);  
        const u64 size_arena        =       os_memory_align_to_page        (size_min_arena);  
        addr      reservation_start = (addr)os_memory_reserve              (NULL,size_reservation);
        if (reservation_start == 0) return(false);

        // initialize the reservation 
        // and add it to the list
        reservation->start                  = reservation_start;
        reservation->size.reserved          = size_reservation;
        reservation->size.arena             = size_arena; 
        reservation->last_error.val         = memory_error_e_success;
        reservation->arena_list.committed   = NULL;
        reservation->arena_list.decommitted = NULL;

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

        reservation->arena_list.committed   = NULL;
        reservation->arena_list.decommitted = NULL;
        reservation->size.reserved          = 0;
        reservation->size.arena             = 0;
        reservation->last_error.val         = memory_error_e_success;
        return(is_released);
    }

    SLD_API bool
    reservation_reset(
        reservation_t* reservation) {

        const bool is_valid = reservation_validate(reservation);
        if (is_valid) {

            bool is_decommitted = true;

            for (
                arena_t* arena = reservation->arena_list.committed;
                arena != NULL;
                arena = arena->next) {

                is_decommitted &= arena_decommit(arena);
            }

            reservation->last_error.val = (is_decommitted)
                ? memory_error_e_critical
                : memory_error_e_success;
        } 

        reservation->arena_list.committed   = NULL;
        reservation->arena_list.decommitted = NULL;

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
};
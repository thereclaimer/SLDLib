#pragma once

#include "sld-memory-internal.cpp"

namespace sld {

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API bool
    memory_reservation_validate(
        const memory_reservation_handle_t reservation_handle) {

        memory_reservation_t* reservation = memory_stack_get_reservation(reservation_handle);
        const bool            is_valid    = memory_reservation_validate_internal(reservation);
        return(is_valid);
    }

    SLD_API const memory_reservation_handle_t
    memory_reservation_acquire(
        const u64 reservation_size, const u64 arena_size) {

        static memory_error_t& error              = memory_last_error_instance(); 
        memory_reservation_handle_t   reservation_handle = { SLD_MEMORY_INVALID_HANDLE };

        // check args
        bool can_reserve = true;
        can_reserve &= (reservation_size > 0);
        can_reserve &= (arena_size       > 0);
        can_reserve &= (arena_size       <= reservation_size);
        if (!can_reserve) {
            error.val = memory_error_e_invalid_args;
            return(reservation_handle);
        } 

        // if there are no released reservations,
        // get a new one
        memory_reservation_list_t& reservation_list = memory_reservation_list_instance();
        memory_reservation_t*      reservation      = (reservation_list.released == NULL)
            ? memory_stack_push_reservation   ()
            : memory_reservation_list_recycle (reservation_list);

        // we should have a reservation structure at this point
        if (reservation == NULL) {
            error.val = memory_error_e_stack_not_enough_memory;
            return(reservation_handle);
        }

        // initialize the reservation
        const bool is_init = memory_reservation_init(
            reservation,
            reservation_size,
            arena_size,
            error
        );

        // update the list        
        if (!is_init) {
            memory_reservation_list_add_released(reservation_list, reservation);
            return(reservation_handle);
        }
        memory_reservation_list_add_reserved(reservation_list, reservation);

        // return the handle
        const memory_reservation_handle_t handle = memory_stack_get_reservation_handle(reservation);
        return(handle);
    }

    SLD_API bool
    memory_reservation_release(
        const memory_reservation_handle_t reservation_handle) {

        // validate the reservation
        static memory_error_t& error       = memory_last_error_instance           (); 
        memory_reservation_t*      reservation = memory_stack_get_reservation         (reservation_handle);
        const bool                 is_valid    = memory_reservation_validate_internal (reservation); 
        if (!is_valid) {
            error.val = memory_error_e_invalid_reservation;
            return(is_valid);
        }

        return(false);
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_INLINE bool
    memory_reservation_validate_internal(
        const memory_reservation_t* reservation) {

        bool result = true;
        result &= (reservation->start          != 0);
        result &= (reservation->next           != reservation);
        result &= (reservation->prev           != reservation);
        result &= (reservation->size.total     != 0);
        result &= (reservation->size.arena     != 0);
        result &= (reservation->size.arena     <= reservation->size.total);
        result &= (reservation->size.committed <= reservation->size.total);
        return(result);
    }

    SLD_INLINE memory_reservation_list_t&
    memory_reservation_list_instance(
        void) {

        static memory_reservation_list_t list = {
            NULL, // reserved
            NULL  // released
        };

        return(list);
    }

    SLD_INLINE memory_reservation_t*
    memory_reservation_list_recycle(
        memory_reservation_list_t& reservation_list) {

        // get the first released reservation if there is one
        memory_reservation_t* reservation = reservation_list.released;
        if (reservation == NULL) return(reservation);

        // zero the struct
        reservation->start          = 0;
        reservation->next           = NULL;
        reservation->prev           = NULL;
        reservation->size.total     = 0;
        reservation->size.arena     = 0;
        reservation->size.committed = 0;

        // update the released list
        reservation_list.released = reservation->next;
        if (reservation_list.released != NULL) {
            reservation_list.released->prev = NULL;
        }

        return(reservation);
    }

    SLD_INLINE bool
    memory_reservation_init(
        memory_reservation_t* reservation,
        const u64             reservation_size,
        const u64             arena_size,
        memory_error_t&   last_error) {

        static void* reservation_start = NULL; 

        reservation->size.committed = 0; 
        reservation->size.total     =       os_memory_align_to_granularity (reservation_size);
        reservation->size.arena     =       os_memory_align_to_page        (arena_size);
        reservation->start          = (addr)os_memory_reserve              (reservation_start, reservation->size.total); 
    
        const bool is_reserved = (reservation->start != 0); 
        last_error             = (s32)((is_reserved) ? memory_error_e_success : memory_error_e_os_failed_to_reserve);
        return(is_reserved);
    }

    SLD_INLINE bool
    memory_reservation_deinit(
        memory_reservation_t* reservation,
        memory_error_t&   last_error) {

        const bool is_released = os_memory_release(
            (vptr)reservation->start,
            reservation->size.total
        );
        return(is_released);
    }

    SLD_INLINE void
    memory_reservation_list_add_released(
        memory_reservation_list_t& reservation_list,
        memory_reservation_t*      reservation) {

        if (reservation_list.released) {
            reservation_list.released->prev = reservation;
        }
        if (reservation) {
            reservation->prev = NULL;
            reservation->next = reservation_list.released;
        }
        reservation_list.released = reservation;
    }

    SLD_INLINE void
    memory_reservation_list_add_reserved(
        memory_reservation_list_t& reservation_list,
        memory_reservation_t*      reservation) {
            
        if (reservation_list.reserved) {
            reservation_list.reserved->prev = reservation;
        }
        if (reservation) {
            reservation->prev = NULL;
            reservation->next = reservation_list.reserved;
        }
        reservation_list.reserved = reservation;
    }
};
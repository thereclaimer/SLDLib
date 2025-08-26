#pragma once

#include "sld-memory-internal-stack.cpp"

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------
    
    struct memory_reservation_t;
    struct memory_reservation_list_t;

    memory_reservation_list_t& memory_reservation_list_instance     (void);
    bool                       memory_reservation_validate_internal (const memory_reservation_t* reservation, const memory_manager_t& memory_manager);
    memory_reservation_t*      memory_reservation_get_new           (void);    

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    struct memory_reservation_t {
        addr                  start;
        memory_reservation_t* next;
        memory_reservation_t* prev;
        struct {
            u64 total;
            u64 arena;
            u64 committed;
        } size;
    };

    struct memory_reservation_list_t {
        memory_reservation_t* reserved;
        memory_reservation_t* released;
    };

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API bool
    memory_reservation_validate(
        const memory_reservation_id_t reservation_id) {

        static memory_manager_t& mem_man  = memory_manager_instance();
        memory_reservation_t*    res      = memory_manager_get_reservation       (mem_man, reservation_id); 
        const bool               is_valid = memory_reservation_validate_internal (res,     mem_man);
        return(is_valid);
    }

    SLD_API const memory_reservation_h32
    memory_reservation_acquire(
        const u64 reservation_size, const u64 arena_size) {
       
        memory_reservation_t* reservation = NULL;

        // if there are no released reservations,
        // get a new one
        if (list.released == NULL) {

            reservation = memory_internal_stack_push_reservation();
        }
        // otherwise, we will recycle the first one
        // off the released list
        else {
            
            reservation   = list.released;
            list.released = reservation->next;

            if (list.released != NULL) {
                list.released->prev = NULL;
            }
        }

        // we should have a reservation structure at this point
        if (reservation == NULL) return(SLD_MEMORY_INVALID_HANDLE);

        // align sizes 
        const u64  reservation_size_aligned = os_memory_align_to_granularity (reservation_size);
        const u64  arena_size_aligned       = os_memory_align_to_page        (arena_size); 
        const bool are_args_valid           = (
            reservation_size_aligned != 0 &&
            arena_size_aligned       != 0 &&
            arena_size_aligned       <= reservation_size_aligned); 
        if (!are_args_valid) return(SLD_MEMORY_INVALID_HANDLE);

        // reserve memory
        void* start = os_memory_reserve(NULL, reservation_size_aligned)
        if (!start) return(SLD_MEMORY_INVALID_HANDLE);

        // initialize the reservation and add it to the front of the 
        // reserved list


        return(res_id);
    }

    SLD_API bool
    memory_reservation_release(
        const memory_reservation_id_t reservation_id) {

        static memory_manager_t& mem_man  = memory_manager_instance();
        memory_reservation_t*    res      = memory_manager_get_reservation       (mem_man, reservation_id); 
        const bool               is_valid = memory_reservation_validate_internal (res, mem_man);

        // check that the reservation is valid
        if (res == NULL) {
            mem_man.last_error = memory_error_e_invalid_id;
            return(false);
        }
        if (!is_valid) {
            mem_man.last_error = memory_error_e_invalid_reservation;
            return(false);            
        }

        // release the memory
        bool result = os_memory_release((void*)res->start, res->total_size);
        if (result) {

            // reset the reservation
            res->start          = 0;
            res->total_size     = 0;
            res->arena_size     = 0;
            res->committed_size = 0;


            // reset the arenas
            for (
                memory_arena_t* arena = mem_man.arenas;
                arena != NULL;
                arena = arena->next) {

                arena->reservation    = NULL;
                arena->stack.start    = 0;
                arena->stack.size     = 0;
                arena->stack.position = 0;
                arena->stack.save     = 0;
            }
        
            mem_man.last_error = memory_error_e_success;
        }
        else {
            mem_man.last_error = memory_error_e_os_failed_to_release;
        }

        return(result);
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_INLINE bool
    memory_reservation_validate_internal(
        const memory_reservation_t* reservation,
        const memory_manager_t&     memory_manager) {

        bool is_valid = (reservation != NULL);
        if  (is_valid) {
            is_valid &= (reservation->start          != 0);
            is_valid &= (reservation->total_size     != 0);
            is_valid &= (reservation->arena_size     != 0);
            is_valid &= (reservation->arena_size     <= reservation->total_size);
            is_valid &= (reservation->committed_size <= reservation->total_size);
            is_valid &= (((addr)reservation - reservation->id) == memory_manager.stack.start);
        }
        return(is_valid);
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
    memory_reservation_get_new(
        void) {

        static memory_reservation_list_t& list = memory_reservation_list_instance();
 


    }    

};
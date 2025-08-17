#pragma once

#include "sld-memory-internal.cpp"
#include "sld-memory-manager.cpp"

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    bool memory_reservation_validate_internal(const memory_reservation_t* reservation, const memory_manager_t& memory_manager);
    
    //-------------------------------------------------------------------
    // PUBLIC
    //-------------------------------------------------------------------

    bool
    memory_reservation_validate(
        const memory_reservation_id_t reservation_id) {

        static memory_manager_t& mem_man  = memory_manager_instance();
        memory_reservation_t*    res      = memory_manager_get_reservation       (mem_man, reservation_id); 
        const bool               is_valid = memory_reservation_validate_internal (res,     mem_man);
        return(is_valid);
    }

    const memory_reservation_id_t
    memory_reservation_acquire(
        const u64 reservation_size, const u64 arena_size) {

        static memory_manager_t& mem_mgr = memory_manager_instance();

        // align sizes 
        const u64  reservation_size_aligned = os_memory_align_to_granularity (reservation_size);
        const u64  arena_size_aligned       = os_memory_align_to_page        (arena_size); 
        const bool are_args_valid           = (
            reservation_size_aligned != 0 &&
            arena_size_aligned       != 0 &&
            arena_size_aligned       <= reservation_size_aligned); 

        // find or alloc a free reservation
        memory_reservation_t* res           = memory_manager_find_free_reservation (mem_mgr);
        if (!res)             res           = memory_manager_alloc_reservation     (mem_mgr);
        const bool            is_res_valid  = (res != NULL);

        // reserve memory
        void* res_start = (are_args_valid && is_res_valid)
            ? os_memory_reserve(NULL, reservation_size_aligned)
            : NULL;
        const bool is_mem_reserved = (res_start != NULL); 

        // check results
        const bool result = (
            are_args_valid &&
            is_res_valid   &&
            is_mem_reserved
        );

        // if everything is good, initialize the reservation
        memory_reservation_id_t res_id = SLD_MEMORY_INVALID_ID;        
        if (result) {

            mem_mgr.last_error = memory_error_e_success;
            res_id             = res->id;
            res->start         = (addr)res_start; 
            res->total_size    = reservation_size_aligned; 
            res->arena_size    = arena_size_aligned;       
        }
        // otherwise, set the error
        else {

            if      (are_args_valid)  mem_mgr.last_error = memory_error_e_invalid_args;
            else if (is_res_valid)    mem_mgr.last_error = memory_error_e_stack_out_of_memory;
            else if (is_mem_reserved) mem_mgr.last_error = memory_error_e_os_failed_to_reserve;
            else                      mem_mgr.last_error = memory_error_e_unknown;
        }

        return(res_id);
    }

    bool
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

    sld_rt_inline bool
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
};
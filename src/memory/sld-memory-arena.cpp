#pragma once

#include "sld-memory-internal.cpp"
#include "sld-memory-manager.cpp"
#include "sld-memory-reservation.cpp"

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    bool memory_arena_validate_internal(const memory_arena_t* arena, const memory_manager_t& memory_manager);

    //-------------------------------------------------------------------
    // PUBLIC
    //-------------------------------------------------------------------

    const memory_arena_id_t
    memory_arena_commit(
        const memory_reservation_id_t reservation_id) {

        memory_arena_id_t arena_id = SLD_MEMORY_INVALID_ID;

        memory_manager_t&     mem_man      = memory_manager_instance();
        memory_reservation_t* res          = memory_manager_get_reservation       (mem_man, reservation_id);
        const bool            res_is_valid = memory_reservation_validate_internal (res, mem_man); 

        // check if everything is valid
        if (res == NULL) {
            mem_man.last_error = memory_error_e_invalid_id;
            return(arena_id);
        }
        if (!res_is_valid) {
            mem_man.last_error = memory_error_e_invalid_reservation;
            return(arena_id);
        }

        // check we have enough memory
        const u64 new_committed_size = res->committed_size + res->arena_size;
        if (new_committed_size > res->total_size) {
            mem_man.last_error = memory_error_e_reservation_out_of_memory;
            return(arena_id);
        }

        // get a new arena
        memory_arena_t* arena = memory_manager_find_free_arena (mem_man);
        if (!arena) {

            arena = memory_manager_alloc_arena(mem_man);
            if (!arena) {
                mem_man.last_error = memory_error_e_stack_out_of_memory;
                return(arena_id);
            }
        }




        return(0);
    }

    bool
    memory_arena_decommit(
        const memory_arena_id_t arena_id) {

        return(false);
    }

    bool
    memory_arena_validate(
        const memory_arena_id_t arena_id) {

        return(false);
    }

    bool
    memory_arena_save(
        const memory_arena_id_t arena_id) {

        return(false);
    }

    bool
    memory_arena_reset(
        const memory_arena_id_t arena_id) {

        return(false);
    }

    bool
    memory_arena_reset_to_save(
        const memory_arena_id_t arena_id) {

        return(false);
    }

    const u64
    memory_arena_space_remaining(
        const memory_arena_id_t arena_id) {

        return(0);
    }

    byte*
    memory_arena_push_bytes(
        const memory_arena_id_t arena_id,
        const u64               size) {

        return(NULL);
    }
    
    bool
    memory_arena_pull_bytes(
        const memory_arena_id_t arena_id,
        const u64               size) {

        return(false);
    }
    
    bool
    memory_arena_can_push_bytes(
        const memory_arena_id_t arena_id,
        const u64               size) {

        return(false);
    }
    
    bool
    memory_arena_can_pull_bytes(
        const memory_arena_id_t arena_id,
        const u64               size) {

        return(false);
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    sld_rt_inline bool
    memory_arena_validate_internal(
        const memory_arena_t*   arena,
        const memory_manager_t& memory_manager) {

        bool is_valid = (arena != NULL);
        if (is_valid) {

            const addr diff = (addr)arena - arena->id;

            is_valid &= (arena->reservation != NULL);
            is_valid &= stack_validate(arena->stack); 
            is_valid &= (diff == memory_manager.stack.start);
        }
        return(is_valid);
    }
};
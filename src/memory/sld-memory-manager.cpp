#pragma once

#include "sld-memory.hpp"
#include "sld-stack.hpp"
#include "sld-os.hpp"

namespace sld {

    struct memory_manager_t;
    struct memory_reservation_t;
    struct memory_arena_t;

    memory_manager_t&     memory_manager_instance            (void);
    const memory_id_t     memory_manager_get_id_from_address (const addr        address);
    const addr            memory_manager_get_address_from_id (const memory_id_t id);
    memory_reservation_t* memory_manager_alloc_reservation   (void);
    memory_arena_t*       memory_manager_alloc_arena         (void);
    memory_reservation_t* memory_manager_get_reservation     (const memory_reservation_id_t reservation_id);
    memory_arena_t*       memory_manager_get_arena           (const memory_arena_id_t       arena_id);

    struct memory_manager_t {
        stack_t                 stack;
        os_system_memory_info_t system_memory_info;
        memory_reservation_t*   reservations;
        memory_arena_t*         arenas;
        memory_error_t          last_error;
    };

    struct memory_reservation_t {
        memory_reservation_t*   next;
        memory_reservation_t*   prev;
        addr                    start;
        u32                     total_page_count;
        u32                     arena_page_count;
        memory_reservation_id_t id;
    };

    struct memory_arena_t {
        memory_reservation_t* reservation;
        memory_arena_t*       next;
        memory_arena_t*       prev;
        stack_t               stack;
        memory_arena_id_t     id;
    };

    sld_rt_inline memory_manager_t&
    memory_manager_instance(
        void) {

        sld_ct_const u64 stack_size = size_kilobytes(SLD_MEMORY_INTERNAL_STACK_SIZE_KB);

        static memory_manager_t memory_manager;
        static byte             stack_memory[stack_size];         
        static bool             initialized = false;

        if (!initialized) {

            initialized = true;

            memory_manager.stack.start    = stack_memory;
            memory_manager.stack.size     = stack_size;
            memory_manager.stack.position = 0;
            memory_manager.stack.save     = 0;
            memory_manager.reservations   = NULL;
            memory_manager.arenas         = NULL;
        };

        return(memory_manager);
    }

    sld_rt_inline const memory_id_t
    memory_manager_get_id_from_address(
        const addr address) {

        static memory_manager_t& memory_manager = memory_manager_instance();

        const s64 addr_diff = address - memory_manager.stack.start;

        bool is_valid = true;
        is_valid &= (addr_diff >= 0);
        is_valid &= (addr_diff <  memory_manager.stack.size);

        memory_id_t memory_id;

        if (is_valid) {

            memory_id                 = (memory_id_t)addr_diff;
            memory_manager.last_error = memory_error_e_success;
        }
        else {

            memory_id                 = SLD_MEMORY_INVALID_ID;
            memory_manager.last_error = memory_error_e_invalid_address;
        }      

        return(memory_id);
    }

    sld_rt_inline const addr
    memory_manager_get_address_from_id(
        const memory_id_t id) {

        static memory_manager_t& memory_manager = memory_manager_instance();

        bool is_valid = true;
        is_valid &= (id >= 0);
        is_valid &= (id <  memory_manager.stack.size);

        addr memory_address;
        if (is_valid) {

            memory_address            = memory_manager.stack.start + id;
            memory_manager.last_error = memory_error_e_success;
        }
        else {
            memory_address = 0;
            memory_manager.last_error = memory_error_e_invalid_id;
        }

        return(memory_address);
    }


    sld_rt_inline memory_reservation_t*
    memory_manager_alloc_reservation(
        void) {

        static memory_manager_t& memory_manager = memory_manager_instance();

        memory_reservation_t* new_reservation = (memory_reservation_t*)stack_push(
            memory_manager.stack, 
            sizeof(memory_reservation_t));

        if (new_reservation != NULL) {

            // initialize the reservation
            new_reservation->next             = memory_manager.reservations;
            new_reservation->prev             = NULL;
            new_reservation->start            = 0;
            new_reservation->total_page_count = 0;
            new_reservation->arena_page_count = 0;
            new_reservation->id               = memory_manager_get_id_from_address((addr)reservation);

            // put the new reservation at the front of the list
            if (memory_manager.reservations != NULL) {
                memory_manager.reservations->prev = new_reservation; 
            }
            memory_manager.reservations = new_reservation;
            memory_manager.last_error   = memory_error_e_success;
        }
        else {
            memory_manager.last_error   = memory_error_e_stack_out_of_memory;
        }

        return(new_reservation);
    }

    sld_rt_inline memory_arena_t*
    memory_manager_alloc_arena(
        void) {

        static memory_manager_t& memory_manager = memory_manager_instance();

        memory_arena_t* new_arena = (memory_arena_t*)stack_push(
            memory_manager.stack, 
            sizeof(memory_arena_t));

        if (new_arena != NULL) {

            // initialize the arena
            new_arena->reservation    = NULL;
            new_arena->next           = memory_manager.arenas;
            new_arena->prev           = NULL;
            new_arena->stack.start    = 0;
            new_arena->stack.size     = 0;
            new_arena->stack.position = 0;
            new_arena->stack.save     = 0;
            new_arena->id             = memory_manager_get_id_from_address((addr)new_arena);

            // put the new reservation at the front of the list
            if (memory_manager.arenas != NULL) {
                memory_manager.arenas->prev = new_arena; 
            }
            memory_manager.arenas     = new_arena;
            memory_manager.last_error = memory_error_e_success;
        }
        else {
            memory_manager.last_error = memory_error_e_stack_out_of_memory;
        }

        return(new_arena); 
    }

    sld_rt_inline memory_reservation_t*
    memory_manager_get_reservation(
        const memory_reservation_id_t reservation_id) {

        static memory_manager_t& memory_manager = memory_manager_instance();
        memory_reservation_t*    reservation    = (memory_reservation_t*)memory_manager_get_address_from_id(reservation_id);
        
        const bool is_valid = (
            reservation     != NULL &&
            reservation->id == reservation_id
        );

        memory_manager.last_error = (is_valid)
            ? memory_error_e_success
            : memory_error_e_invalid_reservation;

        return(reservation);
    }

    sld_rt_inline memory_arena_t*
    memory_manager_get_arena(
        const memory_arena_id_t arena_id) {

        static memory_manager_t& memory_manager = memory_manager_instance();
        memory_arena_t*          arena          = (memory_arena_t*)memory_manager_get_address_from_id(arena_id);
        
        const bool is_valid = (
            reservation     != NULL &&
            reservation->id == reservation_id
        );

        memory_manager.last_error = (is_valid)
            ? memory_error_e_success
            : memory_error_e_invalid_reservation;

        return(reservation);
    }
};
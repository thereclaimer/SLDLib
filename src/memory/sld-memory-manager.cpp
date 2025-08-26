#pragma once

#include "sld-memory.hpp"
#include "sld-memory-internal.cpp"

namespace sld {

    memory_manager_t&     memory_manager_instance                (void);
    memory_reservation_t* memory_manager_alloc_reservation       (memory_manager_t& memory_manager);
    memory_reservation_t* memory_manager_find_free_reservation   (memory_manager_t& memory_manager);
    memory_arena_t*       memory_manager_alloc_arena             (memory_manager_t& memory_manager);
    memory_arena_t*       memory_manager_find_free_arena         (memory_manager_t& memory_manager);
    const memory_id_t     memory_manager_get_id_from_address     (memory_manager_t& memory_manager, const addr        address);
    const addr            memory_manager_get_address_from_id     (memory_manager_t& memory_manager, const memory_id_t id);
    memory_reservation_t* memory_manager_get_reservation         (memory_manager_t& memory_manager, const memory_reservation_id_t reservation_id);
    memory_arena_t*       memory_manager_get_arena               (memory_manager_t& memory_manager, const memory_arena_id_t       arena_id);
    

    SLD_INLINE memory_manager_t&
    memory_manager_instance(
        void) {

        sld_ct_const u64 stack_size = size_kilobytes(SLD_MEMORY_INTERNAL_STACK_SIZE_KB);

        static memory_manager_t memory_manager;
        static byte             stack_memory[stack_size];         
        static bool             initialized = false;

        if (!initialized) {

            initialized = true;


            memory_manager.stack.start    = (addr)stack_memory;
            memory_manager.stack.size     = stack_size;
            memory_manager.stack.position = 0;
            memory_manager.stack.save     = 0;
            memory_manager.reservations   = NULL;
            memory_manager.arenas         = NULL;
            os_system_get_memory_info(memory_manager.system_memory_info);
        };

        return(memory_manager);
    }

    SLD_INLINE const memory_id_t
    memory_manager_get_id_from_address(
        memory_manager_t& memory_manager,
        const addr        address) {

        const s64 addr_diff = address - memory_manager.stack.start;

        bool is_valid = true;
        is_valid &= (addr_diff >= 0);
        is_valid &= (addr_diff <  memory_manager.stack.size);

        memory_id_t memory_id = (is_valid)
            ? (memory_id_t)addr_diff
            : SLD_MEMORY_INVALID_ID;

        return(memory_id);
    }

    SLD_INLINE const addr
    memory_manager_get_address_from_id(
        memory_manager_t& memory_manager,
        const memory_id_t id) {

        bool is_valid = true;
        is_valid &= (id >= 0);
        is_valid &= (id <  memory_manager.stack.size);

        addr memory_address = (is_valid)
            ? memory_manager.stack.start + id
            : 0;

        return(memory_address);
    }

    SLD_INLINE memory_reservation_t*
    memory_manager_alloc_reservation(
        memory_manager_t& memory_manager) {

        memory_reservation_t* new_reservation = (memory_reservation_t*)stack_push(
            memory_manager.stack, 
            sizeof(memory_reservation_t));

        if (new_reservation != NULL) {

            // initialize the reservation
            new_reservation->next       = memory_manager.reservations;
            new_reservation->prev       = NULL;
            new_reservation->start      = 0;
            new_reservation->total_size = 0;
            new_reservation->arena_size = 0;
            new_reservation->id         = memory_manager_get_id_from_address(
                memory_manager,
                (addr)new_reservation);

            // put the new reservation at the front of the list
            if (memory_manager.reservations != NULL) {
                memory_manager.reservations->prev = new_reservation; 
            }
            memory_manager.reservations = new_reservation;
        }

        return(new_reservation);
    }

    SLD_INLINE memory_reservation_t*
    memory_manager_find_free_reservation(
        memory_manager_t& memory_manager) {

        memory_reservation_t* res_free = NULL;

        for (
            memory_reservation_t* res_current = memory_manager.reservations;
            (
                res_current != NULL &&
                res_free    == NULL
            );
            res_current = res_current->next) {

            if (res_current->start == 0) {
                res_free = res_current;
            }
        }

        return(res_free);
    }

    SLD_INLINE memory_arena_t*
    memory_manager_alloc_arena(
        memory_manager_t& memory_manager) {

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
            new_arena->id             = memory_manager_get_id_from_address(
                memory_manager,
                (addr)new_arena);

            // put the new reservation at the front of the list
            if (memory_manager.arenas != NULL) {
                memory_manager.arenas->prev = new_arena; 
            }
            memory_manager.arenas     = new_arena;
        }

        return(new_arena); 
    }

    SLD_INLINE memory_reservation_t*
    memory_manager_get_reservation(
        memory_manager_t&             memory_manager,
        const memory_reservation_id_t reservation_id) {

        memory_reservation_t* reservation = (memory_reservation_t*)memory_manager_get_address_from_id(
            memory_manager,
            reservation_id);

        return(reservation);
    }

    SLD_INLINE memory_arena_t*
    memory_manager_get_arena(
        memory_manager_t&       memory_manager,
        const memory_arena_id_t arena_id) {

        memory_arena_t* arena = (memory_arena_t*)memory_manager_get_address_from_id(
            memory_manager,
            arena_id);
        
        return(arena);
    }

    SLD_INLINE memory_arena_t*
    memory_manager_find_free_arena(
        memory_manager_t& memory_manager) {

        memory_arena_t* arena_free = NULL;

        for (
            memory_arena_t* arena_current = memory_manager.arenas;
            (
                arena_current != NULL &&
                arena_free    == NULL
            );
            arena_current = arena_current->next) {

            if (arena_current->reservation == NULL) {
                arena_free = arena_current;
            }
        }

        return(arena_free);

    }

};
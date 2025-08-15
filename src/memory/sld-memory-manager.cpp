#pragma once

#include "sld-memory.hpp"
#include "sld-stack.hpp"
#include "sld-os.hpp"

namespace sld {

    struct memory_manager_t;
    struct memory_reservation_t;
    struct memory_arena_t;

    memory_manager_t&     memory_manager_instance          (void);
    void*                 memory_manager_stack_push        (const u32 size);
    memory_reservation_t* memory_manager_alloc_reservation (void);
    memory_arena_t*       memory_manager_alloc_arena       (void);
    memory_reservation_t* memory_manager_get_reservation   (const memory_reservation_id_t reservation_id);
    memory_arena_t*       memory_manager_get_arena         (const memory_arena_id_t       arena_id);

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
        memory_reservation_t*   reservation;
        memory_arena_t*         next;
        memory_arena_t*         prev;
        stack_t                 stack;
        memory_arena_id_t       id;
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

    sld_rt_inline void*
    memory_manager_stack_push(
        const u32 size) {

        static auto& memory_manager = memory_manager_instance();

        return(memory);
    }


    sld_rt_inline memory_reservation_t*
    memory_manager_alloc_reservation(
        void) {


        sld_ct_const u32 reservation_size = sizeof(memory_reservation_t);
        memory_reservation_t* reservation = (memory_reservation_t*)stack_push(memory_manager.stack, size);

    }

    sld_rt_inline memory_arena_t*
    memory_manager_alloc_arena(
        void) {

    }

    sld_rt_inline memory_reservation_t*
    memory_manager_get_reservation(
        const memory_reservation_id_t reservation_id) {

    }

    sld_rt_inline memory_arena_t*
    memory_manager_get_arena(
        const memory_arena_id_t arena_id) {

    }
};
#pragma once

#include "sld-memory-internal.cpp"
#include "sld-stack.hpp"
#include "sld-os.hpp"

namespace sld {

    struct memory_manager_t;
    struct memory_reservation_t;
    struct memory_arena_t;

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
        u64                     total_size;
        u64                     arena_size;
        u64                     committed_size;
        memory_reservation_id_t id;
    };

    struct memory_arena_t {
        memory_reservation_t* reservation;
        memory_arena_t*       next;
        memory_arena_t*       prev;
        u32                   index;
        stack_t               stack;
        memory_arena_id_t     id;
    };
};
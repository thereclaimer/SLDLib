#pragma once

#include "sld-memory.hpp"
#include "sld-os.hpp"

#include "sld-memory-manager.cpp"

namespace sld {

    static const memory_reservation_id_t
    memory_reservation_acquire(
        const u64 reservation_size,
        const u64 arena_size) {

        bool result = true; 
        
        os_memory_page_table_t page_table;
        page_table.size_requested = reservation_size;

        result &= os_memory_reserve_page_table(page_table);

    }
    
    static const memory_arena_id_t
    memory_reservation_commit_arena(
        const memory_reservation_id_t reservation_id) {

    }
    
    static bool
    memory_reservation_validate(
        const memory_reservation_id_t reservation_id) {

    }
    
    static bool
    memory_reservation_release(
        const memory_reservation_id_t reservation_id) {

    }
    
    static bool
    memory_reservation_decommit_arena(
        const memory_reservation_id_t reservation_id,
        const memory_arena_id_t       arena_id) {

    }


};
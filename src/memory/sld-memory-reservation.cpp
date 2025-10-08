#pragma once

#include "sld-memory.hpp"


namespace sld {


    SLD_API bool 
    reservation_t::is_valid(
        void) {

        bool is_valid = true;
        is_valid &= (start             != NULL);
        is_valid &= (size.reserved     != 0);
        is_valid &= (size.arena_memory != 0);

        last_error.val = (is_valid) 
            ? memory_error_e_success
            : memory_error_e_invalid_reservation;

        return(is_valid);
    }

    SLD_API void
    reservation_t::assert_valid(
        void) {

        assert(is_valid());
    }

    SLD_API bool
    reservation_t::acquire_system_memory(
        const u64 size_min_reservation,
        const u64 size_min_arena,
        const u64 size_arena_header) {

        assert_valid();
        bool can_reserve = true;
        can_reserve &= (size_min_reservation != 0);
        can_reserve &= (size_min_arena       != 0);        
        if (!can_reserve) return(false);

        // align and reserve memory
        const u64 size_header       = sld::size_round_up_pow2        (size_arena_header); 
        const u64 size_reservation  = os_memory_align_to_granularity (size_min_reservation);  
        const u64 size_arena        = os_memory_align_to_page        (size_min_arena + size_arena_header);  
        addr      reservation_start = (addr)os_memory_reserve        (NULL,size_reservation);
        if (reservation_start == 0) return(false);

        // initialize the reservation 
        // and add it to the list
        start             = reservation_start;
        size.reserved     = size_reservation;
        size.arena_memory = size_arena; 
        size.arena_header = size_header; 
        last_error.val    = memory_error_e_success;
        arenas            = NULL;
        return(true);
   }
    
    SLD_API bool
    reservation_t::release_system_memory(
        void) {

        assert_valid();

        // attempt to release the os memory 
        const bool is_released = os_memory_release((void*)start, size.reserved);
        
        if (!is_released) {
            last_error.val = memory_error_e_os_failed_to_release;
            return(is_released);
        }

        arenas            = NULL;
        size.reserved     = 0;
        size.arena_memory = 0;
        last_error.val    = memory_error_e_success;
        return(is_released);
    }

    SLD_API bool
    reservation_t::reset(
        void) {

        assert_valid();
            bool is_decommitted = true;

        for (
            arena_t* arena = arenas;
            arena != NULL;
            arena = arena->next) {

            is_decommitted &= arena->decommit(); 
        }

        last_error.val = (is_decommitted)
            ? memory_error_e_critical
            : memory_error_e_success;

        arenas = NULL;

        assert_valid();

        const bool result = (last_error.val == memory_error_e_success);
        return(result);            
    }

    SLD_API u64
    reservation_t::size_committed(
        void) {
        
        assert_valid();


        const u64 arena_size     = size.arena_memory; 
        u64       size_committed = 0;
        for (
            arena_t* arena = arenas;
            arena != NULL;
            arena = arena->next) {
            
            size_committed += arena_size;
        }
        
        last_error.val = memory_error_e_success;
        return(size_committed);
    }

    SLD_API u64
    reservation_t::size_decommitted(
        void) {

        assert_valid();
        u64 commit_size      = size_committed();
        u64 size_decommitted = (size.reserved - commit_size);
        
        return(size_decommitted);
    }

    SLD_API arena_t*
    reservation_t::commit_arena(
        void) {

        assert_valid();

        // find the next free arena
        const u32 arena_count_max = (size.reserved / size.arena_memory);
        void*     commit_start    = NULL;
        for (
            u32 arena_index = 0;
            arena_index < arena_count_max;
            ++arena_index) {

            const u32  arena_offset  = (arena_index * size.arena_memory);
            void*      arena_tmp     = (void*)(start + arena_offset); 
            const bool arena_is_free = os_memory_is_reserved(arena_tmp);
            
            if (arena_is_free) {
                commit_start = arena_tmp;
                break;
            } 
        }

        // attempt to commit memory
        arena_t* arena = (arena_t*)os_memory_commit(commit_start, size.arena_memory);
        if ((void*)arena == commit_start) {

            constexpr u32 arena_struct_size  = sizeof(arena_t);
            const u32     arena_start_offset = (arena_struct_size + size.arena_header);
            const void*   stack_memory = (void*)(((addr)arena) + arena_start_offset); 
            const u32     stack_size   = (size.arena_memory - arena_struct_size); 

            // initialize the arena
            arena->stack = data_stack_init_from_memory(stack_memory, stack_size);
            arena->reservation    = this;
            arena->last_error.val = memory_error_e_success;

            // add the arena to the reservation
            arena_t* next = arenas;
            if (next) next->prev = arena;
            arena->next = next;
            arenas = arena;
        }
        return(arena);
    }
};
#pragma once

#include "sld-memory.hpp"


namespace sld {

    SLD_API bool 
    reservation_t::is_valid(
        void) {

        bool is_valid = true;
        is_valid &= (_start         != NULL);
        is_valid &= (_size_reserved != 0);
        is_valid &= (_size_arena    != 0);

        _last_error.val = (is_valid) 
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
        const u64 size_min_arena) {

        bool can_reserve = true;
        can_reserve &= (size_min_reservation != 0);
        can_reserve &= (size_min_arena       != 0);        
        if (!can_reserve) return(false);

        // align and reserve memory
        const u64 size_reservation  = os_memory_align_to_granularity (size_min_reservation);  
        const u64 size_arena        = os_memory_align_to_page        (size_min_arena);  
        addr      reservation_start = (addr)os_memory_reserve        (NULL,size_reservation);
        if (reservation_start == 0) return(false);

        // initialize the reservation 
        // and add it to the list
        _start             = reservation_start;
        _size_reserved     = size_reservation;
        _size_arena        = size_arena; 
        _last_error.val    = memory_error_e_success;
        _arenas            = NULL;
        return(true);
   }
    
    SLD_API bool
    reservation_t::release_system_memory(
        void) {

        assert_valid();

        // attempt to release the os memory 
        const bool is_released = os_memory_release((void*)_start, _size_reserved);
        
        if (!is_released) {
            _last_error.val = memory_error_e_os_failed_to_release;
            return(is_released);
        }

        _arenas         = NULL;
        _size_reserved  = 0;
        _size_arena     = 0;
        _last_error.val = memory_error_e_success;
        return(is_released);
    }

    SLD_API bool
    reservation_t::reset(
        void) {

        assert_valid();
            bool is_decommitted = true;

        for (
            arena_t* arena = _arenas;
            arena != NULL;
            arena = arena->_next) {

            is_decommitted &= arena->decommit(); 
        }

        _last_error.val = (is_decommitted)
            ? memory_error_e_critical
            : memory_error_e_success;

        _arenas = NULL;

        assert_valid();

        const bool result = (_last_error.val == memory_error_e_success);
        return(result);            
    }

    SLD_API u64
    reservation_t::size_committed(
        void) {
        
        assert_valid();


        const u64 arena_size     = _size_arena; 
        u64       size_committed = 0;
        for (
            arena_t* arena = _arenas;
            arena != NULL;
            arena = arena->_next) {
            
            size_committed += arena_size;
        }
        
        _last_error.val = memory_error_e_success;
        return(size_committed);
    }

    SLD_API u64
    reservation_t::size_decommitted(
        void) {

        assert_valid();
        u64 commit_size      = size_committed();
        u64 size_decommitted = (_size_reserved - commit_size);
        
        return(size_decommitted);
    }

    SLD_API arena_t*
    reservation_t::commit_arena(
        void) {

        assert_valid();

        // find the next free arena
        const u32 arena_count_max = (_size_reserved / _size_arena);
        void*     commit_start    = NULL;
        for (
            u32 arena_index = 0;
            arena_index < arena_count_max;
            ++arena_index) {

            const u32  arena_offset  = (arena_index * _size_arena);
            void*      arena_tmp     = (void*)(_start + arena_offset); 
            const bool arena_is_free = os_memory_is_reserved(arena_tmp);
            
            if (arena_is_free) {
                commit_start = arena_tmp;
                break;
            } 
        }

        // attempt to commit memory
        void* arena_memory = os_memory_commit(commit_start, _size_arena);
        if (!arena_memory) return(NULL);

        constexpr u32 arena_struct_size  = sizeof(arena_t);

        arena_t* next          = _arenas;
        arena_t* arena         = (arena_t*)arena_memory;
        arena->_array          = (byte*)((addr)arena_memory + arena_struct_size);
        arena->_capacity       = _size_arena        - arena_struct_size; 
        arena->_reservation    = this;
        arena->_position       = 0;
        arena->_save           = 0;
        arena->_next           = next;
        arena->_prev           = NULL; 
        arena->_last_error.val = memory_error_e_success;
        
        if (next) next->_prev = arena;
        _arenas = arena;

        return(arena);
    }
};
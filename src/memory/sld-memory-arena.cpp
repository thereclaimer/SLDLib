#pragma once

#include "sld-memory.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API bool
    arena_t::is_valid(
        void) const {

        bool is_valid = true;
        is_valid &= _reservation->is_valid();
        is_valid &= (_array    != NULL); 
        is_valid &= (_capacity != 0); 
        is_valid &= (_position <  _capacity); 
        is_valid &= (_save     <= _position); 
        return(is_valid);
    }

    SLD_API void
    arena_t::assert_valid(
        void) const {
        
        assert(is_valid());
    }

    SLD_API bool
    arena_t::decommit(
        void) {

        assert_valid();

        reservation_t* res          = _reservation;
        const arena_t* first_arena  = _reservation->get_first_arena();
        const bool     is_first     = (this == first_arena); 
        arena_t*       tmp_next     = _next;
        arena_t*       tmp_prev     = _prev;
        void*          arena_memory = (void*)this;

        // attempt to decommit the memory
        const bool is_decommitted = os_memory_decommit(arena_memory, res->get_arena_size());
        if (!is_decommitted) {
            _last_error.val = memory_error_e_os_failed_to_decommit;
            return(is_decommitted);
        }

        // remove the arena from the list
        if (tmp_next) tmp_next->_prev      = tmp_prev;
        if (tmp_prev) tmp_prev->_next      = tmp_next;
        if (is_first) res->_arenas         = tmp_next;
        return(is_decommitted);
    }

    SLD_API stack_allocator_t*
    arena_t::push_stack_allocator(
        const u32 size,
        const u32 granularity) {

        // calculate the push size
        constexpr u32 size_struct       = sizeof(stack_allocator_t);
        const     u32 size_gran_aligned = size_round_up_pow2(granularity);
        const     u32 size_mem_aligned  = size_align_pow_2  (size, granularity);
        const     u32 size_push         = size_struct + size_mem_aligned;

        // initialize allocator
        const void*        memory    = push_bytes           (size_push, size_gran_aligned);
        stack_allocator_t* allocator = stack_allocator_init (memory, size_push, size_gran_aligned);
        return(allocator);        
    }
};
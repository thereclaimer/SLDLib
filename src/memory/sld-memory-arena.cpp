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
        is_valid &= reservation->is_valid();
        is_valid &= stack->is_valid();
        is_valid &= last_error.val != memory_error_e_critical;
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

        reservation_t* res          = this->reservation;
        const arena_t* first_arena  = res->arenas;
        const bool     is_first     = (this == first_arena); 
        arena_t*       tmp_next     = this->next;
        arena_t*       tmp_prev     = this->prev;
        void*          arena_memory = (void*)this;

        // attempt to decommit the memory
        const bool is_decommitted = os_memory_decommit(arena_memory, res->size.arena_memory);
        if (!is_decommitted) {
            last_error.val = memory_error_e_os_failed_to_decommit;
            return(is_decommitted);
        }

        // remove the arena from the list
        if (tmp_next) tmp_next->prev      = tmp_prev;
        if (tmp_prev) tmp_prev->next      = tmp_next;
        if (is_first) res->arenas         = tmp_next;
        return(is_decommitted);
    }

    SLD_API byte*
    arena_t::push_bytes(
        const u64 size,
        const u64 alignment) {

        assert_valid();

        // align the size
        const u64 size_aligned = size_is_pow_2(alignment)
            ? size_align_pow_2 (size, alignment)
            : size_align       (size, alignment);

        // do the push
        byte* bytes = stack->push(size_aligned);
        if (bytes == NULL) {
            last_error.val = memory_error_e_arena_not_enough_memory;
            return(bytes);
        }

        last_error.val = memory_error_e_success;
        return(bytes);
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

    SLD_API bool
    arena_t::pull_bytes(
        const u64 size,
        const u64 alignment) {

        assert_valid();

        // align the size
        const u64 size_aligned = size_is_pow_2(alignment)
            ? size_align_pow_2 (size, alignment)
            : size_align       (size, alignment);

        // do the pull
        const bool is_pulled = stack->pull(size_aligned);
        last_error.val = (is_pulled)
            ? memory_error_e_success 
            : memory_error_e_arena_not_enough_memory;
        return(is_pulled);
    }

    SLD_API void
    arena_t::reset(
        void) {

        assert_valid();

        stack->reset();
        last_error.val = memory_error_e_success;
    }

    SLD_API void
    arena_t::roll_back(
        void) {

        assert_valid();

        stack->reset_to_save();
        last_error.val = memory_error_e_success;
    }

    SLD_API void
    arena_t::save_position(
        void) {
 
        assert_valid();
        stack->reset_to_save();
        last_error.val = memory_error_e_success;
    }

    SLD_API u64
    arena_t::size_total(
        void) const {

        assert_valid();
        return(stack->size);
    }

    SLD_API u64
    arena_t::size_free(
        void) const {

        assert_valid();

        const u64 size_free = (stack->size - stack->position);
        return(size_free);
    }

    SLD_API u64
    arena_t::size_used(
        void) const {

        assert_valid();
        return(stack->position);
    }
};
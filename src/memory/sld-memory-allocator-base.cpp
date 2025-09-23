#pragma once

#include "sld-memory.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // PUBLIC
    //-------------------------------------------------------------------

    allocator_base_t::allocator_base_t (
        const void* memory,
        const u32   size) {

        _start            = (addr)memory;
        _size             = size;
        _free_allocations = NULL;
        _used_allocations = NULL;

        assert(_start != 0 && _start != 0);
    }

    allocator_base_t::allocator_base_t(
        arena_t*  arena,
        const u32 size) {

        _start = (addr)arena_push_bytes(arena, _size, alignof(allocator_base_t));
        _size  = _size;

        assert(_start != 0 && _start != 0);
    }

    void* alloc_abs         (const u32   size = 0, const u32 alignment = 0);
    u32   alloc_rel         (const u32   size = 0, const u32 alignment = 0);
    void  free_abs          (const void* pointer);
    void  free_rel          (const u32   handle);
    void* alloc_get_pointer (const void* memory);
    void* alloc_get_handle  (const void* memory);

    //-------------------------------------------------------------------
    // PRIVATE
    //-------------------------------------------------------------------

};
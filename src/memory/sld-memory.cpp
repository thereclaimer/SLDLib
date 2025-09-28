#pragma once

#include "sld-memory.hpp"
#include "sld-memory-internal.cpp"
#include "sld-memory-reservation.cpp"
#include "sld-memory-arena.cpp"
#include "sld-memory-block-allocator.cpp"
#include "sld-memory-stack-allocator.cpp"

namespace sld {

    SLD_API void
    memory_zero(
        byte*     start,
        const u64 size) {

        if (start == NULL || size == 0) return;

        for (
            u64 index = 0;
            index < size;
            ++index) {

            start[index] = 0;
        }
    }

    SLD_API void
    memory_copy(
        byte*       start_dst,
        const byte* start_src,
        const u64   size) {

        if (start_dst == NULL || start_src == NULL || size == 0) {
            return;
        }

        for (
            u64 index = 0;
            index < size;
            ++index) {

            start_dst[index] = start_src[index]; 
        }
    }

    SLD_API byte*
    memory_advance(
        const byte* start,
        const u64   size,
        const u64   stride,
        u64&        offset) {

        const u32 new_offset = (offset + stride);

        if (
            start  == NULL ||
            size   == 0    ||
            stride == 0    ||
            offset > size) {

            return(NULL);
        }

        byte* result = (byte*)(&start[offset]);
        offset = new_offset;
        return(result);
    }
};
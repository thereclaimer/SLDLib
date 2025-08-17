#pragma once

#include "sld-memory.hpp"

#include "sld-memory-manager.cpp"
#include "sld-memory-reservation.cpp"
#include "sld-memory-arena.cpp"

namespace sld {

    bool
    memory_validate(
        const memory_t& memory) {

        const bool is_valid = (
            memory.start != 0 &&
            memory.size  != 0
        );

        return(is_valid);
    }

    bool
    memory_zero(
        memory_t& memory) {

        bool result = memory_validate(memory);
        if (result) {

            byte* data = (byte*)memory.start;

            for (
                u64 index = 0;
                index < memory.size;
                ++index) {

                data[index] = 0;
            }
        }

        return(result);
    }

    u64
    memory_copy(
        const memory_t& memory_src,
        memory_t&       memory_dst) {

        bool result = true;
        result &= memory_validate(memory_src);
        result &= memory_validate(memory_dst);

        u64 size = 0;        
        if (result) {

            size = (memory_src.size < memory_dst.size)
                ? memory_src.size
                : memory_dst.size;

            const byte* data_src = (byte*)memory_src.start;
            byte*       data_dst = (byte*)memory_dst.start;

            for (
                u64 index = 0;
                index < size;
                ++index) {

                data_dst[index] = data_src[index]; 
            }
        }

        return(size);
    }

    addr
    memory_advance(
        const memory_t& memory,
        const u32       stride,
        u32&            offset) {

        const u32 new_offset = (offset + stride);

        bool result = true;
        result &= memory_validate(memory);
        result &= (stride != 0);
        result &= (new_offset < memory.size);

        addr address = 0;

        if (result) {

            offset  = new_offset;
            address = memory.start + offset; 
        }

        return(address);
    }
};
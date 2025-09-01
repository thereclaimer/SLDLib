#pragma once

#include "sld-memory-internal.cpp"
#include "sld-memory-stack.cpp"
#include "sld-memory-reservation.cpp"

namespace sld {

    SLD_API bool
    memory_validate(
        const memory_t& memory) {

        const bool is_valid = (
            memory.start != 0 &&
            memory.size  != 0
        );

        return(is_valid);
    }

    SLD_API bool
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

    SLD_API bool
    memory_copy(
        byte*       memory_dst,
        const byte* memory_src,
        const u64   size) {

        bool result = true;
        result &= (memory_dst != NULL);
        result &= (memory_src != NULL);
        result &= (size       != 0);

        if (result) {

            for (
                u64 index = 0;
                index < size;
                ++index) {

                memory_dst[index] = memory_src[index]; 
            }
        }

        return(result);
    }

    SLD_API addr
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

    SLD_API const memory_error_t
    memory_get_last_error(
        void) {

        const memory_error_t error = memory_last_error_instance();
        return(error);
    }
};
#pragma once

#include "sld-buffer.hpp"

namespace sld {

    bool
    buffer_validate(
        const buffer_t& buffer) {

        bool is_valid = true;
        is_valid &= (buffer.data   != NULL);
        is_valid &= (buffer.size   != 0);
        is_valid &= (buffer.length <= buffer.size);
        return(is_valid);
    }   

    bool
    buffer_reset(
        buffer_t& buffer) {

        const bool result = buffer_validate(buffer);
        if (result) {
            buffer.length = 0;
        }
        return(result);
    }

    bool
    buffer_zero(
        buffer_t& buffer) {

        const bool result = buffer_validate(buffer);

        if (result) {

            buffer.length = 0;

            for (
                u32 index = 0;
                index < buffer.size;
                ++index) {

                buffer.data[index] = 0;
            }
        }

        return(result);        
    }

    u32
    buffer_append(
        buffer_t&   buffer,
        const byte* src_data,
        const u32   src_length) {

        u32 bytes_appended = 0;

        bool result = true;
        result &= buffer_validate(buffer);
        result &= (src_data   != NULL);
        result &= (src_length != 0);
        if (!result) return(bytes_appended);

        for (
            bytes_appended = 0;
            (
                bytes_appended < src_length &&
                buffer.length  < buffer.size
            );
            ++bytes_appended) {

            buffer.data[buffer.length] = src_data[bytes_appended];
            ++buffer.length;
        }

        return(bytes_appended);
    }

    u32
    buffer_copy(
        buffer_t&   buffer,
        const byte* src_data,
        const u32   src_length) {

        bool result = true;
        result &= buffer_validate(buffer);
        result &= (src_data   != NULL);
        result &= (src_length != 0);
        if (!result) return(0);

        const u32 bytes_to_copy = (src_length < buffer.size)
            ? src_length
            : buffer.size;

        for (
            buffer.length = 0;
            buffer.length < bytes_to_copy;
            ++buffer.length) {

            buffer.data[buffer.length] = src_data[buffer.length];
        }

        return(buffer.length);
    }
};
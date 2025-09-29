#pragma once

#include "sld-string.hpp"

namespace sld {

    bool
    cstr_validate(
        const cstr_t& string) {

        bool result = true;
        result &= (string.size  != 0);
        result &= (string.chars != NULL);
        if (result) {
            // make sure the string is terminated
            string.chars[string.size - 1] = 0;
        }
        return(result);
    }
    bool
    cstr_is_empty(
        const cstr_t& string) {

        const bool is_empty = cstr_validate(string)
            ? string.chars[0] == 0
            : true;

        return(is_empty);
    }

    bool
    cstr_reset(
        cstr_t& string) {

        const bool result = cstr_validate(string);

        if (result) {

            string.chars[0]               = 0;
            string.chars[string.size - 1] = 0;
        }

        return(result);
    }

    bool
    cstr_zero(
        cstr_t& string) {

        const bool result = cstr_validate(string);

        if (result) {

            for (
                u32 index = 0;
                index < string.size;
                ++index) {

                string.chars[index] = 0;
            }
        }

        return(result);
    }

    u32
    cstr_length(
        cstr_t& string) {

        const bool result = cstr_validate(string);
        u32 length = 0;

        if (result) {

            for (
                length;
                length < string.size;
                ++length) {

                if (string.chars[length] == 0) break;
            }
        }

        return(length);
    }

    u32
    cstr_copy(
        cstr_t&       str_dst,
        const cstr_t& str_src) {

        bool result = true;
        result &= cstr_validate(str_dst);
        result &= cstr_validate(str_src);

        u32 src_size_copied = 0;
        if (result) {

            // calculate how much we can copy
            const u32 src_size_to_copy = (str_src.size <  str_dst.size)
                ? str_src.size
                : str_dst.size;

            // copy till we hit the size we can copy
            // or until we hit a null terminator
            for (
                u32& char_index = src_size_copied;
                     char_index < src_size_to_copy;
                   ++char_index) {

                const cchar src_char      = str_src.chars[char_index];
                str_dst.chars[char_index] = src_char;
                if (src_char == 0) break;
            }

            // ensure the string is terminated
            str_dst.chars[str_dst.size - 1] = 0;
        }
        return(src_size_copied);
    }

    u32
    cstr_append(
        cstr_t&       str_dst,
        const cstr_t& str_src) {

        bool result = true;
        result &= cstr_validate(str_dst);
        result &= cstr_validate(str_src);

        u32 bytes_appended = 0;
        if (result) {

            // get the length of the destination string
            u32 str_dst_length = 0;
            for (
                  u32& char_index = str_dst_length;
                       char_index < str_dst.size;
                     ++char_index) {

                if (str_dst.chars[char_index] == 0) break;
            }

            // calculate how many bytes we can append at max
            // and where we are starting to append to
            // based on if/where the null terminator exists
            const u32 str_dst_length_remaining = (str_dst.size - str_dst_length);
            const u32 max_bytes_to_append      = (str_dst_length_remaining < str_dst.size) ? str_dst_length_remaining : str_dst.size;   
            const u32 dst_char_index_start     = (str_dst.chars[str_dst_length - 1] == 0)  ? (str_dst_length - 1)     : str_dst_length;

            // append until we hit a null terminator in the source
            // or we hit max length
            for (
                u32& src_char_index = bytes_appended;
                     src_char_index < str_src.size;
                   ++src_char_index) {

                const cchar src_char       = str_src.chars[src_char_index];
                const u32   dst_char_index = dst_char_index_start + src_char_index;

                str_dst.chars[dst_char_index] = src_char;
                if (src_char == 0) break;
            }

            str_dst.chars[str_dst.size - 1] = 0;
        }
        return(bytes_appended);
    }
};
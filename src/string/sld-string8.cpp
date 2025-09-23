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
        cstr_t& string,
        const c8*  src_chars,
        const u32  src_length) {

        bool result = true;
        result &= cstr_validate(string);
        result &= (src_chars  != NULL);
        result &= (src_length != 0);

        u32 bytes_copied = 0;

        if (result) {

            for (
                bytes_copied;
                (
                    bytes_copied < src_length &&
                    bytes_copied < string.size 
                );
                ++bytes_copied) {

                const cchar c = src_chars[bytes_copied];
                string.chars[bytes_copied] = c;
                if (c == 0) break;
            }

            string.chars[string.size - 1] = 0;
        }

        return(bytes_copied);
    }

    u32
    cstr_append(
        cstr_t& string,
        const c8*  src_chars,
        const u32  src_length) {

        const bool result = cstr_validate(string);

        u32 bytes_appended = 0;

        if (result) {

            u32 string_index = 0;
            for (
                  string_index;
                  string_index < string.size;
                ++string_index) {

                if (string.chars[string_index] == 0) break;
            }

            for (
                bytes_appended;
                (
                    bytes_appended < src_length &&
                    string_index   < string.size 
                );
                ++bytes_appended) {

                string.chars[string_index] = src_chars[bytes_appended];
                ++string_index;
            }

            string.chars[string.size - 1] = 0;
        }

        return(bytes_appended);
    }
};
#pragma once

#include "sld-string.hpp"

namespace sld {


    bool
    cstr_is_null_or_empty(
        const cstr_t& cstr) {

        bool is_null_or_empty = (cstr.data == NULL) || (cstr.data[0] == 0);
        is_null_or_empty     &= (cstr.size == 0);
        return(is_null_or_empty);
    }

    bool
    cstr_clear(
        const cstr_t& cstr) {

        if (cstr_is_null_or_empty(cstr)) {
            return(false);
        }

        cstr.data[0] = 0;
    }

    u32
    cstr_length(
        const cstr_t& cstr){

        u32 length = 0;
        if (cstr_is_null_or_empty(cstr)) {
            return(length);
        }

        for (
            length;
            length < cstr.size;
            ++length) {

            const c8 c = cstr.buffer[length];
            if (c == 0) break;
        }

        return(length); 
    }

    bool
    cstr_equals(
        const cstr_t& cstr_a,
        const cstr_t& cstr_b) {

        bool result = true;
        result &= !cstr_is_null_or_empty(cstr_a);
        result &= !cstr_is_null_or_empty(cstr_b);
        if (!result) return(false);

        const u32 size = (cstr_a.size <= cstr_b.size)
            ? cstr_a.size
            : cstr_b.size;
        
        for (
            u32 index = 0;
            (index < size) && result;
            ++index) {

            const c8 a = cstr_a.data[index];
            const c8 b = cstr_b.data[index];
            result &= (a == b);        
        }

        return(result);
    }

    u32
    cstr_copy(
        const cstr_t& cstr_from,
        cstr_t&       cstr_to) {

        bool result = true;
        result &= !cstr_is_null_or_empty(cstr_from);
        result &= !cstr_is_null_or_empty(cstr_to);
        if (!result) return(0);

        const u32 size = (cstr_from.size <= cstr_to.size)
            ? cstr_from.size
            : cstr_to.size;

        u32 bytes_copied = 0;
        for (
            bytes_copied;
            bytes_copied < size;
            ++bytes_copied) {

            const c8 c = cstr_from.data[bytes_copied];
            cstr_to.data[bytes_copied] = c;
            if (c == 0) break;
        }

        return(length);
    }

    u32
    cstr_append(
        const cstr_t& cstr_from,
        cstr_t&       cstr_to) {

        bool result = true;
        result &= !cstr_is_null_or_empty(cstr_from);
        result &= !cstr_is_null_or_empty(cstr_to);
        if (!result) return(0);

        const u32 cstr_to_length = cstr_length(to);
        c8* cstr_to_start        = &cstr_to.buffer[cstr_to_length];
        u32 bytes_appended       = 0;

        const u32 size = (cstr_from.size <= cstr_to.size)
            ? cstr_from.size
            : cstr_to.size;

        for (
            bytes_appended = 0;
            bytes_appended < size;
            ++bytes_appended) {

            const c8 c = from.buffer[bytes_appended];
            cstr_to_start[bytes_appended] = c;
            if (c == 0) break;
        }

        return(bytes_appended);
    }

    bool
    cstr_alloc(
        cstr_memory_t* cstr_mem,
        cstr_t&        cstr) {

        bool result = false;

        c8* cstr_buffer = (c8*)cstr_mem->start;

        for (
            u32 offset = 0;
            (offset < cstr_mem->size) && !result;
            offset += cstr_mem->stride) {

            const c8 cstr_start = cstr_buffer[offset];
            result = (cstr_start == cstr_mem->separator);
            if (result) {

                cstr.data = &cstr_buffer[offset];
                cstr.size = cstr_mem->stride; 
            }
        }

        return(result);
    }

    bool
    cstr_free(
        cstr_memory_t* cstr_mem,
        cstr_t&        cstr) {

        // calculate offset
        const addr cstr_mem_start = cstr_mem->start;
        const addr cstr_mem_end   = cstr_mem_start + cstr_mem->size; 
        const addr cstr_start     = (addr)cstr.data;
        const u32  cstr_offset    = cstr_start - cstr_mem_start; 
    
        // we can free if: 
        // - the string is within the memory range
        // - the string offset is a multiple of the stride        
        bool can_free = (cstr_start < cstr_mem_end);
        can_free     &= (cstr_offset % cstr_mem->stride == 0);   
        if (!can_free) return(can_free);

        // set the start of the string to the separator
        cstr_mem->start[cstr_offset] = cstr_mem->separator;

        // clear the string and return
        cstr.data = NULL;
        cstr.size = 0;
        return(true);
    }

    cstr_memory_t*
    cstr_memory_init(
        const memory_t& memory,
        const u32       cstr_size,
        const c8        cstr_separator) {

        // check args
        bool can_init = (memory.start   != 0);
        can_init     &= (cstr_size      != 0);
        can_init     &= (cstr_separator != 0);
        if (!can_init) return(NULL);

        // cast pointer
        cstr_memory_t* cstr_mem = (cstr_memory_t*)memory.start;

        // set properties
        const u32 struct_size = sizeof(cstr_memory_t);
        cstr_mem->start     = memory.start + struct_size;
        cstr_mem->size      = memory.size  - struct_size;
        cstr_mem->stride    = cstr_size;
        cstr_mem->separator = cstr_separator;

        // set the separators at the start of each string
        c8* cstr_buffer = (c8*)cstr_mem->start;
        for (
            u32 offset = 0;
            offset <  cstr_mem->size;
            offset += cstr_mem->stride) {

            cstr_buffer[offset] = cstr_mem->separator;
        }
    }
    
    bool
    cstr_memory_reset(
        cstr_memory_t* cstr_mem) {

        if (!cstr_mem) return(false);

        // set the separators at the start of each string
        c8* cstr_buffer = (c8*)cstr_mem->start;
        for (
            u32 offset = 0;
            offset <  cstr_mem->size;
            offset += cstr_mem->stride) {

            cstr_buffer[offset] = cstr_mem->separator;
        }
        return(true);
    }

};
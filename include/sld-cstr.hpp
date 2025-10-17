#ifndef SLD_CSTR_HPP
#define SLD_CSTR_HPP

#include "sld.hpp"
#include "sld-arena.hpp"

namespace sld {
    
    //-------------------------------------------------------------------
    // CSTR API  | C-STRING | ASCII | UTF-8
    //-------------------------------------------------------------------

    struct cstr_t {
        cchar* chars;
        u64    size;
    };

    constexpr u32   CSTR_HEADER_SIZE     = sizeof(cstr_t);
    constexpr cchar CSTR_NULL_TERMINATOR = 0;

    SLD_API_INLINE cstr_t* cstr_memory_init  (memory_t& memory);
    SLD_API_INLINE cstr_t* cstr_arena_alloc  (arena_t* arena, const u64 size);
    SLD_API_INLINE bool    cstr_is_valid     (const cstr_t* cstr);
    SLD_API_INLINE bool    cstr_is_empty     (const cstr_t* cstr);
    SLD_API_INLINE void    cstr_assert_valid (const cstr_t* cstr);
    SLD_API_INLINE u64     cstr_get_length   (const cstr_t* cstr);
    SLD_API_INLINE u64     cstr_copy_to      (const cstr_t* cstr, cchar* dst_chars,  const u64 dst_size);
    SLD_API_INLINE void    cstr_reset        (cstr_t* cstr);
    SLD_API_INLINE void    cstr_zero         (cstr_t* cstr);
    SLD_API_INLINE void    cstr_terminate    (cstr_t* cstr);
    SLD_API_INLINE u64     cstr_copy_from    (cstr_t* cstr, const cchar* src_chars,  const u64 src_size);
    SLD_API_INLINE u64     cstr_append       (cstr_t* cstr, const cchar* src_chars,  const u64 src_size);

    //-------------------------------------------------------------------
    // CSTR INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE cstr_t*
    cstr_memory_init(
        memory_t& memory) {

        const bool can_init = memory_is_valid(memory) && (memory.size > CSTR_HEADER_SIZE); 
        assert(can_init);

        cstr_t* cstr = (cstr_t*)memory.ptr;
        cstr->chars  = (cchar*)(memory.addr + CSTR_HEADER_SIZE);  
        cstr->size   = (memory.size - CSTR_HEADER_SIZE);
        cstr->chars[cstr->size - 1] = CSTR_NULL_TERMINATOR;
        cstr->chars[0]              = CSTR_NULL_TERMINATOR;
        cstr_assert_valid(cstr);

        return(cstr);
    }

    SLD_API_INLINE cstr_t*
    cstr_arena_alloc(
        arena_t*  arena,
        const u64 size) {

        memory_t cstr_memory;
        cstr_memory.size = (size + CSTR_HEADER_SIZE);
        cstr_memory.ptr  = arena_push_bytes(arena, cstr_memory.size);
        cstr_t* cstr     = cstr_memory_init(cstr_memory);
        return(cstr);
    }

    SLD_API_INLINE bool
    cstr_is_valid(
        const cstr_t* cstr) {

        bool is_valid = (cstr != NULL);
        if (is_valid) {
            is_valid &= (cstr->chars                 != NULL);
            is_valid &= (cstr->size                  != 0);
            is_valid &= (cstr->chars[cstr->size - 1] == CSTR_NULL_TERMINATOR);
        }
        return(is_valid);
    }


    SLD_API_INLINE void
    cstr_assert_valid(
        const cstr_t* cstr) {

        assert(cstr_is_valid(cstr));
    }

    SLD_API_INLINE bool
    cstr_is_empty(
        const cstr_t* cstr) {

        cstr_assert_valid(cstr);

        const bool is_empty = (cstr->chars[0] == CSTR_NULL_TERMINATOR);
        return(is_empty);        
    }

    SLD_API_INLINE u64
    cstr_get_length(
        const cstr_t* cstr) {

        cstr_assert_valid(cstr);

        u32 length = 0;
        for (
              length = 0;
              length < cstr->size;
            ++length) {

            if (cstr->chars[length] == 0) break;
        }

        assert(length <= cstr->size);
        return(length);
    }

    SLD_API_INLINE void
    cstr_reset(
        cstr_t* cstr) {

        cstr_assert_valid(cstr);

        cstr->chars[0] = CSTR_NULL_TERMINATOR;
        cstr_terminate(cstr);
    }

    SLD_API_INLINE void
    cstr_zero(
        cstr_t* cstr) {

        cstr_assert_valid(cstr);

        memset(cstr->chars, 0, cstr->size);
    }

    SLD_API_INLINE void
    cstr_terminate(
        cstr_t* cstr) {

        bool can_terminate = (cstr != NULL);
        if (can_terminate) {
            can_terminate &= (cstr->chars != NULL);
            can_terminate &= (cstr->size  != 0);
        }
        assert(can_terminate);
        cstr->chars[cstr->size - 1] = CSTR_NULL_TERMINATOR;
    }

    SLD_API_INLINE u64
    cstr_copy_to(
        const cstr_t* cstr,
        cchar*        dst_chars,
        const u64     dst_size) {

        bool is_valid = cstr_is_valid(cstr);
        is_valid &= (dst_chars != NULL);
        is_valid &= (dst_size  != 0);

        const u32 length = (dst_size > cstr->size)
            ? cstr->size
            : dst_size;

        const errno_t memmove_error = memmove_s(dst_chars, dst_size, cstr->chars, cstr->size);
        assert(memmove_error == 0);
        return(length);
    }

    SLD_API_INLINE u64
    cstr_copy_from(
        cstr_t*      cstr,
        const cchar* src_chars,
        const u64    src_size) {

        bool is_valid = cstr_is_valid(cstr);
        is_valid &= (src_chars != NULL);
        is_valid &= (src_size  != 0);

        const u32 length = (src_size > cstr->size)
            ? cstr->size
            : src_size;

        const errno_t memmove_error = memmove_s(cstr->chars, cstr->size, src_chars, src_size);
        assert(memmove_error == 0);
        cstr_terminate(cstr);
        return(length);
    }

    SLD_API_INLINE u64
    cstr_append(
        cstr_t*      cstr,
        const cchar* src_chars,
        const u64    src_size) {

        bool is_valid = cstr_is_valid(cstr);
        is_valid &= (src_chars != NULL);
        is_valid &= (src_size  != 0);
        assert(is_valid);

        u64 dst_length = cstr_get_length(cstr);
        if (dst_length == cstr->size) return(0);

        const u64 dst_length_remaining = (cstr->size - dst_length);
        const u64 append_length        = (dst_length_remaining < src_size)
            ? dst_length_remaining
            : src_size;

        cchar* dst_chars = &cstr->chars[dst_length - 1];

        const errno_t memmove_error = memmove_s(dst_chars, dst_length_remaining, src_chars, src_size);
        assert(memmove_error == 0);
        cstr_terminate(cstr);
        return(append_length);
    }
};

#endif //SLD_CSTR_HPP
#ifndef SLD_STRING_HPP
#define SLD_STRING_HPP

#include "sld.hpp"
#include "sld-memory.hpp"
#include "sld-arena.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // WSTR API | W-STRING | UNICODE | UTF-16
    //-------------------------------------------------------------------

    struct wstr_t {
        wchar* chars;
        u64    size;
    };

    constexpr u32   WSTR_HEADER_SIZE     = sizeof(wstr_t);
    constexpr wchar WSTR_NULL_TERMINATOR = 0;

    SLD_API_INLINE wstr_t* wstr_memory_init  (memory_t& memory);
    SLD_API_INLINE wstr_t* wstr_arena_alloc  (arena_t* arena, const u64 size);
    SLD_API_INLINE bool    wstr_is_valid     (const wstr_t* wstr);
    SLD_API_INLINE bool    wstr_is_empty     (const wstr_t* wstr);
    SLD_API_INLINE void    wstr_assert_valid (const wstr_t* wstr);
    SLD_API_INLINE u64     wstr_get_length   (const wstr_t* wstr);
    SLD_API_INLINE u64     wstr_copy_to      (const wstr_t* wstr, wchar* dst_chars,  const u64 dst_size);
    SLD_API_INLINE void    wstr_reset        (wstr_t* wstr);
    SLD_API_INLINE void    wstr_zero         (wstr_t* wstr);
    SLD_API_INLINE void    wstr_terminate    (wstr_t* wstr);
    SLD_API_INLINE u64     wstr_copy_from    (wstr_t* wstr, const wchar* src_chars,  const u64 src_size);
    SLD_API_INLINE u64     wstr_append       (wstr_t* wstr, const wchar* src_chars,  const u64 src_size);

    //-------------------------------------------------------------------
    // WSTR INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE wstr_t*
    wstr_memory_init(
        memory_t& memory) {

        const bool can_init = memory_is_valid(memory) && (memory.size > WSTR_HEADER_SIZE); 
        assert(can_init);

        wstr_t* wstr = (wstr_t*)memory.ptr;
        wstr->chars  = (wchar*)(memory.addr + WSTR_HEADER_SIZE);  
        wstr->size   = (memory.size - WSTR_HEADER_SIZE);
        wstr->chars[wstr->size - 1] = WSTR_NULL_TERMINATOR;
        wstr->chars[0]              = WSTR_NULL_TERMINATOR;
        wstr_assert_valid(wstr);

        return(wstr);
    }

    SLD_API_INLINE wstr_t*
    wstr_arena_alloc(
        arena_t*  arena,
        const u64 size) {

        memory_t wstr_memory;
        wstr_memory.size = (size + WSTR_HEADER_SIZE);
        wstr_memory.ptr  = arena_push_bytes(arena, wstr_memory.size);
        wstr_t* wstr     = wstr_memory_init(wstr_memory);
        return(wstr);
    }

    SLD_API_INLINE bool
    wstr_is_valid(
        const wstr_t* wstr) {

        bool is_valid = (wstr != NULL);
        if (is_valid) {
            is_valid &= (wstr->chars                 != NULL);
            is_valid &= (wstr->size                  != 0);
            is_valid &= (wstr->chars[wstr->size - 1] == WSTR_NULL_TERMINATOR);
        }
        return(is_valid);
    }


    SLD_API_INLINE void
    wstr_assert_valid(
        const wstr_t* wstr) {

        assert(wstr_is_valid(wstr));
    }

    SLD_API_INLINE bool
    wstr_is_empty(
        const wstr_t* wstr) {

        wstr_assert_valid(wstr);

        const bool is_empty = (wstr->chars[0] == WSTR_NULL_TERMINATOR);
        return(is_empty);        
    }

    SLD_API_INLINE u64
    wstr_get_length(
        const wstr_t* wstr) {

        wstr_assert_valid(wstr);

        u32 length = 0;
        for (
              length = 0;
              length < wstr->size;
            ++length) {

            if (wstr->chars[length] == 0) break;
        }

        assert(length <= wstr->size);
        return(length);
    }

    SLD_API_INLINE void
    wstr_reset(
        wstr_t* wstr) {

        wstr_assert_valid(wstr);

        wstr->chars[0] = WSTR_NULL_TERMINATOR;
        wstr_terminate(wstr);
    }

    SLD_API_INLINE void
    wstr_zero(
        wstr_t* wstr) {

        wstr_assert_valid(wstr);

        memset(wstr->chars, 0, wstr->size);
    }

    SLD_API_INLINE void
    wstr_terminate(
        wstr_t* wstr) {

        bool can_terminate = (wstr != NULL);
        if (can_terminate) {
            can_terminate &= (wstr->chars != NULL);
            can_terminate &= (wstr->size  != 0);
        }
        assert(can_terminate);
        wstr->chars[wstr->size - 1] = WSTR_NULL_TERMINATOR;
    }

    SLD_API_INLINE u64
    wstr_copy_to(
        const wstr_t* wstr,
        wchar*        dst_chars,
        const u64     dst_size) {

        bool is_valid = wstr_is_valid(wstr);
        is_valid &= (dst_chars != NULL);
        is_valid &= (dst_size  != 0);

        const u32 length = (dst_size > wstr->size)
            ? wstr->size
            : dst_size;

        const errno_t memmove_error = memmove_s(dst_chars, dst_size, wstr->chars, wstr->size);
        assert(memmove_error == 0);
        return(length);
    }

    SLD_API_INLINE u64
    wstr_copy_from(
        wstr_t*      wstr,
        const wchar* src_chars,
        const u64    src_size) {

        bool is_valid = wstr_is_valid(wstr);
        is_valid &= (src_chars != NULL);
        is_valid &= (src_size  != 0);

        const u32 length = (src_size > wstr->size)
            ? wstr->size
            : src_size;

        const errno_t memmove_error = memmove_s(wstr->chars, wstr->size, src_chars, src_size);
        assert(memmove_error == 0);
        wstr_terminate(wstr);
        return(length);
    }

    SLD_API_INLINE u64
    wstr_append(
        wstr_t*      wstr,
        const wchar* src_chars,
        const u64    src_size) {

        bool is_valid = wstr_is_valid(wstr);
        is_valid &= (src_chars != NULL);
        is_valid &= (src_size  != 0);
        assert(is_valid);

        u64 dst_length = wstr_get_length(wstr);
        if (dst_length == wstr->size) return(0);

        const u64 dst_length_remaining = (wstr->size - dst_length);
        const u64 append_length        = (dst_length_remaining < src_size)
            ? dst_length_remaining
            : src_size;

        wchar* dst_chars = &wstr->chars[dst_length - 1];

        const errno_t memmove_error = memmove_s(dst_chars, dst_length_remaining, src_chars, src_size);
        assert(memmove_error == 0);
        wstr_terminate(wstr);
        return(append_length);
    }
};

#endif  //SLD_STRING_HPP
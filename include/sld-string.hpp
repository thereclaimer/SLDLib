#ifndef SLD_STRING_HPP
#define SLD_STRING_HPP

#include "sld.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    typedef c8  cchar;
    typedef c16 wchar;

    struct cstr_t { cchar* chars; u64 size; };
    struct wstr_t { wchar* chars; u64 size; };

    //-------------------------------------------------------------------
    // STRING 8 | C-STRING | ASCII | UTF-8
    //-------------------------------------------------------------------
    
    bool cstr_validate (const cstr_t& str);
    bool cstr_is_empty (const cstr_t& str);
    bool cstr_reset    (cstr_t&       str);
    bool cstr_zero     (cstr_t&       str);
    u32  cstr_length   (cstr_t&       str);
    u32  cstr_copy     (cstr_t&       str, const cchar* src_chars, const u32 src_length);
    u32  cstr_append   (cstr_t&       str, const cchar* src_chars, const u32 src_length);

    //-------------------------------------------------------------------
    // STRING 16 | W-STRING | UNICODE | UTF-16
    //-------------------------------------------------------------------

    bool wstr_validate (const wstr_t& str);
    bool wstr_is_empty (const wstr_t& str);
    bool wstr_reset    (wstr_t&       str);
    u32  wstr_length   (wstr_t&       str);
    u32  wstr_copy     (wstr_t&       str, const wchar* src_chars, const u32 src_length);
    u32  wstr_append   (wstr_t&       str, const wchar* src_chars, const u32 src_length);
};

#endif  //SLD_STRING_HPP
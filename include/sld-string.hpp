#ifndef SLD_STRING_HPP
#define SLD_STRING_HPP

#include "sld.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // STRING 8 | C-STRING | ASCII | UTF-8
    //-------------------------------------------------------------------

    struct str8_t {
        c8* chars;
        u32 size;
    };
    
    bool str8_validate (const str8_t& str);
    bool str8_is_empty (const str8_t& str);
    bool str8_reset    (str8_t&       str);
    bool str8_zero     (str8_t&       str);
    u32  str8_length   (str8_t&       str);
    u32  str8_copy     (str8_t&       str, const c8* src_chars, const u32 src_length);
    u32  str8_append   (str8_t&       str, const c8* src_chars, const u32 src_length);

    //-------------------------------------------------------------------
    // STRING 16 | W-STRING | UNICODE | UTF-16
    //-------------------------------------------------------------------

    struct str16_t {
        c16* chars;
        u32  size;
    };

    bool str16_validate (const str16_t& str);
    bool str16_is_empty (const str16_t& str);
    bool str16_reset    (str16_t&       str);
    u32  str16_length   (str16_t&       str);
    u32  str16_copy     (str16_t&       str, const c16* src_chars, const u32 src_length);
    u32  str16_append   (str16_t&       str, const c16* src_chars, const u32 src_length);

};

#endif  //SLD_STRING_HPP
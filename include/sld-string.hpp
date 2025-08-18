#ifndef SLD_STRING_HPP
#define SLD_STRING_HPP

#include "sld.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // STRING 8 | C-STRING | ASCII | UTF-8
    //-------------------------------------------------------------------

    struct string8_t {
        c8* chars;
        u32 size;
    };
    
    bool string8_validate (const string8_t& string);
    bool string8_is_empty (const string8_t& string);
    bool string8_reset    (string8_t&       string);
    bool string8_zero     (string8_t&       string);
    u32  string8_length   (string8_t&       string);
    u32  string8_copy     (string8_t&       string, const c8* src_chars, const u32 src_length);
    u32  string8_append   (string8_t&       string, const c8* src_chars, const u32 src_length);

    //-------------------------------------------------------------------
    // STRING 16 | W-STRING | UNICODE | UTF-16
    //-------------------------------------------------------------------

    struct string16_t {
        c16* chars;
        u32  size;
    };

    bool string16_validate (const string16_t& string);
    bool string16_is_empty (const string16_t& string);
    bool string16_reset    (string16_t&       string);
    u32  string16_length   (string16_t&       string);
    u32  string16_copy     (string16_t&       string, const c16* src_chars, const u32 src_length);
    u32  string16_append   (string16_t&       string, const c16* src_chars, const u32 src_length);

};

#endif  //SLD_STRING_HPP
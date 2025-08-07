#ifndef SLD_STRING_HPP
#define SLD_STRING_HPP

#include "sld.hpp"

namespace sld {

    struct string_c8_t {
        c8* chars;
        u32 size;
    };
    
    bool string_c8_validate (const string_c8_t& string);
    bool string_c8_is_empty (const string_c8_t& string);
    bool string_c8_reset    (string_c8_t&       string);
    u32  string_c8_length   (string_c8_t&       string);
    u32  string_c8_copy     (string_c8_t&       string, const c8* src_chars, const u32 src_length);
    u32  string_c8_append   (string_c8_t&       string, const c8* src_chars, const u32 src_length);

    struct string_c16_t {
        c16* chars;
        u32  size;
    };

    bool string_c16_validate (const string_c16_t& string);
    bool string_c16_is_empty (const string_c16_t& string);
    bool string_c16_reset    (string_c16_t&       string);
    u32  string_c16_length   (string_c16_t&       string);
    u32  string_c16_copy     (string_c16_t&       string, const c16* src_chars, const u32 src_length);
    u32  string_c16_append   (string_c16_t&       string, const c16* src_chars, const u32 src_length);

};

#endif  //SLD_STRING_HPP
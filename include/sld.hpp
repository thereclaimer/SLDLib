#ifndef SLD_HPP
#define SLD_HPP

#include <cstdint>

#define utility  constexpr static inline 
#define global   constexpr static 
#define internal static inline
#define local    static const

namespace sld {

    // signed integers
    typedef int8_t   s8;
    typedef int16_t  s16;
    typedef int32_t  s32;
    typedef int64_t  s64;

    // unsigned integers
    typedef uint8_t  u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    // floats
    typedef float    f32;
    typedef double   f64;
    
    // booleans
    typedef int8_t   b8;
    typedef int16_t  b16;
    typedef int32_t  b32;
    typedef int64_t  b64;

    // chars  
    typedef char      c8;
    typedef char16_t  c16;
    typedef char32_t  c32;

    // memory
    typedef uint8_t  byte;
    typedef intptr_t addr;

};

#define sld_size_kilobytes(n)                 (n * 1024)
#define sld_size_megabytes(n)                 (n * 1024 * 1024)
#define sld_size_gigabytes(n)                 (n * 1024 * 1024 * 1024)
#define sld_size_align(size, alignment)       ((a + b - 1) / b * b)
#define sld_size_align_pow_2(size, alignment) ((a + b - 1) & ~(b - 1))
#define sld_size_is_pow_2(size)               ((size > 0) && ((size & (size - 1)) == 0))

#define sld_bit_test(bit, value)      ((value >> bit) & 1)
#define sld_bit_set_high(bit, value)  (value |=  (1 << bit))
#define sld_bit_set_low(bit, value)   (value &= ~(1 << bit))
#define sld_bit_toggle(bit, value)    (value ^=  (1 << bit))
#define sld_bit_value(bit)            (1 << bit)


#endif //SLD_HPP
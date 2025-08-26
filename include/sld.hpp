#ifndef SLD_HPP
#define SLD_HPP

#include <cstdint>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>

#define sld_ct_inline  static constexpr inline // compile time inline
#define sld_ct_const   static constexpr const  // compile time const
#define sld_rt_inline  static inline           // run time inline
#define sld_rt_const   static const            // run time const
#define sld_os_api     extern                  // os api function

namespace sld {

    //-------------------------------------------------------------------
    // PRIMITIVES
    //-------------------------------------------------------------------

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

    // structured
    struct s8_t   { s8   val; };
    struct s16_t  { s16  val; };
    struct s32_t  { s32  val; };
    struct s64_t  { s64  val; };
    struct u8_t   { u8   val; };
    struct u16_t  { u16  val; };
    struct u32_t  { u32  val; };
    struct u64_t  { u64  val; };
    struct f32_t  { f32  val; };
    struct f64_t  { f64  val; };
    struct b8_t   { b8   val; };
    struct b16_t  { b16  val; };
    struct b32_t  { b32  val; };
    struct b64_t  { b64  val; };
    struct c8_t   { c8   val; };
    struct c16_t  { c16  val; };
    struct c32_t  { c32  val; };
    struct byte_t { byte val; };
    struct addr_t { addr val; };
    
    //-------------------------------------------------------------------
    // SIZE UTILITIES
    //-------------------------------------------------------------------
    
    sld_ct_inline const u64 size_kilobytes   (const u64 n_kilobytes)               { return (n_kilobytes * 1024);                               }
    sld_ct_inline const u64 size_megabytes   (const u64 n_megabytes)               { return (n_megabytes * 1024 * 1024);                        }
    sld_ct_inline const u64 size_gigabytes   (const u64 n_gigabytes)               { return (n_gigabytes * 1024 * 1024 * 1024);                 }
    sld_ct_inline const u64 size_align       (const u64 size, const u64 alignment) { return ((size + alignment - 1) / (alignment * alignment)); }
    sld_ct_inline const u64 size_align_pow_2 (const u64 size, const u64 alignment) { return ((size + alignment - 1) & ~(alignment - 1));        }
    sld_ct_inline bool      size_is_pow_2    (const u64 size)                      { return (((size > 0) && ((size & (size - 1)) == 0)));       }               

    //-------------------------------------------------------------------
    // BITWISE UTILITIES
    //-------------------------------------------------------------------
    
    sld_ct_inline const u32 bit_value        (const u32 bit)                       { return (1 << bit);           }
    sld_ct_inline bool      bit_test         (const u32 bit, const u32 value)      { return ((value >> bit) & 1); }
    sld_ct_inline void      bit_set_high     (const u32 bit, u32&      value)      { value |=  (1 << bit);        }
    sld_ct_inline void      bit_set_low      (const u32 bit, u32&      value)      { value &= ~(1 << bit);        }
    sld_ct_inline void      bit_toggle       (const u32 bit, u32&      value)      { value ^=  (1 << bit);        }

};


#endif //SLD_HPP
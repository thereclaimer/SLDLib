#ifndef SLD_HPP
#define SLD_HPP

#include <cstdint>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>

#define SLD_UTILITY      static constexpr inline // compile time inline
#define sld_ct_const       static constexpr const  // compile time const
#define SLD_INLINE  static inline           // run time inline
#define sld_rt_const       static const            // run time const
#define SLD_API
#define SLD_FUNC           static

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
    typedef void*    vptr;

    //-------------------------------------------------------------------
    // HANDLES
    //-------------------------------------------------------------------

    template<typename t>    
    struct type_t {
        t val;
        
        type_t(t v) : val(v) { }
        type_t()    : val(0) { }

        type_t& operator=(t v) {
            val = v;
            return(*this);
        }

        // defaulted copy/move are fine
        type_t(const type_t&)            = default;
        type_t(type_t&&)                 = default;
        type_t& operator=(const type_t&) = default;
        type_t& operator=(type_t&&)      = default;
    };

    using s8_t   = type_t<s8>;
    using s16_t  = type_t<s16>;
    using s32_t  = type_t<s32>;
    using s64_t  = type_t<s64>;
    using u8_t   = type_t<u8>;
    using u16_t  = type_t<u16>;
    using u32_t  = type_t<u32>;
    using u64_t  = type_t<u64>;
    using addr_t = type_t<addr>;
    using vptr_t = type_t<vptr>;

    //-------------------------------------------------------------------
    // SIZE UTILITIES
    //-------------------------------------------------------------------
    
    SLD_UTILITY const u64 size_kilobytes   (const u64 n_kilobytes)               { return (n_kilobytes * 1024);                               }
    SLD_UTILITY const u64 size_megabytes   (const u64 n_megabytes)               { return (n_megabytes * 1024 * 1024);                        }
    SLD_UTILITY const u64 size_gigabytes   (const u64 n_gigabytes)               { return (n_gigabytes * 1024 * 1024 * 1024);                 }
    SLD_UTILITY const u64 size_align       (const u64 size, const u64 alignment) { return ((size + alignment - 1) / (alignment * alignment)); }
    SLD_UTILITY const u64 size_align_pow_2 (const u64 size, const u64 alignment) { return ((size + alignment - 1) & ~(alignment - 1));        }
    SLD_UTILITY bool      size_is_pow_2    (const u64 size)                      { return (((size > 0) && ((size & (size - 1)) == 0)));       }               

    //-------------------------------------------------------------------
    // BITWISE UTILITIES
    //-------------------------------------------------------------------
    
    SLD_UTILITY const u32 bit_value        (const u32 bit)                       { return (1 << bit);           }
    SLD_UTILITY bool      bit_test         (const u32 bit, const u32 value)      { return ((value >> bit) & 1); }
    SLD_UTILITY void      bit_set_high     (const u32 bit, u32&      value)      { value |=  (1 << bit);        }
    SLD_UTILITY void      bit_set_low      (const u32 bit, u32&      value)      { value &= ~(1 << bit);        }
    SLD_UTILITY void      bit_toggle       (const u32 bit, u32&      value)      { value ^=  (1 << bit);        }
};


#endif //SLD_HPP
#ifndef SLD_HPP
#define SLD_HPP

#include <cstdint>
#include <xmmintrin.h>

/**********************************************************************************/
/* SCOPES                                                                         */
/**********************************************************************************/

#define utility  constexpr static inline 
#define global   constexpr static 
#define internal static inline
#define header   static inline

/**********************************************************************************/
/* PRIMITIVES                                                                     */
/**********************************************************************************/

namespace sld {

    //signed integers
    typedef int8_t  s8;
    typedef int16_t s16;
    typedef int32_t s32;
    typedef int64_t s64;

    //unsigned integer 
    typedef uint8_t  u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    //floats
    typedef float    f32;
    typedef double   f64;

    //booleans
    typedef int8_t   b8;
    typedef int16_t  b16;
    typedef int32_t  b32;
    typedef int64_t  b64;

    //chars  
    typedef char     c8;
    typedef wchar_t  c16;

    //memory
    typedef void*    ptr;
    typedef uint8_t  byte;
    typedef intptr_t addr;
};

/**********************************************************************************/
/* STRUCTURES                                                                     */
/**********************************************************************************/

namespace sld {

    struct memory_t {
        addr start;
        u64  size;
    };

    struct position_t {
        u32 x;
        u32 y;
    };

    struct dimensions_t {
        u32 width;
        u32 height;
    };
};

/**********************************************************************************/
/* SIMD                                                                           */
/**********************************************************************************/

#if _MSC_VER
    #if !defined(__clang__)
        #define INSTRUCTION_REORDER_BARRIER _ReadWriteBarrier()
    #endif
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif //_MSC_VER

namespace sld {

    typedef __m128i xmm_u128; // 4 u32
    typedef __m128  xmm_f128; // 4 f32


    static inline xmm_f128 xmm_f128_load_aligned   (const f32* ptr)                             { return(_mm_load_ps  (ptr));          }
    static inline xmm_f128 xmm_f128_load_unaligned (const f32* ptr)                             { return(_mm_loadu_ps (ptr));          }
    static inline xmm_f128 xmm_f128_add            (const xmm_f128 xmm_a, const xmm_f128 xmm_b) { return(_mm_add_ps   (xmm_a, xmm_b)); }
    static inline xmm_f128 xmm_f128_subtract       (const xmm_f128 xmm_a, const xmm_f128 xmm_b) { return(_mm_sub_ps   (xmm_a, xmm_b)); }
    static inline xmm_f128 xmm_f128_multiply       (const xmm_f128 xmm_a, const xmm_f128 xmm_b) { return(_mm_mul_ps   (xmm_a, xmm_b)); }
    static inline xmm_f128 xmm_f128_divide         (const xmm_f128 xmm_a, const xmm_f128 xmm_b) { return(_mm_div_ps   (xmm_a, xmm_b)); }
    static inline xmm_f128 xmm_f128_sqrt           (const xmm_f128 xmm)                         { return(_mm_sqrt_ps  (xmm));          }
    static inline void     xmm_f128_store          (const xmm_f128 xmm, f32* ptr)               { return(_mm_store_ps (ptr, xmm));     }

    static inline xmm_u128 xmm_u128_load_aligned   (const u32* ptr)                             { return(_mm_load_si128  ((xmm_u128*)ptr));      }
    static inline xmm_u128 xmm_u128_load_unaligned (const u32* ptr)                             { return(_mm_loadu_si128 ((xmm_u128*)ptr));      }
    static inline xmm_u128 xmm_u128_add            (const xmm_u128 xmm_a, const xmm_u128 xmm_b) { return(_mm_add_epi32   (xmm_a, xmm_b));        }
    static inline xmm_u128 xmm_u128_subtract       (const xmm_u128 xmm_a, const xmm_u128 xmm_b) { return(_mm_sub_epi32   (xmm_a, xmm_b));        }
    static inline xmm_u128 xmm_u128_multiply       (const xmm_u128 xmm_a, const xmm_u128 xmm_b) { return(_mm_mul_epi32   (xmm_a, xmm_b));        }
    static inline xmm_u128 xmm_u128_divide         (const xmm_u128 xmm_a, const xmm_u128 xmm_b) { return(_mm_div_epi32   (xmm_a, xmm_b));        }
    static inline void     xmm_u128_store          (const xmm_u128 xmm, u32* ptr)               { return(_mm_store_si128 ((xmm_u128*)ptr, xmm)); }
};

/**********************************************************************************/
/* GLOBALS                                                                        */
/**********************************************************************************/

namespace sld {

    struct global_t {
        global u8  max_u8  = 0xFF; 
        global u16 max_u16 = 0xFFFF; 
        global u32 max_u32 = 0xFFFFFFFF; 
        global u64 max_u64 = 0xFFFFFFFFFFFFFFFF; 
    } global _global;
}

/**********************************************************************************/
/* MEMORY                                                                         */
/**********************************************************************************/

namespace sld {

    utility bool memory_validate       (const memory_t& memory) { return(memory.start != 0 && memory.size != 0); }
    utility u32  memory_size_kilobytes (const u32 kb)           { return(kb * 1024);                             }
    utility u32  memory_size_megabytes (const u32 mb)           { return(mb * 1024 * 1024);                      }
    utility u32  memory_size_gigabytes (const u32 gb)           { return(gb * 1024 * 1024 * 1024);               }
};

/**********************************************************************************/
/* BIT LOGIC                                                                      */
/**********************************************************************************/

namespace sld {
    
    utility bool
    bit_test(
        const u32 value,
        const u32 bit) {

        return((value >> bit) & 1);
    }

    utility void 
    bit_set(
        u32&       value,
        const u32  bit,
        const bool state) {

        if (state) {
            value |=  (1 << bit);
        }
        else {
            value &= ~(1 << bit);
        }
    }

    utility void
    bit_set_high(
        u32&      value,
        const u32 bit) {
        
        value |= (1 << bit);
    }

    utility void
    bit_set_low(
        u32&      value,
        const u32 bit) {

        value &= ~(1 << bit);
    }

    utility void 
    bit_toggle(
        u32&      value,
        const u32 bit) {

        value ^=  (1 << bit);
    }
};

/**********************************************************************************/
/* MACROS                                                                         */
/**********************************************************************************/

#define sld_m_assert(cond) if (!cond) { *((int*)0)=1; }

#endif //SLD_HPP
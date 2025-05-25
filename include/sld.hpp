#ifndef SLD_HPP
#define SLD_HPP

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

    using xmm_f128_load_aligned   = _mm_load_ps;
    using xmm_f128_load_unaligned = _mm_loadu_ps;
    using xmm_f128_add            = _mm_add_ps;
    using xmm_f128_subtract       = _mm_sub_ps;
    using xmm_f128_multiply       = _mm_mul_ps;
    using xmm_f128_divide         = _mm_div_ps;
    using xmm_f128_sqrt           = _mm_sqrt_ps;
    using xmm_f128_store          = _mm_store_ps;

    using xmm_u128_load_aligned   = _mm_load_si128;
    using xmm_u128_load_unaligned = _mm_loadu_si128;
    using xmm_u128_add            = _mm_add_epi32;
    using xmm_u128_subtract       = _mm_sub_epi32;
    using xmm_u128_multiply       = _mm_mul_epi32;
    using xmm_u128_divide         = _mm_div_epi32;
    using xmm_u128_store          = _mm_store_si128;
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
/* UTILITIES                                                                      */
/**********************************************************************************/

namespace sld {

    utility bool memory_validate       (const memory_t& memory) { return(memory.start != 0 && memory.size != 0); }
    utility u32  memory_size_kilobytes (const u32 kb)           { return(kb * 1024);                             }
    utility u32  memory_size_megabytes (const u32 mb)           { return(mb * 1024 * 1024);                      }
    utility u32  memory_size_gigabytes (const u32 gb)           { return(gb * 1024 * 1024 * 1024);               }
};

#endif //SLD_HPP
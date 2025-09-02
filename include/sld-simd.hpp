#ifndef SLD_SIMD_HPP
#define SLD_SIMD_HPP

# if _MSC_VER
# if !defined(__clang__)
#    define INSTRUCTION_REORDER_BARRIER _ReadWriteBarrier()
# else
# endif
#    include <intrin.h>
# else
#    include <x86intrin.h>
# endif

#include "sld.hpp"

#define SLD_SIMD_ALIGN_128 alignas(16)

namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    struct SLD_SIMD_ALIGN_128 f128_t  {
        f32 val[4];
    };
    
    struct SLD_SIMD_ALIGN_128 u128_t {
        u32 val[4];
    };

    typedef __m128  reg_f128_t;
    typedef __m128i reg_u128_t;

    //-------------------------------------------------------------------
    // f128 | 4 x f32 | __m128
    //-------------------------------------------------------------------

    SLD_INLINE reg_f128_t simd_f128_load     (const f128_t&    f128)                           { return(_mm_load_ps(f128.val));    }       
    SLD_INLINE void       simd_f128_store    (const reg_f128_t reg,    f128_t&          f128)  { _mm_store_ps(f128.val, reg);      }                               
    SLD_INLINE reg_f128_t simd_f128_a_add_b  (reg_f128_t       reg_a,  const reg_f128_t reg_b) { return(_mm_add_ps(reg_a, reg_b)); }
    SLD_INLINE reg_f128_t simd_f128_a_sub_b  (reg_f128_t       reg_a,  const reg_f128_t reg_b) { return(_mm_sub_ps(reg_a, reg_b)); }
    SLD_INLINE reg_f128_t simd_f128_a_mul_b  (reg_f128_t       reg_a,  const reg_f128_t reg_b) { return(_mm_mul_ps(reg_a, reg_b)); }
    SLD_INLINE reg_f128_t simd_f128_a_div_b  (reg_f128_t       reg_a,  const reg_f128_t reg_b) { return(_mm_div_ps(reg_a, reg_b)); }
    SLD_INLINE reg_f128_t simd_f128_sqrt     (const reg_f128_t reg)                            { return(_mm_sqrt_ps(reg));         }
    SLD_INLINE reg_f128_t simd_f128_inv_sqrt( const reg_f128_t reg) {

        // initial approx 1/x
        reg_f128_t reg_tmp = _mm_set1_ps (2.0f);
        reg_f128_t reg_out = _mm_rcp_ps  (reg); 

        // Newton-Raphson refinement: y = y * (2 - x*y)
        reg_out = _mm_mul_ps(reg_out, _mm_sub_ps(reg_tmp, _mm_mul_ps(reg, reg_out)));
    }

    //-------------------------------------------------------------------
    // u128 | 4 x u32 | __m128i
    //-------------------------------------------------------------------

    SLD_INLINE reg_u128_t simd_u128_load     (const u128_t     u128,   reg_u128_t       reg);                                  
    SLD_INLINE void       simd_u128_store    (const reg_u128_t reg,    u128_t&          u128);                                 
    SLD_INLINE reg_u128_t simd_u128_a_add_b  (reg_u128_t       reg_a,  const reg_u128_t reg_b); 
    SLD_INLINE reg_u128_t simd_u128_a_sub_b  (reg_u128_t       reg_a,  const reg_u128_t reg_b); 
    SLD_INLINE reg_u128_t simd_u128_a_mul_b  (reg_u128_t       reg_a,  const reg_u128_t reg_b); 
    SLD_INLINE reg_u128_t simd_u128_a_div_b  (reg_u128_t       reg_a,  const reg_u128_t reg_b);
    

};

#endif //SLD_SIMD_HPP
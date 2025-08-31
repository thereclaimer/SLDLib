#ifndef SLD_SIMD_HPP
#define SLD_SIMD_HPP

#include <xmmintrin.h>

#include "sld.hpp"

#define SLD_SIMD_ALIGN_4F32 alignas(16)

#define sld_simd_f128_load  _mm_load_ps
#define sld_simd_f128_store _mm_store_ps
#define sld_simd_f128_add   _mm_add_ps
#define sld_simd_f128_sub   _mm_sub_ps
#define sld_simd_f128_mul   _mm_mul_ps
#define sld_simd_f128_div   _mm_div_ps

namespace sld {

    typedef __m128 simd_f128_t;

    struct SLD_SIMD_ALIGN_4F32 f32x4_t {
        f32 array[4];
    };

    SLD_INLINE void
    simd_a_add_b_to_c(
        const f32x4_t& a,
        const f32x4_t& b,
        f32x4_t&       c) {

        const simd_f128_t simd_a = sld_simd_f128_load (a.array);
        const simd_f128_t simd_b = sld_simd_f128_load (b.array);
        const simd_f128_t simd_c = sld_simd_f128_add  (simd_a, simd_b);
        
        sld_simd_f128_store(c.array,simd_c);
    }

    SLD_INLINE void
    simd_a_sub_b_to_c(
        const f32x4_t& a,
        const f32x4_t& b,
        f32x4_t&       c) {

        const simd_f128_t simd_a = sld_simd_f128_load (a.array);
        const simd_f128_t simd_b = sld_simd_f128_load (b.array);
        const simd_f128_t simd_c = sld_simd_f128_sub  (simd_a, simd_b);
        
        sld_simd_f128_store(c.array,simd_c);
    }

    SLD_INLINE void
    simd_a_mul_b_to_c(
        const f32x4_t& a,
        const f32x4_t& b,
        f32x4_t&       c) {

        const simd_f128_t simd_a = sld_simd_f128_load (a.array);
        const simd_f128_t simd_b = sld_simd_f128_load (b.array);
        const simd_f128_t simd_c = sld_simd_f128_mul  (simd_a, simd_b);
        
        sld_simd_f128_store(c.array,simd_c);
    }

    SLD_INLINE void
    simd_a_div_b_to_c(
        const f32x4_t& a,
        const f32x4_t& b,
        f32x4_t&       c) {


        const simd_f128_t simd_a = sld_simd_f128_load (a.array);
        const simd_f128_t simd_b = sld_simd_f128_load (b.array);
        const simd_f128_t simd_c = sld_simd_f128_div  (simd_a, simd_b);
        
        sld_simd_f128_store(c.array,simd_c);
    }
};

#endif //SLD_SIMD_HPP
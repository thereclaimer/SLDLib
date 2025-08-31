#ifndef SLD_SIMD_HPP
#define SLD_SIMD_HPP

#include <xmmintrin.h>

#include "sld.hpp"

#define SLD_SIMD_ALIGN_4F32 alignas(16)

namespace sld {

    typedef __m128 simd_f128_t;

    struct SLD_SIMD_ALIGN_4F32 f128_t {
        f32 array[4];
    };

    void simd_4f32_load         (const f128_t& data,   simd_f128_t&       reg);                                  
    void simd_4f32_store        (const simd_f128_t&  reg,    f128_t&      data);                                 
    void simd_4f32_a_add_b      (simd_f128_t&        reg_a,  const simd_f128_t& reg_b); 
    void simd_4f32_a_sub_b      (simd_f128_t&        reg_a,  const simd_f128_t& reg_b); 
    void simd_4f32_a_mul_b      (simd_f128_t&        reg_a,  const simd_f128_t& reg_b); 
    void simd_4f32_a_div_b      (simd_f128_t&        reg_a,  const simd_f128_t& reg_b); 
    void simd_4f32_a_add_b_to_c (const simd_f128_t&  reg_a,  const simd_f128_t& reg_b, simd_f128_t& reg_c); 
    void simd_4f32_a_sub_b_to_c (const simd_f128_t&  reg_a,  const simd_f128_t& reg_b, simd_f128_t& reg_c); 
    void simd_4f32_a_mul_b_to_c (const simd_f128_t&  reg_a,  const simd_f128_t& reg_b, simd_f128_t& reg_c); 
    void simd_4f32_a_div_b_to_c (const simd_f128_t&  reg_a,  const simd_f128_t& reg_b, simd_f128_t& reg_c); 
    void simd_4f32_sqrt         (const simd_f128_t&  reg_in, simd_f128_t&       reg_out); 
    void simd_4f32_inv_sqrt     (const simd_f128_t&  reg_in, simd_f128_t&       reg_out); 

    SLD_INLINE void
    simd_4f32_load(
        const f128_t& data,
        simd_f128_t&        reg) {

        reg = _mm_load_ps(data.array);
    }

    SLD_INLINE void
    simd_4f32_store(
        const simd_f128_t& reg,
        f128_t&      data) {

        _mm_store_ps(data.array, reg);
    }

    SLD_INLINE void
    simd_4f32_a_add_b(
        simd_f128_t&       reg_a,
        const simd_f128_t& reg_b) {

        reg_a = _mm_add_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_4f32_a_sub_b(
        simd_f128_t&       reg_a,
        const simd_f128_t& reg_b) {

        reg_a = _mm_sub_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_4f32_a_mul_b(
        simd_f128_t&       reg_a,
        const simd_f128_t& reg_b) {

        reg_a = _mm_mul_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_4f32_a_div_b(
        simd_f128_t&       reg_a,
        const simd_f128_t& reg_b) {

        reg_a = _mm_div_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_4f32_a_add_b_to_c(
        const simd_f128_t& reg_a,
        const simd_f128_t& reg_b,
        simd_f128_t&       reg_c) {

        reg_c = _mm_add_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_4f32_a_sub_b_to_c(
        const simd_f128_t& reg_a,
        const simd_f128_t& reg_b,
        simd_f128_t&       reg_c) {

        reg_c = _mm_sub_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_4f32_a_mul_b_to_c(
        const simd_f128_t& reg_a,
        const simd_f128_t& reg_b,
        simd_f128_t&       reg_c) {

        reg_c = _mm_mul_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_4f32_a_div_b_to_c(
        const simd_f128_t& reg_a,
        const simd_f128_t& reg_b,
        simd_f128_t&       reg_c) {

        reg_c = _mm_div_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_4f32_sqrt(
        const simd_f128_t& reg_in,
        simd_f128_t&       reg_out) {

        reg_out = _mm_sqrt_ps(reg_in);
    }

    SLD_INLINE void
    simd_4f32_inv_sqrt_out(
        const simd_f128_t& reg_in,
        simd_f128_t&       reg_out) {

        simd_f128_t reg_two = _mm_set1_ps (2.0f);
        reg_out                 = _mm_rcp_ps  (reg_in); // initial approx 1/x

        // Newton-Raphson refinement: y = y * (2 - x*y)
        reg_out = _mm_mul_ps(reg_out, _mm_sub_ps(reg_two, _mm_mul_ps(reg_in, reg_out)));
    }
};

#endif //SLD_SIMD_HPP
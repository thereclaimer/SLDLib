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

    struct f128_t SLD_SIMD_ALIGN_128 { f32 val[4]; };
    struct u128_t SLD_SIMD_ALIGN_128 { u32 val[4]; };

    typedef __m128  simd_f128_t;
    typedef __m128i simd_u128_t;

    SLD_INLINE void simd_f128_load         (const f128_t& data,   simd_f128_t&       reg);                                  
    SLD_INLINE void simd_f128_store        (const simd_f128_t&  reg,    f128_t&      data);                                 
    SLD_INLINE void simd_f128_a_add_b      (simd_f128_t&        reg_a,  const simd_f128_t& reg_b); 
    SLD_INLINE void simd_f128_a_sub_b      (simd_f128_t&        reg_a,  const simd_f128_t& reg_b); 
    SLD_INLINE void simd_f128_a_mul_b      (simd_f128_t&        reg_a,  const simd_f128_t& reg_b); 
    SLD_INLINE void simd_f128_a_div_b      (simd_f128_t&        reg_a,  const simd_f128_t& reg_b); 
    SLD_INLINE void simd_f128_a_add_b_to_c (const simd_f128_t&  reg_a,  const simd_f128_t& reg_b, simd_f128_t& reg_c); 
    SLD_INLINE void simd_f128_a_sub_b_to_c (const simd_f128_t&  reg_a,  const simd_f128_t& reg_b, simd_f128_t& reg_c); 
    SLD_INLINE void simd_f128_a_mul_b_to_c (const simd_f128_t&  reg_a,  const simd_f128_t& reg_b, simd_f128_t& reg_c); 
    SLD_INLINE void simd_f128_a_div_b_to_c (const simd_f128_t&  reg_a,  const simd_f128_t& reg_b, simd_f128_t& reg_c); 
    SLD_INLINE void simd_f128_sqrt         (const simd_f128_t&  reg_in, simd_f128_t&       reg_out); 
    SLD_INLINE void simd_f128_inv_sqrt     (const simd_f128_t&  reg_in, simd_f128_t&       reg_out); 

    SLD_INLINE void simd_u128_load         (const u128_t&       data,   simd_u128_t& reg);                                  
    SLD_INLINE void simd_u128_store        (const simd_u128_t&  reg,    u128_t&      data);                                 
    SLD_INLINE void simd_u128_a_add_b      (simd_u128_t&        reg_a,  const simd_u128_t& reg_b); 
    SLD_INLINE void simd_u128_a_sub_b      (simd_u128_t&        reg_a,  const simd_u128_t& reg_b); 
    SLD_INLINE void simd_u128_a_mul_b      (simd_u128_t&        reg_a,  const simd_u128_t& reg_b); 
    SLD_INLINE void simd_u128_a_div_b      (simd_u128_t&        reg_a,  const simd_u128_t& reg_b); 
    SLD_INLINE void simd_u128_a_add_b_to_c (const simd_u128_t&  reg_a,  const simd_u128_t& reg_b, simd_u128_t& reg_c); 
    SLD_INLINE void simd_u128_a_sub_b_to_c (const simd_u128_t&  reg_a,  const simd_u128_t& reg_b, simd_u128_t& reg_c); 
    SLD_INLINE void simd_u128_a_mul_b_to_c (const simd_u128_t&  reg_a,  const simd_u128_t& reg_b, simd_u128_t& reg_c); 
    SLD_INLINE void simd_u128_a_div_b_to_c (const simd_u128_t&  reg_a,  const simd_u128_t& reg_b, simd_u128_t& reg_c); 

    SLD_INLINE void
    simd_f128_load(
        const f128_t& data,
        simd_f128_t&        reg) {

        reg = _mm_load_ps(data.array);
    }

    SLD_INLINE void
    simd_f128_store(
        const simd_f128_t& reg,
        f128_t&      data) {

        _mm_store_ps(data.array, reg);
    }

    SLD_INLINE void
    simd_f128_a_add_b(
        simd_f128_t&       reg_a,
        const simd_f128_t& reg_b) {

        reg_a = _mm_add_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_f128_a_sub_b(
        simd_f128_t&       reg_a,
        const simd_f128_t& reg_b) {

        reg_a = _mm_sub_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_f128_a_mul_b(
        simd_f128_t&       reg_a,
        const simd_f128_t& reg_b) {

        reg_a = _mm_mul_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_f128_a_div_b(
        simd_f128_t&       reg_a,
        const simd_f128_t& reg_b) {

        reg_a = _mm_div_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_f128_a_add_b_to_c(
        const simd_f128_t& reg_a,
        const simd_f128_t& reg_b,
        simd_f128_t&       reg_c) {

        reg_c = _mm_add_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_f128_a_sub_b_to_c(
        const simd_f128_t& reg_a,
        const simd_f128_t& reg_b,
        simd_f128_t&       reg_c) {

        reg_c = _mm_sub_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_f128_a_mul_b_to_c(
        const simd_f128_t& reg_a,
        const simd_f128_t& reg_b,
        simd_f128_t&       reg_c) {

        reg_c = _mm_mul_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_f128_a_div_b_to_c(
        const simd_f128_t& reg_a,
        const simd_f128_t& reg_b,
        simd_f128_t&       reg_c) {

        reg_c = _mm_div_ps(reg_a, reg_b);
    }

    SLD_INLINE void
    simd_f128_sqrt(
        const simd_f128_t& reg_in,
        simd_f128_t&       reg_out) {

        reg_out = _mm_sqrt_ps(reg_in);
    }

    SLD_INLINE void
    simd_f128_inv_sqrt_out(
        const simd_f128_t& reg_in,
        simd_f128_t&       reg_out) {

        simd_f128_t reg_two = _mm_set1_ps (2.0f);
        reg_out                 = _mm_rcp_ps  (reg_in); // initial approx 1/x

        // Newton-Raphson refinement: y = y * (2 - x*y)
        reg_out = _mm_mul_ps(reg_out, _mm_sub_ps(reg_two, _mm_mul_ps(reg_in, reg_out)));
    }
};

#endif //SLD_SIMD_HPP
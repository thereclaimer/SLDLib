#pragma once

#include "sld-math.hpp"

namespace sld {

    struct vec2_reg_t {
        reg_f128_t x;
        reg_f128_t y;
    };

    void
    vec2_simd_normalize(
        const u32    count,
        vec2_f128_t& v2) {

        vec2_reg_t reg_v2;
        reg_f128_t reg_xx;
        reg_f128_t reg_yy;
        reg_f128_t reg_xx_add_yy;
        reg_f128_t reg_inv_sqrt;

        for (
            u32 index = 0;
            index < count;
            ++index) {

            // simdify the next vector
            reg_v2.x = simd_f128_load (v2.x[index]);
            reg_v2.y = simd_f128_load (v2.y[index]);

            // square and add the components
            reg_xx        = simd_f128_a_mul_b (reg_v2.x, reg_v2.x);
            reg_yy        = simd_f128_a_mul_b (reg_v2.y, reg_v2.y);
            reg_xx_add_yy = simd_f128_a_add_b (reg_xx,   reg_yy);

            // calculate the inverse square root
            // and multiply it by the components
            reg_inv_sqrt = simd_f128_inv_sqrt (reg_xx_add_yy);
            reg_v2.x     = simd_f128_a_mul_b  (reg_inv_sqrt);
            reg_v2.y     = simd_f128_a_mul_b  (reg_inv_sqrt);

            // store the normalized vector
            simd_f128_store (reg_v2.x, v2.x[index]);
            simd_f128_store (reg_v2.y, v2.y[index]);
        }
    }

    void
    vec2_simd_magnitude(
        const u32          count,
        const vec2_f128_t& v2,
        f128_t*            m) {

        vec2_f128_t reg_v2;
        reg_f128_t  reg_xx_add_yy;
        reg_f128_t  reg_mag;

        for (
            u32 index = 0;
            index < count;
            ++index) {

            // simdify the next vector
            reg_v2.x = simd_f128_load (v2.x[index]);
            reg_v2.y = simd_f128_load (v2.y[index]);

            // square and add the components
            reg_v2.x      = simd_f128_a_mul_b (reg_v2.x, reg_v2.x);
            reg_v2.y      = simd_f128_a_mul_b (reg_v2.y, reg_v2.y);
            reg_xx_add_yy = simd_f128_a_add_b (reg_v2.x, reg_v2.y);

            // calculate and store the magnitude
            reg_mag = simd_f128_sqrt(reg_xx_add_yy);
            simd_f128_store(reg_mag, m[index]);
        }
    }

    void
    vec2_simd_scalar_mul(
        const u32     count,
        vec2_f128_t&  v2,
        const f128_t* s) {

        vec2_f128_t reg_v2;
        reg_f128_t  reg_s;

        for (
            u32 index = 0;
            index < count;
            ++index) {

            // simdify the next vector and the scalar
            reg_v2.x = simd_f128_load (v2.x[index]);
            reg_v2.y = simd_f128_load (v2.y[index]);
            reg_s    = simd_f128_load (s[index]);

            // do the scalar multiply
            simd_f128_a_mul_b(reg_v2.x, reg_s);
            simd_f128_a_mul_b(reg_v2.y, reg_s);
        
            // store the vector
            simd_f128_store (reg_v2.x, v2.x[index]);
            simd_f128_store (reg_v2.y, v2.y[index]);
        }
    }

    void
    vec2_simd_scalar_div(
        const u32     count,
        vec2_f128_t&  v2,
        const f128_t* s) {


    }

    void
    vec2_simd_scalar_mul_uniform(
        const u32 count,
        vec2_f128_t& v2,
        const f32 s) {

    }
   
    void
    vec2_simd_scalar_div_uniform(
        const u32 count,
        vec2_f128_t&   v2,
        const f32 s) {

    }

    void
    vec2_simd_scalar_mul_new(
        const u32          count,
        const vec2_f128_t& v2,
        const f32*         s,
        vec2_f128_t&       v2_new) {

    }

    void
    vec2_simd_scalar_div_new(
        const u32          count,
        const vec2_f128_t& v2,
        const f32*         s,
        vec2_f128_t&       v2_new) {

    }

    void
    vec2_simd_scalar_mul_new_uniform(
        const u32    count,
        vec2_f128_t& v2,
        const f32    s,
        vec2_f128_t& v2_new) {

    }

    void
    vec2_simd_scalar_div_new_uniform(
        const u32 count,
        vec2_f128_t&   v2,
        const f32      s,
        vec2_f128_t&   v2_new) {

    }

    void
    vec2_simd_a_add_b(
        const u32          count,
        vec2_f128_t&       v2_a,
        const vec2_f128_t& v2_b) {
      
        vec2_reg_t reg_v2_a;
        vec2_reg_t reg_v2_b;

        for (
            u32 index = 0;
            index < count;
            ++index) {
        
            // simdify the vectors
            reg_v2_a.x = simd_f128_load (v2_a.x[index]);
            reg_v2_a.y = simd_f128_load (v2_a.y[index]);
            reg_v2_b.x = simd_f128_load (v2_b.x[index]);
            reg_v2_b.y = simd_f128_load (v2_b.y[index]);
        
            // a add b
            reg_v2_a.x = simd_f128_a_add_b (reg_v2_a.x, reg_v2_a.x);
            reg_v2_a.y = simd_f128_a_add_b (reg_v2_b.y, reg_v2_b.y);

            // store vector a
            simd_f128_store (reg_v2_a.x, v2_a.x[index]);
            simd_f128_store (reg_v2_a.y, v2_a.y[index]);
        }
    }

    void
    vec2_simd_a_sub_b(
        const u32          count,
        vec2_f128_t&       v2_a,
        const vec2_f128_t& v2_b) {
      
        vec2_reg_t reg_v2_a;
        vec2_reg_t reg_v2_b;

        for (
            u32 index = 0;
            index < count;
            ++index) {
        
            // simdify the vectors
            reg_v2_a.x = simd_f128_load (v2_a.x[index]);
            reg_v2_a.y = simd_f128_load (v2_a.y[index]);
            reg_v2_b.x = simd_f128_load (v2_b.x[index]);
            reg_v2_b.y = simd_f128_load (v2_b.y[index]);

            // a add b
            reg_v2_a.x = simd_f128_a_sub_b(reg_v2_a.x, reg_v2_a.x);
            reg_v2_a.y = simd_f128_a_sub_b(reg_v2_b.y, reg_v2_b.y);

            // store vector a
            simd_f128_store (reg_v2_a.x, v2_a.x[index]);
            simd_f128_store (reg_v2_a.y, v2_a.y[index]);
        }
    }

    void
    vec2_simd_a_dot_b(
        const u32         count,
        const vec2_f128_t&   v2_a,
        const vec2_f128_t&   v2_b,
        f128_t* dot) {

        vec2_f128_t reg_v2_a;
        vec2_f128_t reg_v2_b;
        vec2_f128_t reg_v2_ab;
        reg_f128_t  reg_dot;

        for (
            u32 index = 0;
            index < count;
            ++index) {
        
            // simdify the vectors
            reg_v2_a.x = simd_f128_load (v2_a.x[index]);
            reg_v2_a.y = simd_f128_load (v2_a.y[index]);
            reg_v2_b.x = simd_f128_load (v2_b.x[index]);
            reg_v2_b.y = simd_f128_load (v2_b.y[index]);
        
            // dot = axbx + ayby
            v2_simd_ab.x = simd_f128_a_mul_b (v2_simd_a.x,  v2_simd_b.x);
            v2_simd_ab.y = simd_f128_a_mul_b (v2_simd_a.y,  v2_simd_b.y);
            simd_reg_dot = simd_f128_a_add_b (v2_simd_ab.x, v2_simd_ab.y);
        
            // store the dot product
            simd_f128_store(simd_reg_dot, dot[index]);
        }
    }

    void
    vec2_simd_a_add_b_to_c(
        const u32       count,
        const vec2_f128_t& v2_a,
        const vec2_f128_t& v2_b,
        vec2_f128_t&       v2_c) {

    }

    void
    vec2_simd_a_sub_b_to_c(
        const u32          count,
        const vec2_f128_t& v2_a,
        const vec2_f128_t& v2_b,
        vec2_f128_t&       v2_c) {

    }
};
#pragma once

#include "sld-math.hpp"

namespace sld {

    struct vec2_simd_t {
        simd_f128_t x;
        simd_f128_t y;
    };

    void vec2_simd_load  (const f128_t& v2_x,const f128_t& v2_y, vec2_simd_t&       v2_simd);
    void vec2_simd_store (f128_t&       v2_x, f128_t&      v2_y, const vec2_simd_t& v2_simd);

    void
    vec2_simd_normalize(
        const u32    count,
        vec2_f128_t& v2) {

        vec2_f128_t v2_simd;

        simd_f128_t simd_reg_xx;
        simd_f128_t simd_reg_yy;
        simd_f128_t simd_reg_xx_add_yy;
        simd_f128_t simd_reg_inv_sqrt;

        for (
            u32 index = 0;
            index < count;
            ++index) {

            // simdify the next vector
            vec2_simd_load(v2.x[index], v2.y[index], v2_simd);

            // square and add the components
            simd_4f32_a_mul_b      (simd_reg_xx, v2_simd.x);
            simd_4f32_a_mul_b      (simd_reg_yy, v2_simd.y);
            simd_4f32_a_add_b_to_c (simd_reg_xx, simd_reg_yy, simd_reg_xx_add_yy);

            // calculate the inverse square root
            // and multiply it by the components
            simd_4f32_inv_sqrt (simd_reg_inv_sqrt, simd_reg_xx_add_yy);
            simd_4f32_a_mul_b  (v2_simd.x,         simd_reg_inv_sqrt);
            simd_4f32_a_mul_b  (v2_simd.y,         simd_reg_inv_sqrt);

            // store the normalized vector
            vec2_simd_store(v2.x[index], v2.y[index], v2_simd);
        }
    }

    void
    vec2_simd_magnitude(
        const u32         count,
        const vec2_f128_t&   v2,
        f128_t* m) {

        vec2_f128_t     v2_simd;
        simd_f128_t simd_reg_xx;
        simd_f128_t simd_reg_yy;
        simd_f128_t simd_reg_xx_add_yy;
        simd_f128_t simd_reg_mag;

        for (
            u32 index = 0;
            index < count;
            ++index) {

            // simdify the next vector
            vec2_simd_load(v2.x[index], v2.y[index], v2_simd);

            // square and add the components
            simd_4f32_a_mul_b      (simd_reg_xx, simd_reg_x);
            simd_4f32_a_mul_b      (simd_reg_yy, simd_reg_y);
            simd_4f32_a_add_b_to_c (simd_reg_xx, simd_reg_yy, simd_reg_xx_add_yy);

            // calculate and store the magnitude
            simd_4f32_sqrt  (simd_reg_xx_add_yy, simd_reg_mag);
            simd_4f32_store (simd_reg_mag,       m[index]);
        }
    }

    void
    vec2_simd_scalar_mul(
        const u32     count,
        vec2_f128_t&  v2,
        const f128_t* s) {

        vec2_f128_t     v2_simd;
        simd_f128_t simd_reg_s;

        for (
            u32 index = 0;
            index < count;
            ++index) {

            // simdify the next vector and the scalar
            vec2_simd_load (v2[index], v2_simd);
            simd_4f32_load (s[index], simd_reg_s);

            // do the scalar multiply
            simd_4f32_a_mul_b(v2_simd.x, simd_reg_s);
            simd_4f32_a_mul_b(v2_simd.y, simd_reg_s);
        
            // store the vector
            vec2_simd_store(v2[index], v2_simd);
        }
    }

    void
    vec2_simd_scalar_div(
        const u32               count,
        vec2_f128_t&               v2,
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
        const u32     count,
        const vec2_f128_t& v2,
        const f32*    s,
        vec2_f128_t&       v2_new) {

    }

    void
    vec2_simd_scalar_div_new(
        const u32     count,
        const vec2_f128_t& v2,
        const f32*    s,
        vec2_f128_t&       v2_new) {

    }

    void
    vec2_simd_scalar_mul_new_uniform(
        const u32 count,
        vec2_f128_t&   v2,
        const f32 s,
        vec2_f128_t&   v2_new) {

    }

    void
    vec2_simd_scalar_div_new_uniform(
        const u32 count,
        vec2_f128_t&   v2,
        const f32 s,
        vec2_f128_t&   v2_new) {

    }

    void
    vec2_simd_a_add_b(
        const u32       count,
        vec2_f128_t&       v2_a,
        const vec2_f128_t& v2_b) {
      
        vec2_f128_t v2_simd_a;
        vec2_f128_t v2_simd_b;

        for (
            u32 index = 0;
            index < count;
            ++index) {
        
            // simdify the vectors
            vec2_simd_load(v2_a[index], v2_simd_a);
            vec2_simd_load(v2_b[index], v2_simd_b);
        
            // a add b
            simd_4f32_a_add_b(v2_simd_a.x, v2_simd_b.x);
            simd_4f32_a_add_b(v2_simd_a.y, v2_simd_b.y);

            // store vector a
            vec2_simd_store(v2_a[index], v2_simd_a);
        }
    }

    void
    vec2_simd_a_sub_b(
        const u32       count,
        vec2_f128_t&       v2_a,
        const vec2_f128_t& v2_b) {
      
        vec2_f128_t v2_simd_a;
        vec2_f128_t v2_simd_b;

        for (
            u32 index = 0;
            index < count;
            ++index) {
        
            // simdify the vectors
            vec2_simd_load(v2_a[index], v2_simd_a);
            vec2_simd_load(v2_b[index], v2_simd_b);
        
            // a add b
            simd_4f32_a_sub_b(v2_simd_a.x, v2_simd_b.x);
            simd_4f32_a_sub_b(v2_simd_a.y, v2_simd_b.y);

            // store vector a
            vec2_simd_store(v2_a[index], v2_simd_a);
        }
    }

    void
    vec2_simd_a_dot_b(
        const u32         count,
        const vec2_f128_t&   v2_a,
        const vec2_f128_t&   v2_b,
        f128_t* dot) {

        vec2_f128_t     v2_simd_a;
        vec2_f128_t     v2_simd_b;
        vec2_f128_t     v2_simd_ab;
        simd_f128_t simd_reg_dot;

        for (
            u32 index = 0;
            index < count;
            ++index) {
        
            // simdify the vectors
            vec2_simd_load(v2_a[index], v2_simd_a);
            vec2_simd_load(v2_b[index], v2_simd_b);
        
            // dot = axbx + ayby
            simd_4f32_a_mul_b_to_c (v2_simd_a.x,  v2_simd_b.x,  v2_simd_ab.x);
            simd_4f32_a_mul_b_to_c (v2_simd_a.y,  v2_simd_b.y,  v2_simd_ab.y);
            simd_4f32_a_add_b_to_c (v2_simd_ab.x, v2_simd_ab.y, simd_reg_dot);
        
            // store the dot product
            simd_4f32_store(simd_reg_dot, dot[index]);
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
        const u32       count,
        const vec2_f128_t& v2_a,
        const vec2_f128_t& v2_b,
        vec2_f128_t&       v2_c) {

    }

    void
    vec2_simd_a_dot_b_to_c(
        const u32       count,
        const vec2_f128_t& v2_a,
        const vec2_f128_t& v2_b,
        vec2_f128_t&       v2_c) {

    }

    void
    vec2_simd_a_cross_b_to_c(
        const u32       count,
        const vec2_f128_t& v2_a,
        const vec2_f128_t& v2_b,
        vec2_f128_t&       v2_c) {

    }
    void vec2_simd_load  (const vec2_f128_t& v2, vec2_simd_t&       v2_simd);
    void vec2_simd_store (vec2_f128_t&       v2, const vec2_simd_t& v2_simd);

    SLD_INLINE void
    vec2_simd_load(
        const f128_t& v2_x,
        const f128_t& v2_y,
        vec2_simd_t&  v2_simd) {

        simd_4f32_load (v2.x, v2_simd.x);
        simd_4f32_load (v2.y, v2_simd.y);
    }
    
    SLD_INLINE void
    vec2_simd_store(
        f128_t&            v2_x,
        f128_t&            v2_y,
        const vec2_simd_t& v2_simd) {

        // store the data
        simd_4f32_store (v2_simd.x, v2.x);
        simd_4f32_store (v2_simd.y, v2.y);
    }
};
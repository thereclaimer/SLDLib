#pragma once

#include "sld-math.hpp"

namespace sld {

    struct vec2_simd_t {
        struct {
            simd_4f32_data_t x; 
            simd_4f32_data_t y; 
        } data;
        struct {
            simd_4f32_reg_t x; 
            simd_4f32_reg_t y; 
        } reg;
    };

    void vec2_simd_load  (const vec2x4_t& v2, vec2_simd_t& v2_simd);
    void vec2_simd_store (const vec2x4_t& v2, vec2_simd_t& v2_simd);

    void
    vec2_simd_normalize(
        const u32 count,
        vec2x4_t* v2) {

        vec2_simd_t v2_simd;

        simd_4f32_reg_t simd_reg_xx;
        simd_4f32_reg_t simd_reg_yy;
        simd_4f32_reg_t simd_reg_xx_add_yy;
        simd_4f32_reg_t simd_reg_inv_sqrt;

        for (
            u32 index = 0;
            index < count;
            ++index) {
 
            // simdify the next vector
            vec2_simd_load(v2[index], v2_simd);

            // square and add the components
            simd_4f32_a_mul_b      (simd_reg_xx, v2_simd.reg.x);
            simd_4f32_a_mul_b      (simd_reg_yy, v2_simd.reg.y);
            simd_4f32_a_add_b_to_c (simd_reg_xx, simd_reg_yy, simd_reg_xx_add_yy);

            // calculate the inverse square root
            // and multiply it by the components
            simd_4f32_inv_sqrt (simd_reg_inv_sqrt, simd_reg_xx_add_yy);
            simd_4f32_a_mul_b  (v2_simd.reg.x,     simd_reg_inv_sqrt);
            simd_4f32_a_mul_b  (v2_simd.reg.y,     simd_reg_inv_sqrt);

            // store the normalized vector
            vec2_simd_store(v2[index], v2_simd);
        }
    }

    void
    vec2_simd_magnitude(
        const u32         count,
        const vec2x4_t*   v2,
        simd_4f32_data_t* m) {

        vec2_simd_t     v2_simd;
        simd_4f32_reg_t simd_reg_xx;
        simd_4f32_reg_t simd_reg_yy;
        simd_4f32_reg_t simd_reg_xx_add_yy;
        simd_4f32_reg_t simd_reg_mag;

        for (
            u32 index = 0;
            index < count;
            ++index) {

            // simdify the next vector
            vec2_simd_load(v2[index], v2_simd);

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
        const u32               count,
        vec2x4_t*               v2,
        const simd_4f32_data_t* s) {

        vec2_simd_t     v2_simd;
        simd_4f32_reg_t simd_reg_s;

        for (
            u32 index = 0;
            index < count;
            ++index) {

            // simdify the next vector and the scalar
            vec2_simd_load (v2[index], v2_simd);
            simd_4f32_load (s[index], simd_reg_s);

            // do the scalar multiply
            simd_4f32_a_mul_b(v2_simd.reg.x, simd_reg_s);
            simd_4f32_a_mul_b(v2_simd.reg.y, simd_reg_s);
        
            // store the vector
            vec2_simd_store(v2[index], v2_simd);
        }
    }

    void
    vec2_simd_scalar_div(
        const u32               count,
        vec2x4_t*               v2,
        const simd_4f32_data_t* s) {


    }

    void
    vec2_simd_scalar_mul_uniform(
        const u32 count,
        vec2x4_t* v2,
        const f32 s) {

    }
   
    void
    vec2_simd_scalar_div_uniform(
        const u32 count,
        vec2x4_t*   v2,
        const f32 s) {

    }

    void
    vec2_simd_scalar_mul_new(
        const u32     count,
        const vec2x4_t* v2,
        const f32*    s,
        vec2x4_t*       v2_new) {

    }

    void
    vec2_simd_scalar_div_new(
        const u32     count,
        const vec2x4_t* v2,
        const f32*    s,
        vec2x4_t*       v2_new) {

    }

    void
    vec2_simd_scalar_mul_new_uniform(
        const u32 count,
        vec2x4_t*   v2,
        const f32 s,
        vec2x4_t*   v2_new) {

    }

    void
    vec2_simd_scalar_div_new_uniform(
        const u32 count,
        vec2x4_t*   v2,
        const f32 s,
        vec2x4_t*   v2_new) {

    }

    void
    vec2_simd_a_add_b(
        const u32       count,
        vec2x4_t*       v2_a,
        const vec2x4_t* v2_b) {
      
        vec2_simd_t v2_simd_a;
        vec2_simd_t v2_simd_b;

        for (
            u32 index = 0;
            index < count;
            ++index) {
        
            // simdify the vectors
            vec2_simd_load(v2_a[index], v2_simd_a);
            vec2_simd_load(v2_b[index], v2_simd_b);
        
            // a add b
            simd_4f32_a_add_b(v2_simd_a.reg.x, v2_simd_b.reg.x);
            simd_4f32_a_add_b(v2_simd_a.reg.y, v2_simd_b.reg.y);

            // store vector a
            vec2_simd_store(v2_a[index], v2_simd_a);
        }
    }

    void
    vec2_simd_a_sub_b(
        const u32       count,
        vec2x4_t*       v2_a,
        const vec2x4_t* v2_b) {
      
        vec2_simd_t v2_simd_a;
        vec2_simd_t v2_simd_b;

        for (
            u32 index = 0;
            index < count;
            ++index) {
        
            // simdify the vectors
            vec2_simd_load(v2_a[index], v2_simd_a);
            vec2_simd_load(v2_b[index], v2_simd_b);
        
            // a add b
            simd_4f32_a_sub_b(v2_simd_a.reg.x, v2_simd_b.reg.x);
            simd_4f32_a_sub_b(v2_simd_a.reg.y, v2_simd_b.reg.y);

            // store vector a
            vec2_simd_store(v2_a[index], v2_simd_a);
        }
    }

    void
    vec2_simd_a_dot_b(
        const u32         count,
        const vec2x4_t*   v2_a,
        const vec2x4_t*   v2_b,
        simd_4f32_data_t* dot) {

        vec2_simd_t     v2_simd_a;
        vec2_simd_t     v2_simd_b;
        vec2_simd_t     v2_simd_ab;
        simd_4f32_reg_t simd_reg_dot;

        for (
            u32 index = 0;
            index < count;
            ++index) {
        
            // simdify the vectors
            vec2_simd_load(v2_a[index], v2_simd_a);
            vec2_simd_load(v2_b[index], v2_simd_b);
        
            // dot = axbx + ayby
            simd_4f32_a_mul_b_to_c (v2_simd_a.reg.x,  v2_simd_b.reg.x,  v2_simd_ab.reg.x);
            simd_4f32_a_mul_b_to_c (v2_simd_a.reg.y,  v2_simd_b.reg.y,  v2_simd_ab.reg.y);
            simd_4f32_a_add_b_to_c (v2_simd_ab.reg.x, v2_simd_ab.reg.y, simd_reg_dot);
        
            // store the dot product
            simd_4f32_store(simd_reg_dot, dot[index]);
        }
    }

    void
    vec2_simd_a_add_b_to_c(
        const u32       count,
        const vec2x4_t* v2_a,
        const vec2x4_t* v2_b,
        vec2x4_t*       v2_c) {

    }

    void
    vec2_simd_a_sub_b_to_c(
        const u32       count,
        const vec2x4_t* v2_a,
        const vec2x4_t* v2_b,
        vec2x4_t*       v2_c) {

    }

    void
    vec2_simd_a_dot_b_to_c(
        const u32       count,
        const vec2x4_t* v2_a,
        const vec2x4_t* v2_b,
        vec2x4_t*       v2_c) {

    }

    void
    vec2_simd_a_cross_b_to_c(
        const u32       count,
        const vec2x4_t* v2_a,
        const vec2x4_t* v2_b,
        vec2x4_t*       v2_c) {

    }

    SLD_INLINE void
    vec2_simd_load(
        const vec2x4_t& v2,
        vec2_simd_t&    v2_simd) {

        v2_simd.data.x[0] = v2.array_vec[0].x;
        v2_simd.data.x[1] = v2.array_vec[1].x;
        v2_simd.data.x[2] = v2.array_vec[2].x;
        v2_simd.data.x[3] = v2.array_vec[3].x;

        v2_simd.data.y[0] = v2.array_vec[0].y;
        v2_simd.data.y[1] = v2.array_vec[1].y;
        v2_simd.data.y[2] = v2.array_vec[2].y;
        v2_simd.data.y[3] = v2.array_vec[3].y;

        simd_4f32_load (v2_simd.data.x, v2_simd.reg.x);
        simd_4f32_load (v2_simd.data.y, v2_simd.reg.y);
    }
    
    SLD_INLINE void
    vec2_simd_store(
        vec2x4_t&          v2,
        const vec2_simd_t& v2_simd) {

        // store the data
        simd_4f32_store (v2_simd.reg.x, v2_simd.data.x);
        simd_4f32_store (v2_simd.reg.y, v2_simd.data.y);

        // update the vectors
        v2.array_vec[0].x = v2_simd.data.x.array[0]; 
        v2.array_vec[0].y = v2_simd.data.y.array[0];
        v2.array_vec[1].x = v2_simd.data.x.array[1];
        v2.array_vec[1].y = v2_simd.data.y.array[1];
        v2.array_vec[2].x = v2_simd.data.x.array[2];
        v2.array_vec[2].y = v2_simd.data.y.array[2];
        v2.array_vec[3].x = v2_simd.data.x.array[3];
        v2.array_vec[3].y = v2_simd.data.y.array[3];
    }
};
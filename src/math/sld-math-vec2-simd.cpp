#pragma once

#include "sld-math.hpp"

namespace sld {

    void
    vec2_simd_normalize(
        const u32 count,
        vec2x4_t* v2) {

        f32x4_t simd_a; 
        f32x4_t simd_b; 
        f32x4_t simd_c; 

        for (
            u32 index = 0;
            index < count;
            ++index) {
 
            // get the next vector
            vec2x4_t& v2_i  = v2[index];

            // load x components
            simd_a.array[0] = v2_i.array_vec[0].x;
            simd_a.array[1] = v2_i.array_vec[1].x;
            simd_a.array[2] = v2_i.array_vec[2].x;
            simd_a.array[3] = v2_i.array_vec[3].x;

            // load y components
            simd_b.array[0] = v2_i.array_vec[0].y;
            simd_b.array[1] = v2_i.array_vec[1].y;
            simd_b.array[2] = v2_i.array_vec[2].y;
            simd_b.array[3] = v2_i.array_vec[3].y;

            // square components
            simd_a_mul_b_to_c(simd_a, simd_a, simd_a);
            simd_a_mul_b_to_c(simd_b, simd_b, simd_b);



            const f32 xx_0        = (x * v2_i.array_vec[0].x);
            const f32 xx_1        = (x * v2_i.array_vec[1].x);
            const f32 xx_2        = (x * v2_i.array_vec[2].x);
            const f32 xx_3        = (x * v2_i.array_vec[3].x);

            const f32 yy_0        = (v2_i.array_vec[0].y * v2_i.array_vec[0].y);
            const f32 yy_1        = (v2_i.array_vec[1].y * v2_i.array_vec[1].y);
            const f32 yy_2        = (v2_i.array_vec[2].y * v2_i.array_vec[2].y);
            const f32 yy_3        = (v2_i.array_vec[3].y * v2_i.array_vec[3].y);

            const f32 xx_add_yy_0 = (xx_0 + yy_0); 
            const f32 xx_add_yy_1 = (xx_1 + yy_1); 
            const f32 xx_add_yy_2 = (xx_2 + yy_2); 
            const f32 xx_add_yy_3 = (xx_3 + yy_3); 

            const f32 m_0         = sqrtf(xx_add_yy_0);        
            const f32 m_1         = sqrtf(xx_add_yy_1);        
            const f32 m_2         = sqrtf(xx_add_yy_2);        
            const f32 m_3         = sqrtf(xx_add_yy_3);        

            const f32 s_0         = (1.0f / m_0);
            const f32 s_1         = (1.0f / m_1);
            const f32 s_2         = (1.0f / m_2);
            const f32 s_3         = (1.0f / m_3);

            v2_i.array_vec[0].x  *= s_0; 
            v2_i.array_vec[0].y  *= s_0;
            v2_i.array_vec[1].x  *= s_1; 
            v2_i.array_vec[1].y  *= s_1;
            v2_i.array_vec[2].x  *= s_2; 
            v2_i.array_vec[2].y  *= s_2;
            v2_i.array_vec[3].x  *= s_3; 
            v2_i.array_vec[3].y  *= s_3;
        }
    }

    void
    vec2_simd_magnitude(
        const u32     count,
        const vec2x4_t* v2,
        f32x4_t*          m) {

    }

    void
    vec2_simd_scalar_mul(
        const u32      count,
        vec2x4_t*      v2,
        const f32x4_t* s) {

    }

    void
    vec2_simd_scalar_div(
        const u32      count,
        vec2x4_t*      v2,
        const f32x4_t* s) {

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

    }

    void
    vec2_simd_a_sub_b(
        const u32       count,
        vec2x4_t*       v2_a,
        const vec2x4_t* v2_b) {

    }

    void
    vec2_simd_a_dot_b(
        const u32     count,
        vec2x4_t*       v2_a,
        const vec2x4_t* v2_b) {

    }

    void
    vec2_simd_a_cross_b(
        const u32       count,
        vec2x4_t*       v2_a,
        const vec2x4_t* v2_b) {

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
};
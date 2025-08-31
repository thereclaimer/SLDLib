#pragma once

#include "sld-math.hpp"

namespace sld {

    void
    vec2_simd_normalize(
        const u32 count,
        vec2x4_t* v2) {

        simd_4f32_data_t simd_data_vec_x; 
        simd_4f32_data_t simd_data_vec_y; 

        simd_4f32_reg_t simd_reg_x;
        simd_4f32_reg_t simd_reg_y;
        simd_4f32_reg_t simd_reg_xx;
        simd_4f32_reg_t simd_reg_yy;
        simd_4f32_reg_t simd_reg_xx_add_yy;
        simd_4f32_reg_t simd_reg_inv_sqrt;

        for (
            u32 index = 0;
            index < count;
            ++index) {
 
            // get the next vector
            vec2x4_t& v2_i = v2[index];

            // set data
            simd_data_vec_x.array[0] = v2_i.array_vec[0].x;
            simd_data_vec_x.array[1] = v2_i.array_vec[1].x;
            simd_data_vec_x.array[2] = v2_i.array_vec[2].x;
            simd_data_vec_x.array[3] = v2_i.array_vec[3].x;
            simd_data_vec_y.array[0] = v2_i.array_vec[0].y;
            simd_data_vec_y.array[1] = v2_i.array_vec[1].y;
            simd_data_vec_y.array[2] = v2_i.array_vec[2].y;
            simd_data_vec_y.array[3] = v2_i.array_vec[3].y;
            
            // load components
            simd_4f32_load(simd_data_vec_x, simd_reg_x);
            simd_4f32_load(simd_data_vec_y, simd_reg_y);

            // square components
            simd_4f32_a_mul_b(simd_reg_xx, simd_reg_x);
            simd_4f32_a_mul_b(simd_reg_yy, simd_reg_y);

            // add the squared components
            simd_4f32_a_add_b_to_c(simd_reg_xx, simd_reg_yy, simd_reg_xx_add_yy);

            // calculate the inverse square root
            simd_4f32_inv_sqrt(simd_reg_inv_sqrt, simd_reg_xx_add_yy);

            // multiply the components 
            simd_4f32_a_mul_b(simd_reg_x, simd_reg_inv_sqrt);
            simd_4f32_a_mul_b(simd_reg_y, simd_reg_inv_sqrt);

            // store the data
            simd_4f32_store(simd_reg_x, simd_data_vec_x);
            simd_4f32_store(simd_reg_y, simd_data_vec_y);

            // update the vectors
            v2_i.array_vec[0].x = simd_data_vec_x.array[0]; 
            v2_i.array_vec[0].y = simd_data_vec_y.array[0];
            v2_i.array_vec[1].x = simd_data_vec_x.array[1];
            v2_i.array_vec[1].y = simd_data_vec_y.array[1];
            v2_i.array_vec[2].x = simd_data_vec_x.array[2];
            v2_i.array_vec[2].y = simd_data_vec_y.array[2];
            v2_i.array_vec[3].x = simd_data_vec_x.array[3];
            v2_i.array_vec[3].y = simd_data_vec_y.array[3];
        }
    }

    void
    vec2_simd_magnitude(
        const u32         count,
        const vec2x4_t*   v2,
        simd_4f32_data_t* m) {

        simd_4f32_data_t simd_data_vec_x; 
        simd_4f32_data_t simd_data_vec_y; 

        simd_4f32_reg_t simd_reg_x;
        simd_4f32_reg_t simd_reg_y;
        simd_4f32_reg_t simd_reg_xx;
        simd_4f32_reg_t simd_reg_yy;
        simd_4f32_reg_t simd_reg_xx_add_yy;
        simd_4f32_reg_t simd_reg_mag;

        for (
            u32 index = 0;
            index < count;
            ++index) {

            // get the next vector
            vec2x4_t& v2_i = v2[index];

            // set data
            simd_data_vec_x.array[0] = v2_i.array_vec[0].x;
            simd_data_vec_x.array[1] = v2_i.array_vec[1].x;
            simd_data_vec_x.array[2] = v2_i.array_vec[2].x;
            simd_data_vec_x.array[3] = v2_i.array_vec[3].x;
            simd_data_vec_y.array[0] = v2_i.array_vec[0].y;
            simd_data_vec_y.array[1] = v2_i.array_vec[1].y;
            simd_data_vec_y.array[2] = v2_i.array_vec[2].y;
            simd_data_vec_y.array[3] = v2_i.array_vec[3].y;
            
            // load components
            simd_4f32_load(simd_data_vec_x, simd_reg_x);
            simd_4f32_load(simd_data_vec_y, simd_reg_y);

            // square components
            simd_4f32_a_mul_b(simd_reg_xx, simd_reg_x);
            simd_4f32_a_mul_b(simd_reg_yy, simd_reg_y);

            // add the squared components
            simd_4f32_a_add_b_to_c(simd_reg_xx, simd_reg_yy, simd_reg_xx_add_yy);

            // calculate the magnitude
            simd_4f32_sqrt(simd_reg_xx_add_yy, simd_reg_mag);

            // store the magnitude
            simd_4f32_data_t& m_i = m[index]; 
            simd_4f32_store(simd_reg_mag, m_i);
        }
    }

    void
    vec2_simd_scalar_mul(
        const u32      count,
        vec2x4_t*      v2,
        const simd_4f32_t* s) {

    }

    void
    vec2_simd_scalar_div(
        const u32      count,
        vec2x4_t*      v2,
        const simd_4f32_t* s) {

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
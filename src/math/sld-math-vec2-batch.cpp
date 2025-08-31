#pragma once

#include "sld-math.hpp"

namespace sld {

    void
    vec2_batch_normalize(
        const u32 count,
        vec2_t*   v2) {

        for (
            u32 index = 0;
            index < count;
            ++index) {

            const f32 m = sqrtf(
                (v2[index].x * v2[index].x) +
                (v2[index].y * v2[index].y)
            );        

            const f32 s = 1.0f / m;

            v2[index].x *= s;
            v2[index].y *= s;
        }

    }

    void
    vec2_batch_magnitude(
        const u32     count,
        const vec2_t* v2,
        f32*          m) {

        for (
            u32 index = 0;
            index < count;
            ++index) {

            m[index] = sqrtf(
                (v2[index].x * v2[index].x) +
                (v2[index].y * v2[index].y)
            );
        }
    }

    void
    vec2_batch_scalar_mul(
        const u32  count,
        vec2_t*    v2,
        const f32* s) {

        for (
            u32 index = 0;
            index < count;
            ++index) {

            v2[index].x *= s[index];
            v2[index].y *= s[index];
        }
    }

    void
    vec2_batch_scalar_div(
        const u32  count,
        vec2_t*    v2,
        const f32* s) {

        for (
            u32 index = 0;
            index < count;
            ++index) {

            const f32 s_inv = 1.0f / s[index];

            v2[index].x *= s_inv;
            v2[index].y *= s_inv;  
        }
    }

    void
    vec2_batch_scalar_mul_uniform(
        const u32 count,
        vec2_t*   v2,
        const f32 s) {

        for (
            u32 index = 0;
            index < count;
            ++index) {

            v2[index].x *= s;
            v2[index].y *= s;
        }
    }
   
    void
    vec2_batch_scalar_div_uniform(
        const u32 count,
        vec2_t*   v2,
        const f32 s) {

        const f32 s_inv = 1.0f / s[index];

        for (
            u32 index = 0;
            index < count;
            ++index) {

            v2[index].x *= s_inv;
            v2[index].y *= s_inv;  
        }
    }

    void
    vec2_batch_scalar_mul_new(
        const u32     count,
        const vec2_t* v2,
        const f32*    s,
        vec2_t*       v2_new) {

        for (
            u32 index = 0;
            index < count;
            ++index) {

            v2_new[index].x = v2[index].x * s[index];
            v2_new[index].y = v2[index].y * s[index];
        }
    }

    void
    vec2_batch_scalar_div_new(
        const u32     count,
        const vec2_t* v2,
        const f32*    s,
        vec2_t*       v2_new) {

        for (
            u32 index = 0;
            index < count;
            ++index) {

            const f32 s_inv = 1.0f / s[index];

            v2_new[index].x = v2[index].x * s_inv;
            v2_new[index].y = v2[index].y * s_inv;  
        }
    }

    void
    vec2_batch_scalar_mul_new_uniform(
        const u32 count,
        vec2_t*   v2,
        const f32 s,
        vec2_t*   v2_new) {

        for (
            u32 index = 0;
            index < count;
            ++index) {

            v2_new[index].x = v2[index].x * s;
            v2_new[index].y = v2[index].y * s;
        }
    }

    void
    vec2_batch_scalar_div_new_uniform(
        const u32 count,
        vec2_t*   v2,
        const f32 s,
        vec2_t*   v2_new) {

        const f32 s_inv = 1.0f / s;

        for (
            u32 index = 0;
            index < count;
            ++index) {


            v2_new[index].x = v2[index].x * s_inv;
            v2_new[index].y = v2[index].y * s_inv;  
        }
    }

    void
    vec2_batch_a_add_b(
        const u32     count,
        vec2_t*       v2_a,
        const vec2_t* v2_b) {

        for (
            u32 index = 0;
            index < count;
            ++index) {

        }
    }

    void
    vec2_batch_a_sub_b(
        const u32     count,
        vec2_t*       v2_a,
        const vec2_t* v2_b) {

        for (
            u32 index = 0;
            index < count;
            ++index) {

            v2_a[index].x += v2_b[index].x;
            v2_a[index].y += v2_b[index].y;
        }
    }

    void
    vec2_batch_a_dot_b(
        const u32     count,
        vec2_t*       v2_a,
        const vec2_t* v2_b,
        f32*          dot) {

        for (
            u32 index = 0;
            index < count;
            ++index) {
            
            dot[index] = (
                (v2_a[index].x * v2_b[index].x) + 
                (v2_a[index].y * v2_b[index].y) 
            );
        }
    }

    void
    vec2_batch_a_add_b_to_c(
        const u32     count,
        const vec2_t* v2_a,
        const vec2_t* v2_b,
        vec2_t*       v2_c) {

        for (
            u32 index = 0;
            index < count;
            ++index) {

            v2_c[index].x = v2_a[index].x + v2_b[index].x;
            v2_c[index].y = v2_a[index].y + v2_b[index].y;
        }
    }

    void
    vec2_batch_a_sub_b_to_c(
        const u32     count,
        const vec2_t* v2_a,
        const vec2_t* v2_b,
        vec2_t*       v2_c) {

        for (
            u32 index = 0;
            index < count;
            ++index) {

            v2_c[index].x = v2_a[index].x - v2_b[index].x;
            v2_c[index].y = v2_a[index].y - v2_b[index].y;
        }
    }
};
#pragma once

#include "sld-math.hpp"
#include <math.h>

namespace sld {

    void
    vec2_normalize(
        vec2_t& v2) {

        const f32 m = sqrtf(
            (v2.x * v2.x) +
            (v2.y * v2.y)
        );        

        const f32 s = 1.0f / m;

        v2.x *= s;
        v2.y *= s;
    }

    void
    vec2_magnitude(
        const vec2_t& v2,
        f32&          m) {

        m = sqrtf(
            (v2.x * v2.x) +
            (v2.y * v2.y)
        );
    }

    void
    vec2_scalar_mul(
        vec2_t&   v2,
        const f32 s) {
    
        v2.x *= s;
        v2.y *= s;
    }

    void
    vec2_scalar_div(
        vec2_t&   v2,
        const f32 s) {

        const f32 s_inv = 1.0f / s;

        v2.x *= s_inv;
        v2.y *= s_inv;        
    }

    void
    vec2_scalar_mul_new(
        vec2_t&   v2,
        const f32 s,
        vec2_t&   v2_new) {

        v2_new.x = v2.x * s;
        v2_new.y = v2.y * s;
    }

    void
    vec2_scalar_div_new(
        vec2_t&   v2,
        const f32 s,
        vec2_t&   v2_new) {

        const f32 s_inv = 1.0f / s;
        
        v2_new.x = v2.x * s;
        v2_new.y = v2.y * s;
    }

    void
    vec2_a_add_b(
        vec2_t&       v2_a,
        const vec2_t& v2_b) {

        v2_a.x += v2_b.x;
        v2_a.y += v2_b.y;
    }

    void
    vec2_a_sub_b(
        vec2_t&       v2_a,
        const vec2_t& v2_b) {

        v2_a.x -= v2_b.x;
        v2_a.y -= v2_b.y;

    }

    void
    vec2_a_dot_b(
        vec2_t&       v2_a,
        const vec2_t& v2_b,
        f32&          dot) {

        dot = (
            (v2_a.x * v2_b.x) + 
            (v2_a.y * v2_b.y) 
        );
    }

    void
    vec2_a_add_b_to_c(
        const vec2_t& v2_a,
        const vec2_t& v2_b,
        vec2_t&       v2_c) {

        v2_c.x = v2_a.x + v2_b.x;
        v2_c.y = v2_a.y + v2_b.y;
    }

    void
    vec2_a_sub_b_to_c(
        const vec2_t& v2_a,
        const vec2_t& v2_b,
        vec2_t&       v2_c) {

        v2_c.x = v2_a.x - v2_b.x;
        v2_c.y = v2_a.y - v2_b.y;
    }
};
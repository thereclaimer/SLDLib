#ifndef SLD_MATH_HPP
#define SLD_MATH_HPP

#include "sld.hpp"
#include "sld-simd.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    struct vec2_t;     // 2D Vector
    struct vec2x4_t;   // 2D Vector, batch of 4
    struct vec3_t;     // 3D Vector
    struct vec3x4_t;   // 3D Vector, batch of 4
    struct quat_t;     // Quaternion
    struct mat3_t;     // 3x3 Matrix
    struct mat3_row_t; // 3x3 Matrix Row
    struct mat3_col_t; // 3x3 Matrix Column
    struct mat4_t;     // 4x4 Matrix
    struct mat4_row_t; // 4x4 Matrix Row
    struct mat4_col_t; // 4x4 Matrix Column

    //-------------------------------------------------------------------
    // VECTOR 2D
    //-------------------------------------------------------------------

    void vec2_normalize                     (vec2_t&       v3);
    void vec2_magnitude                     (const vec2_t& v3,         f32&    m);
    void vec2_scalar_mul                    (vec2_t&       v2,   const f32     s);
    void vec2_scalar_div                    (vec2_t&       v2,   const f32     s);
    void vec2_scalar_mul_new                (vec2_t&       v2,   const f32     s, vec2_t& v2_new);
    void vec2_scalar_div_new                (vec2_t&       v2,   const f32     s, vec2_t& v2_new);
    void vec2_a_add_b                       (vec2_t&       v2_a, const vec2_t& v2_b);
    void vec2_a_sub_b                       (vec2_t&       v2_a, const vec2_t& v2_b);
    void vec2_a_dot_b                       (vec2_t&       v2_a, const vec2_t& v2_b, f32&    dot);
    void vec2_a_add_b_to_c                  (const vec2_t& v2_a, const vec2_t& v2_b, vec2_t& v2_c); 
    void vec2_a_sub_b_to_c                  (const vec2_t& v2_a, const vec2_t& v2_b, vec2_t& v2_c); 

    void vec2_batch_normalize               (const u32 count, vec2_t*       v2);
    void vec2_batch_magnitude               (const u32 count, const vec2_t* v2,         f32*    m);
    void vec2_batch_scalar_mul              (const u32 count, vec2_t*       v2,   const f32*    s);
    void vec2_batch_scalar_div              (const u32 count, vec2_t*       v2,   const f32*    s);
    void vec2_batch_scalar_mul_uniform      (const u32 count, vec2_t*       v2,   const f32     s);
    void vec2_batch_scalar_div_uniform      (const u32 count, vec2_t*       v2,   const f32     s);
    void vec2_batch_scalar_mul_new          (const u32 count, const vec2_t* v2,   const f32*    s, vec2_t* v2_new);
    void vec2_batch_scalar_div_new          (const u32 count, const vec2_t* v2,   const f32*    s, vec2_t* v2_new);
    void vec2_batch_scalar_mul_new_uniform  (const u32 count, vec2_t*       v2,   const f32     s, vec2_t* v2_new);
    void vec2_batch_scalar_div_new_uniform  (const u32 count, vec2_t*       v2,   const f32     s, vec2_t* v2_new);
    void vec2_batch_a_add_b                 (const u32 count, vec2_t*       v2_a, const vec2_t* v2_b);
    void vec2_batch_a_sub_b                 (const u32 count, vec2_t*       v2_a, const vec2_t* v2_b);
    void vec2_batch_a_dot_b                 (const u32 count, vec2_t*       v2_a, const vec2_t* v2_b, f32*    dot);
    void vec2_batch_a_add_b_to_c            (const u32 count, const vec2_t* v2_a, const vec2_t* v2_b, vec2_t* v2_c); 
    void vec2_batch_a_sub_b_to_c            (const u32 count, const vec2_t* v2_a, const vec2_t* v2_b, vec2_t* v2_c); 

    void vec2_simd_normalize                (const u32 count, vec2x4_t*       v2);
    void vec2_simd_magnitude                (const u32 count, const vec2x4_t* v2,         simd_4f32_data_t*  m);
    void vec2_simd_scalar_mul               (const u32 count, vec2x4_t*       v2,   const simd_4f32_data_t*  s);
    void vec2_simd_scalar_div               (const u32 count, vec2x4_t*       v2,   const simd_4f32_data_t*  s);
    void vec2_simd_scalar_mul_uniform       (const u32 count, vec2x4_t*       v2,   const f32       s);
    void vec2_simd_scalar_div_uniform       (const u32 count, vec2x4_t*       v2,   const f32       s);
    void vec2_simd_scalar_mul_new           (const u32 count, const vec2x4_t* v2,   const simd_4f32_data_t*  s, vec2x4_t* v2_new);
    void vec2_simd_scalar_div_new           (const u32 count, const vec2x4_t* v2,   const simd_4f32_data_t*  s, vec2x4_t* v2_new);
    void vec2_simd_scalar_mul_new_uniform   (const u32 count, vec2x4_t*       v2,   const f32       s, vec2x4_t* v2_new);
    void vec2_simd_scalar_div_new_uniform   (const u32 count, vec2x4_t*       v2,   const f32       s, vec2x4_t* v2_new);
    void vec2_simd_a_add_b                  (const u32 count, vec2x4_t*       v2_a, const vec2x4_t* v2_b);
    void vec2_simd_a_sub_b                  (const u32 count, vec2x4_t*       v2_a, const vec2x4_t* v2_b);
    void vec2_simd_a_dot_b                  (const u32 count, vec2x4_t*       v2_a, const vec2x4_t* v2_b);
    void vec2_simd_a_cross_b                (const u32 count, vec2x4_t*       v2_a, const vec2x4_t* v2_b);
    void vec2_simd_a_add_b_to_c             (const u32 count, const vec2x4_t* v2_a, const vec2x4_t* v2_b, vec2x4_t* v2_c); 
    void vec2_simd_a_sub_b_to_c             (const u32 count, const vec2x4_t* v2_a, const vec2x4_t* v2_b, vec2x4_t* v2_c); 
    void vec2_simd_a_dot_b_to_c             (const u32 count, const vec2x4_t* v2_a, const vec2x4_t* v2_b, vec2x4_t* v2_c); 
    void vec2_simd_a_cross_b_to_c           (const u32 count, const vec2x4_t* v2_a, const vec2x4_t* v2_b, vec2x4_t* v2_c); 

    struct vec2_t {
        union {
            struct {
                f32 x;
                f32 y;
            };
            f32 array[2];
        };
    };

    struct SLD_SIMD_ALIGN_4F32 vec2x4_t {
        union {
            vec2_t array_vec        [4];
            f32    array_components [8];
        };
    };


    //-------------------------------------------------------------------
    // VECTOR 3D
    //-------------------------------------------------------------------

    void vec3_normalize                     (vec3_t&       v3);
    void vec3_magnitude                     (const vec3_t& v3,         f32&    m);
    void vec3_scalar_mul                    (vec3_t&       v3,   const f32     s);
    void vec3_scalar_div                    (vec3_t&       v3,   const f32     s);
    void vec3_scalar_mul_new                (vec3_t&       v3,   const f32     s, vec3_t& v3_new);
    void vec3_scalar_div_new                (vec3_t&       v3,   const f32     s, vec3_t& v3_new);
    void vec3_a_add_b                       (vec3_t&       v3_a, const vec3_t& v3_b);
    void vec3_a_sub_b                       (vec3_t&       v3_a, const vec3_t& v3_b);
    void vec3_a_dot_b                       (vec2_t&       v2_a, const vec2_t& v2_b);
    void vec3_a_cross_b                     (vec2_t&       v2_a, const vec2_t& v2_b);
    void vec3_a_add_b_to_c                  (const vec3_t& v3_a, const vec3_t& v3_b, vec3_t& v3_c); 
    void vec3_a_sub_b_to_c                  (const vec3_t& v3_a, const vec3_t& v3_b, vec3_t& v3_c); 
    void vec2_a_dot_b_to_c                  (const vec3_t& v3_a, const vec3_t& v3_b, vec3_t& v3_c);
    void vec3_a_cross_b_to_c                (const vec3_t& v3_a, const vec3_t& v3_b, vec3_t& v3_c);

    void vec3_batch_normalize               (const u32 count, vec3_t*       v3);
    void vec3_batch_magnitude               (const u32 count, const vec3_t* v3,         f32*    m);
    void vec3_batch_scalar_mul              (const u32 count, vec3_t*       v3,   const f32*    s);
    void vec3_batch_scalar_div              (const u32 count, vec3_t*       v3,   const f32*    s);
    void vec3_batch_scalar_mul_uniform      (const u32 count, vec3_t*       v3,   const f32     s);
    void vec3_batch_scalar_div_uniform      (const u32 count, vec3_t*       v3,   const f32     s);
    void vec3_batch_scalar_mul_new          (const u32 count, const vec3_t* v3,   const f32*    s, vec3_t* v3_new);
    void vec3_batch_scalar_div_new          (const u32 count, const vec3_t* v3,   const f32*    s, vec3_t* v3_new);
    void vec3_batch_scalar_mul_new_uniform  (const u32 count, vec3_t*       v3,   const f32     s, vec3_t* v3_new);
    void vec3_batch_scalar_div_new_uniform  (const u32 count, vec3_t*       v3,   const f32     s, vec3_t* v3_new);
    void vec3_batch_a_add_b                 (const u32 count, vec3_t*       v3_a, const vec3_t* v3_b);
    void vec3_batch_a_sub_b                 (const u32 count, vec3_t*       v3_a, const vec3_t* v3_b);
    void vec3_batch_a_dot_b                 (const u32 count, vec3_t*       v3_a, const vec3_t* v3_b);
    void vec3_batch_a_cross_b               (const u32 count, vec3_t*       v3_a, const vec3_t* v3_b);
    void vec3_batch_a_add_b_to_c            (const u32 count, const vec3_t* v3_a, const vec3_t* v3_b, vec3_t* v3_c); 
    void vec3_batch_a_sub_b_to_c            (const u32 count, const vec3_t* v3_a, const vec3_t* v3_b, vec3_t* v3_c); 
    void vec3_batch_a_dot_b_to_c            (const u32 count, const vec3_t* v3_a, const vec3_t* v3_b, vec3_t* v3_c); 
    void vec3_batch_a_cross_b_to_c          (const u32 count, const vec3_t* v3_a, const vec3_t* v3_b, vec3_t* v3_c); 

    void vec3_simd_normalize                (const u32 count, vec3x4_t*       v3);
    void vec3_simd_magnitude                (const u32 count, const vec3x4_t* v3,         simd_4f32_data_t*  m);
    void vec3_simd_scalar_mul               (const u32 count, vec3x4_t*       v3,   const simd_4f32_data_t*  s);
    void vec3_simd_scalar_div               (const u32 count, vec3x4_t*       v3,   const simd_4f32_data_t*  s);
    void vec3_simd_scalar_mul_uniform       (const u32 count, vec3x4_t*       v3,   const f32       s);
    void vec3_simd_scalar_div_uniform       (const u32 count, vec3x4_t*       v3,   const f32       s);
    void vec3_simd_scalar_mul_new           (const u32 count, const vec3x4_t* v3,   const simd_4f32_data_t*  s, vec3x4_t* v3_new);
    void vec3_simd_scalar_div_new           (const u32 count, const vec3x4_t* v3,   const simd_4f32_data_t*  s, vec3x4_t* v3_new);
    void vec3_simd_scalar_mul_new_uniform   (const u32 count, vec3x4_t*       v3,   const f32       s, vec3x4_t* v3_new);
    void vec3_simd_scalar_div_new_uniform   (const u32 count, vec3x4_t*       v3,   const f32       s, vec3x4_t* v3_new);
    void vec3_simd_a_add_b                  (const u32 count, vec3x4_t*       v3_a, const vec3x4_t* v3_b);
    void vec3_simd_a_sub_b                  (const u32 count, vec3x4_t*       v3_a, const vec3x4_t* v3_b);
    void vec3_simd_a_dot_b                  (const u32 count, vec3x4_t*       v3_a, const vec3x4_t* v3_b);
    void vec3_simd_a_cross_b                (const u32 count, vec3x4_t*       v3_a, const vec3x4_t* v3_b);
    void vec3_simd_a_add_b_to_c             (const u32 count, const vec3x4_t* v3_a, const vec3x4_t* v3_b, vec3x4_t* v3_c); 
    void vec3_simd_a_sub_b_to_c             (const u32 count, const vec3x4_t* v3_a, const vec3x4_t* v3_b, vec3x4_t* v3_c); 
    void vec3_simd_a_dot_b_to_c             (const u32 count, const vec3x4_t* v3_a, const vec3x4_t* v3_b, vec3x4_t* v3_c); 
    void vec3_simd_a_cross_b_to_c           (const u32 count, const vec3x4_t* v3_a, const vec3x4_t* v3_b, vec3x4_t* v3_c); 

    struct vec3_t {
        union {
            struct {
                f32 x;
                f32 y;
                f32 z;
                f32 pad;
            };
            f32 array[4];
        };
    };

    struct SLD_SIMD_ALIGN_4F32 vec3x4_t {
        union {
            vec3_t array_vec        [4];
            f32    array_components [16];
        };
    };

    //-------------------------------------------------------------------
    // MATRIX 3X3
    //-------------------------------------------------------------------

    void mat3_identity     (mat3_t&       m3);
    void mat3_transpose    (mat3_t&       m3);
    f32& mat3_index        (mat3_t&       m3,   const u32 row, const u32 col);
    void mat3_row_to_vec3  (const mat3_t& m3,   const u32 row, vec3_t& v3);
    void mat3_col_to_vec3  (const mat3_t& m3,   const u32 col, vec3_t& v3);
    void mat3_a_mul_b      (mat3_t&       m3_a, const mat3_t& m3_b);
    void mat3_a_mul_b_to_c (const mat3_t& m3_a, const mat3_t& m3_b, mat3_t& m3_c);
    void mat3_mul_vec3     (const mat3_t& m3,   vec3_t& v3);

    struct mat3_col_t {
        union {
            struct {
                f32 row_0;
                f32 row_1;
                f32 row_2;
                f32 pad;
            };
            f32 array[4];
        };
    };

    struct mat3_row_t {
        union {
            struct {
                f32 col_0;
                f32 col_1;
                f32 col_2;
                f32 pad;
            };
            f32 array[4];
        };
    };

    struct mat3_t {
        union {
            struct {
                mat3_row_t row_0;
                mat3_row_t row_1;
                mat3_row_t row_2;
            };
            f32 array[12];
        };
    };

    //-------------------------------------------------------------------
    // MATRIX 4X4
    //-------------------------------------------------------------------

    struct mat4_col_t {
        union {
            struct {
                f32 row_0;
                f32 row_1;
                f32 row_2;
                f32 row_3;
            };
            f32 array[4];
        };
    };

    struct mat4_row_t {
        union {
            struct {
                f32 col_0;
                f32 col_1;
                f32 col_2;
                f32 col_3;
            };
            f32 array[4];
        };
    };

    struct mat4_t {
        union {
            struct {
                mat3_row_t row_0;
                mat3_row_t row_1;
                mat3_row_t row_2;
                mat3_row_t row_3;
            };
            f32 array[16];
        };
    };

    //-------------------------------------------------------------------
    // QUATERNION
    //-------------------------------------------------------------------

    struct quat_t {
        union {
            struct {
                f32 x;
                f32 y;
                f32 z;
                f32 w;
            };
            f32 array[4];
        };
    };

};

#endif //SLD_MATH_HPP

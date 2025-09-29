#ifndef SLD_GEOMETRY_HPP
#define SLD_GEOMETRY_HPP

#include "sld.hpp"

namespace sld {

    struct dims_u32_t;
    struct dims_u32_size_t;
    struct dims_u32_pos_t;

    struct dims_f32_t;
    struct dims_f32_size_t;
    struct dims_f32_pos_t;
    
    SLD_INLINE u32  dims_u32_area              (const dims_u32_size_t& size);
    SLD_INLINE void dims_u32_center            (const dims_u32_size_t& size,      dims_u32_pos_t&        center);
    SLD_INLINE void dims_u32_center_a_inside_b (const dims_u32_size_t& in_size_a, const dims_u32_size_t& in_size_b, dims_u32_pos_t& out_center_a_inside_b);
    
    SLD_INLINE f32  dims_f32_area              (const dims_f32_size_t& size);
    SLD_INLINE void dims_f32_center            (const dims_f32_size_t& size,      dims_f32_pos_t&        center);
    SLD_INLINE void dims_f32_center_a_inside_b (const dims_f32_size_t& in_size_a, const dims_f32_size_t& in_size_b, dims_f32_pos_t& out_center_a_inside_b);

    struct dims_u32_size_t {
        u32 width;
        u32 height;
    };

    struct dims_u32_pos_t {
        u32 x;
        u32 y;
    };

    struct dims_u32_t {
        dims_u32_size_t size;
        dims_u32_pos_t  pos;
    };

    SLD_INLINE u32
    dims_u32_area(
        const dims_u32_size_t& size) {

        const u32 area = (size.width * size.height);
        return(area);
    }

    SLD_INLINE void
    dims_u32_center(
        const dims_u32_size_t& size,
        dims_u32_pos_t&        center) {
        
        center.x = (size.width  / 2);
        center.y = (size.height / 2);
    }

    SLD_INLINE void
    dims_u32_center_a_inside_b(
        const dims_u32_size_t& in_size_a,
        const dims_u32_size_t& in_size_b,
        dims_u32_pos_t&        out_center_a_inside_b) {

        dims_u32_pos_t center_a;
        dims_u32_pos_t center_b;
        dims_u32_center(in_size_a, center_a);        
        dims_u32_center(in_size_b, center_b);        

        out_center_a_inside_b.x = (center_b.x - center_a.x);
        out_center_a_inside_b.y = (center_b.y - center_a.y);
    }

    struct dims_f32_size_t {
        f32 width;
        f32 height;
    };

    struct dims_f32_pos_t {
        f32 x;
        f32 y;
    };

    struct dims_f32_t {
        dims_f32_size_t size;
        dims_f32_pos_t  pos;
    };

    SLD_INLINE f32
    dims_f32_area(
        const dims_f32_size_t& size) {

        const f32 area = (size.width * size.height);
        return(area);
    }

    SLD_INLINE void
    dims_f32_center(
        const dims_f32_size_t& size,
        dims_f32_pos_t&        center) {
        
        center.x = (size.width  / 2.0f);
        center.y = (size.height / 2.0f);
    }

    SLD_INLINE void
    dims_f32_center_a_inside_b(
        const dims_f32_size_t& in_size_a,
        const dims_f32_size_t& in_size_b,
        dims_f32_pos_t&        out_center_a_inside_b) {

        dims_f32_pos_t center_a;
        dims_f32_pos_t center_b;
        dims_f32_center(in_size_a, center_a);        
        dims_f32_center(in_size_b, center_b);        

        out_center_a_inside_b.x = (center_b.x - center_a.x);
        out_center_a_inside_b.y = (center_b.y - center_a.y);
    }
};

#endif //SLD_GEOMETRY_HPP
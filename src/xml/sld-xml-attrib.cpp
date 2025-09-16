#pragma once

#include "sld-xml-internal.cpp"

namespace sld {
    
    SLD_API const xml_utf8_t*
    xml_attrib_get_value_utf8(
        const xml_hnd_attrib_t h_attrib) {

        xml_attrib_t* attrib = xml_memory_get_ptr_attrib(h_attrib);
        assert(attrib);

        const xml_utf8_t* val = attrib->pugi.value();
        return(val);
    }

    SLD_API const u32
    xml_attrib_get_value_u32(
        const xml_hnd_attrib_t h_attrib) {

        xml_attrib_t* attrib = xml_memory_get_ptr_attrib(h_attrib);
        assert(attrib);

        const u32 val = attrib->pugi.as_uint();
        return(val);
    }

    SLD_API const u64
    xml_attrib_get_value_u64(
        const xml_hnd_attrib_t h_attrib) {

        xml_attrib_t* attrib = xml_memory_get_ptr_attrib(h_attrib);
        assert(attrib);

        const u64 val = attrib->pugi.as_ullong();
        return(val);
    }

    SLD_API const f32
    xml_attrib_get_value_f32(
        const xml_hnd_attrib_t h_attrib) {

        xml_attrib_t* attrib = xml_memory_get_ptr_attrib(h_attrib);
        assert(attrib);

        const f32 val = attrib->pugi.as_float();
        return(val);
    }

    SLD_API bool
    xml_attrib_set_value_utf8(
        const xml_hnd_attrib_t h_attrib,
        const xml_utf8_t*      value) {

        xml_attrib_t* attrib = xml_memory_get_ptr_attrib(h_attrib);
        assert(attrib);

        const bool is_set = attrib->pugi.set_value(value);
        return(is_set);
    }

    SLD_API bool
    xml_attrib_set_value_u32(
        const xml_hnd_attrib_t h_attrib,
        const u32              value) {

        xml_attrib_t* attrib = xml_memory_get_ptr_attrib(h_attrib);
        assert(attrib);

        const bool is_set = attrib->pugi.set_value(value);
        return(is_set);
    }

    SLD_API bool
    xml_attrib_set_value_u64(
        const xml_hnd_attrib_t h_attrib,
        const u64              value) {

        xml_attrib_t* attrib = xml_memory_get_ptr_attrib(h_attrib);
        assert(attrib);

        const bool is_set = attrib->pugi.set_value(value);
        return(is_set);
    }

    SLD_API bool
    xml_attrib_set_value_f32(
        const xml_hnd_attrib_t h_attrib,
        const f32              value) {

        xml_attrib_t* attrib = xml_memory_get_ptr_attrib(h_attrib);
        assert(attrib);

        const bool is_set = attrib->pugi.set_value(value);
        return(is_set);
    }

};
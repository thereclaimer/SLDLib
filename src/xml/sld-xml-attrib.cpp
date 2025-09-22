#pragma once

#include "sld-xml-internal.cpp"

namespace sld {
    

    SLD_API bool
    xml_attrib_set_val_utf8(
        const xml_hnd_node_t      h_node,
        const xml_utf8_t*         name,
        const xml_attrib_value_t& value) {

        xml_node_t* node = xml_memory_get_ptr_node(h_node);
        assert(node);

        pugi::xml_attribute attrib = node->pugi.attribute(name);
        if (attrib == NULL) {
            attrib = node->pugi.append_attribute(name);
            assert(attrib);
        }

        bool is_set = attrib.set_value(value.as_utf8);
        return(is_set);
    }

    SLD_API bool
    xml_attrib_set_val_u32(
        const xml_hnd_node_t          h_node,
        const xml_utf8_t*             name,
        const xml_attrib_value_t& value) {

        xml_node_t* node = xml_memory_get_ptr_node(h_node);
        assert(node);

        pugi::xml_attribute attrib = node->pugi.attribute(name);
        if (attrib == NULL) {
            attrib = node->pugi.append_attribute(name);
            assert(attrib);
        }

        bool is_set = attrib.set_value(value.as_u32);
        return(is_set);
    }

    SLD_API bool
    xml_attrib_set_val_u64(
        const xml_hnd_node_t          h_node,
        const xml_utf8_t*             name,
        const xml_attrib_value_t& value) {

        xml_node_t* node = xml_memory_get_ptr_node(h_node);
        assert(node);

        pugi::xml_attribute attrib = node->pugi.attribute(name);
        if (attrib == NULL) {
            attrib = node->pugi.append_attribute(name);
            assert(attrib);
        }

        bool is_set = attrib.set_value(value.as_u64);
        return(is_set);
    }

    SLD_API bool
    xml_attrib_set_val_f32(
        const xml_hnd_node_t          h_node,
        const xml_utf8_t*             name,
        const xml_attrib_value_t& value) {

        xml_node_t* node = xml_memory_get_ptr_node(h_node);
        assert(node);

        pugi::xml_attribute attrib = node->pugi.attribute(name);
        if (attrib == NULL) {
            attrib = node->pugi.append_attribute(name);
            assert(attrib);
        }

        bool is_set = attrib.set_value(value.as_f32);
        return(is_set);
    }

    SLD_API bool
    xml_attrib_get_val_utf8(
        const xml_hnd_node_t    h_node,
        const xml_utf8_t*       name,
        xml_attrib_value_t& value) {

        xml_node_t* node = xml_memory_get_ptr_node(h_node);
        assert(node);

        pugi::xml_attribute attrib = node->pugi.attribute(name);
        if (attrib == NULL) {
            return(false);
        }

        value.as_utf8 = (xml_utf8_t*)attrib.as_string();
        return(true);
    }

    SLD_API bool
    xml_attrib_get_val_u32(
        const xml_hnd_node_t    h_node,
        const xml_utf8_t*       name,
        xml_attrib_value_t& value) {

        xml_node_t* node = xml_memory_get_ptr_node(h_node);
        assert(node);

        pugi::xml_attribute attrib = node->pugi.attribute(name);
        if (attrib == NULL) {
            return(false);
        }

        value.as_u32 = attrib.as_uint();
        return(true);
    }

    SLD_API bool
    xml_attrib_get_val_u64(
        const xml_hnd_node_t    h_node,
        const xml_utf8_t*       name,
        xml_attrib_value_t& value) {

        xml_node_t* node = xml_memory_get_ptr_node(h_node);
        assert(node);

        pugi::xml_attribute attrib = node->pugi.attribute(name);
        if (attrib == NULL) {
            return(false);
        }

        value.as_u64 = attrib.as_ullong();
        return(true);
    }

    SLD_API bool
    xml_attrib_get_val_f32(
        const xml_hnd_node_t    h_node,
        const xml_utf8_t*       name,
        xml_attrib_value_t& value) {

        xml_node_t* node = xml_memory_get_ptr_node(h_node);
        assert(node);

        pugi::xml_attribute attrib = node->pugi.attribute(name);
        if (attrib == NULL) {
            return(false);
        }

        value.as_f32 = attrib.as_float();
        return(true);
    }
};
#pragma once

#include "sld-xml-internal.cpp"

namespace sld {

    SLD_API const xml_hnd_node_t
    xml_node_add_sibling(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    sibling_name) {
   
        xml_node_t* node     = xml_memory_get_ptr_node (h_node);
        xml_node_t* node_sib = xml_memory_alloc_node   (node->doc); 
        assert(node != NULL && node_sib != NULL);

        node_sib->pugi = node->pugi.append_child(sibling_name);
        assert(node_sib->pugi != NULL);

        const xml_hnd_node_t hnd = xml_memory_get_hnd_node(node_sib);        
        return(hnd);
    }

    SLD_API const xml_hnd_node_t
    xml_node_get_sibling(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    sibling_name) {
   
        xml_node_t* node     = xml_memory_get_ptr_node (h_node);
        xml_node_t* node_sib = xml_memory_alloc_node   (node->doc); 

        node_sib->pugi = node->pugi.next_sibling(sibling_name);
        assert(node_sib->pugi != NULL);

        const xml_hnd_node_t hnd = xml_memory_get_hnd_node(node_sib);        
        return(hnd);
    }

    SLD_API const xml_hnd_node_t
    xml_node_add_child(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    child_name) {
   
        xml_node_t* node       = xml_memory_get_ptr_node (h_node);
        xml_node_t* node_child = xml_memory_alloc_node   (node->doc); 

        node_child->pugi = node->pugi.append_child(child_name);
        assert(node_child->pugi != NULL);

        const xml_hnd_node_t hnd = xml_memory_get_hnd_node(node_child);        
        return(hnd);
    }

    SLD_API const xml_hnd_node_t
    xml_node_get_child(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    child_name) {
   
        xml_node_t* node       = xml_memory_get_ptr_node (h_node);
        xml_node_t* node_child = xml_memory_alloc_node   (node->doc); 

        node_child->pugi = node->pugi.child(child_name);
        assert(node_child->pugi != NULL);

        const xml_hnd_node_t hnd = xml_memory_get_hnd_node(node_child);        
        return(hnd);
    }

    SLD_API const xml_hnd_attrib_t
    xml_node_add_attribute(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    attribute_name) {

        xml_node_t*   node   = xml_memory_get_ptr_node (h_node);
        xml_attrib_t* attrib = xml_memory_alloc_attrib (node); 

        attrib->pugi = node->pugi.append_attribute(attribute_name);
        assert(attrib->pugi != NULL);

        const xml_hnd_attrib_t hnd = xml_memory_get_hnd_attrib(attrib);        
        return(hnd);
    }

    SLD_API const xml_hnd_attrib_t
    xml_node_get_attribute(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    attribute_name) {

        xml_node_t*   node   = xml_memory_get_ptr_node (h_node);
        xml_attrib_t* attrib = xml_memory_alloc_attrib (node); 

        attrib->pugi = node->pugi.attribute(attribute_name);
        assert(attrib->pugi != NULL);

        const xml_hnd_attrib_t hnd = xml_memory_get_hnd_attrib(attrib);        
        return(hnd);   
    }

    SLD_API u32
    xml_node_get_child_count(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    child_name) {

        xml_node_t* node = xml_memory_get_ptr_node (h_node);
        assert(node);

        auto children = node->pugi.children(child_name);

        u32 count = 0;
        for (
            auto iter = children.begin();
            iter != children.end();
            ++iter) {
            ++count;
        }
        return(count);
    }

    SLD_API bool
    xml_node_set_attrib_utf8(
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

        bool is_set = attrib.set_value(value.as_utf8);
        return(is_set);
    }

    SLD_API bool
    xml_node_set_attrib_u32(
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
    xml_node_set_attrib_u64(
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
    xml_node_set_attrib_f32(
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
    xml_node_get_attrib_utf8(
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
    xml_node_get_attrib_u32(
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
    xml_node_get_attrib_u64(
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
    xml_node_get_attrib_f32(
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
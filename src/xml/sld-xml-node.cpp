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
};
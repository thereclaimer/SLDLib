#pragma once

#include "sld-xml-internal.cpp"
#include <meow-hash/meow_hash_x64_aesni.h>

namespace sld {

    constexpr u32 _node_name_length_max = 256;

    SLD_API const xml_hnd_node_t
    xml_node_add_sibling(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    sibling_name) {
   
        xml_node_t* node     = xml_memory_get_ptr_node (h_node);
        xml_node_t* node_sib = NULL;

        bool is_valid = true;
        is_valid &= (node         != NULL); 
        is_valid &= (node->doc    != NULL); 
        is_valid &= (sibling_name != NULL);
        assert(is_valid); 

        pugi::xml_node pugi_node_parent  = node->pugi.parent();
        pugi::xml_node pugi_node_sibling = pugi_node_parent.append_child(sibling_name);
        
        if (pugi_node_sibling != NULL) {

            node_sib = xml_memory_alloc_node(); 
            assert(node_sib);

            node_sib->pugi = pugi_node_sibling;
            node_sib->next = node;
            node_sib->doc  = node->doc;

            xml_doc_t* doc = node->doc;
            doc->nodes = node_sib;
        }

        const xml_hnd_node_t hnd = xml_memory_get_hnd_node(node_sib);        
        return(hnd);
    }

    SLD_API const xml_hnd_node_t
    xml_node_get_next_sibling(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    sibling_name) {
   
        xml_node_t* node     = xml_memory_get_ptr_node (h_node);
        xml_node_t* node_sib = NULL; 

        bool is_valid = true;
        is_valid &= (node         != NULL); 
        is_valid &= (node->doc    != NULL); 
        is_valid &= (sibling_name != NULL);
        assert(is_valid); 
        
        pugi::xml_node pugi_node = node->pugi.next_sibling(sibling_name);
        if (pugi_node != NULL) {

            node_sib = xml_memory_alloc_node();
            assert(node_sib);

            node_sib->pugi = pugi_node;
            node_sib->next = node;
            node_sib->doc  = node->doc;

            xml_doc_t* doc = node->doc;
            doc->nodes = node_sib;
        }
        
        // get the handle
        const xml_hnd_node_t hnd = xml_memory_get_hnd_node(node_sib);        
        return(hnd);
    }


    SLD_API const xml_hnd_node_t
    xml_node_get_prev_sibling(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    sibling_name) {
      
        xml_node_t* node     = xml_memory_get_ptr_node (h_node);
        xml_node_t* node_sib = NULL; 

        bool is_valid = true;
        is_valid &= (node         != NULL); 
        is_valid &= (node->doc    != NULL); 
        is_valid &= (sibling_name != NULL);
        assert(is_valid); 
        
        pugi::xml_node pugi_node = node->pugi.previous_sibling(sibling_name);
        if (pugi_node != NULL) {

            node_sib = xml_memory_alloc_node();
            assert(node_sib);

            node_sib->pugi = pugi_node;
            node_sib->next = node;
            node_sib->doc  = node->doc;

            xml_doc_t* doc = node->doc;
            doc->nodes = node_sib;
        }
        
        // get the handle
        const xml_hnd_node_t hnd = xml_memory_get_hnd_node(node_sib);        
        return(hnd);
    }

    SLD_API const xml_hnd_node_t
    xml_node_add_child(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    child_name) {
   
        xml_node_t* node       = xml_memory_get_ptr_node (h_node);
        xml_node_t* node_child = NULL;

        pugi::xml_node pugi_node = node->pugi.append_child(child_name);
        if (pugi_node != NULL) {

            node_child = xml_memory_alloc_node(); 
            assert(node_child);

            node_child->pugi     = pugi_node;
            node_child->next     = node;
            node_child->doc      = node->doc;

            xml_doc_t* doc = node->doc;
            doc->nodes = node_child;
        }

        const xml_hnd_node_t hnd = xml_memory_get_hnd_node(node_child);        
        return(hnd);
    }

    SLD_API const xml_hnd_node_t
    xml_node_get_child(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    child_name) {
   
        xml_node_t* node       = xml_memory_get_ptr_node (h_node);
        xml_node_t* node_child = NULL;

        pugi::xml_node pugi_node_child = node->pugi.child(child_name);
        if (pugi_node_child != NULL) {

            node_child = xml_memory_alloc_node();
            assert(node_child);

            node_child->pugi = pugi_node_child;
            node_child->next = node;
            node_child->doc  = node->doc;
            
            xml_doc_t* doc = node->doc;
            doc->nodes = node_child;
        }
        
        // get the handle
        const xml_hnd_node_t hnd = xml_memory_get_hnd_node(node_child);        
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
};
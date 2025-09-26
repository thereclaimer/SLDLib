#pragma once

#include "sld-xml-internal.cpp"
#include <meow-hash/meow_hash_x64_aesni.h>

namespace sld {

    constexpr u32 _node_name_length_max = 256;

    SLD_API const bool
    xml_node_add_sibling(
        const xml_node_t* node,
        const xml_utf8_t* sibling_name,
        xml_node_t*       sibling) {

        bool is_valid = true;
        is_valid &= (node         != NULL); 
        is_valid &= (sibling_name != NULL);
        is_valid &= (sibling      != NULL);
        assert(is_valid); 

        pugi::xml_node pugi_node_parent  = node->parent();
        pugi::xml_node pugi_node_sibling = pugi_node_parent.append_child(sibling_name);
        *sibling = *(xml_node_t*)&pugi_node_sibling; 

        return(pugi_node_sibling != NULL);
    }

    SLD_API bool 
    xml_node_get_next_sibling(
        const xml_node_t* node,
        const xml_utf8_t* sibling_name,
        xml_node_t*       sibling) {
   
        bool is_valid = true;
        is_valid &= (node         != NULL); 
        is_valid &= (sibling_name != NULL);
        is_valid &= (sibling != NULL);
        assert(is_valid); 
        
        pugi::xml_node pugi_node_sibling = node->next_sibling(sibling_name);
        *sibling = *(xml_node_t*)&pugi_node_sibling; 

        return(pugi_node_sibling != NULL);
    }

    SLD_API bool
    xml_node_get_prev_sibling(
        const xml_node_t* node,
        const xml_utf8_t* sibling_name,
        xml_node_t*       sibling) {
   
        bool is_valid = true;
        is_valid &= (node         != NULL); 
        is_valid &= (sibling_name != NULL);
        is_valid &= (sibling != NULL);
        assert(is_valid); 
        
        pugi::xml_node pugi_node_sibling = sibling->previous_sibling(sibling_name);
        *sibling = *(xml_node_t*)&pugi_node_sibling; 

        return(pugi_node_sibling != NULL);
    }

    SLD_API bool 
    xml_node_add_child(
        xml_node_t* const node,
        const xml_utf8_t* name,
        xml_node_t*       child) {
   
        bool is_valid = true;
        is_valid &= (node  != NULL); 
        is_valid &= (name  != NULL);
        is_valid &= (child != NULL);
        assert(is_valid);

        pugi::xml_node pugi_node = node->append_child(name);
        *child = *(xml_node_t*)&pugi_node; 

        return(pugi_node != NULL);
    }

    SLD_API bool 
    xml_node_get_child(
        xml_node_t* const node,
        const xml_utf8_t* name,
        xml_node_t*       child) {
   
        bool is_valid = true;
        is_valid &= (node  != NULL); 
        is_valid &= (name  != NULL);
        is_valid &= (child != NULL);
        assert(is_valid);

        pugi::xml_node pugi_node = node->child(name);
        *child = *(xml_node_t*)&pugi_node; 

        return(pugi_node != NULL);
    }

    SLD_API u32
    xml_node_get_child_count(
        xml_node_t* const node,
        const xml_utf8_t* child_name) {

        auto children = node->children(child_name);

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
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

        // add this new node to the first parent or the doc
        xml_doc_t*     doc       = node->doc;
        xml_node_t*    parent    = node->parent;
        pugi::xml_node pugi_node = (parent == NULL)
            ?    doc->pugi.append_child (sibling_name)
            : parent->pugi.append_child (sibling_name);

        if (pugi_node != NULL) {

            node_sib = xml_memory_alloc_node(); 
            assert(node_sib);

            node_sib->pugi     = pugi_node;
            node_sib->parent   = node->parent;
            node_sib->next     = node;
            node_sib->children = NULL;
            node_sib->doc      = node->doc;

            node->next = node_sib;
        }

        const xml_hnd_node_t hnd = xml_memory_get_hnd_node(node_sib);        
        return(hnd);
    }

    SLD_API const xml_hnd_node_t
    xml_node_get_next_sibling(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    sibling_name) {
   
        xml_node_t* node = xml_memory_get_ptr_node (h_node);

        bool is_valid = true;
        is_valid &= (node         != NULL); 
        is_valid &= (node->doc    != NULL); 
        is_valid &= (sibling_name != NULL);
        assert(is_valid); 

        // first, try to find a pre-allocated node with the same name        
        xml_node_t* sib_node     = NULL;
        const u32   sib_name_len = strnlen_s(sibling_name, _node_name_length_max);
        meow_u128   sib_hash     = MeowHash(MeowDefaultSeed, sib_name_len, (void*)sibling_name);
        for (
            xml_node_t* n = node->next;
            n != NULL;
            n = n->next) {

            // compare the lengths
            const c8* n_name     = n->pugi.name(); 
            const u32 n_name_len = strnlen_s(n_name, _node_name_length_max);
            if (n_name_len != sib_name_len) continue;

            // if the lengths match, check the hashes
            meow_u128 n_hash = MeowHash(MeowDefaultSeed, n_name_len, (void*)n_name);
            if (MeowHashesAreEqual(sib_hash, n_hash)) {
                sib_node = n;
                break;
            }
        }

        // if we didn't find a pre-allocated node,
        // so we need to search the doc and create a new one
        if (sib_node == NULL) {
                
            pugi::xml_node pugi_node = node->pugi.next_sibling(sibling_name);
            if (pugi_node != NULL) {

                sib_node = xml_memory_alloc_node();
                assert(sib_node);

                sib_node->pugi     = pugi_node;
                sib_node->parent   = node->parent;
                sib_node->next     = node;
                sib_node->children = NULL;
                sib_node->doc      = node->doc;

                node->next = sib_node;
            }
        }
        
        // get the handle
        const xml_hnd_node_t hnd = xml_memory_get_hnd_node(sib_node);        
        return(hnd);
    }


    SLD_API const xml_hnd_node_t
    xml_node_get_prev_sibling(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    sibling_name) {
      
        xml_node_t* node = xml_memory_get_ptr_node (h_node);

        bool is_valid = true;
        is_valid &= (node         != NULL); 
        is_valid &= (node->doc    != NULL); 
        is_valid &= (sibling_name != NULL);
        assert(is_valid); 

        // first, try to find a pre-allocated node with the same name        
        xml_node_t* sib_node     = NULL;
        const u32   sib_name_len = strnlen_s(sibling_name, _node_name_length_max);
        meow_u128   sib_hash     = MeowHash(MeowDefaultSeed, sib_name_len, (void*)sibling_name);

        xml_doc_t*  doc    = node->doc;
        xml_node_t* parent = node->parent;

        for (
            xml_node_t* n = parent ? parent->children : doc->nodes;
            n != node;
            n = n->next) {

            // compare the lengths
            const c8* n_name     = n->pugi.name(); 
            const u32 n_name_len = strnlen_s(n_name, _node_name_length_max);
            if (n_name_len != sib_name_len) continue;

            // if the lengths match, check the hashes
            meow_u128 n_hash = MeowHash(MeowDefaultSeed, n_name_len, (void*)n_name);
            if (MeowHashesAreEqual(sib_hash, n_hash)) {
                sib_node = n;
                break;
            }
        }

        // if we didn't find a ore-allocated node,
        // so we need to search the doc and create a new one
        if (sib_node == NULL) {
                
            pugi::xml_node pugi_node = node->pugi.previous_sibling(sibling_name);
            if (pugi_node != NULL) {

                sib_node = xml_memory_alloc_node();
                assert(sib_node);

                sib_node->pugi     = pugi_node;
                sib_node->parent   = node->parent;
                sib_node->next     = node;
                sib_node->children = NULL;
                sib_node->doc      = node->doc;

                node->next = sib_node;
            }
        }
        
        // get the handle
        const xml_hnd_node_t hnd = xml_memory_get_hnd_node(sib_node);        
        return(hnd);
    }

    SLD_API const xml_hnd_node_t
    xml_node_add_child(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    child_name) {
   
        xml_node_t* node       = xml_memory_get_ptr_node (h_node);
        xml_node_t* child_node = NULL;

        pugi::xml_node pugi_node = node->pugi.append_child(child_name);
        if (pugi_node != NULL) {

            xml_node_t* child_node = xml_memory_alloc_node(); 
            assert(child_node);

                child_node->pugi     = pugi_node;
                child_node->parent   = node->parent;
                child_node->next     = node->children;
                child_node->children = NULL;
                child_node->doc      = node->doc;
                
                node->children = child_node;
        }

        const xml_hnd_node_t hnd = xml_memory_get_hnd_node(child_node);        
        return(hnd);
    }

    SLD_API const xml_hnd_node_t
    xml_node_get_child(
        const xml_hnd_node_t h_node,
        const xml_utf8_t*    child_name) {
   
        xml_node_t* node       = xml_memory_get_ptr_node (h_node);
        xml_node_t* node_child = NULL;

        // first, try to find a pre-allocated node with the same name        
        xml_node_t* child_node     = NULL;
        const u32   child_name_len = strnlen_s(child_name, _node_name_length_max);
        meow_u128   child_hash     = MeowHash(MeowDefaultSeed, child_name_len, (void*)child_name);
        for (
            xml_node_t* n = node->children;
            n != NULL;
            n = n->next) {

            // compare the lengths
            const c8* n_name     = n->pugi.name(); 
            const u32 n_name_len = strnlen_s(n_name, _node_name_length_max);
            if (n_name_len != child_name_len) continue;

            // if the lengths match, check the hashes
            meow_u128 n_hash = MeowHash(MeowDefaultSeed, n_name_len, (void*)n_name);
            if (MeowHashesAreEqual(child_hash, n_hash)) {
                child_node = n;
                break;
            }
        }

        // if we didn't find a pre-allocated node,
        // so we need to search the doc and create a new one
        if (child_node == NULL) {
                
            pugi::xml_node pugi_node = node->pugi.child(child_name);
            if (pugi_node != NULL) {

                child_node = xml_memory_alloc_node();
                assert(child_node);

                child_node->pugi     = pugi_node;
                child_node->parent   = node;
                child_node->next     = node->children;
                child_node->children = NULL;
                child_node->doc      = node->doc;

                node->children = child_node;
            }
        }
        
        // get the handle
        const xml_hnd_node_t hnd = xml_memory_get_hnd_node(child_node);        
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
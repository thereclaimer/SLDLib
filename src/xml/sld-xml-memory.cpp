#pragma once

#include "sld-xml.hpp"
#include "sld-allocator.hpp"

#include "sld-xml-internal.cpp"

namespace sld {
    
    SLD_API void
    xml_memory_init(
        const void* memory,
        const u32   size) {

        const bool can_init = (
            memory != NULL && 
            size   != 0
        );
        assert(can_init);

        if (_xml_mem.alctr == NULL) {

            // initialize custom heap allocator for pugixml
            _xml_mem.alctr = heap_alctr_init_from_memory(
                memory, size, _xml_mem_granularity
            );
            assert(_xml_mem.alctr);

            // set the memory functions
            pugi::set_memory_management_functions(
                xml_memory_alloc,
                xml_memory_free
            );
        }
    }

    SLD_API void
    xml_memory_init_from_arena(
        arena_t* arena) {

        const bool is_valid = arena_validate(arena);        assert(is_valid); 

        // we will use the entire arena
        const u64   size   = arena->size;
        const void* memory = (void*)arena_push_bytes(arena, size); 

        xml_memory_init(memory, size);
    }

    SLD_API void
    xml_memory_reset(
        void) {

        const bool is_reset = heap_alctr_reset(_xml_mem.alctr);
        assert(is_reset);
    }
    
    SLD_FUNC void*
    xml_memory_alloc(
        size_t size) {

        void* xml_mem = heap_alctr_alloc_abs(_xml_mem.alctr, size);
        assert(xml_mem);
        return(xml_mem);
    }

    SLD_FUNC void
    xml_memory_free(
        void*  ptr) {

        const bool is_free = heap_alctr_free_abs(_xml_mem.alctr, ptr);
        assert(is_free);
    }

    SLD_FUNC xml_doc_t*
    xml_memory_alloc_doc(
        void) {

        static const u64 size = sizeof(xml_doc_t); 
        void*            mem  = heap_alctr_alloc_abs(_xml_mem.alctr, size);
        xml_doc_t*       doc  = (xml_doc_t*) new (mem) xml_doc_t();
        assert(doc);

        doc->next  = NULL;
        doc->nodes = NULL;

        return(doc);
    }

    SLD_FUNC xml_node_t*
    xml_memory_alloc_node(
        void) {

        static const u64 size = sizeof(xml_node_t); 
        void*            mem  = heap_alctr_alloc_abs(_xml_mem.alctr, size);
        xml_node_t*      node = (xml_node_t*)(new (mem) xml_node_t());
        assert(node != NULL);

        node->next = NULL;
        node->doc  = NULL;
        return(node);
    }

    SLD_FUNC void
    xml_memory_free_doc(
        xml_doc_t*    doc) {

        bool       can_free = false;
        xml_doc_t* next     = doc->next;

        if (_xml_mem.docs == doc) {
            can_free       = true;
            _xml_mem.docs = next;
            doc->next     = NULL;
        }
        else {
            for (
                xml_doc_t* prev = _xml_mem.docs;
                prev != NULL;
                prev = prev->next) {

                if (prev->next == doc) {
                    can_free    = true;
                    prev->next = next;
                }
            }
        }

        assert(can_free);
        bool is_free = true;

        for (
            xml_node_t* node = doc->nodes;
            node != NULL;
            node = node->next) {

            is_free &= heap_alctr_free_abs(_xml_mem.alctr, (void*)node);
        }
        doc->nodes = NULL;
        is_free &= heap_alctr_free_abs(_xml_mem.alctr, (void*)doc);
        assert(is_free);
    }
    
    SLD_FUNC void
    xml_memory_reset_doc(
        xml_doc_t* doc) {

        bool       is_free = true;
        xml_doc_t* next    = doc->next;

        for (
            xml_node_t* node = doc->nodes;
            node != NULL;
            node = node->next) {

            is_free &= heap_alctr_free_abs(_xml_mem.alctr, (void*)node);
        }

        assert(is_free);
    }

    SLD_FUNC const xml_hnd_doc_t
    xml_memory_get_hnd_doc(
        const xml_doc_t* ptr) {

        const alloc_hnd_t   hnd_mem = heap_alctr_get_hnd(_xml_mem.alctr, ptr);
        const xml_hnd_doc_t hnd_xml = {hnd_mem.val};
        return(hnd_xml);
    }

    SLD_FUNC const xml_hnd_node_t
    xml_memory_get_hnd_node(
        const xml_node_t* ptr) {

        const alloc_hnd_t    hnd_mem = heap_alctr_get_hnd(_xml_mem.alctr, ptr);
        const xml_hnd_node_t hnd_xml = {hnd_mem.val};
        return(hnd_xml);
    }

    SLD_FUNC xml_doc_t*
    xml_memory_get_ptr_doc(
        const xml_hnd_doc_t hnd) {
        
        const alloc_hnd_t hnd_mem = {hnd.val};
        xml_doc_t*        ptr     = (xml_doc_t*)heap_alctr_get_ptr(_xml_mem.alctr, hnd_mem);
        return(ptr);
    }

    SLD_FUNC xml_node_t*
    xml_memory_get_ptr_node(
        const xml_hnd_node_t hnd) {

        const alloc_hnd_t hnd_mem = {hnd.val};
        xml_node_t*       ptr     = (xml_node_t*)heap_alctr_get_ptr(_xml_mem.alctr, hnd_mem);
        return(ptr);
    }

    SLD_FUNC xml_attrib_t*
    xml_memory_get_ptr_attrib(
        const xml_hnd_attrib_t hnd) {

        const alloc_hnd_t hnd_mem = {hnd.val};
        xml_attrib_t*     ptr     = (xml_attrib_t*)heap_alctr_get_ptr(_xml_mem.alctr, hnd_mem);
        return(ptr);
    }
};
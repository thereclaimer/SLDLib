#pragma once

#include "sld-xml.hpp"
#include "sld-allocator.hpp"

#include "sld-xml-internal.cpp"

namespace sld {
    
    SLD_API void
    xml_memory_init(
        const void* memory,
        const u64   size) {

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

        doc->nodes    = NULL;
        doc->next     = _xml_mem.docs;
        _xml_mem.docs = doc;
        
        return(doc);
    }

    SLD_FUNC xml_node_t*
    xml_memory_alloc_node(
        xml_doc_t* doc) {

        static const u64 size = sizeof(xml_node_t); 
        void*            mem  = heap_alctr_alloc_abs(_xml_mem.alctr, size);
        xml_node_t*      node = (xml_node_t*)(new (mem) xml_node_t());
        assert(node != NULL && doc != NULL);

        node->attribs = NULL;
        node->doc     = doc;
        node->attribs = NULL;
        node->next    = doc->nodes;
        doc->nodes    = node;  

        return(node);
    }

    SLD_FUNC xml_attrib_t*
    xml_memory_alloc_attrib(
        xml_node_t* node) {

        static const u64 size   = sizeof(xml_attrib_t); 
        void*            mem    = heap_alctr_alloc_abs(_xml_mem.alctr, size);
        xml_attrib_t*    attrib = (xml_attrib_t*)(new (mem) xml_attrib_t());
        assert(attrib != NULL && node != NULL);
        
        attrib->node  = node;
        attrib->next  = node->attribs;
        node->attribs = attrib;

        return(attrib);
    }

    SLD_FUNC void
    xml_memory_free_doc(
        xml_doc_t*    doc) {

        bool       is_free = false;
        xml_doc_t* next    = doc->next;

        if (_xml_mem.docs == doc) {
            is_free       = true;
            _xml_mem.docs = next;
            doc->next     = NULL;
        }
        else {
            for (
                xml_doc_t* prev = _xml_mem.docs;
                prev != NULL;
                prev = prev->next) {

                if (prev->next == doc) {
                    is_free    = true;
                    prev->next = next;
                }
            }
        }

        for (
            xml_node_t* node = doc->nodes;
            node != NULL;
            node = node->next) {

            for (
                xml_attrib_t* attrib = node->attribs;
                attrib != NULL;
                attrib = attrib->next) {

                is_free &= heap_alctr_free_abs(_xml_mem.alctr, (void*)attrib);
            }
            is_free &= heap_alctr_free_abs(_xml_mem.alctr, (void*)node);
        }
        doc->nodes = NULL;
        is_free &= heap_alctr_free_abs(_xml_mem.alctr, (void*)doc);
        assert(is_free);
    }

    SLD_FUNC const xml_hnd_t
    xml_memory_get_hnd(
        const void* ptr) {

        const alloc_hnd_t hnd_mem = heap_alctr_get_hnd(_xml_mem.alctr, ptr);
        const xml_hnd_t   hnd_xml = {hnd_mem.val};
        assert(hnd_xml.val != 0);
        return(hnd_xml);
    }
    
    SLD_FUNC void*
    xml_memory_get_ptr(
        const xml_hnd_t hnd) {

        const alloc_hnd_t hnd_mem = {hnd.val};
        void*             ptr     = heap_alctr_get_ptr(_xml_mem.alctr, hnd_mem);
        assert(ptr);
        return(ptr);
    }
};
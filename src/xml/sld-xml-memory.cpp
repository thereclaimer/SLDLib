#pragma once

#include "sld-xml.hpp"
#include "sld-allocator.hpp"

#include "sld-xml-internal.cpp"

namespace sld {

    SLD_API void
    xml_memory_init(
        const void* memory,
        const u64   size,
        const u64   granularity) {

        const bool can_init = (
            memory      != NULL && 
            size        != 0    &&
            granularity != 0
        );

        if (can_init && _xml_mem.alctr == NULL) {

            // initialize custom heap allocator for pugixml
            _xml_mem.alctr = heap_alctr_init_from_memory(
                memory, size, granularity
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

        void* xml_mem = heap_alctr_alloc(_xml_mem.alctr, size);
        assert(xml_mem);
        return(xml_mem);
    }

    SLD_FUNC void
    xml_memory_free(
        void*  ptr) {

        const bool is_free = heap_alctr_free(_xml_mem.alctr, ptr);
        assert(is_free);
    }

    SLD_API xml_doc_t*
    xml_memory_alloc_doc(
        void) {

        static const u64 size = sizeof(pugi::xml_document); 
        void*            mem  = heap_alctr_alloc(_xml_mem.alctr, size);
        xml_doc_t*       doc  = (xml_doc_t*)(new (mem) pugi::xml_document());
        assert(doc);
        return(doc);
    }

    SLD_API xml_node_t*
    xml_memory_alloc_node(
        void) {

        static const u64 size = sizeof(pugi::xml_node); 
        void*            mem  = heap_alctr_alloc(_xml_mem.alctr, size);
        xml_node_t*      node = (xml_node_t*)(new (mem) pugi::xml_node());
        assert(node);
        return(node);
    }

    SLD_API xml_attrib_t*
    xml_memory_alloc_attrib(
        void) {

        static const u64 size   = sizeof(pugi::xml_attribute); 
        void*            mem    = heap_alctr_alloc(_xml_mem.alctr, size);
        xml_attrib_t*    attrib = (xml_attrib_t*)(new (mem) pugi::xml_attribute());
        assert(attrib);
        return(attrib);
    }

    SLD_API void
    xml_memory_free_doc(
        xml_doc_t*    doc) {

        const bool is_free = heap_alctr_free(_xml_mem.alctr, (void*)doc);
        assert(is_free);
    }

    SLD_API void
    xml_memory_free_node(
        xml_node_t* node) {

        const bool is_free = heap_alctr_free(_xml_mem.alctr, (void*)node);
        assert(is_free);
    }

    SLD_API void
    xml_memory_free_attrib(
        xml_attrib_t* attrib) {

        const bool is_free = heap_alctr_free(_xml_mem.alctr, (void*)attrib);
        assert(is_free);
    }
};
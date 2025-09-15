#pragma once

#include <pugixml.hpp>
#include "sld-xml.hpp"
#include "sld-allocator.hpp"

namespace sld {

    struct xml_memory_t {
        heap_alctr_t* alloc;
    } static _xml_mem;

    void* xml_memory_alloc (size_t size);
    void  xml_memory_free  (void*  ptr);

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

        if (can_init && _xml_mem.alloc == NULL) {

            // initialize custom heap allocator for pugixml
            _xml_mem.alloc = heap_alctr_init_from_memory(
                memory, size, granularity
            );
            assert(_xml_mem.alloc);

            // set the memory functions
            pugi::set_memory_management_functions(
                xml_memory_alloc,
                xml_memory_free
            );
        }
    }

    SLD_API void
    xml_memory_reset(
        xml_memory_t* alloc) {

    }

    
    SLD_FUNC void*
    xml_memory_alloc(
        size_t size) {

        void* xml_mem = heap_alloc(_xml_mem.alloc, size);
        assert(xml_mem);
        return(xml_mem);
    }

    SLD_FUNC void
    xml_memory_free(
        void*  ptr) {

        const bool is_free = heap_free(_xml_mem.alloc, ptr);
        assert(is_free);
    }
};
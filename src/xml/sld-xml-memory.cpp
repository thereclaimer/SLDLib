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
        void) {

        const bool is_reset = heap_alctr_reset(_xml_mem.alloc);
        assert(is_reset);
    }
    
    SLD_FUNC void*
    xml_memory_alloc(
        size_t size) {

        void* xml_mem = heap_alctr_alloc(_xml_mem.alloc, size);
        assert(xml_mem);
        return(xml_mem);
    }

    SLD_FUNC void
    xml_memory_free(
        void*  ptr) {

        const bool is_free = heap_alctr_free(_xml_mem.alloc, ptr);
        assert(is_free);
    }
};
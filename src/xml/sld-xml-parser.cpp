#pragma once

#include "sld-xml.hpp"
#include "sld-memory.hpp"

#include "sld-xml-internal.cpp"

namespace sld {

    static xml_parser_t _parser;

    SLD_API u32 
    xml_parser_minimum_size(
        void) {

        return(XML_PARSER_GRANULARITY);
    }

    SLD_API void
    xml_parser_init(
        const void* memory,
        const u32   size) {

        bool can_init = true;
        can_init &= (memory != NULL);
        can_init &= (size >= XML_PARSER_GRANULARITY);
        assert(can_init);

        if (_parser.alctr == NULL) {

            _parser.alctr = block_allocator_init(memory, size, XML_PARSER_GRANULARITY);
            assert(_parser.alctr != NULL);

            pugi::set_memory_management_functions(
                xml_parser_alloc,
                xml_parser_free
            );
        }
    }
    
    SLD_FUNC void*
    xml_parser_alloc(
        size_t size) {

        void* xml_mem = block_allocator_alloc_abs(_parser.alctr, size);
        assert(xml_mem);
        return(xml_mem);
    }

    SLD_FUNC void
    xml_parser_free(
        void*  ptr) {

        const bool is_free = block_allocator_free_abs(_parser.alctr, ptr);
        assert(is_free);
    }
};
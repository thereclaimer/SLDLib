#pragma once

#include <pugixml.hpp>
#include "sld-xml.hpp"
#include "sld-buffer.hpp"
#include "sld-memory.hpp"
#include "sld-stack.hpp"

namespace sld {
    

    SLD_FUNC void* xml_parser_alloc (size_t size);
    SLD_FUNC void  xml_parser_free  (void*  ptr);

    struct xml_stack_t  : stack_t { };
    struct xml_doc_t    : pugi::xml_document  { };     
    struct xml_node_t   : pugi::xml_node      { };      
    struct xml_attrib_t : pugi::xml_attribute { };      
    struct xml_doc_writer_t;

    constexpr u64 XML_PARSER_GRANULARITY = size_kilobytes(32);
    constexpr u32 XML_STACK_MIN_SIZE     = sizeof(xml_stack_t) + sizeof(xml_doc_t);

    struct xml_writer_t : pugi::xml_writer {

        buffer_t buffer;

        xml_writer_t() {
            buffer.data   = NULL;
            buffer.size   = 0;
            buffer.length = 0;
        }

        void
        set_buffer_memory(
            byte* data,
            u64   size) {

            buffer.data   = data;
            buffer.size   = size;
            buffer.length = 0;
        }

        void
        clear_buffer_memory(
            void) {

            buffer.data   = NULL;
            buffer.size   = 0;
            buffer.length = 0;
        }

        virtual void
        write(
            const void* data,
            size_t      size) {

            // set the length
            buffer.length = size;

            // check if we can copy the buffer
            // if not, return
            bool can_write = true;
            can_write &= (data          != NULL);
            can_write &= (buffer.length != 0);
            can_write &= (buffer.length <= buffer.size);
            if (!can_write) return;

            // copy the buffer
            (void)buffer_copy(buffer, (byte*)data, size);
        }
    };

    struct xml_parser_t {
        block_allocator_t* alctr;
    };

};
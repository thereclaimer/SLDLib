#pragma once

#include <pugixml.hpp>
#include "sld-xml.hpp"
#include "sld-buffer.hpp"

namespace sld {
    
    struct xml_doc_t {
        pugi::xml_document pugi_doc;
    };     
    struct xml_node_t {
        pugi::xml_node pugi_node;
    };      
    struct xml_attribute_t {
        pugi::xml_attribute pugi_attrib;
    };

    struct xml_memory_t {
        heap_alctr_t* alctr;
    } static _xml_mem;

    struct xml_writer_t : pugi::xml_writer {

        buffer_t buffer;

        xml_writer_t() {
            buffer.data   = NULL;
            buffer.size   = 0;
            buffer.length = 0;
        }

        virtual void
        write(
            const void* data,
            size_t      size) {

            // set the size
            buffer.size = size;

            // check if we can copy the buffer
            // if not, return
            bool can_write = true;
            can_write &= (data != NULL);
            can_write &= (size != 0);
            can_write &= (size <= buffer.size);
            if (!can_write) return;

            // copy the buffer
            (void)buffer_copy(buffer, (byte*)data, size);
        }
    };

    void* xml_memory_alloc (size_t size);
    void  xml_memory_free  (void*  ptr);
};
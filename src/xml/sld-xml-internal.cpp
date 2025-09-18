#pragma once

#include <pugixml.hpp>
#include "sld-xml.hpp"
#include "sld-buffer.hpp"
#include "sld-allocator.hpp"

namespace sld {
    
    struct xml_doc_t;
    struct xml_node_t;
    struct xml_attrib_t;
    struct xml_memory_t;
    struct xml_writer_t;

    constexpr u64 _xml_mem_granularity = sizeof(pugi::xml_document);

    SLD_FUNC xml_doc_t*             xml_memory_alloc_doc      (void);
    SLD_FUNC xml_node_t*            xml_memory_alloc_node     (xml_doc_t*             doc);
    SLD_FUNC xml_attrib_t*          xml_memory_alloc_attrib   (xml_node_t*            node);
    SLD_FUNC void                   xml_memory_free_doc       (xml_doc_t*             doc);
    SLD_FUNC void                   xml_memory_reset_doc      (xml_doc_t*             doc);
    SLD_FUNC void*                  xml_memory_alloc          (size_t                 size);
    SLD_FUNC void                   xml_memory_free           (void*                  ptr);
    SLD_FUNC const xml_hnd_doc_t    xml_memory_get_hnd_doc    (const xml_doc_t*       ptr);
    SLD_FUNC const xml_hnd_node_t   xml_memory_get_hnd_node   (const xml_node_t*      ptr);
    SLD_FUNC const xml_hnd_attrib_t xml_memory_get_hnd_attrib (const xml_attrib_t*    ptr);
    SLD_FUNC xml_doc_t*             xml_memory_get_ptr_doc    (const xml_hnd_doc_t    hnd);
    SLD_FUNC xml_node_t*            xml_memory_get_ptr_node   (const xml_hnd_node_t   hnd);
    SLD_FUNC xml_attrib_t*          xml_memory_get_ptr_attrib (const xml_hnd_attrib_t hnd);

    struct xml_doc_t {
        pugi::xml_document pugi;
        xml_doc_t*         next;
        xml_node_t*        nodes;
    };     
    struct xml_node_t {
        pugi::xml_node pugi;
        xml_node_t*    next;
        xml_attrib_t*  attribs;
        xml_doc_t*     doc;
    };      
    struct xml_attrib_t {
        pugi::xml_attribute pugi;
        xml_node_t*         node;
        xml_attrib_t*       next;
    };

    struct xml_memory_t {
        heap_alctr_t* alctr;
        xml_doc_t*    docs;
    } static _xml_mem;

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

};

#pragma once

#include <pugixml.hpp>

#include "sld.hpp"
#include "sld-buffer.hpp"
#include "sld-memory.hpp"

/*
/* TODO(SAM): The memory layout works, but we don't need to create a new
/* object every single time. Most of the time we should be able to recycle
/* stuff, but it works for now
*/

#define SLD_XML_INVALID_HANDLE 0

namespace sld {

    typedef c8 xml_utf8_t;

    struct xml_hnd_t        : u32_t     { };
    struct xml_hnd_doc_t    : xml_hnd_t { }; 
    struct xml_hnd_node_t   : xml_hnd_t { }; 
    struct xml_hnd_attrib_t : xml_hnd_t { }; 

    struct xml_attrib_value_t {
        union {
            xml_utf8_t* as_utf8;
            u32         as_u32;
            u64         as_u64;
            f32         as_f32;
        };
    };

    struct xml_memory_t;
    struct xml_doc_t;
    struct xml_node_t;
    struct xml_attrib_t;

    SLD_API void                   xml_memory_init              (const void* memory, const u32 size);
    SLD_API void                   xml_memory_init_from_arena   (arena_t*    arena);
    SLD_API void                   xml_memory_reset             (void);

    SLD_API const xml_hnd_doc_t    xml_doc_create                (void);
    SLD_API void                   xml_doc_reset                 (const xml_hnd_doc_t h_doc);
    SLD_API void                   xml_doc_destroy               (const xml_hnd_doc_t h_doc);
    SLD_API u64                    xml_doc_buffer_length         (const xml_hnd_doc_t h_doc);
    SLD_API bool                   xml_doc_buffer_read           (const xml_hnd_doc_t h_doc, const buffer_t&   buffer);
    SLD_API bool                   xml_doc_buffer_write          (const xml_hnd_doc_t h_doc, buffer_t&         buffer);
    SLD_API const xml_hnd_node_t   xml_doc_get_child_node        (const xml_hnd_doc_t h_doc, const xml_utf8_t* name);
    SLD_API const xml_hnd_node_t   xml_doc_add_child_node        (const xml_hnd_doc_t h_doc, const xml_utf8_t* name);
    SLD_API const xml_hnd_node_t   xml_doc_get_or_add_child_node (const xml_hnd_doc_t h_doc, const xml_utf8_t* name);
    SLD_API u32                    xml_doc_get_child_node_count  (const xml_hnd_doc_t h_doc, const xml_utf8_t* name);

    SLD_API const xml_hnd_node_t   xml_node_add_sibling          (const xml_hnd_node_t h_node, const xml_utf8_t* name);
    SLD_API const xml_hnd_node_t   xml_node_get_next_sibling     (const xml_hnd_node_t h_node, const xml_utf8_t* name);
    SLD_API const xml_hnd_node_t   xml_node_get_prev_sibling     (const xml_hnd_node_t h_node, const xml_utf8_t* name);
    SLD_API const xml_hnd_node_t   xml_node_add_child            (const xml_hnd_node_t h_node, const xml_utf8_t* name);
    SLD_API const xml_hnd_node_t   xml_node_get_child            (const xml_hnd_node_t h_node, const xml_utf8_t* name);
    SLD_API u32                    xml_node_get_child_count      (const xml_hnd_node_t h_node, const xml_utf8_t* name);
    SLD_API u32                    xml_node_get_attrib_count     (const xml_hnd_node_t h_node, const xml_utf8_t* name);

    SLD_API bool                   xml_attrib_set_val_utf8      (const xml_hnd_node_t h_node, const xml_utf8_t* name, const xml_attrib_value_t& value);
    SLD_API bool                   xml_attrib_set_val_u32       (const xml_hnd_node_t h_node, const xml_utf8_t* name, const xml_attrib_value_t& value);
    SLD_API bool                   xml_attrib_set_val_u64       (const xml_hnd_node_t h_node, const xml_utf8_t* name, const xml_attrib_value_t& value);
    SLD_API bool                   xml_attrib_set_val_f32       (const xml_hnd_node_t h_node, const xml_utf8_t* name, const xml_attrib_value_t& value);
    SLD_API bool                   xml_attrib_get_val_utf8      (const xml_hnd_node_t h_node, const xml_utf8_t* name, xml_attrib_value_t&       value);
    SLD_API bool                   xml_attrib_get_val_u32       (const xml_hnd_node_t h_node, const xml_utf8_t* name, xml_attrib_value_t&       value);
    SLD_API bool                   xml_attrib_get_val_u64       (const xml_hnd_node_t h_node, const xml_utf8_t* name, xml_attrib_value_t&       value);
    SLD_API bool                   xml_attrib_get_val_f32       (const xml_hnd_node_t h_node, const xml_utf8_t* name, xml_attrib_value_t&       value);


};

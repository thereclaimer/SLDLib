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

    struct xml_memory_t;
    struct xml_doc_t;
    struct xml_node_t;
    struct xml_attrib_t;

    SLD_API void                   xml_memory_init            (const void* memory, const u32 size);
    SLD_API void                   xml_memory_init_from_arena (arena_t*    arena);
    SLD_API void                   xml_memory_reset           (void);

    SLD_API const xml_hnd_doc_t    xml_doc_create            (void);
    SLD_API void                   xml_doc_reset             (const xml_hnd_doc_t h_doc);
    SLD_API void                   xml_doc_destroy           (const xml_hnd_doc_t h_doc);
    SLD_API u64                    xml_doc_buffer_size       (const xml_hnd_doc_t h_doc);
    SLD_API bool                   xml_doc_buffer_read       (const xml_hnd_doc_t h_doc, const buffer_t&   buffer);
    SLD_API bool                   xml_doc_buffer_write      (const xml_hnd_doc_t h_doc, buffer_t&         buffer);
    SLD_API const xml_hnd_node_t   xml_doc_get_child_node    (const xml_hnd_doc_t h_doc, const xml_utf8_t* child_name);
    SLD_API const xml_hnd_node_t   xml_doc_add_child_node    (const xml_hnd_doc_t h_doc, const xml_utf8_t* child_name);

    SLD_API const xml_hnd_node_t   xml_node_add_sibling      (const xml_hnd_node_t h_node, const xml_utf8_t* sibling_name);
    SLD_API const xml_hnd_node_t   xml_node_get_sibling      (const xml_hnd_node_t h_node, const xml_utf8_t* sibling_name);
    SLD_API const xml_hnd_node_t   xml_node_add_child        (const xml_hnd_node_t h_node, const xml_utf8_t* child_name);
    SLD_API const xml_hnd_node_t   xml_node_get_child        (const xml_hnd_node_t h_node, const xml_utf8_t* child_name);
    SLD_API const xml_hnd_attrib_t xml_node_add_attribute    (const xml_hnd_node_t h_node, const xml_utf8_t* attribute_name);
    SLD_API const xml_hnd_attrib_t xml_node_get_attribute    (const xml_hnd_node_t h_node, const xml_utf8_t* attribute_name);
    
    SLD_API const xml_utf8_t*      xml_attrib_get_value_utf8 (const xml_hnd_attrib_t h_attrib);      
    SLD_API const u32              xml_attrib_get_value_u32  (const xml_hnd_attrib_t h_attrib);      
    SLD_API const u64              xml_attrib_get_value_u64  (const xml_hnd_attrib_t h_attrib);      
    SLD_API const f32              xml_attrib_get_value_f32  (const xml_hnd_attrib_t h_attrib);      
    SLD_API bool                   xml_attrib_set_value_utf8 (const xml_hnd_attrib_t h_attrib, const xml_utf8_t* value);      
    SLD_API bool                   xml_attrib_set_value_u32  (const xml_hnd_attrib_t h_attrib, const u32         value);      
    SLD_API bool                   xml_attrib_set_value_u64  (const xml_hnd_attrib_t h_attrib, const u64         value);      
    SLD_API bool                   xml_attrib_set_value_f32  (const xml_hnd_attrib_t h_attrib, const f32         value);      

};

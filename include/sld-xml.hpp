#pragma once

#include <pugixml.hpp>

#include "sld.hpp"
#include "sld-buffer.hpp"

namespace sld {

    typedef c8 xml_utf8_t;

    struct xml_memory_t;
    struct xml_doc_t;
    struct xml_node_t;
    struct xml_attrib_t;

    SLD_API void              xml_memory_init             (const void* memory, const u64 size, const u64 granularity);
    SLD_API void              xml_memory_reset            (void);
    SLD_API xml_doc_t*        xml_memory_alloc_doc        (void);
    SLD_API xml_node_t*       xml_memory_alloc_node       (void);
    SLD_API xml_attrib_t*     xml_memory_alloc_attrib     (void);
    SLD_API void              xml_memory_free_doc         (xml_doc_t*    doc);
    SLD_API void              xml_memory_free_node        (xml_node_t*   node);
    SLD_API void              xml_memory_free_attrib      (xml_attrib_t* attrib);

    SLD_API bool              xml_doc_load_from_buffer    (xml_doc_t*       doc, const buffer_t&   buffer);
    SLD_API bool              xml_doc_write_to_buffer     (xml_doc_t*       doc, buffer_t&         buffer);
    SLD_API bool              xml_doc_get_next_child_node (const xml_doc_t* doc, const xml_utf8_t* child_name, xml_node_t*       child_node);
    SLD_API bool              xml_doc_add_child_node      (xml_doc_t*       doc, const xml_utf8_t* child_name, const xml_node_t* child_node);

    SLD_API bool              xml_node_get_next_sibling   (const xml_node_t* node, const xml_utf8_t* sibling_name,   const xml_node_t* sibling_node);
    SLD_API bool              xml_node_add_sibling        (xml_node_t*       node, const xml_utf8_t* sibling_name,   xml_node_t*       sibling_node);
    SLD_API bool              xml_node_get_attribute      (const xml_node_t* node, const xml_utf8_t* attribute_name, xml_attrib_t*     attrib);
    
    SLD_API const xml_utf8_t* xml_attrib_get_value        (const xml_attrib_t* attrib);      
    SLD_API const bool        xml_attrib_set_value        (xml_attrib_t*       attrib, const xml_utf8_t* value);      

};

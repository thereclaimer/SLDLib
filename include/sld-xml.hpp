#pragma once

#include <pugixml.hpp>

#include "sld.hpp"
#include "sld-buffer.hpp"

namespace sld {

    struct xml_memory_t;
    struct xml_doc_t;
    struct xml_node_t;
    struct xml_attrib_t;

    SLD_API void          xml_memory_init             (const void* memory, const u64 size, const u64 granularity);
    SLD_API void          xml_memory_reset            (void);

    SLD_API xml_doc_t*    xml_doc_load_from_buffer    (const buffer_t& buffer);
    SLD_API bool          xml_doc_write_to_buffer     (xml_doc_t* doc, buffer_t& buffer);
    SLD_API xml_node_t*   xml_doc_get_next_child_node (xml_doc_t* doc, const c8* child_name);
    SLD_API xml_node_t*   xml_doc_add_child_node      (xml_doc_t* doc, const c8* child_name);

    SLD_API xml_node_t*   xml_node_get_next_sibling   (xml_node_t* node, const c8* sibling_name);
    SLD_API xml_node_t*   xml_node_add_sibling        (xml_node_t* node, const c8* sibling_name);
    SLD_API xml_attrib_t* xml_node_get_attribute      (xml_node_t* node, const c8* attribute_name);
    
    SLD_API const c8*     xml_attrib_get_value        (xml_attrib_t* attrib);      
    SLD_API const bool    xml_attrib_set_value        (xml_attrib_t* attrib, const c8* value);      

};

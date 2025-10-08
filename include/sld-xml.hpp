
#pragma once

#include "sld.hpp"
#include "sld-collections.hpp"
#include "sld-memory.hpp"

namespace sld {

    typedef c8 xml_utf8_t;

    struct xml_doc_t;
    struct xml_node_t;
    struct xml_attrib_t;

    struct xml_doc_t;
    struct xml_node_t;
    struct xml_attrib_t;
    struct xml_stack_t;

    SLD_API u32           xml_parser_minimum_size       (void);
    SLD_API void          xml_parser_init               (const void* memory, const u32 size);

    SLD_API u32           xml_stack_minimum_size        (void);
    SLD_API xml_stack_t*  xml_stack_init                (const void* memory, const u32 size); 
    SLD_API bool          xml_stack_validate            (xml_stack_t* const stack); 
    SLD_API void          xml_stack_reset               (xml_stack_t* const stack); 
    SLD_API xml_doc_t*    xml_stack_push_doc            (xml_stack_t* const stack);
    SLD_API xml_node_t*   xml_stack_push_node           (xml_stack_t* const stack);
    SLD_API xml_attrib_t* xml_stack_push_attrib         (xml_stack_t* const stack);
    SLD_API u32           xml_stack_size_total          (xml_stack_t* const stack);
    SLD_API u32           xml_stack_size_used           (xml_stack_t* const stack);

    SLD_API void          xml_doc_reset                 (xml_doc_t*   const doc);
    SLD_API u32           xml_doc_buffer_length         (xml_doc_t*   const doc);
    SLD_API bool          xml_doc_buffer_read           (xml_doc_t*   const doc,  const data_buffer_t* buffer);
    SLD_API bool          xml_doc_buffer_write          (xml_doc_t*   const doc,  data_buffer_t*       buffer);
    SLD_API u32           xml_doc_get_child_node_count  (xml_doc_t*   const doc,  const xml_utf8_t* name);
    SLD_API bool          xml_doc_get_child_node        (xml_doc_t*   const doc,  const xml_utf8_t* name, xml_node_t* node);
    SLD_API bool          xml_doc_add_child_node        (xml_doc_t*   const doc,  const xml_utf8_t* name, xml_node_t* node);
    SLD_API bool          xml_doc_get_or_add_child_node (xml_doc_t*   const doc,  const xml_utf8_t* name, xml_node_t* node);

    SLD_API bool          xml_node_add_sibling          (xml_node_t*   const node,   const xml_utf8_t* name, xml_node_t* sibling);
    SLD_API bool          xml_node_get_next_sibling     (xml_node_t*   const node,   const xml_utf8_t* name, xml_node_t* sibling);
    SLD_API bool          xml_node_get_prev_sibling     (xml_node_t*   const node,   const xml_utf8_t* name, xml_node_t* sibling);
    SLD_API bool          xml_node_add_child            (xml_node_t*   const node,   const xml_utf8_t* name, xml_node_t* child);
    SLD_API bool          xml_node_get_child            (xml_node_t*   const node,   const xml_utf8_t* name, xml_node_t* child);
    SLD_API u32           xml_node_get_child_count      (xml_node_t*   const node,   const xml_utf8_t* name);
    SLD_API u32           xml_node_get_attrib_count     (xml_node_t*   const node,   const xml_utf8_t* name);
    SLD_API bool          xml_node_get_attrib           (xml_node_t*   const node,   const xml_utf8_t* name, xml_attrib_t* attrib);
    SLD_API bool          xml_node_add_attrib           (xml_node_t*   const node,   const xml_utf8_t* name, xml_attrib_t* attrib);
    
    SLD_API bool          xml_attrib_set_val_utf8       (xml_attrib_t* const attrib, const xml_utf8_t* value);
    SLD_API bool          xml_attrib_set_val_u32        (xml_attrib_t* const attrib, const u32         value);
    SLD_API bool          xml_attrib_set_val_u64        (xml_attrib_t* const attrib, const u64         value);
    SLD_API bool          xml_attrib_set_val_f32        (xml_attrib_t* const attrib, const f32         value);
    SLD_API bool          xml_attrib_get_val_utf8       (xml_attrib_t* const attrib, xml_utf8_t*&      value);
    SLD_API bool          xml_attrib_get_val_u32        (xml_attrib_t* const attrib, const u32&        value);
    SLD_API bool          xml_attrib_get_val_u64        (xml_attrib_t* const attrib, const u64&        value);
    SLD_API bool          xml_attrib_get_val_f32        (xml_attrib_t* const attrib, const f32&        value);
};

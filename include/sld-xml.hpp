#pragma once

#include <pugixml.hpp>

#include "sld.hpp"
#include "sld-buffer.hpp"

namespace sld {

    struct xml_allocator_t;
    struct xml_doc_t;
    struct xml_node_t;
    struct xml_attribute_t;

    xml_allocator_t* xml_allocator_init  (const void* memory, const u64 memory_size, const u64 block_size_min);
    bool             xml_allocator_reset (xml_allocator_t* alloc);

    void       xml_doc_load_from_buffer    (xml_doc_t& doc, const buffer_t& buffer);
    void       xml_doc_write_to_buffer     (xml_doc_t& doc, buffer_t&       buffer);
    xml_node_t xml_doc_get_next_child_node (xml_doc_t& doc, const c8*       child_name);
    xml_node_t xml_doc_add_child_node      (xml_doc_t& doc, const c8*       child_name);

    xml_node_t      xml_node_get_next_sibling (xml_node_t& node, const c8* sibling_name);
    xml_node_t      xml_node_add_sibling      (xml_node_t& node, const c8* sibling_name);
    xml_attribute_t xml_node_get_attribute    (xml_node_t& node, const c8* attribute_name);
};

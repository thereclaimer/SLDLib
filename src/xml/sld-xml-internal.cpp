#pragma once

#include <pugixml.hpp>
#include "sld-xml.hpp"

namespace sld {

    struct xml_memory_t {
        heap_alctr_t* alloc;
    } static _xml_mem;

    struct xml_doc_t       : pugi::xml_document  { };
    struct xml_node_t      : pugi::xml_node      { };
    struct xml_attribute_t : pugi::xml_attribute { };

    void* xml_memory_alloc (size_t size);
    void  xml_memory_free  (void*  ptr);
};
#pragma once

#include "sld-xml-internal.cpp"

namespace sld {

    SLD_API xml_doc_t*
    xml_doc_load_from_buffer(
        const buffer_t& buffer) {
    
        return(NULL);
    }

    SLD_API bool
    xml_doc_write_to_buffer(
        xml_doc_t* doc,
        buffer_t&  buffer) {

        return(false);
    }

    SLD_API xml_node_t*
    xml_doc_get_next_child_node(
        xml_doc_t* doc,
        const c8*  child_name) {

        return(NULL);
    }

    SLD_API xml_node_t*
    xml_doc_add_child_node(
        xml_doc_t* doc,
        const c8*  child_name) {

        return(NULL);
    }
};
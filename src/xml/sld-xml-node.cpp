#pragma once

#include "sld-xml-internal.cpp"

namespace sld {

    SLD_API xml_node_t*
    xml_node_get_next_sibling(
        xml_node_t* node,
        const c8*   sibling_name) {

        return(NULL);
    }

    SLD_API xml_node_t*
    xml_node_add_sibling(
        xml_node_t* node,
        const c8*   sibling_name) {

        return(NULL);
    }

    SLD_API xml_attrib_t*
    xml_node_get_attribute(
        xml_node_t* node,
        const c8*   attribute_name) {

        return(NULL);
    }
};
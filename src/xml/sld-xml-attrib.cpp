#pragma once

#include "sld-xml-internal.cpp"

namespace sld {
    
    SLD_API const c8*
    xml_attrib_get_value(
        xml_attrib_t* attrib) {

        return(NULL);
    }

    SLD_API const bool
    xml_attrib_set_value(
        xml_attrib_t* attrib,
        const c8*     value) {

        return(false);
    }
};
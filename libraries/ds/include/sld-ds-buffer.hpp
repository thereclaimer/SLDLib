#ifndef SLD_DS_BUFFER_HPP
#define SLD_DS_BUFFER_HPP

#include "sld.hpp"

namespace sld {

    struct buffer_t {
        byte* data;
        u32   size;
        u32   length;
    };

};

#endif //SLD_DS_BUFFER_HPP
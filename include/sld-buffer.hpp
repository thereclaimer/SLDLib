#ifndef SLD_BUFFER_HPP
#define SLD_BUFFER_HPP

#include "sld.hpp"

namespace sld {

    struct buffer_t {
        byte* data;
        u32   size;
        u32   length;
    };

    bool buffer_validate (const buffer_t& buffer);
    bool buffer_reset    (buffer_t& buffer);
    bool buffer_zero     (buffer_t& buffer);
    u32  buffer_append   (buffer_t& buffer, const byte* src_data, const u32 src_length);
    u32  buffer_copy     (buffer_t& buffer, const byte* src_data, const u32 src_length);

};

#endif //SLD_BUFFER_HPP
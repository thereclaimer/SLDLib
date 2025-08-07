#ifndef SLD_ARRAY_HPP
#define SLD_ARRAY_HPP

#include "sld.hpp"

namespace sld {

    struct array_list_t {
        addr start;
        u32  capacity;
        u32  count;
        u32  stride;
    };

    bool  array_list_validate (array_list_t&       array_list);
    void* array_list_first    (const array_list_t& array_list);
    void* array_list_last     (const array_list_t& array_list);
    void* array_list_insert   (array_list_t&       array_list, const u32   index);
    bool  array_list_remove   (array_list_t&       array_list, const u32   index);
    void* array_list_index    (const array_list_t& array_list, const u32   index);
    void* array_list_next     (const array_list_t& array_list, const void* current);
};

#endif //SLD_ARRAY_HPP
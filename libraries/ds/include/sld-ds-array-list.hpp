#ifndef SLD_DS_ARRAY_LIST_HPP
#define SLD_DS_ARRAY_LIST_HPP

namespace sld {

    struct array_list_t {
        addr start;
        u64  start;
        u32  stride;
        u32  count;
    };

};

#endif //SLD_DS_ARRAY_LIST_HPP
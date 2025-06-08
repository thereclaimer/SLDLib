#ifndef SLD_STRING_HPP
#define SLD_STRING_HPP

#include "sld.hpp"

namespace sld {


    struct cstr_memory_t;

    cstr_memory_t* cstr_memory_init  (const memory_t& memory, const u32 cstr_size, const c8 cstr_separator);
    bool           cstr_memory_reset (cstr_memory_t* cstr_mem);

    struct cstr_t {
        c8* data;
        u64 size;
    };

    bool cstr_is_null_or_empty (const cstr_t&  cstr);
    bool cstr_clear            (const cstr_t&  cstr);
    u32  cstr_length           (const cstr_t&  cstr);
    bool cstr_equals           (const cstr_t&  cstr_a,    const cstr_t& cstr_b);
    u32  cstr_copy             (const cstr_t&  cstr_from, cstr_t& cstr_to);
    u32  cstr_append           (const cstr_t&  cstr_from, cstr_t& cstr_to);
    bool cstr_alloc            (cstr_memory_t* cstr_mem,  cstr_t& cstr);
    bool cstr_free             (cstr_memory_t* cstr_mem,  cstr_t& cstr);
    

};

#endif //SLD_STRING_HPP
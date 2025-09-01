#ifndef SLD_HASH_HPP
#define SLD_HASH_HPP

#include "sld.hpp"

#define SLD_HASH_ALIGNMENT alignas(16)

namespace sld {

    struct SLD_HASH_ALIGNMENT hash_t {
        union {
            byte as_bytes [16];
            u32  as_u32   [4];
            u64  as_u64   [2];
        } val;
    };

    SLD_API bool hash_data       (const byte*   data,   const u32     length, hash_t&    hash);
    SLD_API bool hash_data_batch (const u32     count,  const byte*   data,   const u32  stride, hash_t* hashes);
    SLD_API bool hash_search     (const u32     count,  const hash_t  search, const hash_t* array,  u32&       index);
    SLD_API bool hash_is_equal   (const byte*   data_a, const byte*   data_b, const u32  length);
    SLD_API bool hash_is_equal   (const hash_t& hash,   const byte*   data,   const u32  length);
};

#endif //SLD_HASH_HPP
#ifndef SLD_HASH_HPP
#define SLD_HASH_HPP

#include <meow-hash/meow_hash_x64_aesni.h>
#include "sld.hpp"

#define SLD_HASH_ALIGNMENT alignas(16)

namespace sld {

    struct hash_seed_t {
        byte buffer[128];
    };

    struct hash_state_t : meow_state { };

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

    SLD_API void         hash_block_begin   (hash_state_t& state, const hash_seed_t& seed);
    SLD_API void         hash_block_consume (hash_state_t& state, const u64 block_size, const byte* block_data);
    SLD_API const hash_t hash_block_end     (hash_state_t& state);

};

#endif //SLD_HASH_HPP
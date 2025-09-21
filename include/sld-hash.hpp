#ifndef SLD_HASH_HPP
#define SLD_HASH_HPP

#include <meow-hash/meow_hash_x64_aesni.h>
#include "sld.hpp"

#define SLD_HASH_ALIGN_128 alignas(16)
#define SLD_HASH_ALIGN_32  alignas(4)

namespace sld {

    struct hash_t;
    struct hash_seed_t;
    struct hash_state_t;

    struct hash32_t;
    struct hash32_seed_t;

    //-------------------------------------------------------------------
    // HASH 128
    //-------------------------------------------------------------------

    SLD_API const hash_t hash_data          (const hash_seed_t& seed, const byte*   data,   const u32     length);
    SLD_API bool         hash_data_batch    (const hash_seed_t& seed, const u32     count,  const byte*   data,   const u32  stride, hash_t* hashes);
    SLD_API bool         hash_is_equal      (const hash_seed_t& seed, const byte*   data_a, const byte*   data_b, const u32  length);
    SLD_API bool         hash_is_equal      (const hash_seed_t& seed, const hash_t& hash,   const byte*   data,   const u32  length);
    SLD_API bool         hash_search        (const u32 count,   const hash_t  search, const hash_t* array,  u32&       index);
    SLD_API void         hash_block_begin   (hash_state_t& state, const hash_seed_t& seed);
    SLD_API void         hash_block_consume (hash_state_t& state, const u64 block_size, const byte* block_data);
    SLD_API const hash_t hash_block_end     (hash_state_t& state);

    struct SLD_HASH_ALIGN_128 hash_t {
        union {
            u32  as_u32   [4];
            u64  as_u64   [2];
            u32  as_u16   [8];
            byte as_bytes [16];
        } val;
    };

    struct hash_seed_t {
        byte buffer[128];
    };

    struct hash_state_t : meow_state { };

    //-------------------------------------------------------------------
    // HASH 32
    //-------------------------------------------------------------------

    SLD_API const hash32_t hash32          (const hash_seed_t seed, const byte*   data,   const u32     length);
    SLD_API bool           hash32_batch    (const hash_seed_t seed, const byte*   data,   const u32     stride, const u32      count, hash32_t* hashes);
    SLD_API bool           hash32_is_equal (const hash_seed_t seed, const byte*   data,   const u32     length, const hash32_t hash);
    SLD_API bool           hash32_search   (const u32         count, const hash_t search, const hash_t* array,  u32&           index);

    struct SLD_HASH_ALIGN_32 hash32_t {
        union {
           u32  as_u32;
           u16  as_u16   [2];
           byte as_bytes [4];
        };
    };

    struct hash32_seed_t : u32_t { }; 
};

#endif //SLD_HASH_HPP
#ifndef SLD_HASH_TABLE_HPP
#define SLD_HASH_TABLE_HPP

#include "sld.hpp"
#include "sld-hash.hpp"
#include "sld-memory.hpp"

namespace sld {

    struct hash_table_t;
    struct hash_table_key_t;
    struct hash_table_value_t;
    struct hash_table_kv_pair_t;
    struct hash_table_error_t;

    SLD_API const u32 hash_table_memory_size    (const hash_table_t& hash_table);
    SLD_API bool      hash_table_memory_init    (hash_table_t&       hash_table, const memory_t& memory);
    SLD_API bool      hash_table_validate       (const hash_table_t& hash_table);
    SLD_API bool      hash_table_validate_key   (const hash_table_t& hash_table, const hash_table_key_t&   key);
    SLD_API bool      hash_table_validate_value (const hash_table_t& hash_table, const hash_table_value_t& value);
    SLD_API bool      hash_table_reset          (hash_table_t&       hash_table);
    SLD_API bool      hash_table_insert         (hash_table_t&       hash_table, const byte*             value, hash_table_key_t& key);
    SLD_API bool      hash_table_insert_at      (hash_table_t&       hash_table, const byte*             data, const u32 index, hash_t& key);
    SLD_API bool      hash_table_remove_at      (hash_table_t&       hash_table, const u32               index);
    SLD_API bool      hash_table_remove         (hash_table_t&       hash_table, const hash_table_key_t& key);
    SLD_API bool      hash_table_search         (const hash_table_t& hash_table, const hash_table_key_t& key,   hash_table_value_t& value);
    SLD_API bool      hash_table_get_key_at     (const hash_table_t& hash_table, const u32               index, hash_table_key_t&   key);
    SLD_API bool      hash_table_get_value_at   (const hash_table_t& hash_table, const u32               index, hash_table_value_t& value);

    struct hash_table_error_t : s32_t { };

    struct hash_table_t {
        u32                capacity;
        u32                stride;
        u32                count;
        hash_table_error_t error;
        struct {
            hash_t* hash;
            byte*   value;
        } array;
    };

    struct hash_table_key_t {
        byte* data;
        u64   length;
    };

    struct hash_table_value_t {
        byte* data;
        u64   index;
    };

    struct hash_table_kv_pair_t {
        hash_table_key_t   key; 
        hash_table_value_t value; 
    };

    enum hash_table_error_e {
        hash_table_error_e_success             =  1,
        hash_table_error_e_invalid_table       = -1,
        hash_table_error_e_invalid_key         = -2,
        hash_table_error_e_invalid_value       = -3,
        hash_table_error_e_not_enough_memory   = -4,
        hash_table_error_e_max_count           = -5,
        hash_table_error_e_hash_failed         = -6,
        hash_table_error_e_index_out_of_bounds = -7
    };
};

#endif //SLD_HASH_TABLE_HPP
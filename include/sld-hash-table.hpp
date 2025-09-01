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

    const u32 hash_table_memory_size  (const u32           capacity,   const u32       value_size);
    bool      hash_table_memory_init  (hash_table_t&       hash_table, const memory_t& memory);
    bool      hash_table_reset        (hash_table_t&       hash_table);
    bool      hash_table_remove       (hash_table_t&       hash_table, const hash_table_key_t& key);
    bool      hash_table_remove_at    (hash_table_t&       hash_table, const u32               index);
    bool      hash_table_insert       (hash_table_t&       hash_table, const byte*             value, hash_table_key_t& key);
    bool      hash_table_insert_at    (hash_table_t&       hash_table, const byte*             data, const u32 index, hash_t& key);
    bool      hash_table_search       (const hash_table_t& hash_table, const hash_table_key_t& key,   hash_table_value_t& value);
    bool      hash_table_get_key_at   (const hash_table_t& hash_table, const u32               index, hash_table_key_t&   key);
    bool      hash_table_get_value_at (const hash_table_t& hash_table, const u32               index, hash_table_value_t& value);

    struct hash_table_t {
        u64 capacity;
        u64 count;
        u64 stride;
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
};

#endif //SLD_HASH_TABLE_HPP
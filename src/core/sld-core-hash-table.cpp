#pragma once

#include "sld-hash-table.hpp"

namespace sld {

    SLD_FUNC bool
    hash_table_validate(
        const hash_table_t& hash_table) {

        bool is_valid = true;

        is_valid &= (hash_table.capacity    != 0);
        is_valid &= (hash_table.stride      != 0);
        is_valid &= (hash_table.count       <= hash_table.capacity);
        is_valid &= (hash_table.array.hash  != NULL);
        is_valid &= (hash_table.array.value != NULL);

        return(is_valid);
    }

    SLD_FUNC bool
    hash_table_validate_key(
        const hash_table_t&     hash_table,
        const hash_table_key_t& key) {

        bool is_valid = true;
        
        is_valid &= (key.data   != NULL);
        is_valid &= (key.length != 0);
        
        return(is_valid);
    }

    SLD_FUNC bool
    hash_table_validate_value(
        const hash_table_t&         hash_table,
        const hash_table_value_t&   value) {

        const u32  size_hash  = (hash_table.capacity  * sizeof(hash_t));
        const u32  size_value = (hash_table.capacity  * hash_table.stride);
        const u32  size_total = (size_hash + size_value);
        const addr data_start = (addr)hash_table.array.hash + size_hash;
        const addr data_end   = (addr)hash_table.array.hash + size_total;
        const addr data_value = (addr)value.data;

        bool is_valid = true;
        
        is_valid &= (value.index < hash_table.count);
        is_valid &= (data_value >= data_start);
        is_valid &= (data_value  < data_end);
        
        return(is_valid);
    }

    SLD_FUNC const u32
    hash_table_memory_size(
        const hash_table_t& hash_table) {

        const u32 size_hash  = (hash_table.capacity  * sizeof(hash_t));
        const u32 size_value = (hash_table.capacity  * hash_table.stride);
        const u32 size_total = (size_hash + size_value);

        return(size_total);
    }
    
    SLD_FUNC bool
    hash_table_memory_init(
        hash_table_t&   hash_table,
        const memory_t& memory) {

        const u32 size_hash  = (hash_table.capacity  * sizeof(hash_t));
        const u32 size_value = (hash_table.capacity  * hash_table.stride);
        const u32 size_total = (size_hash + size_value);

        bool can_init = true;
        can_init &= (memory.size  >= size_needed);
        can_init &= (memory.start != 0);
        if (!can_init) return(can_init);

        hash_table.count       = 0;
        hash_table.array.hash  = (hash_t*)(memory.start);
        hash_table.array.value =   (byte*)(memory.start + size_hash);
    }
    
    SLD_FUNC bool
    hash_table_reset(
        hash_table_t& hash_table) {

        const bool is_valid = hash_table_validate(hash_table);
        
        if (is_valid) {
            hash_table.count = 0;
        }

        return(is_valid);
    }
    
    SLD_FUNC bool
    hash_table_remove(
        hash_table_t&           hash_table,
        const hash_table_key_t& key) {

    }
    
    SLD_FUNC bool
    hash_table_remove_at(
        hash_table_t& hash_table,
        const u32     index) {

    }
    
    SLD_FUNC byte*
    hash_table_insert(
        hash_table_t&     hash_table,
        hash_table_key_t& key) {

        byte*      value       = NULL;
        const u32  index       = hash_table.count; 
        const bool valid_table = hash_table_validate     (hash_table);
        const bool valid_key   = hash_table_validate_key (hash_table, key);
        const bool valid_count = (hash_table.count < hash_table.capacity); 
        const bool valid_hash  = hash_data(key.data, key.length, hash_table.array.hash[index]);
        
        if (!valid_table) { hash_table.error.val = hash_table_error_e_invalid_table; return(value); }
        if (!valid_key)   { hash_table.error.val = hash_table_error_e_invalid_key;   return(value); }
        if (!valid_count) { hash_table.error.val = hash_table_error_e_max_count;     return(value); }
        if (!valid_hash)  { hash_table.error.val = hash_table_error_e_hash_failed;   return(value); }

        ++hash_table.count;
        const u32 offset =  hash_table.stride * index;
        value            = &hash_table.array.value[offset];
        return(value);
    }
    
    SLD_FUNC byte*
    hash_table_insert_at(
        hash_table_t&     hash_table,
        hash_table_key_t& key,
        const u32         index) {

        byte*      value       = NULL;
        const bool valid_table = hash_table_validate     (hash_table);
        const bool valid_key   = hash_table_validate_key (hash_table, key);
        const bool valid_count = (hash_table.count < hash_table.capacity); 
        const bool valid_hash  = hash_data(key.data, key.length, hash_table.array.hash[index]);
        const bool valid_index = (index <= hash_table.count);

        if (!valid_table) { hash_table.error.val = hash_table_error_e_invalid_table;       return(value); }
        if (!valid_key)   { hash_table.error.val = hash_table_error_e_invalid_key;         return(value); }
        if (!valid_count) { hash_table.error.val = hash_table_error_e_max_count;           return(value); }
        if (!valid_hash)  { hash_table.error.val = hash_table_error_e_hash_failed;         return(value); }
        if (!valid_index) { hash_table.error.val = hash_table_error_e_index_out_of_bounds; return(value); }

        ++hash_table.count;
        
        const u32 offset_src =  hash_table.stride * index;
        const u32 offset_dst =  hash_table.stride + offset_src;
        const u32 copy_size  =  hash_table.count  - index;  
        const byte*      src = &hash_table.array.value[offset_src];
        byte*            dst = &hash_table.array.value[offset_dst];

        (void)memory_copy(dst, src, copy_size);

        return(src);
    }
    
    SLD_FUNC bool
    hash_table_search(
        const hash_table_t&     hash_table,
        const hash_table_key_t& key,
        u32&                    index) {

        bool is_valid = true; 
        is_valid &= hash_table_validate     (hash_table);
        is_valid &= hash_table_validate_key (hash_table, key);
        if (!is_valid) return(is_valid);

        const hash_t key_hash = hash_data(key.data, key.length);
        const bool is_found   = hash_search(
            hash_table.count,      // number of values
            key_hash,              // hashed key
            hash_table.array.hash, // stored hashes
            value.index);          // result index

        return(is_found);
    }
    
    SLD_FUNC bool
    hash_table_get_hash_at(
        const hash_table_t& hash_table,
        const u32           index,
        hash_t&             hash) {

        bool is_valid = true; 
        is_valid &= hash_table_validate(hash_table);
        is_valid &= (index < hash_table.count); 
        
        if (is_valid) {
            hash = hash_table.array.hash[index];
        }

        return(is_valid);
    }
    
    SLD_FUNC bool
    hash_table_get_value_at(
        const hash_table_t& hash_table,
        hash_table_value_t& value) {

        bool is_valid = true; 
        is_valid &= hash_table_validate(hash_table);
        is_valid &= (value.index < hash_table.count); 
        
        if (is_valid) {

            const u32 offset =  hash_table.stride * value.index; 
            value.data       = &hash_table.array.value[offset];
        }

        return(is_valid);
    }
};
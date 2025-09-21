#pragma once

#include <zlib-ng.h>

#include "sld-hash.hpp"

namespace sld {

    SLD_API const hash32_t
    hash32(
        const hash32_seed_t seed, 
        const byte*         data,
        const u32           length) {
 
        const hash32_t hash = {
            zng_crc32(seed.val, data, length)
        };

        return(hash);
    }

    SLD_API bool
    hash32_batch(
        const hash32_seed_t seed,
        const byte*         data,
        const u32           stride,
        const u32           count,
        hash32_t*           hashes) {

        bool can_hash = true;
        can_hash &= (data   != NULL);
        can_hash &= (stride != 0);
        can_hash &= (count  != 0);
        can_hash &= (hashes != NULL);

        if (can_hash) {

            for (
                u32 index = 0;
                index < count;
                ++index) {

                const u32 offset     = index * stride; 
                hashes[index].as_u32 = zng_crc32(seed.val, &data[offset], stride);
            }
        }
        return(can_hash);
    }

    SLD_API bool
    hash32_is_equal(
        const hash32_seed_t seed,
        const byte*         data,
        const u32           length,
        const hash32_t      hash) {

        bool is_equal = false;
        bool can_hash = true;
        can_hash &= (data   != NULL);
        can_hash &= (length != 0);

        if (can_hash) {
            const u32 data_hash = zng_crc32(seed.val, data, length);
            is_equal            = (data_hash == hash.as_u32); 
        }

        return(is_equal);
    }

    SLD_API bool
    hash32_search(
        const u32       count,
        const hash32_t  search,
        const hash32_t* array,
        u32&            index) {

        //TODO: SIMD

        bool is_found   = false;
        bool can_search = true;
        can_search &= (count != 0);
        can_search &= (array != NULL);
        if (can_search) {
            for (
                index = 0;
                (index < count) && !is_found;
                ++index) {

                is_found = (search.as_u32 == array[index].as_u32);
            }
        }

        return(is_found);
    }
};
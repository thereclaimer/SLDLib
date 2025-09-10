#pragma once

#include <meow-hash/meow_hash_x64_aesni.h>
#include "sld-hash.hpp"
#include "sld-simd.hpp"

namespace sld {

    SLD_API bool
    hash_data(
        const byte* data,
        const u32   length,
        hash_t&     hash) {

        bool can_hash = true;
        can_hash &= (data   != NULL);
        can_hash &= (length != 0);

        if (can_hash) {

            const meow_u128 meow_hash = MeowHash(MeowDefaultSeed, length, (void*)data);
            _mm_storeu_si128((__m128i*)&hash, meow_hash);
        }

        return(can_hash);
    }

    SLD_API bool
    hash_data_batch(
        const u32   count,
        const byte* data,
        const u32   stride,
        hash_t*     hashes) {

        bool can_hash = true;
        can_hash &= (count  != 0);
        can_hash &= (data   != NULL);
        can_hash &= (stride != 0);
        can_hash &= (hashes != NULL);
        
        if (can_hash) {
            for (
                u32 index = 0;
                index < count;
                ++index) {

                const u32     offset   = index * stride;
                const __m128i hash_reg = MeowHash(MeowDefaultSeed, stride, (void*)&data[offset]);
                _mm_store_si128((__m128i*)&hashes[index], hash_reg);
            }
        }

        return(can_hash);
    }

    SLD_API bool
    hash_search(
        const u32     count,
        const hash_t  search,
        const hash_t* array,
        u32&          index) {

        bool can_search = true;
        can_search &= (count  != 0);
        can_search &= (array != NULL);
        if (!can_search) return(can_search);

        const meow_u128 meow_search = _mm_load_si128((meow_u128*)&search);

        bool is_found = false;
        for (
            index = 0;
            (
                index < count &&
                is_found == false
            );
            ++index) {

            const meow_u128 meow_current = _mm_load_si128((meow_u128*)&array[index]);
            is_found = MeowHashesAreEqual(meow_search, meow_current);
        }

        return(is_found);
    }

    SLD_API bool
    hash_is_equal(
        const byte* data_a,
        const byte* data_b,
        const u32   length) {

        const meow_u128 meow_a   = MeowHash           (MeowDefaultSeed, length, (void*)data_a);
        const meow_u128 meow_b   = MeowHash           (MeowDefaultSeed, length, (void*)data_b);
        const bool      is_equal = MeowHashesAreEqual (meow_a, meow_b); 
        return(is_equal);
    }

    SLD_API bool
    hash_is_equal(
        const hash_t& hash,
        const byte*   data,
        const u32     length) {
  
        const meow_u128 meow_a   = _mm_load_si128((meow_u128*)&hash);
        const meow_u128 meow_b   = MeowHash           (MeowDefaultSeed, length, (void*)data);
        const bool      is_equal = MeowHashesAreEqual (meow_a, meow_b); 
        return(is_equal);
    }
};
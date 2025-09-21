#pragma once

#include <meow-hash/meow_hash_x64_aesni.h>
#include "sld-hash.hpp"
#include "sld-simd.hpp"

namespace sld {

    SLD_API const hash128_t
    hash128_data(
        const hash128_seed_t& seed,
        const byte*        data,
        const u32          length) {

        bool can_hash = true;
        can_hash &= (data   != NULL);
        can_hash &= (length != 0);

        hash128_t hash = {0,0,0,0};
        if (can_hash) {

            const meow_u128 meow_hash = MeowHash((void*)seed.buffer, length, (void*)data);
            _mm_storeu_si128((__m128i*)&hash, meow_hash);
        }

        return(hash);
    }

    SLD_API bool
    hash128_data_batch(
        const hash128_seed_t& in_seed,
        const u32          in_count,
        const byte*        in_data,
        const u32          in_stride,
        hash128_t*            out_hashes) {

        bool can_hash = true;
        can_hash &= (in_count   != 0);
        can_hash &= (in_data    != NULL);
        can_hash &= (in_stride  != 0);
        can_hash &= (out_hashes != NULL);
        
        if (can_hash) {
            for (
                u32 index = 0;
                index < in_count;
                ++index) {

                const u32     offset   = index * in_stride;
                const __m128i hash_reg = MeowHash((void*)in_seed.buffer, in_stride, (void*)&in_data[offset]);
                _mm_store_si128((__m128i*)&out_hashes[index], hash_reg);
            }
        }

        return(can_hash);
    }

    SLD_API bool
    hash128_search(
        const u32      in_count,
        const hash128_t   in_search,
        const hash128_t*  in_array,
        u32&          out_index) {

        bool can_search = true;
        can_search &= (in_count != 0);
        can_search &= (in_array != NULL);
        if (!can_search) return(can_search);

        const meow_u128 meow_search = _mm_load_si128((meow_u128*)&in_search);

        bool is_found = false;
        for (
            out_index = 0;
            (
                out_index    <  in_count &&
                is_found == false
            );
            ++out_index) {

            const meow_u128 meow_current = _mm_load_si128((meow_u128*)&in_array[out_index]);
            is_found = MeowHashesAreEqual(meow_search, meow_current);
        }

        return(is_found);
    }

    SLD_API bool
    hash128_is_equal(
        const hash128_seed_t& seed,
        const byte*        data_a,
        const byte*        data_b,
        const u32          length) {

        const meow_u128 meow_a   = MeowHash           ((void*)seed.buffer, length, (void*)data_a);
        const meow_u128 meow_b   = MeowHash           ((void*)seed.buffer, length, (void*)data_b);
        const bool      is_equal = MeowHashesAreEqual (meow_a, meow_b); 
        return(is_equal);
    }

    SLD_API bool
    hash128_is_equal(
        const hash128_seed_t& seed,
        const hash128_t&      hash,
        const byte*        data,
        const u32          length) {
  
        const meow_u128 meow_a   = _mm_load_si128((meow_u128*)&hash);
        const meow_u128 meow_b   = MeowHash           ((void*)seed.buffer, length, (void*)data);
        const bool      is_equal = MeowHashesAreEqual (meow_a, meow_b); 
        return(is_equal);
    }

    SLD_API void
    hash128_block_begin(
        hash128_state_t&      state,
        const hash128_seed_t& seed) {

        MeowBegin(&state, (void*)seed.buffer);
    }

    SLD_API void
    hash128_block_consume(
        hash128_state_t& state,
        const u64     block_size,
        const byte*   block_data) {

        MeowAbsorb(&state, block_size, (void*)block_data);
    }

    SLD_API const hash128_t
    hash128_block_end(
        hash128_state_t& state) {

        static meow_u8* store_128 = NULL;
        const meow_u128 meow_hash = MeowEnd(&state, store_128);

        hash128_t hash;
        _mm_storeu_si128((__m128i*)&hash, meow_hash);

        return(hash);
    }
};
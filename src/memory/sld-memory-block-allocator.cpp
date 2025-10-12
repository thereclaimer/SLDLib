#pragma once

#include "sld-memory.hpp"
#include "sld-os.hpp"

namespace sld {

    SLD_API void 
    block_allocator_acquire_os_memory(
        block_allocator_t* alctr,
        const u32          size,
        const u32          granularity) {

        // check args
        bool can_acquire = true;
        can_acquire &= (alctr       != NULL);
        can_acquire &= (size        != 0);
        can_acquire &= (granularity <  size);
        assert(can_acquire);

        // align sizes
        const u32 os_aligned_size = os_memory_align_to_granularity (size);
        const u32 block_size      = os_memory_align_to_page        (granularity);
        const u32 alctr_size      = size_align_pow_2(os_aligned_size, block_size); 
        const u32 block_count     = alctr_size / block_size; 

        // check aligned sizes
        can_acquire &= (block_count != 0);
        can_acquire &= (block_size < alctr_size);
        assert(can_acquire);

        // initialize the allocator
        alctr->start       = (addr)os_memory_reserve(NULL, alctr_size);
        alctr->block_count = block_count;
        alctr->block_size  = block_size;
        assert(alctr->start != 0);
    }

    SLD_API void
    block_allocator_release_os_memory(
        block_allocator_t* alctr) {

        block_allocator_assert_valid(alctr);

        const u32  size        = (alctr->block_count * alctr->block_size);
        const bool did_release = os_memory_release((void*)alctr->start, size);
        assert(did_release);
    }

    SLD_API void*
    block_allocator_alloc(
        block_allocator_t* alctr) {

        block_allocator_assert_valid(alctr);

        void* block_memory = NULL;
        u32   block_offset = 0;
        for (
            u32 block = 0;
            block < alctr->block_count;
            ++block) {

            void*      block_commit  = (void*)(alctr->start + block_offset); 
            const bool is_free       = !os_memory_is_committed(block_commit); 
            if (is_free) {

                block_memory = os_memory_commit(block_commit, alctr->block_size);
                if (block_memory != NULL) {
                    assert(block_memory == block_commit);
                }
            }
            block_offset += alctr->block_size; 
        }

        return(block_memory);
    }
    
    SLD_API void
    block_allocator_free(
        block_allocator_t* alctr,
        void*              block) {

        bool can_free = true;
        can_free &= block_allocator_is_valid(alctr);
        can_free &= (block != NULL);
        assert(can_free);

        // make sure this block is managed by the allocator;
        const u32  size_total   = (alctr->block_count * alctr->block_size);
        const addr block_start  = (addr)block;
        const addr alctr_end    = (alctr->start + size_total);
        const u32  block_offset = (block_start  - alctr->start);
        can_free &= (block_start < alctr_end); 
        can_free &= ((block_offset % alctr->block_size) == 0);
        assert(can_free);

        // attempt to free the block
        can_free &= os_memory_decommit(block, alctr->block_size);
        assert(can_free);
    }
    
    SLD_API bool
    block_allocator_is_valid(
        const block_allocator_t* alctr) {

        bool is_valid = (alctr != NULL);
        if (is_valid) {
            is_valid &= (alctr->start       != 0);
            is_valid &= (alctr->block_count != 0);
            is_valid &= (alctr->block_size  != 0);
        }
    }
    
    SLD_API void
    block_allocator_assert_valid(
        const block_allocator_t* alctr) {

        assert(block_allocator_is_valid(alctr));
    }
    
    SLD_API u32
    block_allocator_get_size_total(
        const block_allocator_t* alctr) {

        block_allocator_assert_valid(alctr);

        const u32 size_total = (alctr->block_size * alctr->block_count);
        return(size_total);
    }
    
    SLD_API u32
    block_allocator_get_size_free(
        const block_allocator_t* alctr) {

        block_allocator_assert_valid(alctr);

        u32 size_free    = 0;
        u32 block_offset = 0;
        for (
            u32 block = 0;
            block < alctr->block_count;
            ++block) {

            void*      memory  = (void*)(alctr->start + block_offset);
            const bool is_free = !os_memory_is_committed(memory); 
            if (is_free) {
                size_free += alctr->block_size;
            } 
            block_offset += alctr->block_size;
        }

        return(size_free);
    }
    
    SLD_API u32
    block_allocator_get_size_used(
        const block_allocator_t* alctr) {

        block_allocator_assert_valid(alctr);

        u32 size_used    = 0;
        u32 block_offset = 0;
        for (
            u32 block = 0;
            block < alctr->block_count;
            ++block) {

            void*      memory  = (void*)(alctr->start + block_offset);
            const bool is_used = os_memory_is_committed(memory); 
            if (is_used) {
                size_used += alctr->block_size;
            } 
            block_offset += alctr->block_size;
        }

        return(size_used);
    }
    
    SLD_API u32
    block_allocator_get_blocks_free(
        const block_allocator_t* alctr) {

        block_allocator_assert_valid(alctr);

        u32 blocks_free  = 0;
        u32 block_offset = 0;
        for (
            u32 block = 0;
            block < alctr->block_count;
            ++block) {

            void*      memory  = (void*)(alctr->start + block_offset);
            const bool is_free = !os_memory_is_committed(memory); 
            if (is_free) {
                ++blocks_free;
            } 
            block_offset += alctr->block_size;
        }

        return(blocks_free);
    }
    
    SLD_API u32
    block_allocator_get_blocks_used(
        const block_allocator_t* alctr) {

        block_allocator_assert_valid(alctr);
        
        u32 blocks_used  = 0;
        u32 block_offset = 0;
        for (
            u32 block = 0;
            block < alctr->block_count;
            ++block) {

            void*      memory  = (void*)(alctr->start + block_offset);
            const bool is_used = os_memory_is_committed(memory); 
            if (is_used) {
                ++blocks_used;
            } 
            block_offset += alctr->block_size;
        }

        return(blocks_used);
    }
};
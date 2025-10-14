#ifndef SLD_BLOCK_ALLOCATOR_HPP
#define SLD_BLOCK_ALLOCATOR_HPP

#include "sld-memory.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    struct block_allocator_t {
        memory_t memory;
        u32      block_size;
        u32      block_count;
    };

    SLD_API_INLINE void     block_allocator_reserve_os_memory (block_allocator_t* alctr, const u32 size_total, const u32 size_block);
    SLD_API_INLINE void     block_allocator_release_os_memory (block_allocator_t* alctr);
    SLD_API_INLINE bool     block_allocator_is_valid          (const block_allocator_t* alctr);
    SLD_API_INLINE bool     block_allocator_is_block_valid    (const block_allocator_t* alctr, const memory_t& block);
    SLD_API_INLINE void     block_allocator_assert_valid      (const block_allocator_t* alctr);
    SLD_API_INLINE void*    block_allocator_commit            (const block_allocator_t* alctr);
    SLD_API_INLINE memory_t block_allocator_commit_memory     (const block_allocator_t* alctr);
    SLD_API_INLINE void     block_allocator_decommit          (const block_allocator_t* alctr, void* block);
    SLD_API_INLINE u32      block_allocator_get_size_total    (const block_allocator_t* alctr);
    SLD_API_INLINE u32      block_allocator_get_size_free     (const block_allocator_t* alctr);
    SLD_API_INLINE u32      block_allocator_get_size_used     (const block_allocator_t* alctr);
    SLD_API_INLINE u32      block_allocator_get_blocks_free   (const block_allocator_t* alctr);
    SLD_API_INLINE u32      block_allocator_get_blocks_used   (const block_allocator_t* alctr);

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API_INLINE void 
    block_allocator_reserve_os_memory(
        block_allocator_t* alctr,
        const u32          size_total,
        const u32          size_block) {

        // check args
        bool can_acquire = true;
        can_acquire &= (alctr      != NULL);
        can_acquire &= (size_total != 0);
        can_acquire &= (size_block <  size_total);
        assert(can_acquire);

        // calculate the aligned sizes
        const u32 size_block_aligned = os_memory_align_to_page (size_block);
        const u32 size_total_aligned = size_align_pow_2        (size_total, size_block_aligned);

        // reserve memory
        memory_t memory;
        memory.addr = 0;
        memory.size = size_total_aligned;
        memory_os_reserve(memory);

        // initialize the allocator
        alctr->memory      = memory;
        alctr->block_size  = size_block_aligned;
        alctr->block_count = alctr->memory.size / size_block_aligned;
        block_allocator_assert_valid(alctr);
    }

    SLD_API_INLINE void
    block_allocator_release_os_memory(
        block_allocator_t* alctr) {

        block_allocator_assert_valid(alctr);
        memory_os_release(alctr->memory);
        alctr->memory.addr = 0;
        alctr->memory.size = 0;
        alctr->block_count = 0;
        alctr->block_size  = 0;
    }

    SLD_API_INLINE bool
    block_allocator_is_valid(
        const block_allocator_t* alctr) {

        bool is_valid = (alctr != NULL);
        if (is_valid) {

            const bool is_reserved  = os_memory_is_reserved  (alctr->memory.ptr);
            const bool is_committed = os_memory_is_committed (alctr->memory.ptr);

            is_valid &= (is_reserved || is_committed);
            is_valid &= (alctr->memory.addr != 0);  
            is_valid &= (alctr->block_count != 0);  
            is_valid &= (alctr->block_size  != 0);
            is_valid &= size_is_pow_2(alctr->block_size); 
        }
        return(is_valid);
    }

    SLD_API_INLINE bool
    block_allocator_is_block_valid(
        const block_allocator_t* alctr,
        const memory_t&          block_memory) {

        bool is_valid = true;
        is_valid &= (alctr != NULL && memory_is_valid(alctr->memory));
        is_valid &= memory_is_valid(block_memory);

        if (is_valid) {

            // make sure this block is managed by the allocator;
            const addr last_block_addr = (alctr->memory.addr + alctr->memory.size) - alctr->block_size; 
            const u32  block_offset    = (block_memory.addr  - alctr->memory.addr);
            is_valid &= (block_memory.addr < last_block_addr); 
            is_valid &= ((block_offset % alctr->block_size) == 0);
        }

        return(is_valid);
    }

    SLD_API_INLINE void
    block_allocator_assert_valid(
        const block_allocator_t* alctr) {

        assert(block_allocator_is_valid(alctr));
    }

    SLD_API_INLINE void*
    block_allocator_commit(
        const block_allocator_t* alctr) {

        block_allocator_assert_valid(alctr);

        memory_t block_memory;
        block_memory.size = alctr->block_size;
        block_memory.addr = alctr->memory.addr;

        for (
            u32 block = 0;
                block < alctr->block_count;
              ++block) {

            const bool is_free = memory_is_os_reserved(block_memory);
            if (is_free) {
                memory_os_commit(block_memory);
                return(block_memory.ptr);
            }
            block_memory.addr += alctr->block_size;
        }

        return(NULL);
    }
    
    SLD_API_INLINE memory_t
    block_allocator_commit_memory(
        const block_allocator_t* alctr) {
        
        memory_t memory;
        memory.ptr  = block_allocator_commit(alctr);
        memory.size = alctr->block_size;
        return(memory);
    }


    SLD_API_INLINE void
    block_allocator_free(
        const block_allocator_t* alctr,
        void*                    block) {

        bool can_free = true;
        can_free &= block_allocator_is_valid(alctr);
        can_free &= (block != NULL);
        assert(can_free);

        // make sure this block is managed by the allocator;
        memory_t block_memory;
        block_memory.ptr  = block;
        block_memory.size = alctr->block_size;
        can_free &= block_allocator_is_block_valid(alctr, block_memory); 
        assert(can_free);

        // free the block
        memory_os_decommit(block_memory);
    }
    
    SLD_API_INLINE u32
    block_allocator_get_size_total(
        const block_allocator_t* alctr) {

        block_allocator_assert_valid(alctr);

        const u32 size_total = (alctr->block_size * alctr->block_count);
        return(size_total);
    }
    
    SLD_API_INLINE u32
    block_allocator_get_size_free(
        const block_allocator_t* alctr) {

        block_allocator_assert_valid(alctr);

        u32 size_free    = 0;
        memory_t block_memory;
        block_memory.size = alctr->block_size;
        block_memory.ptr  = alctr->memory.ptr; 
        for (
            u32 block = 0;
            block < alctr->block_count;
            ++block) {
            
            const bool is_free = memory_is_os_reserved(block_memory); 
            if (is_free) {
                size_free += block_memory.size;
            } 
            block_memory.addr += block_memory.size;
        }

        assert(size_free <= (block_allocator_get_size_total(alctr)));
        return(size_free);
    }
    
    SLD_API_INLINE u32
    block_allocator_get_size_used(
        const block_allocator_t* alctr) {

        block_allocator_assert_valid(alctr);

        u32 size_used    = 0;
        memory_t block_memory;
        block_memory.size = alctr->block_size;
        block_memory.ptr  = alctr->memory.ptr; 
        for (
            u32 block = 0;
                block < alctr->block_count;
              ++block) {
            
            const bool is_used = memory_is_os_committed(block_memory); 
            if (is_used) {
                size_used += block_memory.size;
            } 
            block_memory.addr += block_memory.size;
        }


        assert(size_used <= (block_allocator_get_size_total(alctr)));
        return(size_used);
    }
    
    SLD_API_INLINE u32
    block_allocator_get_blocks_free(
        const block_allocator_t* alctr) {
        
        const u32 free_size  = block_allocator_get_size_free(alctr); 
        const u32 free_count = free_size / alctr->block_size;
        assert(free_count <= alctr->block_count);
        return(free_count);
    }
    
    SLD_API_INLINE u32
    block_allocator_get_blocks_used(
        const block_allocator_t* alctr) {

        const u32 used_size  = block_allocator_get_size_free(alctr); 
        const u32 used_count = used_size / alctr->block_size;
        assert(used_count <= alctr->block_count);
        return(used_count);
    }
};

#endif //SLD_BLOCK_ALLOCATOR_HPP
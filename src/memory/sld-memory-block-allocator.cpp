#pragma once

#include "sld-memory.hpp"

namespace sld {

    SLD_API block_allocator_t*
    block_allocator_memory_init(
        const void* memory,
        const u32   total_size,
        const u32   block_size) {

        // calculate aligned block size and count 
        constexpr u32 size_struct_alctr  = sizeof(block_allocator_t);
        constexpr u32 size_struct_alloc  = sizeof(allocation_t);
        const     u32 size_block_aligned = size_align_pow_2(block_size, size_struct_alloc);
        const     u32 size_block_mem     = (total_size - size_struct_alctr); 
        const     u32 block_count        = (total_size / size_block_mem);   

        // check args 
        bool is_valid = true;
        is_valid &= (memory      != nullptr); 
        is_valid &= (total_size  != 0);
        is_valid &= (block_size  != 0);
        is_valid &= (block_size  <  total_size);
        is_valid &= (block_count != 0);
        if (!is_valid) return(NULL);

        // cast pointers 
        const addr         block_mem_start   = ((addr)memory) + size_struct_alctr;
        allocation_t*      block_allocations = (allocation_t*)block_mem_start;
        block_allocator_t* block_allocator   = (block_allocator_t*)memory;

        // initialize allocation list
        for (
            u32 block = 0;
            block < block_count;
            ++block) {

            // calculate addresses
            const u32  block_offset        = (block * size_block_aligned);
            const addr block_start_current = block_mem_start + block_offset;
            const addr block_start_prev    = (block == 0)               ? 0 : (block_start_current - size_block_aligned);
            const addr block_start_next    = (block == block_count - 1) ? 0 : (block_start_current + size_block_aligned);  

            // initialize the allocation
            allocation_t* block_current = (allocation_t*)block_start_current;
            block_current->alctr        = block_allocator;
            block_current->next         = (allocation_t*)block_start_next;
            block_current->prev         = (allocation_t*)block_start_prev;
            block_current->size         = block_size;
            block_current->offset       = block_offset;
        }

        // initialize the allocator
        block_allocator->allocation_list.used = nullptr;
        block_allocator->allocation_list.free = block_allocations;
        block_allocator->size                 = size_block_mem;
        block_allocator->granularity          = block_size; 
        block_allocator->block_count          = block_count; 
        return(block_allocator);
    }

    SLD_API block_allocator_t*
    block_allocator_arena_init(
        arena_t* const arena,
        const u32      total_size,
        const u32      block_size) {

        // check args
        bool is_valid = true;
        is_valid &= (arena      != nullptr);
        is_valid &= (total_size != 0);
        is_valid &= (block_size != 0);        
        is_valid &= (block_size <  total_size);        
        if (!is_valid) return(nullptr);

        // allocate arena memory
        constexpr u32   alignment = sizeof(allocation_t);
        const     void* memory    = arena_push_bytes(arena,  total_size, alignment);
        if (!memory) return(nullptr);

        // initialize the allocator
        block_allocator_t* allocator = block_allocator_memory_init (memory, total_size, block_size);        
        
        // if that failed, assert that we can free
        // the memory we just allocated
        if (!allocator) {
            const bool did_free = arena_pull_bytes(arena, total_size, alignment); 
            assert(did_free);
        } 
        return(allocator);
    }

    SLD_API void*
    block_allocator_alloc_abs(
        block_allocator_t* const allocator) {

        allocation_t* block  = allocator_reuse_next_free_allocation (allocator);
        void*         memory = allocation_get_memory                (block);
        return(memory); 
    }

    SLD_API u32
    block_allocator_alloc_rel(
        block_allocator_t* const allocator) {

        allocation_t* block  = allocator_reuse_next_free_allocation (allocator);
        return(block != NULL ? block->offset : 0);
    }

    SLD_API bool
    block_allocator_free_abs(
        block_allocator_t* const allocator,
        void* const              block) {

        constexpr bool do_not_consolidate = false;

        allocation_t* allocation = allocation_from_memory(block);

        const bool result = (allocation != NULL)
            ? allocator_free_allocation(allocator, allocation, do_not_consolidate)
            : false;

        return(result);
    }

    SLD_API bool
    block_allocator_free_rel(
        block_allocator_t* const allocator,
        const u32                block_number) {

        constexpr bool do_not_consolidate = false;

    }
};
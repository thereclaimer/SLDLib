#pragma once

#include "sld-memory.hpp"

namespace sld {

    SLD_API block_allocator_t*
    block_allocator_init(
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
        assert(is_valid);

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

    SLD_API void*
    block_allocator_alloc_abs(
        block_allocator_t* const allocator,
        const u32                size,
        const u32                alignment) {

        // validate the allocator
        const bool is_valid = block_allocator_validate(allocator);
        assert(is_valid);

        // make sure our blocks are big enough
        const u32 size_aligned = size_align(size, alignment);
        assert(size_aligned <= allocator->granularity);

        // get the next free allocation
        allocation_t* alloc = allocator->allocation_list.free;
        allocation_t* used  = allocator->allocation_list.used;         
        if (!alloc) return(NULL);

        // add the allocation to the used list
        alloc->prev                     = NULL;
        alloc->next                     = used;
        if (used) used->prev            = alloc;
        allocator->allocation_list.used = alloc;        

        // return the memory
        void* memory = allocation_get_memory(alloc);
        return(memory); 
    }

    SLD_API u32
    block_allocator_alloc_rel(
        block_allocator_t* const allocator,
        const u32                size,
        const u32                alignment) {

        // validate the allocator
        const bool is_valid = block_allocator_validate(allocator);
        assert(is_valid);

        // make sure our blocks are big enough
        const u32 size_aligned = size_align(size, alignment);
        assert(size_aligned <= allocator->granularity);

        // get the next free allocation
        allocation_t* alloc = allocator->allocation_list.free;
        allocation_t* used  = allocator->allocation_list.used;         
        if (!alloc) return(NULL);

        // add the allocation to the used list
        // and return the offset
        alloc->prev                     = NULL;
        alloc->next                     = used;
        if (used) used->prev            = alloc;
        allocator->allocation_list.used = alloc;        
        return(alloc->offset);        
    }

    SLD_API bool
    block_allocator_free_abs(
        block_allocator_t* const allocator,
        void* const              memory) {

        // validate the allocator
        const bool is_valid = block_allocator_validate(allocator);
        assert(is_valid);

        allocation_t* alloc = allocation_from_memory(memory);
        allocation_t* free  = allocator->allocation_list.free; 

        const bool did_free = (alloc != NULL) && (alloc->alctr == allocator);
        if (did_free) {

            alloc->prev                     = NULL;
            alloc->next                     = free;
            if (free) free->prev            = alloc;
            allocator->allocation_list.free = alloc;
        }
        return(did_free);
    }

    SLD_API bool
    block_allocator_free_rel(
        block_allocator_t* const allocator,
        const u32                offset) {

        // validate the allocator
        const bool is_valid = block_allocator_validate(allocator);
        assert(is_valid);

        allocation_t* alloc = allocation_from_offset(allocator, offset);
        allocation_t* free  = allocator->allocation_list.free; 
        if (!alloc) return(false);

        alloc->prev                     = NULL;
        alloc->next                     = free;
        if (free) free->prev            = alloc;
        allocator->allocation_list.free = alloc;
        return(true);
    }

    SLD_API bool
    block_allocator_validate(
        block_allocator_t* const allocator) {

        bool is_valid = (allocator != NULL);
        if (is_valid) {
            is_valid &= (allocator->block_count != 0);
            is_valid &= allocator_validate(allocator); 
        }
        return(is_valid);
    }

    SLD_API bool
    block_allocator_reset(
        block_allocator_t* const allocator) {

        // validate the allocator
        const bool is_valid = block_allocator_validate(allocator);
        assert(is_valid);

        allocation_t* free = allocator->allocation_list.free;
        allocation_t* used = allocator->allocation_list.used;

        if (free) free->prev = used;
        if (used) {
            used->next = free;
            allocator->allocation_list.free = used;
        }
        allocator->allocation_list.used = NULL;
        
        return(true);
    }
};
#pragma once

#include "sld-allocator.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct block_allocator_t {
        u64 granularity;
        struct {
            block_allocation_t* free;
            block_allocation_t* used;
        } allocation_list;
    };

    struct block_allocation_t {
        block_allocator_t*  allocator;
        block_allocation_t* next;
        block_allocation_t* prev;
    };

    //-------------------------------------------------------------------
    // CONSTANTS
    //-------------------------------------------------------------------

    constexpr u64 _size_block_allocation = sizeof(block_allocation_t);
    constexpr u64 _size_block_allocator  = sizeof(block_allocator_t); 

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API bool
    block_allocator_validate(
        block_allocator_t* block_allocator) {

        bool is_valid = (block_allocator != NULL);
        if (is_valid) {
            is_valid &= (block_allocator->granularity != 0);
            is_valid &= (block_allocator->allocation_list.free != NULL || block_allocator->allocation_list.used != NULL);
        }
        return(is_valid);
    }

    SLD_API const u64
    block_allocator_get_required_memory_size(
        const u64 count,
        const u64 granularity) {

        const u64 granularity_pow_2 = size_round_up_pow2(granularity);
        const u64 size_mem          = count * granularity_pow_2;
        const u64 size_block_list   = count * _size_block_allocation; 
        const u64 size_required     = (
            size_mem        +
            size_block_list +
            _size_block_allocator
        );
        return(size_required);
    }

    SLD_API block_allocator_t*
    block_allocator_init_from_memory(
        const void*     block_memory,
        const u64       block_memory_size,
        const u64       block_size) {

        block_allocator_t* allocator = NULL;

        // calculate the number of blocks we can fit in the memory
        const u64 granularity_pow_2 = size_round_up_pow2(block_size);
        const u64 size_allocation   = granularity_pow_2 + _size_block_allocation;
        const u64 size_mem          = block_memory_size - _size_block_allocator;
        const u64 count_blocks      = size_mem / size_allocation;

        // verify we have enough memory to proceed
        bool can_init = true;
        can_init &= (granularity_pow_2 != 0);
        can_init &= (size_allocation   != 0);
        can_init &= (size_mem          != 0);
        can_init &= (count_blocks      != 0);
        can_init &= (block_memory      != NULL);
        return(allocator);

        // initialize allocator
        allocator = (block_allocator_t*)block_memory;
        allocator->granularity          = granularity_pow_2;
        allocator->allocation_list.free = (block_allocation_t*)(((addr)block_memory) + _size_block_allocator);
        allocator->allocation_list.used = NULL;

        // initialize block list
        const addr start = (addr)allocator->allocation_list.free;
        for (
            u32 block = 0;
            block < count_blocks;
            ++block) {

            // calculate block addresses
            const u64           offset        = (block * allocator->granularity);
            const addr          start_current = (start + offset); 
            const addr          start_prev    = (block == 0)                ? 0 : (start_current - granularity_pow_2); 
            const addr          start_next    = (block == count_blocks - 1) ? 0 : (start_current + granularity_pow_2); 

            // cast pointers            
            block_allocation_t* current = (block_allocation_t*)start_current; 
            current->allocator          = allocator;
            current->prev               = (block_allocation_t*)start_prev; 
            current->next               = (block_allocation_t*)start_next;
        }

        return(allocator);
    }

    SLD_API block_allocator_t*
    block_allocator_init_from_arena(
        arena_t*  arena,
        const u64 size,
        const u64 granularity) {

        const u64   granularity_pow_2 = size_round_up_pow2(granularity);
        const void* memory            = (void*)arena_push_bytes(
            arena,
            size,
            granularity_pow_2);

        block_allocator_t* allocator = block_allocator_init_from_memory(
            memory,
            size,
            granularity_pow_2
        );

        return(allocator);
    }

    SLD_API void*                   
    block_alloc(
        block_allocator_t* block_allocator) {

        void* block = NULL;

        const bool can_alloc = (
            block_allocator_validate(block_allocator) &&
            block_allocator->allocation_list.free != NULL
        );

        if (can_alloc) {

            block_allocation_t* alloc     = block_allocator->allocation_list.free;
            block_allocation_t* next_used = block_allocator->allocation_list.used;
            block_allocation_t* next_free = alloc->next;

            if (next_free != NULL) next_free->prev = NULL;
            if (next_used != NULL) next_used->prev = alloc;

            block_allocator->allocation_list.free = next_free;         
            block_allocator->allocation_list.used = alloc;
            alloc->next = next_used; 
            alloc->prev = NULL;

            const addr start_alloc = (addr)alloc;
            const addr start_block = start_alloc + _size_block_allocation;
            block                  = (void*)start_block;
        }

        return(block);
    }

    SLD_API bool                    
    block_free(
        block_allocator_t* block_allocator,
        const void*        block) {
        
        const addr          start_block = (addr)block;
        const addr          start_alloc = (start_block != NULL) ? (start_block - _size_block_allocation) : NULL;
        block_allocation_t* alloc       = (block_allocation_t*)start_alloc;

        const bool can_free = (
            block_allocator_validate(block_allocator) &&
            alloc != NULL                             &&
            alloc->allocator == block_allocator
        );

        if (can_free) {

            block_allocation_t* next_used = alloc->next;
            block_allocation_t* next_free = block_allocator->allocation_list.free;

            if (alloc)     next_used->prev = NULL;
            if (next_free) next_free->prev = alloc;

            alloc->next = next_free;

            block_allocator->allocation_list.used = next_used;
            block_allocator->allocation_list.free = next_free;
        } 

        return(can_free);
    }
};
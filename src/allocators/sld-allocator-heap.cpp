#pragma once

#include "sld-allocator.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct heap_allocator_t {
        u64 granularity;
        struct {
            heap_allocation_t* free;
            heap_allocation_t* used;
        } allocation_list;
    };

    struct heap_allocation_t {
        heap_allocator_t*  allocator;
        heap_allocation_t* next;
        heap_allocation_t* prev;
        u64                size;
    };

    //-------------------------------------------------------------------
    // CONSTANTS
    //-------------------------------------------------------------------

    constexpr u64 _size_heap_allocator  = sizeof(heap_allocator_t);
    constexpr u64 _size_heap_allocation = sizeof(heap_allocation_t);

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API const u64
    heap_allocator_get_required_memory_size(
        const u64 heap_size) {

        const u64 size_required = (
            _size_heap_allocator  +
            _size_heap_allocation +
            heap_size            
        );

        return(size_required);
    }

    SLD_API heap_allocator_t*
    heap_allocator_init_from_memory(
        const void* heap_start,
        const u64   heap_size,
        const u64   heap_granularity) {

        heap_allocator_t* allocator = NULL;

        bool can_init = true;
        can_init &= (heap_start       != NULL);
        can_init &= (heap_size        != 0);
        can_init &= (heap_granularity != 0);        
        can_init &= (heap_granularity <= heap_size);        
        if (!can_init) return(allocator);

        const addr free_start   = (addr)heap_start + _size_heap_allocator;
        heap_allocation_t* free = (heap_allocation_t*)free_start;
        free->allocator = allocator;
        free->next      = NULL;
        free->prev      = NULL;
        free->size      = heap_size - (_size_heap_allocator + _size_heap_allocation); 

        allocator                       = (heap_allocator_t*)heap_start;
        allocator->granularity          = size_round_up_pow2(heap_granularity);
        allocator->allocation_list.free = free;
        allocator->allocation_list.used = NULL;
        return(allocator);
    }

    SLD_API heap_allocator_t*
    heap_allocator_init_from_arena(
        arena_t*  arena,
        const u64 heap_size,
        const u64 heap_granularity) {

        const void* heap_memory = (void*)arena_push_bytes(arena, heap_size);

        heap_allocator_t* allocator = heap_allocator_init_from_memory(
            heap_memory,
            heap_size,
            heap_granularity
        );

        return(allocator);
    }   

    SLD_API bool
    heap_allocator_validate(
        heap_allocator_t* heap_allocator) {

        bool is_valid = (heap_allocator != NULL);
        if (is_valid) {
            is_valid &= size_is_pow_2(heap_allocator->granularity);
            is_valid &= (heap_allocator->allocation_list.free != NULL || heap_allocator->allocation_list.used != NULL);  
        }
        return(is_valid);
    }

    SLD_API void*
    heap_alloc(
        heap_allocator_t* heap_allocator, 
        const u64         size) {

        void* memory = NULL;

        // check we can allocate
        const bool can_alloc = (
            heap_allocator_validate(heap_allocator) &&
            size != 0
        );
        if (!can_alloc) return(memory);

        // calculate the size aligned to the next power of 2
        const u64 size_min  = size + sizeof(heap_allocation_t);
        const u64 size_pow2 = (size_min <= heap_allocator->granularity)
            ? heap_allocator->granularity
            : size_round_up_pow2(size_min);
        
        // find the first free node that can fit the size
        heap_allocation_t* alloc = NULL;
        for (
            heap_allocation_t* node = heap_allocator->allocation_list.free;
            node != NULL && alloc == NULL;
            node = node->next) {

            if (node->size >= size_pow2) alloc = node;
        }

        // return if we don't have one
        if (!alloc) return(memory);

        // if the node size is greater than the needed size
        // we need to split it
        const u64 space_remaining = (alloc->size - size_pow2);  
        alloc->size               = size_pow2;
        if (space_remaining > 0) {

            heap_allocation_t* split = (heap_allocation_t*)(((addr)alloc) + size_pow2);
            split->prev      = alloc;
            split->next      = alloc->next;
            split->allocator = heap_allocator;
            split->size      = space_remaining;

            heap_allocation_t* split_next = split->next;
            if (split_next) split_next->prev = split;
            alloc->next = split;
        };

        // remove this node from the free list
        heap_allocation_t* next_free = alloc->next;        
        heap_allocation_t* prev_free = alloc->prev;        
        if (next_free) next_free->prev = prev_free;
        if (prev_free) prev_free->next = next_free;
        if (heap_allocator->allocation_list.free == alloc) heap_allocator->allocation_list.free = alloc->next;

        // add the node to the used list
        heap_allocation_t* next_used = heap_allocator->allocation_list.used;
        if (next_used) next_used->prev = alloc;
        alloc->prev = NULL;
        alloc->next = next_used;
        heap_allocator->allocation_list.used = alloc;                

        // calculate the memory for this allocation
        const addr start_alloc  = (addr)alloc;
        const addr start_memory = start_alloc + sizeof(heap_allocation_t); 
        memory                  = (void*)start_memory; 
        return(memory);
    }

    SLD_API bool
    heap_free(
        heap_allocator_t* heap_allocator,
        const void*       memory) {

        heap_allocation_t* tmp  = NULL;
        heap_allocation_t* next = NULL;
        heap_allocation_t* prev = NULL;

        // calculate the allocation header
        const addr         start_memory = (addr)memory;
        const addr         start_alloc  = (start_memory != 0) ? (start_memory - _size_heap_allocation) : 0; 
        heap_allocation_t* alloc        = (heap_allocation_t*)start_alloc; 

        // make sure we can free the allocation
        const bool can_free = (
            heap_allocator_validate(heap_allocator) &&
            alloc            != NULL                &&
            alloc->allocator == heap_allocator
        );
        if (!can_free) return(can_free);

        // remove the allocation from the used list
        next = alloc->next;
        prev = alloc->prev;
        if (next) next->prev = prev;
        if (prev) prev->next = next;
        if (heap_allocator->allocation_list.used == alloc) heap_allocator->allocation_list.used = next;

        // calculate where we need to add the allocation
        // in the free list
        for (
            heap_allocation_t* current = heap_allocator->allocation_list.free;
            current != NULL;
            current = current->next) {

            const addr start_current = (addr)current;
            const addr start_next    = (addr)current->next; 

            const bool insert = (
                start_alloc > start_current &&
                start_alloc < start_next
            );

            if (insert) {
                prev = current;
                next = current->next;
                break; 
            }            
        }

        // update the list
        alloc->next = next;
        alloc->prev = prev;
        if (next) next->prev = alloc;
        if (prev) prev->next = alloc;

        // determine if any of these nodes should be merged
        const addr start_prev        = (addr)prev;
        const addr start_next        = (addr)next;
        const bool should_merge_prev = prev && (start_alloc == (start_prev  + prev->size));
        const bool should_merge_next = next && (start_next  == (start_alloc + alloc->size));

        if (should_merge_prev) {

            prev->size += alloc->size;
            prev->next  = alloc->next;

            if (alloc->next) {
                tmp       = alloc->next;
                tmp->prev = prev;
            }
            alloc = prev;
            next  = alloc->next;
        }
        if (should_merge_next) {

            alloc->size += next->size;
            alloc->next  = next->next;

            if (next->next) {
                tmp       = next->next;
                tmp->prev = alloc;
            }
        }
        return(can_free);
    }
};
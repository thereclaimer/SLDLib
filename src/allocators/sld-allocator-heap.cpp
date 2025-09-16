#pragma once

#include "sld-allocator.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct heap_alctr_t {
        u64 granularity;
        u64 size_total;
        u64 size_used;
        struct {
            heap_alloc_t* free;
            heap_alloc_t* used;
        } alloc_list;
    };

    struct heap_alloc_t {
        heap_alctr_t* alctr;
        heap_alloc_t* next;
        heap_alloc_t* prev;
        u64           size;
    };

    //-------------------------------------------------------------------
    // CONSTANTS
    //-------------------------------------------------------------------

    constexpr u64 _size_alctr  = sizeof(heap_alctr_t);
    constexpr u64 _size_heap_allocation = sizeof(heap_alloc_t);

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API heap_alctr_t*
    heap_alctr_init_from_memory(
        const void* start,
        const u64   size,
        const u64   granularity) {

        // check args
        bool can_init = true;
        can_init &= (start       != NULL);
        can_init &= (size        != 0);
        can_init &= (granularity != 0);        
        can_init &= (granularity <= size);        
        if (!can_init) return(NULL);

        // cast pointers
        const addr start_addr = (addr)start;
        heap_alctr_t* alctr   = (heap_alctr_t*)start_addr;
        heap_alloc_t* free    = (heap_alloc_t*)(start_addr + _size_alctr);

        // init first free node
        free->alctr = alctr;
        free->next  = NULL;
        free->prev  = NULL;
        free->size  = size - (_size_alctr + _size_heap_allocation); 

        // init allocator
        alctr->granularity     = size_round_up_pow2(granularity);
        alctr->size_total      = size;
        alctr->size_used       = size - free->size;  
        alctr->alloc_list.free = free;
        alctr->alloc_list.used = NULL;
        return(alctr);
    }

    SLD_API heap_alctr_t*
    heap_alctr_init_from_arena(
        arena_t*  arena,
        const u64 heap_size,
        const u64 heap_granularity) {

        const void* heap_memory = (void*)arena_push_bytes(arena, heap_size);

        heap_alctr_t* allocator = heap_alctr_init_from_memory(
            heap_memory,
            heap_size,
            heap_granularity
        );

        return(allocator);
    }   

    SLD_API bool
    heap_alctr_validate(
        heap_alctr_t* alctr) {

        bool is_valid = (alctr != NULL);
        if (is_valid) {
            is_valid &= size_is_pow_2(alctr->granularity);
            is_valid &= (alctr->alloc_list.free != NULL || alctr->alloc_list.used != NULL);  
        }
        return(is_valid);
    }

    SLD_API void*
    heap_alctr_alloc(
        heap_alctr_t* alctr, 
        const u64     size) {

        void* memory = NULL;

        // check we can allocate
        const bool can_alloc = (
            heap_alctr_validate(alctr) &&
            size != 0
        );
        if (!can_alloc) return(memory);

        // calculate the size aligned to the next power of 2
        const u64 size_min  = size + sizeof(heap_alloc_t);
        const u64 size_pow2 = (size_min <= alctr->granularity)
            ? alctr->granularity
            : size_round_up_pow2(size_min);
        
        // find the first free node that can fit the size
        heap_alloc_t* alloc = NULL;
        for (
            heap_alloc_t* node = alctr->alloc_list.free;
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

            heap_alloc_t* split = (heap_alloc_t*)(((addr)alloc) + size_pow2);
            split->prev  = alloc;
            split->next  = alloc->next;
            split->alctr = alctr;
            split->size  = space_remaining;

            heap_alloc_t*   split_next       = split->next;
            if (split_next) split_next->prev = split;

            alloc->next = split;
        };

        // remove this node from the free list
        heap_alloc_t* next_free = alloc->next;        
        heap_alloc_t* prev_free = alloc->prev;        
        if (next_free) next_free->prev = prev_free;
        if (prev_free) prev_free->next = next_free;
        if (alctr->alloc_list.free == alloc) alctr->alloc_list.free = alloc->next;

        // add the node to the used list
        heap_alloc_t* next_used = alctr->alloc_list.used;
        if (next_used) next_used->prev = alloc;
        alloc->prev = NULL;
        alloc->next = next_used;
        alctr->alloc_list.used = alloc;                

        // update the size used
        alctr->size_used += size_pow2;

        // calculate the memory for this allocation
        const addr start_alloc  = (addr)alloc;
        const addr start_memory = start_alloc + sizeof(heap_alloc_t); 
        memory                  = (void*)start_memory; 
        return(memory);
    }

    SLD_API bool
    heap_alctr_free(
        heap_alctr_t* alctr,
        const void*       memory) {

        heap_alloc_t* tmp  = NULL;
        heap_alloc_t* next = NULL;
        heap_alloc_t* prev = NULL;

        // calculate the allocation header
        const addr         start_memory = (addr)memory;
        const addr         start_alloc  = (start_memory != 0) ? (start_memory - _size_heap_allocation) : 0; 
        heap_alloc_t* alloc        = (heap_alloc_t*)start_alloc; 

        // make sure we can free the allocation
        bool can_free = true;
        can_free &= heap_alctr_validate(alctr);
        can_free &= can_free && (alloc->alctr == alctr);
        if (!can_free) return(can_free);

        // cache this for when we update the heap size used
        const u64 alloc_size = alloc->size;

        // remove the allocation from the used list
        next = alloc->next;
        prev = alloc->prev;
        if (next) next->prev = prev;
        if (prev) prev->next = next;
        if (alctr->alloc_list.used == alloc) alctr->alloc_list.used = next;

        // calculate where we need to add the allocation
        // in the free list
        for (
            heap_alloc_t* current = alctr->alloc_list.free;
            current != NULL;
            current = current->next) {

            const addr start_next = (addr)current; 
            const addr start_prev = (addr)current->prev;

            bool insert = true;
            insert &= (start_alloc > start_prev);
            insert &= (start_alloc < start_next);
            if (insert) {
                next = current;
                prev = current->prev;
                break; 
            }            
        }

        // there should always be something here
        const bool can_insert = (next != NULL || prev != NULL);
        assert(can_insert);

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

        // update heap size
        alctr->size_used -= alloc_size;
        return(can_free);
    }

    SLD_API bool
    heap_alctr_reset(
        heap_alctr_t* alctr) {

        const bool can_reset = heap_alctr_validate(alctr);
        if (can_reset) {

            const addr free_start   = (addr)alctr + _size_alctr;
            heap_alloc_t* free = (heap_alloc_t*)free_start;
            free->alctr = alctr;
            free->next      = NULL;
            free->prev      = NULL;
            free->size      = alctr->size_total - (_size_alctr + _size_heap_allocation); 

            alctr->size_used            = alctr->size_total - free->size;  
            alctr->alloc_list.free = free;
            alctr->alloc_list.used = NULL;
        }
        return(can_reset);
    }
};
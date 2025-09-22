#pragma once

#include "sld-allocator.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct heap_alctr_t {
        struct {
            heap_alloc_t* free;
            heap_alloc_t* used;
        } alloc_list;
        u32   granularity;
        u32   size_total;
        u32   size_used;
        pad32 pad;
    };

    struct heap_alloc_t {
        heap_alctr_t* alctr;
        heap_alloc_t* next;
        heap_alloc_t* prev;
        u32           size;
        pad32         pad;
    };

    //-------------------------------------------------------------------
    // CONSTANTS
    //-------------------------------------------------------------------

    constexpr u32 _size_alctr           = sizeof(heap_alctr_t);
    constexpr u32 _size_heap_allocation = sizeof(heap_alloc_t);

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API heap_alctr_t*
    heap_alctr_init_from_memory(
        const void* start,
        const u32   size,
        const u32   granularity) {

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
        const u32 heap_size,
        const u32 heap_granularity) {

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
    heap_alctr_alloc_abs(
        heap_alctr_t* alctr, 
        const u32     size) {

        void* memory = NULL;

        // check we can allocate
        const bool can_alloc = (
            heap_alctr_validate(alctr) &&
            size != 0
        );
        if (!can_alloc) return(memory);

        // calculate the size aligned to the next power of 2
        const u32 size_aligned = size_align_pow_2(size, alctr->granularity);
        
        // find the first free node that can fit the size
        heap_alloc_t* alloc = NULL;
        for (
            heap_alloc_t* node = alctr->alloc_list.free;
            node != NULL && alloc == NULL;
            node = node->next) {

            if (node->size >= size_aligned) alloc = node;
        }

        // return if we don't have one
        if (!alloc) return(memory);

        // if the node size is greater than the needed size
        // we need to split it
        const u32 space_remaining = (alloc->size - size_aligned);  
        alloc->size               = size_aligned;
        if (space_remaining > 0) {

            heap_alloc_t* split = (heap_alloc_t*)(((addr)alloc) + size_aligned);
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
        alctr->size_used += size_aligned;

        // calculate the memory for this allocation
        const addr start_alloc  = (addr)alloc;
        const addr start_memory = start_alloc + sizeof(heap_alloc_t); 
        memory                  = (void*)start_memory; 
        return(memory);
    }

    SLD_API const alloc_hnd_t
    heap_alctr_alloc_rel(
        heap_alctr_t* alctr,
        const u32     size) {

        alloc_hnd_t hnd = {0};

        void* mem = heap_alctr_alloc_abs(alctr, size);
        if (mem) {
            
            const addr start_mem   = (addr)mem;
            const addr start_alctr = (addr)alctr;
            const u32  diff        = (start_mem - start_alctr);
            hnd.val                = *(u32*)&diff;
        }
        return(hnd);
    }

    SLD_API bool
    heap_alctr_free_abs(
        heap_alctr_t* alctr,
        const void*   memory) {

        heap_alloc_t* tmp  = NULL;
        heap_alloc_t* next = NULL;
        heap_alloc_t* prev = NULL;

        // calculate the allocation header
        const addr    start_memory = (addr)memory;
        const addr    start_alloc  = (start_memory != 0) ? (start_memory - _size_heap_allocation) : 0; 
        heap_alloc_t* alloc        = (heap_alloc_t*)start_alloc; 

        // make sure we can free the allocation
        bool can_free = true;
        can_free &= heap_alctr_validate(alctr);
        can_free &= can_free && (alloc->alctr == alctr);
        if (!can_free) return(can_free);

        // cache this for when we update the heap size used
        const u32 alloc_size = alloc->size;

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
    heap_alctr_free_rel(
        heap_alctr_t*     alctr,
        const alloc_hnd_t h_memory) {

        bool is_valid = true;
        is_valid &= heap_alctr_validate(alctr);
        is_valid &= (h_memory.val != 0);
        is_valid &= (is_valid && h_memory.val < alctr->size_used);

        if (is_valid) {
            const void* mem = (void*)(((addr)alctr) + h_memory.val);
            is_valid        = heap_alctr_free_abs(alctr, mem);
        }

        return(is_valid);
    }

    SLD_API void*
    heap_alctr_get_ptr(
        heap_alctr_t*     alctr,
        const alloc_hnd_t h_memory) {

        bool is_valid = true;
        is_valid &= heap_alctr_validate(alctr);
        is_valid &= (h_memory.val != 0);        
        is_valid &= (is_valid && h_memory.val < alctr->size_used);

        const addr    start = ((addr)alctr + h_memory.val); 
        heap_alloc_t* alloc = (heap_alloc_t*)(start - _size_heap_allocation);
        void*         mem   = (alloc->alctr == alctr) ? (void*)start : NULL;

        return(mem);
    }

    SLD_API const alloc_hnd_t
    heap_alctr_get_hnd(
        heap_alctr_t* alctr,
        const void*   memory) {

        // calculate the allocation header
        const addr    start_memory = (addr)memory;
        const addr    start_alloc  = (start_memory != 0) ? (start_memory - _size_heap_allocation) : 0; 
        const u64     diff         = start_memory - ((addr)alctr);
        heap_alloc_t* alloc        = (heap_alloc_t*)start_alloc; 

        // validate args
        bool is_valid = true;
        is_valid &= heap_alctr_validate(alctr);
        is_valid &= is_valid && (alloc->alctr == alctr);

        alloc_hnd_t hnd;
        hnd.val = is_valid ? *(u32*)&diff : 0;

        return(hnd);
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

            alctr->size_used       = alctr->size_total - free->size;  
            alctr->alloc_list.free = free;
            alctr->alloc_list.used = NULL;
        }
        return(can_reset);
    }
};
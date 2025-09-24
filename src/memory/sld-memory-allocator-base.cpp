#pragma once

#include "sld-memory.hpp"

namespace sld {

    SLD_API allocation_t*
    allocator_reuse_next_free_allocation(
        allocator_base_t* const allocator) {

        // get the next free allocation
        allocation_t* alloc = allocator->allocation_list.free;
        if (!alloc) return(alloc);

        // remove the allocation from the free list
        allocation_t*  next_free        = alloc->next;
        if (next_free) next_free->prev  = NULL;
        allocator->allocation_list.free = next_free;
        
        // add the allocation to the used list
        allocation_t* next_used = allocator->allocation_list.used;
        alloc->next = next_used;
        alloc->prev = nullptr;
        if (next_used) alloc->next = next_used;
        allocator->allocation_list.used = alloc;
        return(alloc);
    }

    SLD_API bool
    allocator_free_allocation(
        allocator_base_t* const allocator,
        allocation_t*           allocation, 
        const bool              consolidate) {

        bool can_free = true;
        can_free &= allocator_validate  (allocator);
        can_free &= allocation_validate (allocation);
        if (!can_free) return(can_free);

        // if the free list is empty,
        // just add it to the front of the free list
        if (allocator->allocation_list.free == NULL) {
            
            allocation->next                = NULL;
            allocation->prev                = NULL;
            allocator->allocation_list.free = allocation;
            return(true);            
        }

        // otherwis, find the next and previous
        // allocations to insert this one in between
        allocation_t* free_prev      = NULL;
        allocation_t* free_next      = NULL;
        const u32     max_iterations = allocator_max_possible_allocations(allocator);
        const addr    alloc_start    = (addr)allocation; 
        const u32     alloc_size     = allocation_size_total(allocation); 
        u32 index = 0;
        for (
            allocation_t* tmp = allocator->allocation_list.free;
            (tmp != NULL) && (index < max_iterations);
            tmp = tmp->next) {

            ++index;

            // calculate addresesses
            const u32  tmp_size   = allocation_size_total(tmp);
            const addr tmp_start  = (addr)tmp;
            const addr tmp_next   = (tmp_start + tmp_size); 
            const addr alloc_prev = alloc_start - tmp_size;

            // deterine if we found the next or previous
            const bool is_free_next_found = (free_next != NULL);
            const bool is_free_prev_found = (free_prev != NULL); 
            const bool is_tmp_free_next   = (!is_free_next_found  && (alloc_start == tmp_next));
            const bool is_tmp_free_prev   = (!is_free_prev_found  && (alloc_prev  == tmp));
            const bool is_fatal_error     = (is_tmp_free_next     && is_tmp_free_prev);
            const bool should_break       = (is_free_next_found   && is_free_prev_found); 

            // do assignments and final checking            
            if (is_tmp_free_next) free_next = tmp;
            if (is_tmp_free_prev) free_prev = tmp;
            if (should_break)     break;
            assert(!is_fatal_error);
        }

        // check scenarios that should never happen
        // if pointers are set, they should ALL be unique
        const bool is_alloc_next  = (allocation == free_next);
        const bool is_alloc_prev  = (allocation == free_prev);
        const bool are_both_null  = ((free_prev == NULL) && (free_next     == NULL));
        const bool is_next_prev   = (!are_both_null      && (is_alloc_next == is_alloc_prev)); 
        const bool is_fatal_error = (
            is_alloc_next ||
            is_alloc_prev ||
            is_next_prev            
        );
        assert(!is_fatal_error);

        // both are null, add to the front of the list
        if (are_both_null) {

            allocation_t* free_list         = allocator->allocation_list.free;
            allocation->next                = free_list;
            allocation->prev                = NULL;
            if (free_list) free_list->prev  = allocation;
            allocator->allocation_list.free = allocation;            
        }

        // determine how we're adding this back to the free list
        const bool is_free_next_and_do_not_consolidate = (free_next && !consolidate); 
        const bool is_free_prev_and_do_not_consolidate = (free_prev && !consolidate);
        const bool is_free_next_and_consolidate        = (free_next && consolidate);
        const bool is_free_prev_and_consolidate        = (free_prev && consolidate);

        if (is_free_next_and_do_not_consolidate) {

            ///////////////////////////////////////////////////////////
            //
            // ...|-->|========|-->|==========|-->|=========|-->|...
            // ...|   |tmp_prev|   |allocation|   |free_next|   |...
            // ...|<--|========|<--|=========-|<--|=========|<--|...
            //
            ///////////////////////////////////////////////////////////

            allocation_t* tmp_prev        = free_next->prev;             
            allocation->next              = free_next;
            allocation->prev              = tmp_prev;
            free_next->prev               = allocation;
            if (tmp_prev) tmp_prev->next  = allocation;

        }
        else if (is_free_next_and_consolidate) {

            ///////////////////////////////////////////////////////////
            //
            // ...|-->|========|-->|======================|-->|...
            // ...|   |tmp_prev|   |allocation + free_next|   |...
            // ...|<--|========|<--|======================|<--|...
            //
            ///////////////////////////////////////////////////////////

            
        }

        //TODO(SAM): for now, don't consolidate
        assert(false);
        return(can_free);
    }


};
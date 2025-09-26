#pragma once

#include "sld-memory.hpp"

namespace sld {
       
    SLD_API stack_allocator_t*
    stack_allocator_init(
        const void* memory,
        const u32   size,
        const u32   granularity) {

        // check args
        bool can_init = true;
        can_init &= (memory      != NULL);
        can_init &= (size        != 0);
        can_init &= (granularity != 0);
        can_init &= (granularity <  size);
        if (!can_init) return(NULL);

        // calculate sizes and addresses
        constexpr u32 size_allocator_struct = sizeof(stack_allocator_t);
        const u32     granularity_pow_2     = size_round_up_pow2(granularity); 
        const addr    start_memory          = (addr)memory;
        const addr    start_stack           = (size_allocator_struct + start_memory); 
        const u32     size_stack            = (size_allocator_struct - size);

        // initialize the allocator
        stack_allocator_t* allocator    = (stack_allocator_t*)memory;
        allocator->allocation_list.used = NULL;
        allocator->allocation_list.free = NULL;
        allocator->size                 = size_stack;
        allocator->granularity          = granularity_pow_2;
        allocator->stack.start          = start_stack;        
        allocator->stack.size           = size_stack;
        allocator->stack.position       = 0;
        allocator->stack.save           = 0;
        return(allocator);
    }

    SLD_API bool
    stack_allocator_validate(
        stack_allocator_t* const allocator) {

        bool is_valid = (allocator != NULL);
        if (is_valid) {

            is_valid &= (allocator->size        != 0);
            is_valid &= (allocator->granularity != 0);
            is_valid &= (allocator->granularity <  allocator->size);
            is_valid &= stack_validate(&allocator->stack);
        }
        return(is_valid);
    }

    SLD_API void*
    stack_allocator_alloc_abs(
        stack_allocator_t* const allocator,
        const u32                size) {

        // check args
        const bool is_valid = stack_allocator_validate(allocator) && (size != 0);
        if (!is_valid) return(NULL);

        // calculate sizes
        const u32 size_aligned = size_align_pow_2(size, allocator->granularity);
        const u32 size_alloc   = allocator_aligned_allocation_size(allocator, size);
        if (size_alloc == 0) return(NULL);

        // allocate memory
        allocation_t* used       = allocator->head_used; 
        allocation_t* allocation = (allocation_t*)stack_push(&allocator->stack, size_alloc);
        if (!allocation) return(NULL);

        // initialize the allocation
        allocation->alctr  = allocator;
        allocation->next   = NULL;
        allocation->prev   = allocator->head_used; 
        allocation->size   = size_aligned;
        allocation->offset = (((addr)allocation) - ((addr)allocator));

        // update the allocator
        if (!allocator->allocation_list.used) {
            allocator->allocation_list.used = allocation;
        }
        else {
            allocation_t* head_used = allocator->head_used;
            assert(head_used);
            head_used->next = allocation;
        }
        allocator->head_used = allocation;

        // return the memory
        void* memory = allocation_get_memory(allocation);
        return(memory);
    }

    SLD_API u32
    stack_allocator_alloc_rel(
        stack_allocator_t* const allocator,
        const u32                size) {

        // check args
        const bool is_valid = stack_allocator_validate(allocator) && (size != 0);
        if (!is_valid) return(NULL);

        // calculate sizes
        const u32 size_aligned = size_align_pow_2(allocator->granularity, size);
        const u32 size_alloc   = allocator_aligned_allocation_size(allocator, size);
        if (size_alloc == 0) return(NULL);

        // allocate memory
        allocation_t* used       = allocator->head_used; 
        allocation_t* allocation = (allocation_t*)stack_push(&allocator->stack, size_alloc);
        if (!allocation) return(NULL);

        // initialize the allocation
        allocation->alctr  = allocator;
        allocation->next   = NULL;
        allocation->prev   = allocator->head_used; 
        allocation->size   = size_aligned;
        allocation->offset = (((addr)allocation) - ((addr)allocator));

        // update the allocator
        if (!allocator->allocation_list.used) {
            allocator->allocation_list.used = allocation;
        }
        else {
            allocation_t* head_used = allocator->head_used;
            assert(head_used);
            head_used->next = allocation;
        }
        allocator->head_used = allocation;

        // return the offset
        return(allocation->offset);
    }

    SLD_API bool
    stack_allocator_free_abs(
        stack_allocator_t* const allocator,
        void* const              memory) {

        // check args
        allocation_t* alloc    = allocation_from_memory(memory);
        bool          is_valid = stack_allocator_validate(allocator) && (alloc != NULL);
        if (!is_valid) return(false);

        // remove the allocation from the used list
        allocation_t* alloc_next         = alloc->next;        
        allocation_t* alloc_prev         = alloc->prev;
        if (alloc_next) alloc_next->prev = alloc_prev;
        if (alloc_prev) alloc_prev->next = alloc_next;        

        //TODO
        return(false);

    }

    SLD_API bool
    stack_allocator_free_rel(
        stack_allocator_t* const allocator,
        const u32                offset) {

        const bool is_valid = stack_allocator_validate(allocator);
        if (!is_valid) return(false);

        //TODO
        return(false);
    }
};
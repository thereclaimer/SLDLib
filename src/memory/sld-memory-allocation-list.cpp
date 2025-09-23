#pragma once

#include "sld-memory.hpp"

namespace sld {

    allocation_t*
    allocation_list_recycle_free_allocation(
        allocation_list_t* const list,
        const u32                size      = 0,
        const u32                alignment = SLD_MEMORY_DEFAULT_ALIGNMENT) {

        const u32 size_align_pow_2 = sld::size_is_pow_2(alignment)
            ? sld::size_align_pow_2 (size, alignment)
            : sld::size_align       (size, alignment);


        allocation_t* recycle = nullptr;
        for (
            allocation_t* a = list->free;
            a != nullptr;
            a = a->next) {

            if (a->size <= size_align_pow_2) {
                recycle = a;
                break;
            }
        }

    }
    
    void
    allocation_list_free_used_allocation(
        allocation_list_t* const list,
        allocation_t*            allocation) {

        

    void*
    allocator_base_t::get_memory_from_allocation(
        const allocation_t* allocation) {

        assert(allocation != nullptr);

        const addr allocation_start = (addr)allocation;
        const addr memory_start     = allocation_start + sizeof(allocation_t);
        void*      memory           = (void*)memory_start;  

        return(memory);
    }
    }
    
    allocation_t*
    allocation_list_get_allocation_from_memory(
        allocation_list_t* const list,
        const void*              memory) {

        const addr memory_start     = (addr)memory;
        const addr allocation_start = memory_start - sizeof(allocation_t);

        allocation_t* allocation = (memory != NULL)
            ? (allocation_t*)allocation_start
            : nullptr;

        assert(allocation && allocation->list == list);
        return(allocation);

    }

    void*
    allocation_list_get_memory_from_allocation(
        allocation_list_t* const list,
        const allocation_t*      allocation) {

    }

    u32
    allocation_list_get_count_free(
        allocation_list_t* const list) {

        assert(list != nullptr);

        u32 count = 0;
        for (
            allocation_t* alloc = list->free;
            alloc != NULL;
            alloc = alloc->next) {

            ++count;
        }

        return(count);
    }

    u32
    allocation_list_get_count_used(
        allocation_list_t* const list) {

        assert(list != nullptr);

        u32 count = 0;
        for (
            allocation_t* alloc = list->used;
            alloc != NULL;
            alloc = alloc->next) {

            ++count;
        }

        return(count);
    }

    u64
    allocation_list_get_size_free(
        allocation_list_t* const list) {

        allocator_base_t* alctr = list->alctr;

        const u32 size_alctr = alctr->size_total();
        const u32 size_used  = allocation_list_get_size_used(list);
        const u32 size_free  = (size_alctr - size_used);  
        return(size_free);
    }


    u64
    allocation_list_get_size_used(
        allocation_list_t* const list) {

        assert(list != nullptr);

        u64 size = 0;
        for (
            allocation_t* alloc = list->used;
            alloc != NULL;
            alloc = alloc->next) {

            size += alloc->size;
        }
        return(size);
    }
};
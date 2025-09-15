#pragma once

#include "sld-heap.hpp"

namespace sld {

    struct heap_t {
        u64          size_total;
        u64          size_free;
        u64          granularity;
        heap_node_t* left;
        heap_node_t* right;
    };

    struct heap_root_t {
        heap_node_t* left;
        heap_node_t* right;
        u64          size_total;
        u64          size_free;
    };

    struct heap_node_t {
        heap_node_t* parent;
        heap_node_t* left;
        heap_node_t* right;
        u64          size_total;
        u64          size_free;
    };

    constexpr u32 _struct_size_heap = sizeof(heap_t);
    constexpr u32 _struct_size_node = sizeof(heap_node_t);

    SLD_API const u64
    heap_memory_size(
        const u64 heap_size_min) {

        const u64 heap_size = 
            _struct_size_heap +
            size_round_up_pow2(heap_size_min);
        
        return(heap_size);
    }

    SLD_API bool
    heap_validate(
        const heap_t* heap) {

        bool is_valid = (heap != NULL);
        if (is_valid) {
            is_valid &= (heap->granularity != 0);
            is_valid &= size_is_pow_2(heap->granularity);
            is_valid &= (heap->size_free <= (heap->size_total - _struct_size_heap));
        }

        return(is_valid);
    }

    SLD_API heap_t*
    heap_init(
        const memory_t& memory,
        const u64       granularity) {

        const bool is_valid = (
            memory.start != NULL &&
            memory.size  != 0
        );
        if (!is_valid) return(NULL);

        heap_t* heap = (heap_t*)memory.start;

        heap->size_total  = memory.size; 
        heap->size_free   = memory.size - _struct_size_heap; 
        heap->granularity = size_round_up_pow2(granularity);
        heap->left        = NULL;
        heap->right       = NULL;

        return(heap);
    }

    SLD_API bool
    heap_reset(
        heap_t* heap) {

        const bool is_valid = heap_validate(heap); 
        if (is_valid) {
            heap->size_free = heap->size_total - _struct_size_heap;
            heap->left      = NULL;
            heap->right     = NULL;
        } 
        return(is_valid);
    }

    SLD_API heap_node_t*
    heap_insert(
        heap_t*   heap,
        const u64 size) {

        heap_node_t* node = NULL;

        bool is_valid =  heap_validate(heap);
        if (!is_valid) return(node);

        const u64 size_aligned = size_is_pow_2(size)
            ? size_align_pow_2 (size, heap->granularity)
            : size_align       (size, heap->granularity);

        is_valid &= (size_aligned <= heap->size_free);
        if (!is_valid) return(node);

        

        return(NULL);
    }

    SLD_API bool
    heap_remove(
        heap_t*     heap,
        const void* data) {

        return(false);
    }
};
#pragma once

#include "sld-heap.hpp"

namespace sld {

    heap_node_t* heap_first_node    (const heap_t&      heap);
    bool         heap_node_validate (const heap_node_t& heap_node, const heap_t& heap);
    void*        heap_node_data     (const heap_node_t& heap_node);

    bool
    heap_validate(
        const heap_t& heap) {

        const u32 min_size = (sizeof(heap_node_t) + heap.granularity);

        bool result = true;
        result &= (heap.start       != 0);
        result &= (heap.size        >= min_size);
        result &= (heap.granularity >= min_size);
        result &= (heap.granularity <= heap.size);
        result &= (sld_size_is_pow_2(heap.granularity));
        // one list or the other should always be present
        result &= (heap.nodes.free != NULL || heap.nodes.reserved != NULL);
        return(result);
    }

    bool
    heap_init(
        heap_t& heap) {

        // calculate the mininmum size and adjust the granularity
        const u32 min_size = (sizeof(heap_node_t) + heap.granularity);
        heap.granularity   = sld_size_align_pow_2(heap.granularity, 2); 

        // check args
        bool result = true;
        result &= (heap.start       != 0);
        result &= (heap.size        >= min_size);
        result &= (heap.granularity >= min_size);
        result &= (heap.granularity <= heap.size);
        if (result) {

            // initialize the nodes
            heap.nodes.free         = (heap_node_t*)(heap.start + sizeof(heap_t));
            heap.nodes.free->parent = NULL;
            heap.nodes.free->next   = NULL;
            heap.nodes.free->prev   = NULL;
            heap.nodes.free->size   = (heap.size - sizeof(heap_node_t));
            heap.nodes.reserved     = NULL;
            return(result);
        }
    }

    bool
    heap_reset(
        heap_t& heap) {

        bool result = heap_validate(heap);
        if (result) {

            heap.nodes.free         = (heap_node_t*)(heap.start + sizeof(heap_t));
            heap.nodes.free->parent = NULL;
            heap.nodes.free->next   = NULL;
            heap.nodes.free->prev   = NULL;
            heap.nodes.free->size   = (heap.size - sizeof(heap_node_t));
            heap.nodes.reserved     = NULL;
        }
        return(result);
    }

    void*
    heap_add(
        heap_t&   heap,
        const u32 size) {

        heap_node_t* new_node = NULL;

        // align the requested size
        // and determine the actual size to
        // put on the heap
        const u32 size_aligned  = sld_size_align_pow_2(size, heap.granularity);
        const u32 size_required = (sizeof(heap_node_t) + size_aligned); 

        // check args
        bool result = true;
        result &= heap_validate(heap);
        result &= (size_aligned < heap.size);
        if (!result) return(new_node);

        // find a free node that can fit the requested size
        heap_node_t* free_node       = NULL;
        u32          free_node_size  = 0;
        bool         free_node_split = false;
        for (
            heap_node_t* node = heap.nodes.free;
            node = node->next;
            node != NULL) {
            
            const u32 node_size = (sizeof(heap_node_t) + node->size);
            if (size_requred <= free_node_size) {
                free_node       = node;
                free_node_size  = node_size;
                free_node_split = size_required < free_node_size; 
                break;
            }
        }

        // if we didn't find one, we're done
        if (free_node == NULL || free_node_size < size_required) return(new_node);

        // if we need to split the node,
        // we initialize the new node
        // off the end of the free node
        if (free_node_split) {

            free_node->size -= size_aligned;

            // calcualte the start of the new node
            const u32  new_node_offset = free_node->size + sizeof(heap_node_t); 
            const addr free_node_start = (addr)free_node;
            const addr new_node_start  = free_node_start + new_node_offset;

            // cast the new node
            new_node         = (heap_node_t*)new_node_start;
            new_node->parent = free_node;

        }
        // otherwise, this free node becomes the new node
        // and we remove it from the free list
        else {

            new_node = free_node;

            heap_node_t* next_free = free_node->next;
            heap_node_t* prev_free = free_node->prev;

            if (next_free) next_free->prev = prev_free;
            if (prev_free) prev_free->next = next_free;
        }

        // at this point, we should have a new node
        if (new_node == NULL) return(new_node);

        // initialize the node and add it to the 
        // front of the reserved list
        new_node->size = size_aligned;
        new_node->prev = NULL;
    }

    bool
    heap_remove(
        heap_t&     heap,
        const void* node) {

    }

    internal heap_node_t*
    heap_first_node(
        const heap_t& heap) {

        heap_node_t* first_node = (heap_node_t*)(heap.start + sizeof(heap_t));
        return(first_node);
    }


    internal bool
    heap_node_validate(
        const heap_node_t* heap_node,
        const heap_t&      heap) {

        bool result = (heap_node != NULL);
        if (result) {

            const heap_node_t* first_node = heap_first_node(heap);
            
            result &= (heap_node->size >= heap.granularity);
            result &= (heap_node >= first_node);
            result &= ((addr)heap_node < heap.start + heap.size);
        }

        return(result);
    }

    internal void*
    heap_node_data(
        const heap_node_t* heap_node) {

        void* node_data = (void*)heap_node + heap_node->size;
        return(node_data);
    }



};
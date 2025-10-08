#pragma once

#include "sld-queue.hpp"
#include "sld-memory.hpp"

namespace sld {

    SLD_API queue_t*
    queue_init_from_arena(
        arena_t* const arena,
        const u32      size,
        const u32      stride) {

        const u32 size_struct  = sizeof(queue_t);
        const u32 stride_pow_2 = size_round_up_pow2 (stride);
        const u32 size_data    = size_align_pow_2   (size, stride_pow_2);
        const u32 size_total   = size_struct + size_data; 
        
        bool can_init = true;
        can_init &= (arena        != NULL);
        can_init &= (size_total   >= QUEUE_MIN_SIZE);
        can_init &= (stride_pow_2 != 0);        
        can_init &= (stride_pow_2 < size_total);
        assert(can_init);        

        void*       memory     = arena->push_bytes      (size_total, stride_pow_2);
        queue_t*    queue      = queue_init_from_memory (memory, size_total, stride_pow_2); 
        const bool  is_valid   = queue_validate         (queue);
        assert(is_valid);
        
        return(queue);
    }

    SLD_API queue_t*
    queue_init_from_memory(
        void* const memory,
        const u32   size,
        const u32   stride) {

        bool can_init = true;
        can_init &= (memory != NULL);
        can_init &= (size   >= QUEUE_MIN_SIZE);
        can_init &= (stride != 0); 
        can_init &= (stride <  size);
        assert(can_init);

        const u32 struct_size = sizeof(queue_t);

        queue_t* queue = (queue_t*)memory;
        queue->start   = ((addr)memory) + struct_size; 
        queue->head    = QUEUE_START_HEAD; 
        queue->tail    = QUEUE_START_TAIL; 
        queue->size    = size - struct_size; 
        queue->stride  = stride; 

        const bool is_valid = queue_validate(queue);
        assert(is_valid);

        return(queue);
    }

    SLD_API bool
    queue_validate(
        queue_t* const queue) {

        bool is_valid = (queue != NULL);
        if (is_valid) {
            
            is_valid &= (queue->start  != 0);
            is_valid &= (queue->size   != 0);
            is_valid &= (queue->stride != 0);
            is_valid &= (queue->stride <= queue->size);
            is_valid &= (queue->head   != queue->tail);
            is_valid &= (queue->tail   == 0) ? (queue->head == QUEUE_START_HEAD) : (queue->head < queue->tail); 

            // the head should be no greater than the size, or equal to the start
            is_valid &= ((queue->head <= queue->size) || queue->head == QUEUE_START_HEAD);

            // if the head is at the start, the tail should be as well
            is_valid &= (queue->head == QUEUE_START_HEAD) ? (queue->tail == QUEUE_START_TAIL) : ( queue->tail<= queue->size);
        }
        return(is_valid);
    }

    SLD_API void
    queue_reset(
        queue_t* const queue) {

        const bool is_valid = queue_validate(queue);
        assert(is_valid);

        queue->head = QUEUE_START_HEAD;
        queue->tail = QUEUE_START_TAIL;
    }

    SLD_API bool
    queue_is_empty(
        queue_t* const queue) {

        const bool is_valid = queue_validate(queue);
        assert(is_valid);

        const bool is_empty = (queue->head == QUEUE_START_HEAD);
        return(is_empty);
    }

    SLD_API bool
    queue_is_full(
        queue_t* const queue) {

        const bool is_valid = queue_validate(queue);
        assert(is_valid);

        const u32  next_tail = (queue->tail + queue->stride) % queue->size;
        const bool is_full   = (next_tail == queue->head);

        return(is_full);
    }

    SLD_API u32
    queue_space_remaining(
        queue_t* const queue) {

        const bool is_valid = queue_validate(queue);
        assert(is_valid);

        const u32 space_remaining = queue->size - queue->tail;
        return(space_remaining);
    }

    SLD_API u32
    queue_space_used(
        queue_t* const queue) {

        const bool is_valid = queue_validate(queue);
        assert(is_valid);

        return(queue->tail);        
    }

    SLD_API bool
    queue_push(
        queue_t* const queue,
        const void*    data) {

        bool is_valid = queue_validate(queue);
        assert(is_valid);

        bool can_push = true;        
        can_push &= !queue_is_full(queue); 
        can_push &= (data  != NULL);

        if (can_push) {

            // copy the data
            byte*       dst = (byte*)(queue->start + queue->tail);
            const byte* src = (byte*)data;
            memory_copy(dst, src, queue->stride);

            // update the head and tail
            queue->tail = (queue->tail + queue->stride) % queue->size;
            if (queue->head == QUEUE_START_HEAD) queue->head = 0;
            assert(queue->head != queue->tail);
        }
        return(can_push);
    }

    SLD_API void*
    queue_pop(
        queue_t* const queue) {

        bool is_valid = queue_validate(queue);
        assert(is_valid);

        const bool can_pop = !queue_is_empty(queue);
        if (!can_pop) return(NULL);
        
        void* data = (void*)(queue->start + queue->head);

        const u32 new_head = (queue->head == queue->tail)
            ? (queue->tail - queue->stride)
            : (queue->head + queue->stride) % queue->size;
        queue->head = new_head;

        if (queue->head == queue->tail) {
            queue->head = QUEUE_START_HEAD;
            queue->tail = QUEUE_START_TAIL;
        } 

        return(data);
    }

    SLD_API void*
    queue_peek(
        queue_t* const queue,
        const u32      count) {

        bool is_valid = queue_validate(queue);
        assert(is_valid);

        const u32 space_needed = (count * queue->stride);
        const u32 space_used   = queue_space_used(queue);

        bool can_peek = true;
        can_peek &= (space_needed > 0);
        can_peek &= (space_needed <= space_used);
        if (!can_peek) return(NULL);

        const u32 space_to_index = (space_used  - queue->stride);
        const u32 queue_offset   = (queue->head + space_to_index); 

        void* data = (void*)(queue->start + queue_offset);
        return(data);
    }

    SLD_API u32
    queue_count(
        queue_t* const queue) {

        const bool is_valid = queue_validate(queue);
        assert(is_valid);

        if (queue->head == QUEUE_START_HEAD) return(0);

        const u32 space_used = (queue->head < queue->tail)
            ? (queue->tail - queue->head)
            : (queue->head - queue->tail);

        const u32 count = (space_used / queue->stride);
        return(count);
    }

};
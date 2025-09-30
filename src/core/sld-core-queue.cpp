#pragma once

#include "sld-queue.hpp"
#include "sld-memory.hpp"

namespace sld {

    constexpr s32 QUEUE_START_HEAD = 0xFFFFFFFF;
    constexpr s32 QUEUE_START_TAIL = 0;

    SLD_API bool
    queue_validate(
        queue_t* const queue) {

        bool is_valid = (queue != NULL);
        if (is_valid) {
            
            is_valid &= (queue->start  != 0);
            is_valid &= (queue->size   != 0);
            is_valid &= (queue->stride != 0);
            is_valid &= (queue->stride <= queue->size);

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

        const u32 space_remaining = (queue->size - queue->tail); 

        bool is_full = false;
        is_full |= (queue->tail     == queue->size);
        is_full |= (space_remaining <  queue->stride);
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
        const void*    data,
        const u32      count) {

        bool is_valid = queue_validate(queue);
        assert(is_valid);

        const u32  space_remaining = queue_space_remaining(queue);
        const u32  space_needed    = (count * queue->stride); 

        bool can_push = true;        
        can_push &= (space_needed <= space_remaining);
        can_push &= (data  != NULL);
        can_push &= (count != 0);

        if (can_push) {

            // update the tail
            const u32 new_tail = (queue->tail + space_needed) % queue->size;
            queue->tail = new_tail;

            // copy the data
            byte*       dst = (byte*)(queue->start + queue->tail);
            const byte* src = (byte*)data;
            memory_copy(dst, src, space_needed);

        }
        return(can_push);
    }

    SLD_API void*
    queue_pop(
        queue_t* const queue,
        const u32      count) {

        bool is_valid = queue_validate(queue);
        assert(is_valid);

        const u32  space_used   = queue_space_used(queue);
        const u32  space_needed = (count * queue->stride); 

        bool can_pop = true;
        can_pop &= (count        != 0);
        can_pop &= (space_needed <= space_used);
        if (!can_pop) return(NULL);
        
        void* data = (void*)(queue->start + queue->head);

        const u32 new_head = (queue->head == queue->tail)
            ? (queue->tail - queue->stride)
            : (queue->head + queue->stride) % queue->size;
        queue->head = new_head;

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

};
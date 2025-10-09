#pragma once

#include "sld-queue.hpp"
#include "sld-memory.hpp"

namespace sld {

    SLD_API u32
    queue_space_used(
        queue_t* const queue) {

        const bool is_valid = queue_validate(queue);
        assert(is_valid);

        return(queue->tail);        
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
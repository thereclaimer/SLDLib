#ifndef SLD_QUEUE_HPP
#define SLD_QUEUE_HPP

#include "sld.hpp"

namespace sld {

    struct queue_t {
        addr start;
        u32  head;
        u32  tail;
        u32  size;
        u32  stride;
    };

    SLD_API bool  queue_validate        (queue_t* const queue);
    SLD_API void  queue_reset           (queue_t* const queue);
    SLD_API bool  queue_is_empty        (queue_t* const queue);
    SLD_API bool  queue_is_full         (queue_t* const queue);
    SLD_API u32   queue_space_remaining (queue_t* const queue);
    SLD_API u32   queue_space_used      (queue_t* const queue);
    SLD_API u32   queue_count           (queue_t* const queue);
    SLD_API bool  queue_push            (queue_t* const queue, const void* data, const u32 count = 1);
    SLD_API void* queue_pop             (queue_t* const queue, const u32   count = 1);
    SLD_API void* queue_peek            (queue_t* const queue, const u32   count = 1);
};

#endif //SLD_QUEUE_HPP
#ifndef SLD_QUEUE_HPP
#define SLD_QUEUE_HPP

#include "sld.hpp"
#include "sld-memory.hpp"

namespace sld {

    struct queue_t {
        addr start;
        u32  head;
        u32  tail;
        u32  size;
        u32  stride;
    };

    constexpr u32 QUEUE_DEFAULT_STRIDE = 4; 
    constexpr u32 QUEUE_MIN_SIZE       = sizeof(queue_t) + QUEUE_DEFAULT_STRIDE;  
    constexpr u32 QUEUE_START_HEAD     = 0xFFFFFFFF;
    constexpr u32 QUEUE_START_TAIL     = 0;

    SLD_API queue_t* queue_init_from_arena  (arena_t* const arena,  const u32 size, const u32 stride = QUEUE_DEFAULT_STRIDE);
    SLD_API queue_t* queue_init_from_memory (void*    const memory, const u32 size, const u32 stride = QUEUE_DEFAULT_STRIDE);
    SLD_API bool     queue_validate         (queue_t* const queue);
    SLD_API void     queue_reset            (queue_t* const queue);
    SLD_API bool     queue_is_empty         (queue_t* const queue);
    SLD_API bool     queue_is_full          (queue_t* const queue);
    SLD_API u32      queue_space_remaining  (queue_t* const queue);
    SLD_API u32      queue_space_used       (queue_t* const queue);
    SLD_API u32      queue_count            (queue_t* const queue);
    SLD_API bool     queue_push             (queue_t* const queue, const void* data, const u32 count = 1);
    SLD_API void*    queue_pop              (queue_t* const queue, const u32   count = 1);
    SLD_API void*    queue_peek             (queue_t* const queue, const u32   count = 1);
};

#endif //SLD_QUEUE_HPP
#ifndef SLD_ARENA_ALLOCATOR_HPP
#define SLD_ARENA_ALLOCATOR_HPP

#include "sld-memory.hpp"

namespace sld {

    struct arena_allocator_t {
        memory_t memory;
        u32      arena_size;
        u32      arena_count;
    };

    SLD_API_INLINE void     arena_allocator_reserve_os_memory (arena_allocator_t* alctr, const u32 size, const u32 granularity);
    SLD_API_INLINE void     arena_allocator_release_os_memory (arena_allocator_t* alctr);
    SLD_API_INLINE bool     arena_allocator_is_valid          (const arena_allocator_t* alctr);
    SLD_API_INLINE bool     arena_allocator_is_arena_valid    (const arena_allocator_t* alctr, const arena_t* arena);
    SLD_API_INLINE void     arena_allocator_assert_valid      (const arena_allocator_t* alctr);
    SLD_API_INLINE arena_t* arena_allocator_commit            (const arena_allocator_t* alctr);
    SLD_API_INLINE void     arena_allocator_decommit          (const arena_allocator_t* alctr, arena_t* arena);
    SLD_API_INLINE bool     arena_allocator_is_valid          (const arena_allocator_t* alctr);
    SLD_API_INLINE u32      arena_allocator_get_size_total    (const arena_allocator_t* alctr);
    SLD_API_INLINE u32      arena_allocator_get_size_free     (const arena_allocator_t* alctr);
    SLD_API_INLINE u32      arena_allocator_get_size_used     (const arena_allocator_t* alctr);
    SLD_API_INLINE u32      arena_allocator_get_arenas_free   (const arena_allocator_t* alctr);
    SLD_API_INLINE u32      arena_allocator_get_arenas_used   (const arena_allocator_t* alctr);
};


#endif //SLD_ARENA_ALLOCATOR_HPP
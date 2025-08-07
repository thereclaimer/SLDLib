#ifndef SLD_MEMORY_HPP
#define SLD_MEMORY_HPP

#include "sld.hpp"

namespace sld {

    struct memory_t;
    struct memory_reservation_t;
    struct memory_arena_t;

    struct memory_t {
        addr start;
        u64  size;
    };

    bool memory_validate (const memory_t& memory);
    bool memory_zero     (memory_t&       memory);
    u32  memory_copy     (memory_t&       memory, const addr src_addr, const u32 src_size);
    addr memory_advance  (const memory_t& memory, const u32  stride,   u32&      offset);

    const memory_reservation_t* memory_reservation_acquire        (const u64 reservation_size, const u64 arena_size);
                           bool memory_reservation_validate       (const memory_reservation_t* reservation);
                           bool memory_reservation_release        (const memory_reservation_t* reservation);
          const memory_arena_t* memory_reservation_commit_arena   (const memory_reservation_t* reservation);
                           bool memory_reservation_decommit_arena (const memory_reservation_t* reservation, const memory_arena_t* arena);

    bool      memory_arena_save            (memory_arena_t* arena);
    bool      memory_arena_reset           (memory_arena_t* arena);
    bool      memory_arena_reset_to_save   (memory_arena_t* arena);
    const u64 memory_arena_space_remaining (memory_arena_t* arena);
    byte*     memory_arena_push_bytes      (memory_arena_t* arena, const u64 size);
    bool      memory_arena_pull_bytes      (memory_arena_t* arena, const u64 size);
    bool      memory_arena_can_push_bytes  (memory_arena_t* arena, const u64 size);
    bool      memory_arena_can_pull_bytes  (memory_arena_t* arena, const u64 size);
};

#define sld_memory_arena_push_type(arena, type)             (type*)sld::memory_arena_push_bytes     (arena, sizeof(type))
#define sld_memory_arena_pull_type(arena, type)             (type*)sld::memory_arena_pull_bytes     (arena, sizeof(type))
#define sld_memory_arena_push_array(arena, count, type)     (type*)sld::memory_arena_push_bytes     (arena, sizeof(type) * count)
#define sld_memory_arena_pull_array(arena, count, type)     (type*)sld::memory_arena_pull_bytes     (arena, sizeof(type) * count)
#define sld_memory_arena_can_push_type(arena, type)                sld::memory_arena_can_push_bytes (arena, sizeof(type))
#define sld_memory_arena_can_pull_type(arena, type)                sld::memory_arena_can_pull_bytes (arena, sizeof(type))
#define sld_memory_arena_can_push_array(arena, count, type)        sld::memory_arena_can_push_bytes (arena, sizeof(type) * count)
#define sld_memory_arena_can_pull_array(arena, count, type)        sld::memory_arena_can_pull_bytes (arena, sizeof(type) * count)

#endif //SLD_MEMORY_HPP
#ifndef SLD_MEMORY_HPP
#define SLD_MEMORY_HPP

#include "sld.hpp"

#define SLD_MEMORY_INTERNAL_STACK_SIZE_KB  64
#define SLD_MEMORY_INVALID_ID             -1


namespace sld {

    typedef s32 memory_reservation_id_t;
    typedef s32 memory_arena_id_t;
    typedef s32 memory_error_t;

    struct memory_t;
    struct memory_reservation_t;
    struct memory_arena_t;

    struct memory_t {
        addr start;
        u64  size;
    };

    bool                          memory_validate                   (const memory_t& memory);
    bool                          memory_zero                       (memory_t&       memory);
    u32                           memory_copy                       (memory_t&       memory, const addr src_addr, const u32 src_size);
    addr                          memory_advance                    (const memory_t& memory, const u32  stride,   u32&      offset);

    const memory_reservation_id_t memory_reservation_acquire        (const u64 reservation_size, const u64 arena_size);
    const memory_arena_id_t       memory_reservation_commit_arena   (const memory_reservation_id_t reservation_id);
    bool                          memory_reservation_validate       (const memory_reservation_id_t reservation_id);
    bool                          memory_reservation_release        (const memory_reservation_id_t reservation_id);
    bool                          memory_reservation_decommit_arena (const memory_reservation_id_t reservation_id, const memory_arena_id_t arena_id);

    bool                          memory_arena_save                 (const memory_arena_id_t arena_id);
    bool                          memory_arena_reset                (const memory_arena_id_t arena_id);
    bool                          memory_arena_reset_to_save        (const memory_arena_id_t arena_id);
    const u64                     memory_arena_space_remaining      (const memory_arena_id_t arena_id);
    byte*                         memory_arena_push_bytes           (const memory_arena_id_t arena_id, const u64 size);
    bool                          memory_arena_pull_bytes           (const memory_arena_id_t arena_id, const u64 size);
    bool                          memory_arena_can_push_bytes       (const memory_arena_id_t arena_id, const u64 size);
    bool                          memory_arena_can_pull_bytes       (const memory_arena_id_t arena_id, const u64 size);

    enum memory_error_e {
        memory_error_e_success                         =  0,
        memory_error_e_stack_out_of_memory             = -1,
        memory_error_e_invalid_id                      = -2,
        memory_error_e_invalid_reservation             = -3,
        memory_error_e_invalid_arena                   = -4,
        memory_error_e_reservation_out_of_memory       = -5,
        memory_error_e_arena_out_of_memory             = -6,
        memory_error_e_os_failed_to_reserve_page_table = -7,
        memory_error_e_os_failed_to_commit_pages       = -8,
        memory_error_e_os_failed_to_dcommit_pages      = -9,
        memory_error_e_os_failed_to_release_page_table = -10

    };
};

#define sld_memory_arena_push_type(arena_id, type)             (type*)sld::memory_arena_push_bytes     (arena_id, sizeof(type))
#define sld_memory_arena_pull_type(arena_id, type)             (type*)sld::memory_arena_pull_bytes     (arena_id, sizeof(type))
#define sld_memory_arena_push_array(arena_id, count, type)     (type*)sld::memory_arena_push_bytes     (arena_id, sizeof(type) * count)
#define sld_memory_arena_pull_array(arena_id, count, type)     (type*)sld::memory_arena_pull_bytes     (arena_id, sizeof(type) * count)
#define sld_memory_arena_can_push_type(arena_id, type)                sld::memory_arena_can_push_bytes (arena_id, sizeof(type))
#define sld_memory_arena_can_pull_type(arena_id, type)                sld::memory_arena_can_pull_bytes (arena_id, sizeof(type))
#define sld_memory_arena_can_push_array(arena_id, count, type)        sld::memory_arena_can_push_bytes (arena_id, sizeof(type) * count)
#define sld_memory_arena_can_pull_array(arena_id, count, type)        sld::memory_arena_can_pull_bytes (arena_id, sizeof(type) * count)

#endif //SLD_MEMORY_HPP
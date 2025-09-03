#ifndef SLD_MEMORY_HPP
#define SLD_MEMORY_HPP

#include "sld.hpp"
#include "sld-stack.hpp"

#ifndef    SLD_MEMORY_INTERNAL_STACK_SIZE_KB
#   define SLD_MEMORY_INTERNAL_STACK_SIZE_KB 128
#endif 

namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    struct memory_error_t;
    struct reservation_t;
    struct arena_t;

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API void           memory_zero                   (addr       start,     const u64 size);
    SLD_API addr           memory_advance                (const addr start,     const u64 size,      const u64 stride, u64& offset);
    SLD_API void           memory_copy                   (addr       start_dst, addr      start_src, const u64 size);

    SLD_API byte*          global_stack_push_bytes       (const u64 size, const u64 alignment = 0);
    SLD_API reservation_t* global_stack_push_reservation (void);
    SLD_API arena_t*       global_stack_push_arena       (void);
    SLD_API memory_error_t global_stack_last_error       (void);

    SLD_API bool           reservation_validate          (reservation_t* reservation);
    SLD_API reservation_t* reservation_acquire           (const u64 size_min_reservation = 0, const u64 size_min_arena = 0);
    SLD_API bool           reservation_release           (reservation_t* reservation);
    SLD_API bool           reservation_reset             (reservation_t* reservation);
    SLD_API u64            reservation_size_committed    (reservation_t* reservation);
    SLD_API u64            reservation_size_decommitted  (reservation_t* reservation);

    SLD_API bool           arena_validate                (arena_t*       arena);
    SLD_API arena_t*       arena_commit                  (reservation_t* reservation);
    SLD_API bool           arena_decommit                (arena_t*       arena);
    SLD_API byte*          arena_push_bytes              (arena_t*       arena, const u64 size, const u64 alignment = 0);
    SLD_API bool           arena_pull_bytes              (arena_t*       arena, const u64 size, const u64 alignment = 0);
    SLD_API bool           arena_can_push_bytes          (arena_t*       arena, const u64 size, const u64 alignment = 0);
    SLD_API bool           arena_can_pull_bytes          (arena_t*       arena, const u64 size, const u64 alignment = 0);
    SLD_API u64            arena_space_remaining         (arena_t*       arena);

    //-------------------------------------------------------------------
    // ENUMS
    //-------------------------------------------------------------------

    enum memory_error_e {
        memory_error_e_success                   =  1,
        memory_error_e_unknown                   = -1,
        memory_error_e_invalid_args              = -2,
        memory_error_e_invalid_reservation       = -4,
        memory_error_e_invalid_arena             = -5,
        memory_error_e_stack_not_enough_memory   = -7,
        memory_error_e_reservation_out_of_memory = -8,
        memory_error_e_arena_not_enough_memory   = -9,
        memory_error_e_os_failed_to_reserve      = -10,
        memory_error_e_os_failed_to_release      = -11,
        memory_error_e_os_failed_to_commit       = -12,
        memory_error_e_os_failed_to_decommit     = -13,
        memory_error_e_critical                  = -14
    };

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct memory_error_t : s32_t { };

    struct memory_reservation_t {
        addr start;
        struct {
            u64 reserved;
            u64 arena;
        } size;
        struct {
            arena_base_t* committed;
            arena_base_t* decommitted;
        } arena_list;
        memory_error_t last_error;
    }; 

    struct arena_t {
        stack_t               stack;
        memory_arena_t*       next;
        memory_arena_t*       prev;
        memory_reservation_t* reservation;
        memory_error_t        last_error;
    };
};

#endif //SLD_MEMORY_HPP
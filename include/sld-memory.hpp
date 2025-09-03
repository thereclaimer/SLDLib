#ifndef SLD_MEMORY_HPP
#define SLD_MEMORY_HPP

#include "sld.hpp"
#include "sld-stack.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    typedef u64     memory_size_t;
    typedef addr    memory_start_t;
    typedef stack_t memory_stack_t;

    struct memory_block_t;
    struct memory_error_t;
    struct memory_reservation_t;
    struct memory_arena_t;

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API void           memory_zero                         (memory_start_t       start,     const memory_size_t size);
    SLD_API memory_start_t memory_advance                      (const memory_start_t start,     const memory_size_t size,      const memory_size_t stride, memory_size_t& offset);
    SLD_API void           memory_copy                         (memory_start_t       start_dst, memory_start_t      start_src, const memory_size_t size);

    SLD_API bool           memory_reservation_acquire          (memory_reservation_t* reservation, const memory_size_t reservation_size_min, const memory_size_t arena_size_min);
    SLD_API bool           memory_reservation_release          (memory_reservation_t* reservation);
    SLD_API bool           memory_reservation_size_committed   (memory_reservation_t* reservation);
    SLD_API bool           memory_reservation_size_decommitted (memory_reservation_t* reservation);

    SLD_API bool           memory_arena_commit                 (memory_arena_t*       arena);
    SLD_API bool           memory_arena_decommit               (memory_arena_t*       arena);
    SLD_API byte*          memory_arena_push_bytes             (memory_arena_t*       arena, const memory_size_t size, const memory_size_t alignment = 0);
    SLD_API byte*          memory_arena_pull_bytes             (memory_arena_t*       arena, const memory_size_t size, const memory_size_t alignment = 0);
    SLD_API bool           memory_arena_can_push_bytes         (memory_arena_t*       arena, const memory_size_t size, const memory_size_t alignment = 0);
    SLD_API bool           memory_arena_can_pull_bytes         (memory_arena_t*       arena, const memory_size_t size, const memory_size_t alignment = 0);
    SLD_API memory_size_t  memory_arena_space_remaining        (const memory_arena_t* arena);

    //-------------------------------------------------------------------
    // ENUMS
    //-------------------------------------------------------------------

    enum memory_error_e {
        memory_error_e_success                   =  1,
        memory_error_e_unknown                   = -1,
        memory_error_e_invalid_args              = -2,
        memory_error_e_invalid_id                = -3,
        memory_error_e_invalid_reservation       = -4,
        memory_error_e_invalid_arena             = -5,
        memory_error_e_invalid_address           = -6,
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

    struct memory_block_t {
        memory_start_t start;
        memory_size_t  size;
    };

    struct memory_reservation_t {
        memory_start_t start;
        struct {
            memory_size_t reserved;
            memory_size_t arena;
        } size;
        struct {
            memory_arena_t* committed;
            memory_arena_t* decommitted;
        } arena_list;
        memory_error_t last_error;
    }; 

    struct memory_arena_t {
        memory_stack_t        stack;
        memory_arena_t*       next;
        memory_arena_t*       prev;
        memory_reservation_t* reservation;
        memory_error_t        last_error;
    };
};

#endif //SLD_MEMORY_HPP
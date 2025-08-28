#ifndef SLD_MEMORY_HPP
#define SLD_MEMORY_HPP

#include "sld.hpp"

#ifndef    SLD_MEMORY_STACK_SIZE_KB
#   define SLD_MEMORY_STACK_SIZE_KB 256
#endif

#define SLD_MEMORY_INVALID_HANDLE -1

namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    struct memory_reservation_handle_t;
    struct memory_arena_handle_t;
    struct memory_error_t;
    struct memory_t;
    struct memory_reservation_info_t;
    struct memory_arena_info_t;

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    // memory
    SLD_API bool                           memory_validate              (const memory_t& memory);
    SLD_API bool                           memory_zero                  (memory_t&       memory);
    SLD_API u64                            memory_copy                  (const memory_t& memory_src, memory_t& memory_dst);
    SLD_API addr                           memory_advance               (const memory_t& memory, const u32 stride, u32& offset);
    SLD_API const memory_error_t       memory_get_last_error        (void); 

    // reservation
    SLD_API const memory_reservation_handle_t memory_reservation_acquire   (const u64 reservation_size, const u64 arena_size);
    SLD_API bool                           memory_reservation_validate  (const memory_reservation_handle_t reservation_handle);
    SLD_API bool                           memory_reservation_release   (const memory_reservation_handle_t reservation_handle);
    SLD_API bool                           memory_reservation_info      (const memory_reservation_handle_t reservation_handle, memory_reservation_info_t& info);
    
    // arena
    SLD_API const memory_arena_handle_t       memory_arena_commit          (const memory_reservation_handle_t reservation_handle);
    SLD_API bool                           memory_arena_decommit        (const memory_arena_handle_t       arena_handle);
    SLD_API bool                           memory_arena_validate        (const memory_arena_handle_t       arena_handle);
    SLD_API bool                           memory_arena_reset           (const memory_arena_handle_t       arena_handle);
    SLD_API const u64                      memory_arena_space_remaining (const memory_arena_handle_t       arena_handle);
    SLD_API byte*                          memory_arena_push_bytes      (const memory_arena_handle_t       arena_handle, const u64 size);
    SLD_API bool                           memory_arena_pull_bytes      (const memory_arena_handle_t       arena_handle, const u64 size);
    SLD_API bool                           memory_arena_can_push_bytes  (const memory_arena_handle_t       arena_handle, const u64 size);
    SLD_API bool                           memory_arena_can_pull_bytes  (const memory_arena_handle_t       arena_handle, const u64 size);
    SLD_API bool                           memory_arena_info            (const memory_arena_handle_t       arena_handle, memory_arena_info_t& info);

    //-------------------------------------------------------------------
    // ENUMS
    //-------------------------------------------------------------------

    enum memory_error_e : s32 {
        memory_error_e_success                   =  0,
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
        memory_error_e_critical                  = -14,
    };

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct memory_reservation_handle_t : s32_t { };
    struct memory_arena_handle_t       : s32_t { };
    struct memory_error_t              : s32_t { };

    struct memory_t {
        addr start;
        u64  size;
    };

    struct memory_reservation_info_t {
        memory_reservation_handle_t handle;
        struct {
            u64 total;
            u64 arena;
            u64 committed;
        } size;
    };
    
    struct memory_arena_info_t {
        struct {
            memory_reservation_handle_t reservation;
            memory_arena_handle_t       arena;
        } handle;
        struct {
            u64 total;
            u64 used;
        } size;
    };


};

#define sld_memory_arena_push_type(arena_h, type)             (type*)sld::memory_arena_push_bytes     (arena_h, sizeof(type))
#define sld_memory_arena_pull_type(arena_h, type)             (type*)sld::memory_arena_pull_bytes     (arena_h, sizeof(type))
#define sld_memory_arena_push_array(arena_h, count, type)     (type*)sld::memory_arena_push_bytes     (arena_h, sizeof(type) * count)
#define sld_memory_arena_pull_array(arena_h, count, type)     (type*)sld::memory_arena_pull_bytes     (arena_h, sizeof(type) * count)
#define sld_memory_arena_can_push_type(arena_h, type)                sld::memory_arena_can_push_bytes (arena_h, sizeof(type))
#define sld_memory_arena_can_pull_type(arena_h, type)                sld::memory_arena_can_pull_bytes (arena_h, sizeof(type))
#define sld_memory_arena_can_push_array(arena_h, count, type)        sld::memory_arena_can_push_bytes (arena_h, sizeof(type) * count)
#define sld_memory_arena_can_pull_array(arena_h, count, type)        sld::memory_arena_can_pull_bytes (arena_h, sizeof(type) * count)

#endif //SLD_MEMORY_HPP
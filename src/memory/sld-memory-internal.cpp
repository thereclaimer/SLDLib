#pragma once

#include "sld-memory.hpp"
#include "sld-stack.hpp"
#include "sld-os.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    struct memory_reservation_t;
    struct memory_reservation_list_t;
    struct memory_arena_t;
    struct memory_arena_list_t;

    memory_error_t&            memory_last_error_instance();

    stack_t&                       memory_stack_instance               (void);    
    memory_reservation_t*          memory_stack_push_reservation       (void);    
    memory_arena_t*                memory_stack_push_arena             (void);    
    const memory_reservation_handle_t memory_stack_get_reservation_handle (const memory_reservation_t*  reservation);
    const memory_arena_handle_t       memory_stack_get_arena_handle       (const memory_arena_t*        arena);
    memory_reservation_t*          memory_stack_get_reservation        (const memory_reservation_handle_t reservation_handle);
    memory_arena_t*                memory_stack_get_arena              (const memory_arena_handle_t       arena_handle);

    memory_reservation_list_t&     memory_reservation_list_instance     (void);
    memory_reservation_t*          memory_reservation_list_recycle      (memory_reservation_list_t& reservation_list);
    void                           memory_reservation_list_add_reserved (memory_reservation_list_t& reservation_list, memory_reservation_t* reservation);
    void                           memory_reservation_list_add_released (memory_reservation_list_t& reservation_list, memory_reservation_t* reservation);
    bool                           memory_reservation_validate_internal (const memory_reservation_t* reservation);
    memory_reservation_t*          memory_reservation_get_new           (void);    
    bool                           memory_reservation_init              (memory_reservation_t* reservation, const u64 reservation_size, const u64 arena_size, memory_error_t& last_error);
    bool                           memory_reservation_deinit            (memory_reservation_t* reservation, memory_error_t& last_error);

    bool                           memory_arena_validate_internal       (const memory_arena_t* arena);
    memory_arena_list_t&           memory_arena_list_instance           (void);
    memory_arena_t*                memory_arena_list_recycle            (memory_arena_list_t& arena_list, const memory_reservation_t* reservation);
    void                           memory_arena_list_add_committed      (memory_arena_list_t& arena_list, memory_arena_t*             arena);
    void                           memory_arena_list_add_decommitted    (memory_arena_list_t& arena_list, memory_arena_t*             arena, const bool keep_reservation);
    bool                           memory_arena_init                    (memory_arena_t* arena, memory_reservation_t* reservation, memory_error_t& last_error);
    bool                           memory_arena_deinit                  (memory_arena_t* arena, memory_error_t& last_error);

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct memory_reservation_t {
        addr                  start;
        memory_reservation_t* next;
        memory_reservation_t* prev;
        struct {
            u64 total;
            u64 arena;
            u64 committed;
        } size;
    };

    struct memory_reservation_list_t {
        memory_reservation_t* reserved;
        memory_reservation_t* released;
    };

    struct memory_arena_t {
        memory_reservation_t* reservation;
        memory_arena_t*       next;
        memory_arena_t*       prev;
        addr                  start;
        u64                   position;
        u64                   save;
    };

    struct memory_arena_list_t {
        memory_arena_t* committed;        
        memory_arena_t* decommitted;      
    };

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_INLINE memory_error_t&
    memory_last_error_instance(
        void) {

        static memory_error_t error = { memory_error_e_success };
        return(error);
    }
};
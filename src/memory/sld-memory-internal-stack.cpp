#pragma once

#include "sld-memory.hpp"
#include "sld-memory-internal.cpp"
#include "sld-stack.hpp"
#include "sld-os.hpp"

namespace sld {

    struct memory_reservation_t;
    struct memory_arena_t;

    stack_t&                     memory_internal_stack_instance               (void);    
    memory_reservation_t*        memory_internal_stack_push_reservation       (void);    
    memory_arena_t*              memory_internal_stack_push_arena             (void);    
    const memory_reservation_h32 memory_internal_stack_get_reservation_handle (const memory_reservation_t*  reservation);
    const memory_arena_h32       memory_internal_stack_get_arena_handle       (const memory_arena_t*        arena);
    memory_reservation_t*        memory_internal_stack_get_reservation        (const memory_reservation_h32 reservation_handle);
    memory_arena_t*              memory_internal_stack_get_arena              (const memory_arena_h32       arena_handle);

    static inline stack_t&
    memory_internal_stack_instance(
        void) {
        
        static const stack_size  = size_kilobytes(SLD_MEMORY_INTERNAL_STACK_SIZE_KB); 
        static byte  stack_memory[stack_size];
        
        static stack_t internal_stack = {
            (addr)stack_memory, // start
            stack_size,         // size
            0,                  // position
            0                   // save            
        };

        return(internal_stack);
    }

    static inline memory_reservation_t*
    memory_internal_stack_push_reservation(
        void) {
        
        static stack_t& stack = memory_internal_stack_instance();
        static const    size  = sizeof(memory_reservation_t);

        auto reservation = (memory_reservation_t*)stack_push(stack, size);
        return(reservation);
    }

    static inline memory_arena_t*
    memory_internal_stack_push_arena(
        void) {

        static stack_t& stack = memory_internal_stack_instance();
        static const    size  = sizeof(memory_arena_t);

        auto reservation = (memory_arena_t*)stack_push(stack, size);
        return(reservation);
    }

    static inline const memory_reservation_h32 
    memory_internal_stack_get_reservation_handle(
        const memory_reservation_t* reservation) {

        static stack_t& stack = memory_internal_stack_instance();

        memory_reservation_h32 handle;
        handle.val = (s32)(((addr)reservation) - stack.start);

        return(handle);
    }
    
    static inline const memory_arena_h32       
    memory_internal_stack_get_arena_handle(
        const memory_arena_t* arena) {

        static stack_t& stack = memory_internal_stack_instance();

        memory_arena_h32 handle;
        handle.val = (s32)(((addr)arena) - stack.start);

        return(handle);
    }

    static inline memory_reservation_t*
    memory_internal_stack_get_reservation(
        const memory_reservation_h32 reservation_handle) {

        static stack_t& stack = memory_internal_stack_instance();

        auto reservation = (memory_reservation_t*)(stack.start + reservation_handle.val);
        return(reservation);
    }

    static inline memory_arena_t*
    memory_internal_stack_get_arena(
        const memory_arena_h32 arena_handle) {

        static stack_t& stack = memory_internal_stack_instance();

        auto arena = (memory_arena_t*)(stack.start + arena_handle.val);
        return(arena);
    }

};
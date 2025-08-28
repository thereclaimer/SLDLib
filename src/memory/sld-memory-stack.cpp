#pragma once

#include "sld-memory.hpp"
#include "sld-memory-internal.cpp"

namespace sld {

    SLD_INLINE stack_t&
    memory_stack_instance(
        void) {
        
        static const u32 stack_size  = size_kilobytes(SLD_MEMORY_STACK_SIZE_KB); 
        static byte  stack_memory[stack_size];
        
        static stack_t internal_stack = {
            (addr)stack_memory, // start
            stack_size,         // size
            0,                  // position
            0                   // save            
        };

        return(internal_stack);
    }

    SLD_INLINE memory_reservation_t*
    memory_stack_push_reservation(
        void) {
        
        static stack_t&  stack = memory_stack_instance();
        static const u32 size  = sizeof(memory_reservation_t);

        auto reservation = (memory_reservation_t*)stack_push(stack, size);
        return(reservation);
    }

    SLD_INLINE memory_arena_t*
    memory_stack_push_arena(
        void) {

        static stack_t&  stack = memory_stack_instance();
        static const u32 size  = sizeof(memory_arena_t);

        auto reservation = (memory_arena_t*)stack_push(stack, size);
        return(reservation);
    }

    SLD_INLINE const memory_reservation_handle_t 
    memory_stack_get_reservation_handle(
        const memory_reservation_t* reservation) {

        static stack_t& stack = memory_stack_instance();

        memory_reservation_handle_t handle;
        handle.val = (s32)(((addr)reservation) - stack.start);

        return(handle);
    }
    
    SLD_INLINE const memory_arena_handle_t       
    memory_stack_get_arena_handle(
        const memory_arena_t* arena) {

        static stack_t& stack = memory_stack_instance();

        memory_arena_handle_t handle;
        handle.val = (s32)(((addr)arena) - stack.start);

        return(handle);
    }

    SLD_INLINE memory_reservation_t*
    memory_stack_get_reservation(
        const memory_reservation_handle_t reservation_handle) {

        static stack_t& stack = memory_stack_instance();

        auto reservation = (memory_reservation_t*)(stack.start + reservation_handle.val);
        return(reservation);
    }

    SLD_INLINE memory_arena_t*
    memory_stack_get_arena(
        const memory_arena_handle_t arena_handle) {

        static stack_t& stack = memory_stack_instance();

        auto arena = (memory_arena_t*)(stack.start + arena_handle.val);
        return(arena);
    }

};
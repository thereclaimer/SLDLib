#pragma once

#include "sld-memory.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    struct global_stack_t : stack_t {
        memory_error_t last_error;
    };

    global_stack_t& global_stack_instance (void);

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_FUNC byte*
    global_stack_alloc_bytes(
        const u64 size,
        const u64 alignment = 0) {

        static global_stack_internal_t& stack        = global_stack_instance();
        const  u64                      size_aligned = size_align(size, alignment);

        byte* bytes = (byte*)stack_push(stack, size_aligned);        

        stack.last_error = (bytes != NULL)
            ? memory_error_e_success
            : memory_error_e_stack_not_enough_memory;
        
        return(reservation);
    }
    
    SLD_FUNC memory_reservation_t*
    global_stack_alloc_reservation(
        void) {

        static global_stack_t& stack = global_stack_instance();
        static const u64       size  = sizeof(memory_reservation_t);
    
        memory_reservation_t* reservation = (memory_reservation_t*)stack_push(stack, size);
        
        stack.last_error = (reservation != NULL)
            ? memory_error_e_success
            : memory_error_e_stack_not_enough_memory;
        
        return(reservation);
    }

    SLD_FUNC memory_arena_t*
    global_stack_alloc_arena(
        void) {
        
        static global_stack_t& stack = global_stack_instance();
        static const u64       size  = sizeof(memory_arena_t);
    
        memory_arena_t* arena = (memory_arena_t*)stack_push(stack, size);

        stack.last_error = (arena != NULL)
            ? memory_error_e_success
            : memory_error_e_stack_not_enough_memory;

        return(arena);
    }

    SLD_FUNC memory_error_t
    global_stack_last_error(
        void) {
        
        static global_stack_t& stack = global_stack_instance();
        return(stack.last_error);
    }

    //-------------------------------------------------------------------
    // INLINE
    //-------------------------------------------------------------------

    SLD_INLINE global_stack_t&
    global_stack_instance(
        void) {

        static const u64      stack_size = size_kilobytes(SLD_MEMORY_INTERNAL_STACK_SIZE_KB);
        static byte           stack_mem[stack_size];
        static global_stack_t stack = {
            (addr)stack_mem,       // start
            stack_size,            // size
            0,                     // position
            0                      // save
            memory_error_e_success // last_error
        };
        return(stack);
    }
};
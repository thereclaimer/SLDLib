#pragma once

#include "sld-memory-internal.cpp"

namespace sld {

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

    SLD_FUNC memory_error_t
    global_stack_last_error(
        void) {
        
        static global_stack_t& stack = global_stack_instance();
        return(stack.last_error);
    }

    SLD_FUNC u64 
    global_stack_size_total(
        void) {

        static global_stack_t& stack = global_stack_instance();
        return(stack.size);
    }

    SLD_FUNC u64 
    global_stack_size_used(
        void) {

        static global_stack_t& stack = global_stack_instance();
        return(stack.position);
    }

    SLD_FUNC u64 
    global_stack_size_free(
        void) {
        
        static global_stack_t& stack = global_stack_instance();
        return(stack.size - stack.position);
    }

    //-------------------------------------------------------------------
    // INLINE
    //-------------------------------------------------------------------

    SLD_INLINE global_stack_t&
    global_stack_instance(
        void) {
        
        static bool           initialized = false;
        static const u64      stack_size = size_kilobytes(SLD_MEMORY_INTERNAL_STACK_SIZE_KB);
        static byte           stack_mem[stack_size];
        static global_stack_t stack;

        if (!initialized) {
            stack.start                     = (addr)stack_mem;
            stack.size                      = stack_size;
            stack.position                  = 0;
            stack.save                      = 0;
            stack.reservation_list.released = NULL;
            stack.reservation_list.reserved = NULL;
            stack.arena_list.released       = NULL;
            stack.last_error                = memory_error_e_success;
        }

        return(stack);
    }

    SLD_INLINE reservation_list_t&
    global_stack_get_reservation_list(
        void) {

        static global_stack_t& stack = global_stack_instance();
        return(stack.reservation_list);
    }

    SLD_INLINE arena_list_t&      
    global_stack_get_arena_list(
        void) {

        static global_stack_t& stack = global_stack_instance();
        return(stack.arena_list);
    }
};
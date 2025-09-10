#pragma once

#include "sld-memory-internal.cpp"

namespace sld {

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API byte*
    global_stack_push_bytes(
        const u64 size,
        const u64 alignment) {

        static global_stack_t& stack        = global_stack_instance();
        const  u64                      size_aligned = size_align(size, alignment);

        byte* bytes = (byte*)stack_push(stack, size_aligned);        

        stack.last_error.val = (bytes != NULL)
            ? memory_error_e_success
            : memory_error_e_stack_not_enough_memory;
        
        return(bytes);
    }

    SLD_API memory_error_t
    global_stack_last_error(
        void) {
        
        static global_stack_t& stack = global_stack_instance();
        return(stack.last_error);
    }

    SLD_API u64 
    global_stack_size_total(
        void) {

        static global_stack_t& stack = global_stack_instance();
        return(stack.size);
    }

    SLD_API u64 
    global_stack_size_used(
        void) {

        static global_stack_t& stack = global_stack_instance();
        return(stack.position);
    }

    SLD_API u64 
    global_stack_size_free(
        void) {
        
        static global_stack_t& stack = global_stack_instance();
        return(stack.size - stack.position);
    }

    SLD_API addr
    global_stack_start(
        void) {

        static global_stack_t& stack = global_stack_instance();
        return(stack.start);
    }


    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_INTERNAL global_stack_t&
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
            stack.last_error.val            = memory_error_e_success;
        }

        return(stack);
    }

    SLD_INTERNAL reservation_list_t& global_stack_get_reservation_list(void) {

        static global_stack_t& stack = global_stack_instance();
        return(stack.reservation_list);
    }

    SLD_INTERNAL arena_list_t&      
    global_stack_get_arena_list(
        void) {

        static global_stack_t& stack = global_stack_instance();
        return(stack.arena_list);
    }
};
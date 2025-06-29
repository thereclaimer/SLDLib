#pragma once

#include "sld-ds-stack.hpp"

namespace sld {

    bool
    stack_init(
        stack_t&        stack,
        const memory_t& memory) {

        const bool result = (memory.size != 0 && memory.start != 0); 
        if (result) {
            stack.start    = memory.start;
            stack.size     = memory.size;
            stack.position = 0;
            stack.save     = 0;
        }
        return(result);
    }

    ptr
    stack_push(
        stack_t&  stack,
        const u32 size) {

        const u32  new_position = stack.position + size;
        const bool valid        = (
            stack_validate(stack) &&
            new_position < stack.size
        ); 

        ptr result = NULL;
        if (valid) {
            result         = (ptr)(stack.start + stack.position);
            stack.position = new_position;
        }
        return(result);
    }

    ptr
    stack_push_and_save(
        stack_t&  stack,
        const u32 size) {

        const ptr result = stack_push(stack,size);
        if (result) {
            stack_save(stack);
        }
        return(result);
    }

    bool
    stack_pull(
        stack_t&  stack,
        const u32 size) {

        bool result = true;
        result &= stack_validate(stack);
        result &= (size <= stack.position);         
        if (result) {

            stack.position -= size;
            if (stack.size > stack.position) {
                stack.save = 0;
            }
        }
        return(result);
    }

    void
    stack_reset(
        stack_t& stack) {

        if (stack_validate(stack)) {
            stack.position = 0;
            stack.save     = 0;
        }        
    }

    void
    stack_reset_to_save(
        stack_t& stack) {

        if (stack_validate(stack)) {
            stack.position = stack.save;
        }
    }

    void
    stack_save(
        stack_t& stack) {

        if (stack_validate(stack)) {
            stack.save = stack.position;
        }
    }

    bool
    stack_validate(
        const stack_t& stack) {

        bool is_valid = false;
        is_valid &= (stack.start    != 0);
        is_valid &= (stack.size     != 0);
        is_valid &= (stack.position < stack.size);
        is_valid &= (stack.save     <= stack.position);
        return(is_valid);
    }

    void
    stack_assert(
        const stack_t& stack) {

        const bool result = stack_validate(stack);
        sld_m_assert(result);
    }

    ptr
    stack_get_pointer_position(
        const stack_t& stack) {

        const bool is_valid = stack_validate(stack);
        ptr result          = NULL;
        if (is_valid) {

            const addr address = stack.start + stack.position;
            result             = (ptr)address;
        }
        return(result);
    }

    ptr
    stack_get_pointer_save(
        const stack_t& stack) {

        const bool is_valid = stack_validate(stack);
        ptr result          = NULL;
        if (is_valid) {

            const addr address = stack.start + stack.save;
            result             = (ptr)address;
        }
        return(result);
    }

    ptr
    stack_get_pointer_start(
        const stack_t& stack) {

        ptr result = stack_validate(stack)
            ? (ptr)stack.start
            : NULL; 
        return(result);
    }

    u32
    stack_get_space_remaining(
        const stack_t& stack) {

        const u32 result = stack_validate(stack)
            ? stack.size - stack.position
            : 0;
        
        return(result);
    }
};
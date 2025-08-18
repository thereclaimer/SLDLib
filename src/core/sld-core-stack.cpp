#pragma once

#include "sld-stack.hpp"

namespace sld {

    bool
    stack_validate(
        const stack_t& stack) {

        bool is_valid = true;
        is_valid &= (stack.start    != 0);
        is_valid &= (stack.size     != 0);
        is_valid &= (stack.position <  stack.size);
        is_valid &= (stack.save     <= stack.position);
        return(is_valid);
    }

    bool
    stack_reset(
        stack_t& stack) {

        const bool result = (
            stack.start != 0 &&
            stack.size  != 0
        );

        stack.position = 0;
        stack.save     = 0;

        return(result);
    }

    bool
    stack_reset_to_save(
        stack_t& stack) {

        const bool result = stack_validate(stack);

        if (result) {
            stack.position = stack.save;
        }

        return(result);
    }

    bool
    stack_save(
        stack_t& stack) {

        const bool result = stack_validate(stack);

        if (result) {
            stack.save = stack.position;
        }

        return(result);
    }

    void*
    stack_push(
        stack_t&  stack,
        const u32 size) {

        const u32 new_position = stack.position + size;
        
        bool result = true;
        result &= stack_validate(stack);
        result &= (new_position < stack.size); 
        if (!result) return(NULL);

        void* ptr = (void*)(stack.start + stack.position);
        stack.position = new_position;

        return(ptr);
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
            if (stack.save > stack.position) {
                stack.save = 0;
            }
        }
        
        return(result);
    }
};
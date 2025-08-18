#pragma once

#include "sld-stack.hpp"

namespace sld {

    bool
    stack_list_validate(
        const stack_list_t& stack_list) {

        bool result = true;
        result &= (stack_list.start    != 0);
        result &= (stack_list.capacity != 0);
        result &= (stack_list.count    <=  stack_list.capacity);
        result &= (stack_list.stride   != 0);
        result &= (stack_list.save     <= stack_list.capacity);
        return(result);
    }

    bool
    stack_list_reset(
        stack_list_t& stack_list) {

        bool result = true;
        result &= (stack_list.start    != 0);
        result &= (stack_list.capacity != 0);
        result &= (stack_list.stride   != 0);

        stack_list.count = 0;
        stack_list.save  = 0;

        return(result);
    }

    bool
    stack_list_reset_to_save(
        stack_list_t& stack_list) {

        const bool result = stack_list_validate(stack_list);
        
        if (result) {
            stack_list.count = 0;
            stack_list.save  = 0;
        }

        return(result);
    }

    bool
    stack_list_save(
        stack_list_t& stack_list) {

        const bool result = stack_list_validate(stack_list);
        
        if (result) {
            stack_list.save = stack_list.count;
        }

        return(result);
    }

    void*
    stack_list_push(
        stack_list_t& stack_list,
        const u32     count) {

        const u32 new_count = stack_list.count + count;
        
        bool result = true; 
        result &= stack_list_validate(stack_list);
        result &= (new_count <= stack_list.count); 
        if (!result) return(NULL);

        const u32  element_offset = (stack_list.stride * stack_list.count);
        const addr element_start  = (stack_list.start  + element_offset);  
        void*      element_ptr    = (void*)(element_start);                

        stack_list.count = new_count;

        return(element_ptr);
    }

    bool
    stack_list_pull(
        stack_list_t& stack_list,
        const u32     count) {

        bool result = true;
        result &= stack_list_validate(stack_list);
        result &= (count <= stack_list.count);
        
        if (result) {
            
            stack_list.count -= count;
        }

        return(result);
    }

    void*
    stack_list_index(
        stack_list_t& stack_list,
        const u32     index) {

        bool result = true;
        result &= stack_list_validate(stack_list);
        result &= (index < stack_list.count);
        if (!result) return(NULL);

        const u32  element_offset = (stack_list.stride * index);
        const addr element_start  = (stack_list.start  + element_offset);  
        void*      element_ptr    = (void*)(element_start);

        return(element_ptr);
    } 
};
#ifndef SLD_STACK_HPP
#define SLD_STACK_HPP

#include "sld.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // STACK
    //-------------------------------------------------------------------

    struct stack_t {
        addr start;
        u32  size;
        u32  position;
        u32  save;
    };

    bool  stack_validate        (const stack_t& stack);
    bool  stack_reset           (stack_t& stack);
    bool  stack_reset_to_save   (stack_t& stack);
    bool  stack_save            (stack_t& stack);
    void* stack_push            (stack_t& stack, const u32 size);
    bool  stack_pull            (stack_t& stack, const u32 size);

    //-------------------------------------------------------------------
    // STACK LIST
    //-------------------------------------------------------------------

    struct stack_list_t {
        addr start;
        u32  capacity;
        u32  count;
        u32  stride;        
        u32  save;
    };

    bool  stack_list_validate        (const stack_list_t& stack_list);
    bool  stack_list_reset           (stack_list_t& stack_list);
    bool  stack_list_reset_to_save   (stack_list_t& stack_list);
    bool  stack_list_save            (stack_list_t& stack_list);
    void* stack_list_push            (stack_list_t& stack_list, const u32 count);
    bool  stack_list_pull            (stack_list_t& stack_list, const u32 count);
    void* stack_list_index           (stack_list_t& stack_list, const u32 index);    
};

#endif //SLD_STACK_HPP
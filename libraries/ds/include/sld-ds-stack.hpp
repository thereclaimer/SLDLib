#ifndef SLD_DS_STACK_HPP
#define SLD_DS_STACK_HPP

#include <sld.hpp>

namespace sld {

    struct stack_t {
        addr start;
        u64  size;
        u32  position;
        u32  save;
    };

    bool stack_init                 (stack_t& stack, const memory_t& memory);
    ptr  stack_push                 (stack_t& stack, const u32 size);
    ptr  stack_push_and_save        (stack_t& stack, const u32 size);
    bool stack_pull                 (stack_t& stack, const u32 size);
    void stack_reset                (stack_t& stack);
    void stack_reset_to_save        (stack_t& stack);
    void stack_save                 (stack_t& stack);
    bool stack_validate             (const stack_t& stack);
    void stack_assert               (const stack_t& stack);
    ptr  stack_get_pointer_position (const stack_t& stack);
    ptr  stack_get_pointer_save     (const stack_t& stack);
    ptr  stack_get_pointer_start    (const stack_t& stack);
    u32  stack_get_space_remaining  (const stack_t& stack);
};

#define sld_m_stack_push_struct(stack,type) (type*)sld::stack_push(stack,sizeof(type))
#define sld_m_stack_pull_struct(stack,type)        sld::stack_pull(stack,sizeof(type))

#endif //SLD_DS_STACK_HPP
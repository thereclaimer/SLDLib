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
    
    ///--------------------------------------------------------
    /// @brief Initializes a stack from a block of memory
    /// @param stack  - stack reference
    /// @param memory - block of memory
    /// @return (T/F) stack is successfully initialized
    ///--------------------------------------------------------
    
    bool
    stack_init(
        stack_t&        stack,
        const memory_t& memory
    );

    ///--------------------------------------------------------
    /// @brief Checks if the stack properties are valid
    /// @param stack stack reference
    /// @return (T/F) stack and all properties are valid
    ///--------------------------------------------------------

    bool
    stack_validate(
        const stack_t& stack
    );

    ///--------------------------------------------------------
    /// @brief Checks if the stack properties are valid, will
    ///        cause a runtime assertion if not
    /// @param stack stack reference
    ///--------------------------------------------------------

    void
    stack_assert(
        const stack_t& stack
    );

    ///--------------------------------------------------------
    /// @brief Initializes a stack from a fixed block of memory
    /// @param stack stack reference
    /// @param memory memory reference
    /// @return (T/F) successfully initialized the stack
    ///--------------------------------------------------------

    bool
    stack_init(
        stack_t&        stack,
        const memory_t& memory
    );

    ///--------------------------------------------------------
    /// @brief Pushes bytes onto the stack and returns 
    ///        a pointer to the position prior to the push
    /// @param stack - stack reference 
    /// @param size - the number of bytes to push
    /// @return pointer prior to push, NULL if push failed
    ///--------------------------------------------------------

    ptr
    stack_push(
        stack_t&  stack,
        const u32 size
    );

    ///--------------------------------------------------------
    /// @brief Pushes bytes onto the stack and returns 
    ///        a pointer to the position prior to the push,
    ///        saves the new position
    /// @param stack - stack reference 
    /// @param size - the number of bytes to push
    /// @return pointer prior to push, NULL if push failed
    ///--------------------------------------------------------

    ptr
    stack_push_and_save(
        stack_t&  stack,
        const u32 size
    );

    ///--------------------------------------------------------
    /// @brief Pulls bytes off the stack
    /// @param stack - stack reference
    /// @param size - the number of bytes to pull
    /// @return (T/F) the pull succeeded
    ///--------------------------------------------------------
    
    bool
    stack_pull(
        stack_t&  stack,
        const u32 size
    );

    ///--------------------------------------------------------
    /// @brief Resets the stack position and save to 0 
    /// @param stack - stack reference
    ///--------------------------------------------------------

    void
    stack_reset(
        stack_t& stack
    );

    ///--------------------------------------------------------
    /// @brief Sets the stack position to the save 
    /// @param stack - stack reference
    ///--------------------------------------------------------

    void
    stack_reset_to_save(
        stack_t& stack
    );

    ///--------------------------------------------------------
    /// @brief Saves the current position of the stack
    /// @param stack - the stack reference
    ///--------------------------------------------------------

    void
    stack_save(
        stack_t& stack
    );

    ///--------------------------------------------------------
    /// @brief Returns a pointer to the stack position 
    /// @param stack - stack reference
    /// @return pointer to stack position
    ///--------------------------------------------------------

    ptr
    stack_get_pointer_position(
        const stack_t& stack
    );

    ///--------------------------------------------------------
    /// @brief Returns a pointer to the stack save 
    /// @param stack - stack reference
    /// @return pointer to stack position
    ///--------------------------------------------------------

    ptr
    stack_get_pointer_save(
        const stack_t& stack
    );

    ///--------------------------------------------------------
    /// @brief Returns a pointer to the start of the stack
    /// @param stack - stack reference
    /// @return pointer to the start of the stack
    ///--------------------------------------------------------
    
    ptr
    stack_get_pointer_start(
        const stack_t& stack
    );

    ///--------------------------------------------------------
    /// @brief Returns the size of the available stack space
    /// @param stack - stack reference
    /// @return space remaining
    ///--------------------------------------------------------
    
    u32
    stack_get_space_remaining(
        const stack_t& stack
    );
};

#define sld_m_stack_push_struct(stack,type) (type*)sld::stack_push(stack,sizeof(type))
#define sld_m_stack_pull_struct(stack,type)        sld::stack_pull(stack,sizeof(type))

#endif //SLD_DS_STACK_HPP
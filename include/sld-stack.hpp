#ifndef SLD_STACK_HPP
#define SLD_STACK_HPP

#include "sld.hpp"

#define SLD_STACK_IMPL_INLINE    template<typename t> inline           auto stack_t<t>::
#define SLD_STACK_IMPL_STATIC    template<typename t> inline static    auto stack_t<t>::
#define SLD_STACK_IMPL_CONSTEXPR template<typename t> inline constexpr auto stack_t<t>::

namespace sld {

    //-------------------------------------------------------------------
    // STACK
    //-------------------------------------------------------------------

    template<typename t>
    struct stack_t {
        t*  start;
        u32 size;
        u32 position;
        u32 save;
    
        inline constexpr bool is_valid      (void);
        inline constexpr void assert_valid  (void);
        inline constexpr void reset         (void);
        inline constexpr void reset_to_save (void);
        inline constexpr void save_position (void);
        inline t*             push          (const u32 count);
        inline bool           pull          (const u32 count);
    };

    using data_stack_t = stack_t<byte>;

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    template<typename t>
    inline stack_t<t>*
    stack_init_from_memory(
        const void* memory,
        const u32  size) {

        assert(memory != NULL && size != 0);

        constexpr u32 struct_size = sizeof(stack_t<t>);

        stack_t<t>* stack = (stack_t<t>*)memory;
        stack->start    = (t*)((addr)memory + struct_size);
        stack->size     = (size - struct_size);
        stack->position = 0;
        stack->save     = 0;
        stack->assert_valid();

        return(stack); 
    }

    inline data_stack_t*
    data_stack_init_from_memory(
        const void* memory,
        const u32   size) {

        data_stack_t* stack = stack_init_from_memory<byte>(memory, size);
        return(stack);
    }

    SLD_STACK_IMPL_CONSTEXPR
    is_valid(
        void) -> bool {

        bool is_valid = true; 
        is_valid &= (start    != NULL);
        is_valid &= (size     != 0);
        is_valid &= (position <  size);
        is_valid &= (save     <= position);
        return(is_valid);
    }

    SLD_STACK_IMPL_CONSTEXPR
    assert_valid(
        void) -> void {

        assert(is_valid());
    }

    SLD_STACK_IMPL_CONSTEXPR    
    reset(
        void) -> void {

        assert_valid();

        position = 0;
        save     = 0;
    }

    SLD_STACK_IMPL_CONSTEXPR    
    reset_to_save(
        void) -> void {

        assert_valid();

        position = save;
    }

    SLD_STACK_IMPL_CONSTEXPR    
    save_position(
        void) -> void {

        assert_valid();

        save = position;
    }

    SLD_STACK_IMPL_INLINE    
    push(
        const u32 count) -> t* {

        assert_valid();

        const u32 new_position = position + count;
        
        bool can_push = (new_position <= size); 
        if (!can_push) return(NULL);

        t* ptr = &start[position];
        position = new_position;

        return(ptr);
    }

    SLD_STACK_IMPL_INLINE
    pull(
        const u32 count) -> bool {

        assert_valid();

        bool can_pull = (count <= position);
        if (can_pull) {

            position -= count;
            if (save > position) {
                save = 0;
            }
        }
        
        return(can_pull);
    }
};

#undef SLD_STACK_IMPL_INLINE
#undef SLD_STACK_IMPL_STATIC
#undef SLD_STACK_IMPL_CONSTEXPR

#endif //SLD_STACK_HPP
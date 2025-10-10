#ifndef SLD_STACK_LIST_HPP
#define SLD_STACK_LIST_HPP

#include "sld.hpp"

#define SLD_STACK_LIST_IMPL_STATIC    template<typename t> inline           auto
#define SLD_STACK_LIST_IMPL_INLINE    template<typename t> inline           auto stack_list_t<t>::
#define SLD_STACK_LIST_IMPL_CONSTEXPR template<typename t> inline constexpr auto stack_list_t<t>::

namespace sld {

    //-------------------------------------------------------------------
    // STACK LIST
    //-------------------------------------------------------------------

    template<typename t>
    struct stack_list_t {
        
        t*  array;
        u32 capacity;
        u32 position;
        u32 save;
    
        SLD_API inline           void init          (t* array, const u32 capacity);
        SLD_API inline constexpr bool is_valid      (void) const;
        SLD_API inline constexpr void assert_valid  (void) const;
        SLD_API inline constexpr void reset         (void);
        SLD_API inline constexpr void reset_to_save (void);
        SLD_API inline constexpr void save_position (void);
        SLD_API inline           t*   push_element  (const u32 count);
        SLD_API inline           bool pull_element  (const u32 count);
    };

    template<typename t> inline stack_list_t<t>* stack_list_init_from_memory(const void* memory, const u32 size);

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    SLD_STACK_LIST_IMPL_STATIC
    stack_list_init_from_memory(
        const void* memory,
        const u32   size) -> stack_list_t<t>* {

        assert(memory != NULL && size != 0);

        constexpr u32 size_struct  = sizeof(stack_list_t<t>);
        constexpr u32 size_element = sizeof(t);
        const     u32 size_array   = (size - size_struct); 

        stack_list_t<t>* stack = (stack_list_t<t>*)memory;

        stack->array    = (t*)((addr)memory + struct_size);
        stack->capacity = size_array / size_element;  
        stack->position = 0;
        stack->save     = 0;
        stack->assert_valid();

        return(stack); 
    }

    SLD_STACK_LIST_IMPL_INLINE
    init(
        t*        array,
        const u32 capacity) -> void {

        this->array    = array;
        this->capacity = capacity;
        this->position = 0;
        this->save     = 0;
        this->assert_valid();
    }


    SLD_STACK_LIST_IMPL_CONSTEXPR
    is_valid(void) const -> bool {

        bool is_valid = true; 
        is_valid &= (this->array    != NULL);
        is_valid &= (this->capacity != 0);
        is_valid &= (this->position <  this->capacity);
        is_valid &= (this->save     <= this->position);
        return(is_valid);
    }

    SLD_STACK_LIST_IMPL_CONSTEXPR
    assert_valid(void) const -> void {

        assert(is_valid());
    }

    SLD_STACK_LIST_IMPL_CONSTEXPR    
    reset(void) -> void {

        assert_valid();

        this->position = 0;
        this->save     = 0;
    }

    SLD_STACK_LIST_IMPL_CONSTEXPR    
    reset_to_save(void) -> void {

        assert_valid();
        this->position = this->save;
    }

    SLD_STACK_LIST_IMPL_CONSTEXPR    
    save_position(void) -> void {

        assert_valid();
        this->save = this->position;
    }

    SLD_STACK_LIST_IMPL_INLINE    
    push_element(const u32 count) -> t* {

        assert_valid();

        const u32 new_position = this->position + count;
        
        bool can_push = (new_position <= this->capacity); 
        if (!can_push) return(NULL);

        t* ptr = &this->array[this->position];
        this->position = new_position;

        return(ptr);
    }

    SLD_STACK_LIST_IMPL_INLINE
    pull_element(const u32 count) -> bool {

        assert_valid();

        bool can_pull = (count <= this->position);
        if (can_pull) {

            this->position -= count;
            if (this->save > this->position) {
                this->save = 0;
            }
        }
        
        return(can_pull);
    }
};

#undef SLD_STACK_LIST_IMPL_STATIC
#undef SLD_STACK_LIST_IMPL_INLINE
#undef SLD_STACK_LIST_IMPL_CONSTEXPR

#endif //SLD_STACK_LIST_HPP

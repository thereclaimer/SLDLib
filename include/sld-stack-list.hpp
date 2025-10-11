#ifndef SLD_STACK_LIST_HPP
#define SLD_STACK_LIST_HPP

#include "sld.hpp"

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
    };

    template<typename t> SLD_API_INLINE stack_list_t<t>* stack_list_init_from_memory (const void* memory, const u32 size);
    template<typename t> SLD_API_INLINE void             stack_list_init_from_aray   (stack_list_t<t>* stack_list, t* array, const u32 capacity);
    template<typename t> SLD_API_INLINE bool             stack_list_is_valid         (stack_list_t<t>* stack_list);
    template<typename t> SLD_API_INLINE void             stack_list_assert_valid     (stack_list_t<t>* stack_list);
    template<typename t> SLD_API_INLINE void             stack_list_reset            (stack_list_t<t>* stack_list);
    template<typename t> SLD_API_INLINE void             stack_list_reset_to_save    (stack_list_t<t>* stack_list);
    template<typename t> SLD_API_INLINE void             stack_list_save_position    (stack_list_t<t>* stack_list);
    template<typename t> SLD_API_INLINE t*               stack_list_push_element     (stack_list_t<t>* stack_list, const u32 count = 1);
    template<typename t> SLD_API_INLINE bool             stack_list_pull_element     (stack_list_t<t>* stack_list, const u32 count = 1);

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    template<typename t>
    SLD_API_INLINE stack_list_t<t>*
    stack_list_init_from_memory(
        const void* memory,
        const u32   size) ->  {

        assert(memory != NULL && size != 0);

        constexpr u32 size_struct  = sizeof(stack_list_t<t>);
        constexpr u32 size_element = sizeof(t);
        const     u32 size_array   = (size - size_struct); 

        stack_list_t<t>* stack_list = (stack_list_t<t>*)memory;

        stack_list->array    = (t*)((addr)memory + struct_size);
        stack_list->capacity = size_array / size_element;  
        stack_list->position = 0;
        stack_list->save     = 0;

        stack_list_assert_valid(stack_list);
        return(stack); 
    }

    template<typename t>
    SLD_API_INLINE void
    stack_list_init_from_array(
        stack_list_t<t>* stack_list,
        t*               array,
        const u32        capacity) {

        assert(array != NULL && capacity != 0);
        stack_list_assert_valid(stack_list);

        stack_list->array    = array;
        stack_list->capacity = capacity;
        stack_list->position = 0;
        stack_list->save     = 0;
    }

    template<typename t>
    SLD_API_INLINE bool
    stack_list_is_valid(
        stack_list_t<t>* stack_list) {

        bool is_valid = (stack_list != NULL); 
        if (is_valid) {
            is_valid &= (stack_list->array    != NULL);
            is_valid &= (stack_list->capacity != 0);
            is_valid &= (stack_list->position <  stack_list->capacity);
            is_valid &= (stack_list->save     <= stack_list->position);
        }
        return(is_valid);
    }

    template<typename t>
    SLD_API_INLINE void
    stack_list_assert_valid(
        stack_list_t<t>* stack_list) {
        assert(stack_list_is_valid(stack_list));
    }

    template<typename t>
    SLD_API_INLINE void    
    stack_list_reset(
        stack_list_t<t>* stack_list) {

        stack_list_assert_valid(stack_list);

        stack_list->position = 0;
        stack_list->save     = 0;
    }

    template<typename t>
    SLD_API_INLINE void
    stack_list_reset_to_save(
        stack_list_t<t>* stack_list) {

        stack_list_assert_valid(stack_list);
        stack_list->position = stack_list->save;
    }

    template<typename t>
    SLD_API_INLINE    
    stack_list_save_position(
        stack_list_t<t*> stack_list) -> void {

        stack_list_assert_valid(stack_list);
        stack_list->save = stack_list->position;
    }

    template<typename t>
    SLD_API_INLINE t*
    stack_list_push_element(
        stack_list_t<t*> stack_list,
        const u32        count) {

        stack_list_assert_valid(stack_list);

        const u32 new_position = stack_list->position + count;
        
        bool can_push = (new_position <= stack_list->capacity); 
        if (!can_push) return(NULL);

        t* ptr = &stack_list->array[stack_list->position];
        stack_list->position = new_position;

        return(ptr);
    }

    template<typename t>
    SLD_API_INLINE bool
    stack_list_pull_element(
        stack_list_t<t*> stack_list,
        const u32        count) {

        stack_list_assert_valid(stack_list);

        bool can_pull = (count <= stack_list->position);
        if (can_pull) {

            stack_list->position -= count;
            if (stack_list->save > stack_list->position) {
                stack_list->save = 0;
            } 
        }
        
        return(can_pull);
    }
};

#undef SLD_STACK_LIST_IMPL_STATIC
#undef SLD_STACK_LIST_IMPL_INLINE
#undef SLD_STACK_LIST_IMPL_CONSTEXPR

#endif //SLD_STACK_LIST_HPP

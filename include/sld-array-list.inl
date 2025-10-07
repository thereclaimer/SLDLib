#ifndef SLD_ARRAY_HPP
#define SLD_ARRAY_HPP

#include "sld.hpp"

#define SLD_ARRAY_LIST_IMPL_INLINE    SLD_COLLECTION_IMPL_INLINE    (array_list_t)
#define SLD_ARRAY_LIST_IMPL_STATIC    SLD_COLLECTION_IMPL_STATIC    (array_list_t)
#define SLD_ARRAY_LIST_IMPL_CONSTEXPR SLD_COLLECTION_IMPL_CONSTEXPR (array_list_t)

namespace sld {

    SLD_ARRAY_LIST_IMPL_STATIC 
    init_from_memory(
        const void* memory,
        const u32   size) -> array_list_t<t>* {

        array_list_t<t>* array_list = (array_list_t<t>*)memory;
        if (array_list) {

            constexpr u32 struct_size    = sizeof(array_list_t<t>);
            const u32     stride_aligned = sizeof(t); 
            const u32     data_size      = (size - struct_size); 

            array_list->start    = (addr)memory + struct_size; 
            array_list->capacity = data_size / stride_aligned;
            array_list->count    = 0;
        }
        array_list->assert_valid();
        return(array_list);
    }

    SLD_ARRAY_LIST_IMPL_INLINE
    init_from_array(
        t* const  array,
        const u32 count) -> bool {

        bool can_init = true;
        can_init &= (array_list != NULL);
        can_init &= (array      != NULL);
        can_init &= (count      != 0);

        array_list->start    = array;
        array_list->capacity = count;
        array_list->count    = 0;
        array_list_assert_valid(array_list);
    }

    SLD_ARRAY_LIST_IMPL_CONSTEXPR
    assert_valid(
        void) -> void {
        assert(is_valid(array_list));
    }

    SLD_ARRAY_LIST_IMPL_CONSTEXPR
    is_valid(
        void) -> bool {

        bool is_valid &=  (start    != nullptr);
        is_valid      &&= (capacity != 0);
        is_valid      &&= (count    <= capacity);
        return(is_valid)
    }

    SLD_ARRAY_LIST_IMPL_CONSTEXPR
    is_empty(
        void) -> bool {

        assert_valid();
        return(count == 0);
    }

    SLD_ARRAY_LIST_IMPL_CONSTEXPR
    is_full(
        void) -> bool {

        assert_valid();
        return(count >= capacity);
    }

    SLD_ARRAY_LIST_IMPL_CONSTEXPR
    is_not_full(
        void) -> bool {

        assert_valid();
        return(count < capacity);
    }

    template<typename t>
    SLD_INLINE u32
    array_list_get_index_of(
        const t* element) {

        assert_valid();

        const addr element_start = (addr)element;
        const u32  addr_diff     = (element_start - array_list->start); 
        const bool does_exist    = (addr_diff % array_list->stride) == 0;
        const u32  index         = (does_exist) 
            ? (addr_diff / array_list->stride)
            : ARRAY_LIST_INDEX_INVALID;

        return(does_exist ? index : ARRAY_LIST_INDEX_INVALID);
    }

    template<typename t>
    SLD_INLINE t*
    array_list_get_first_element(
        array_list_t<t>* const array_list) {

        array_list_assert_valid(array_list);

        return(array_list->start);
    }

    template<typename t>
    SLD_INLINE t*
    array_list_get_last_element(
        array_list_t<t>* const array_list) {

        array_list_assert_valid(array_list);

        if (array_list_is_empty(array_list)) {
            return(NULL);
        }

        const u32  index     = (array_list->count  - 1);
        const u32  offset    = (array_list->stride * index); 
        const addr last_addr = (array_list->start  + offset);
        void*      last_ptr  = (void*)last_addr; 

        return(last_ptr);
    }

    template<typename t>
    SLD_INLINE bool
    array_list_add(
        array_list_t<t>* const array_list,
        const void*         element) {

        array_list_assert_valid(array_list);
        
        bool can_add = true;
        can_add &= (element != NULL);
        can_add &= array_list_is_not_full(array_list);
        if(!can_add) return(false);




    }

    template<typename t>
    SLD_INLINE bool
    array_list_insert_at(
        array_list_t<t>* const array_list,
        const void*         element,
        const u32           index) {

        array_list_assert_valid(array_list);

        bool can_insert = true;
        can_insert &= (element != NULL);
        can_insert &= (index   <  array_list->count);
        can_insert &= (index   <  array_list->count);
        can_insert &= array_list_is_not_full(array_list);
        if (!can_insert) return(false);

        // there should always be an element here at this point to move
        void* existing_element = array_list_get_element(array_list, index);
        assert(existing_element);


        return(true);
   }

    template<typename t>    
    SLD_INLINE bool
    array_list_remove_at(
        array_list_t<t>* const array_list,
        const u32              index) {

    }

    SLD_INLINE void*
    array_list_get_element(
        const u32 index) {

        array_list_assert_valid(array_list);

        bool can_get = (index < array_list->count);
        if (!can_get) return(NULL);

        const u32  offset       = (array_list->stride * index);
        const addr element_addr = (array_list->start  + offset);
        void*      element_ptr  = (void*)element_addr;

        return(element_ptr); 
    }

    SLD_INLINE void*
    array_list_next(
        array_list_t<t>* const array_list,
        const void*         current) {

        array_list_assert_valid(array_list);
        
        const addr  current_addr  = (addr)current;
        const void* last_ptr      = array_list_last(array_list); 
        const addr  last_addr     = (addr)last_ptr;
        const u32   current_index = array_list_index_of(array_list, current); 
        const u32   next_index    = (current_index == ARRAY_LIST_INDEX_INVALID)
            ? ARRAY_LIST_INDEX_INVALID 
            : (current_index + 1); 

        bool can_get = true;
        can_get &= (current_addr  >= array_list->start);
        can_get &= (current_addr  <= last_addr);
        can_get &= (current_index <  array_list->count);
        can_get &= (next_index    <  array_list->count);
        if (!can_get) return(NULL);
        
        // we've done so much validation at this point,
        // this should never be null
        void* next_ptr = array_list_get_element(array_list, next_index);
        assert(next_ptr);
        return(next_ptr);
    }
};

#undef SLD_ARRAY_LIST_IMPL_INLINE
#undef SLD_ARRAY_LIST_IMPL_STATIC
#undef SLD_ARRAY_LIST_IMPL_CONSTEXPR
#endif //SLD_ARRAY_HPP
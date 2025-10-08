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

    SLD_ARRAY_LIST_IMPL_INLINE
    get_index_of_element(
        const t* element) -> u32 {

        assert_valid();

        const addr element_start = (addr)element;
        const u32  addr_diff     = (element_start - array_list->start); 
        const bool does_exist    = (addr_diff % array_list->stride) == 0;
        const u32  index         = (does_exist) 
            ? (addr_diff / array_list->stride)
            : ARRAY_LIST_INDEX_INVALID;

        return(does_exist ? index : ARRAY_LIST_INDEX_INVALID);
    }

    SLD_ARRAY_LIST_IMPL_CONSTEXPR
    get_first_element(
        void) -> t* {

        array_list_assert_valid(array_list);
        return(array_list->start);
    }

    SLD_ARRAY_LIST_IMPL_CONSTEXPR
    get_last_element(
        void) -> t* {

        assert_valid();
        return(&start[count - 1]);
    }

    SLD_ARRAY_LIST_IMPL_INLINE
    add_element(
        const t* element) -> bool {

        array_list_assert_valid(array_list);
        
        bool can_add = true;
        can_add &= (element != NULL);
        can_add &= array_list_is_not_full(array_list);
        if(!can_add) return(false);

        return(false);
    }

    SLD_ARRAY_LIST_IMPL_INLINE 
    insert_element_at(
        const t*  element,
        const u32 index) -> bool {

        assert_valid();

        bool can_insert = true;
        can_insert &= (element != NULL);
        can_insert &= (index   <  count);
        can_insert &= (index   <  count);
        can_insert &= is_not_full();
        if (!can_insert) return(false);


        return(true);
    }

    SLD_ARRAY_LIST_IMPL_INLINE 
    remove_element_at(
        const u32 index) -> bool {

        return(false);
    }

    SLD_ARRAY_LIST_IMPL_CONSTEXPR 
    get_element_at(
        const u32 index) -> t* {

        array_list_assert_valid(array_list);

        bool can_get = (index < count);
        if (!can_get) return(NULL);



        return(element_ptr); 
    }

    SLD_ARRAY_LIST_IMPL_CONSTEXPR 
    get_next_element(
        const t* current) -> t* {

        assert_valid();
        
    }

    SLD_ARRAY_LIST_IMPL_CONSTEXPR
    get_size_of_element(
        void) -> u32 {

        return(sizeof(t));
    }

    SLD_ARRAY_LIST_IMPL_INLINE
    operator[] (
        u32 index) -> t& {

        
    }
};

#endif //SLD_ARRAY_HPP
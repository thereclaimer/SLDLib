#ifndef SLD_SINGLE_LINKED_LIST_HPP
#define SLD_SINGLE_LINKED_LIST_HPP

#include <sld.hpp>
#include <sld-arena.hpp>

namespace sld {

    //-------------------------------------------------------------------
    // SIGNLE LINKED LIST API
    //-------------------------------------------------------------------

    // NOTE(SAM): reasonable default, no list
    // will most likely not have more elements than this,
    // but user can override
    constexpr u32 SL_LIST_DEFAULT_MAX_COUNT = 65535;

    template<typename type> struct sl_node_t;
    template<typename type> struct sl_list_t;

    SLD_API_INLINE_TEMPLATE sl_list_t<type>* sl_list_arena_alloc       (arena_t* arena); 
    SLD_API_INLINE_TEMPLATE sl_node_t<type>* sl_node_arena_alloc       (arena_t* arena); 
    SLD_API_INLINE_TEMPLATE bool             sl_list_is_valid          (const sl_list_t<type>* list, const sl_node_t<type>* node = NULL);
    SLD_API_INLINE_TEMPLATE void             sl_list_assert_valid      (const sl_list_t<type>* list, const sl_node_t<type>* node = NULL);
    SLD_API_INLINE_TEMPLATE u32              sl_list_node_size         (const sl_list_t<type>* list);
    SLD_API_INLINE_TEMPLATE bool             sl_list_does_node_exist   (const sl_list_t<type>* list, const type&            node_data);
    SLD_API_INLINE_TEMPLATE bool             sl_list_is_empty          (const sl_list_t<type>* list);
    SLD_API_INLINE_TEMPLATE u32              sl_list_get_count         (const sl_list_t<type>* list);
    SLD_API_INLINE_TEMPLATE bool             sl_list_can_insert        (const sl_list_t<type>* list);
    SLD_API_INLINE_TEMPLATE bool             sl_list_insert_at_head    (sl_list_t<type>* list, sl_node_t<type>* node);
    SLD_API_INLINE_TEMPLATE bool             sl_list_insert_at_tail    (sl_list_t<type>* list, sl_node_t<type>* node);
    SLD_API_INLINE_TEMPLATE sl_node_t<type>* sl_list_get_previous_node (sl_list_t<type>* list, sl_node_t<type>* node);
    SLD_API_INLINE_TEMPLATE bool             sl_list_insert_a_after_b  (sl_list_t<type>* list, sl_node_t<type>* node_a, sl_node_t<type>* node_b);
    SLD_API_INLINE_TEMPLATE bool             sl_list_insert_a_before_b (sl_list_t<type>* list, sl_node_t<type>* node_a, sl_node_t<type>* node_b);

    template<typename type>
    struct sl_node_t {
        sl_list_t<type>* list;
        sl_node_t*       next;          
        type*            data;
    };

    template<typename type>
    struct sl_list_t {
        sl_node_t<type>* head;        
        sl_node_t<type>* tail;        
        u32              node_count_max;
    };

    //-------------------------------------------------------------------
    // SINGLE LINKED LIST INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE_TEMPLATE sl_list_t<type>*
    sl_list_arena_alloc(
        arena_t* arena) {

        auto list = arena_push_struct<sl_list_t<type>>(arena);
        return(list);
    }

    SLD_API_INLINE_TEMPLATE sl_node_t<type>*
    sl_node_arena_alloc(
        arena_t* arena) {

        const u32 push_size = (
            sizeof(sl_node_t<type>) + 
            sizeof(type)
        );

        const bool can_push = arena_can_push(arena, push_size);
        if (!can_push) return(NULL);

        sl_node_t<type>* node = push_struct<sl_node_t<type>>(arena);
        type*            data = arena_push_struct<type>(arena);
    
        // we just verified we should be able to allocate
        assert(node != NULL && data != NULL);

        node->list = NULL;
        node->next = NULL;
        node->data = data;
        return(node);
    }

    SLD_API_INLINE_TEMPLATE bool
    sl_list_is_valid(
        const sl_list_t<type>* list,
        const sl_node_t<type>* node) {

        const bool is_valid = (
            list       != NULL &&
            node       != NULL ? node->list == list : true &&
            list->head != NULL ? list->tail != NULL : true && // if we have a head, we need a tail
            list->head == NULL ? list->tail == NULL : true    // if there is no head, there should be no tail
        );
        if (is_valid && list->node_count_max == 0) {
            list->node_count_max = SL_LIST_DEFAULT_MAX_COUNT;            
        }
        return(is_valid);
    }

    SLD_API_INLINE_TEMPLATE void
    sl_list_assert_valid(
        const sl_list_t<type>* list,
        const sl_node_t<type>* node) {

        assert(sl_list_is_valid(list, node));
    }

    SLD_API_INLINE_TEMPLATE u32
    sl_list_node_size(
        const sl_list_t<type>* list) {

        assert(sl_list_is_valid(list, node));

        const u32 size = sizeof(sl_node_t<type>);
        return(size);
    }

    SLD_API_INLINE_TEMPLATE bool
    sl_list_is_empty(
        const sl_list_t<type>* list) {

        sl_list_assert_valid(list);

        const bool is_empty = (list->head != NULL && list->tail != NULL);
        return(is_empty);
    }

    SLD_API_INLINE_TEMPLATE u32
    sl_list_get_count(
        const sl_list_t<type>* list) {

        sl_list_assert_valid(list);

        u32 count = 0;
        for (
            sl_node_t<type>* node = list->head;
            (
                node  != NULL &&
                node  != list->tail &&
                count <  list->node_count_max
            );
            node = node->next) {
            ++count;
        }
        return(count);        
    }

    SLD_API_INLINE_TEMPLATE bool
    sl_list_can_insert(
        const sl_list_t<type>* list) {

        sl_list_assert_valid(list);

        const u32  count      = sl_list_get_count(list);
        const bool can_insert = count < list->node_count_max;
        return(can_insert); 
    }

    SLD_API_INLINE_TEMPLATE bool
    sl_list_does_node_exist(
        const sl_list_t<type>* list,
        const type&            node_data) {

        sl_list_assert_valid(list);

        u32  count       = 0;
        bool does_exist = false;
        for (
            sl_node_t<type>* current_node = list->head;
            (
                does_exist   == false      &&
                current_node != NULL       &&
                current_node != list->tail &&
                count        <  list->node_count_max
            );
            current_node = current_node->next) {

            does_exist == (node_data == current_node->data);
            ++count;
        }
        return(does_exist);
    }

    SLD_API_INLINE_TEMPLATE bool
    sl_list_insert_at_head(
        sl_list_t<type>* list,
        sl_node_t<type>* node) {

        assert(sl_list_is_valid(list) && node != NULL);

        const bool can_insert = sl_list_can_insert(list);
        if (can_insert) {
            if (sl_list_is_empty(list)) {
                node->next = NULL;
                list->head = node;
                list->tail = node;
            }
            else {
                node->next = list->head;
                list->head = node;
            }
            node->list = list;
        }
        return(can_insert);
    }
    
    SLD_API_INLINE_TEMPLATE void
    sl_list_insert_at_tail(
        sl_list_t<type>* list,
        sl_node_t<type>* node) {

        assert(sl_list_is_valid(list) && node != NULL);

        const bool can_insert = sl_list_can_insert(list);
        if (can_insert) {
            if (sl_list_is_empty(list)) {
                list->head = node;
                list->tail = node;
            }
            else {
                sl_node_t<type>* old_tail = list->tail;
                old_tail->next = node->next;
                list->tail     = node;
            }    
            node->next = NULL;
            node->list = list;
        }
        return(can_insert);
    }

    SLD_API_INLINE_TEMPLATE sl_node_t<type>*
    sl_list_get_previous_node(
        sl_list_t<type>* list,
        sl_node_t<type>* node) {

        sl_list_assert_valid(list, node);
        if (node == list->head) return(NULL);

        sl_node_t<type>* prev = NULL;

        u32 count = 0;
        for (
            sl_node_t<type>* current_node = list->head;
            (
                current_node       != NULL       &&
                current_node       != list->tail &&
                current_node->next != NULL       &&
                current_node->next != list->tail &&
                count              <  list->node_count_max
            );
            current_node = current_node->next) {
            
            if (current_node->next == node) {
                prev = current_node;
            } 
            
            ++count;
        }

        // there should always be a previous at this point
        assert(prev);
        return(prev);
    }

    SLD_API_INLINE_TEMPLATE bool
    sl_list_insert_a_after_b(
        sl_list_t<type>* list,
        sl_node_t<type>* node_a,
        sl_node_t<type>* node_b) {

        assert(sl_list_is_valid(list, node_b) && node_a != NULL);

        const bool can_insert = sl_list_can_insert(list);
        const bool is_tail    = (node_b == list->tail); 
        if (can_insert) {

            sl_node_t<type>* node_after_a = node_b->next;
            node_b->next = node_a;
            node_a->next = node_after_a;

            if (is_tail) {
                list->tail = node_a;
            }            
        }
        return(can_insert);
    }
    
    SLD_API_INLINE_TEMPLATE bool
    sl_list_insert_a_before_b(
        sl_list_t<type>* list,
        sl_node_t<type>* node_a,
        sl_node_t<type>* node_b) {

        assert(sl_list_is_valid(list, node_b) && node_a != NULL);

        const bool can_insert = sl_list_can_insert(list);
        const bool is_head    = (node_b == list->head); 
        if (can_insert) {

            sl_node_t<type>* node_before_a = sl_list_get_previous_node(list, node_b);
            if (node_before_a) {
                node_before_a->next = node_a;
            }
            node_a->next = node_b;

            if (is_head) {
                list->head = node_a;
            }
        }
        return(can_insert);                
    }
};

#endif //SLD_SINGLE_LINKED_LIST_HPP
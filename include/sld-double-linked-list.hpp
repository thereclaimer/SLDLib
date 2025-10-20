#ifndef SLD_DOUBLE_LINKED_LIST_HPP
#define SLD_DOUBLE_LINKED_LIST_HPP

#include <sld.hpp>

namespace sld {

    //-------------------------------------------------------------------
    // SINLE LINKED LIST API
    //-------------------------------------------------------------------

    // NOTE(SAM): reasonable default, no list
    // will most likely not have more elements than this,
    // but user can override
    constexpr u32 DL_LIST_DEFAULT_MAX_COUNT = 65535;

    template<typename type> struct dl_node_t;
    template<typename type> struct dl_list_t;

    SLD_API_INLINE_TEMPLATE dl_list_t<type>* dl_list_arena_alloc       (arena_t* arena); 
    SLD_API_INLINE_TEMPLATE dl_node_t<type>* dl_node_arena_alloc       (arena_t* arena); 
    SLD_API_INLINE_TEMPLATE bool             dl_list_is_valid          (const dl_list_t<type>* list, const dl_node_t<type>* node = NULL);
    SLD_API_INLINE_TEMPLATE void             dl_list_assert_valid      (const dl_list_t<type>* list, const dl_node_t<type>* node = NULL);
    SLD_API_INLINE_TEMPLATE bool             dl_list_does_node_exist   (const dl_list_t<type>* list, const type&            node_data);
    SLD_API_INLINE_TEMPLATE bool             dl_list_is_empty          (const dl_list_t<type>* list);
    SLD_API_INLINE_TEMPLATE u32              dl_list_get_count         (const dl_list_t<type>* list);
    SLD_API_INLINE_TEMPLATE bool             dl_list_can_insert        (const dl_list_t<type>* list);
    SLD_API_INLINE_TEMPLATE bool             dl_list_insert_at_head    (dl_list_t<type>* list, dl_node_t<type>* node);
    SLD_API_INLINE_TEMPLATE bool             dl_list_insert_at_tail    (dl_list_t<type>* list, dl_node_t<type>* node);
    SLD_API_INLINE_TEMPLATE bool             dl_list_insert_a_after_b  (dl_list_t<type>* list, dl_node_t<type>* node_a, dl_node_t<type>* node_b);
    SLD_API_INLINE_TEMPLATE bool             dl_list_insert_a_before_b (dl_list_t<type>* list, dl_node_t<type>* node_a, dl_node_t<type>* node_b);

    template<typename type>
    struct dl_node_t {
        dl_list_t<type>* list;
        dl_node_t*       next;          
        dl_node_t*       prev;          
        type             data;
    };

    template<typename type>
    struct dl_list_t {
        dl_node_t<type>* head;        
        dl_node_t<type>* tail;        
        u32              node_count_max;
    };

    //-------------------------------------------------------------------
    // DOUBLE LINKED LIST INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE_TEMPLATE dl_list_t<type>*
    dl_list_arena_alloc(
        arena_t* arena) {

        auto list = arena_push_struct<dl_list_t<type>>(arena);
        return(list);
    }

    SLD_API_INLINE_TEMPLATE dl_node_t<type>*
    dl_node_arena_alloc(
        arena_t* arena) {

        const u32 push_size = (
            sizeof(dl_node_t<type>) + 
            sizeof(type)
        );

        const bool can_push = arena_can_push(arena, push_size);
        if (!can_push) return(NULL);

        dl_node_t<type>* node = push_struct<dl_node_t<type>>(arena);
        type*            data = arena_push_struct<type>(arena);
    
        // we just verified we should be able to allocate
        assert(node != NULL && data != NULL);

        node->list = NULL;
        node->next = NULL;
        node->prev = NULL;
        node->data = data;
        return(node);
    }

    SLD_API_INLINE_TEMPLATE bool
    dl_list_is_valid(
        const dl_list_t<type>* list,
        const dl_node_t<type>* node) {

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
    dl_list_assert_valid(
        const dl_list_t<type>* list,
        const dl_node_t<type>* node) {

        assert(dl_list_is_valid(list, node));
    }

    SLD_API_INLINE_TEMPLATE bool
    dl_list_is_empty(
        const dl_list_t<type>* list) {

        dl_list_assert_valid(list);

        const bool is_empty = (list->head != NULL && list->tail != NULL);
        return(is_empty);
    }

    SLD_API_INLINE_TEMPLATE u32
    dl_list_get_count(
        const dl_list_t<type>* list) {

        dl_list_assert_valid(list);

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
    dl_list_can_insert(
        const dl_list_t<type>* list) {

        dl_list_assert_valid(list);

        const u32  count      = dl_list_get_count(list);
        const bool can_insert = count < list->node_count_max;
        return(can_insert); 
    }

    SLD_API_INLINE_TEMPLATE bool
    dl_list_does_node_exist(
        const dl_list_t<type>* list,
        const type&            node_data) {

        dl_list_assert_valid(list);

        u32  count       = 0;
        bool does_exist = false;
        for (
            dl_node_t<type>* current_node = list->head;
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
    dl_list_insert_at_head(
        dl_list_t<type>* list,
        dl_node_t<type>* node) {

        assert(dl_list_is_valid(list) && node != NULL);

        const bool can_insert = dl_list_can_insert(list);
        if (can_insert) {
            if (dl_list_is_empty(list)) {
                node->next = NULL;
                list->head = node;
                list->tail = node;
            }
            else {
                node->next = list->head;
                list->head = node;
            }
            node->prev = NULL; 
            node->list = list;
        }
        return(can_insert);
    }
    
    SLD_API_INLINE_TEMPLATE void
    dl_list_insert_at_tail(
        dl_list_t<type>* list,
        dl_node_t<type>* node) {

        assert(dl_list_is_valid(list) && node != NULL);

        const bool can_insert = dl_list_can_insert(list);
        if (can_insert) {
            if (dl_list_is_empty(list)) {
                list->head = node;
                list->tail = node;
                node->prev = NULL;
            }
            else {
                sl_node_t<type>* old_tail = list->tail;
                old_tail->next = node->next;
                node->prev     = old_tail;
                list->tail     = node;
            }    
            node->next = NULL;
            node->list = list;
        }
        return(can_insert);
    }


    SLD_API_INLINE_TEMPLATE bool
    dl_list_insert_a_after_b(
        dl_list_t<type>* list,
        dl_node_t<type>* node_a,
        dl_node_t<type>* node_b) {

        assert(dl_list_is_valid(list, node_b) && node_a != NULL);

        const bool can_insert = dl_list_can_insert(list);
        const bool is_tail    = (node_b == list->tail); 
        if (can_insert) {

            dl_node_t<type>* node_a_next = node_b->next;

            node_b->next = node_a;
            node_a->next = node_a_next;
            if (node_a_next) {
                node_a_next->prev = node_a;
            }

            if (is_tail) {
                list->tail = node_a;
            }            
        }
        return(can_insert);
    }
    
    SLD_API_INLINE_TEMPLATE bool
    dl_list_insert_a_before_b(
        dl_list_t<type>* list,
        dl_node_t<type>* node_a,
        dl_node_t<type>* node_b) {

        assert(dl_list_is_valid(list, node_b) && node_a != NULL);

        const bool can_insert = dl_list_can_insert(list);
        const bool is_head    = (node_b == list->head); 
        if (can_insert) {

            dl_node_t<type>* node_a_prev = node_b->prev;
            if (node_a_prev) {
                node_a_prev->next = node_a;
            }

            node_a->prev = node_a_prev;
            node_a->next = node_b;

            node_b->prev = node_a;

            if (is_head) {
                list->head = node_a;
            }
        }
        return(can_insert);                
    }
};

#endif //SLD_DOUBLE_LINKED_LIST_HPP
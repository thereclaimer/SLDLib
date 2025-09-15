#pragma once

#include "sld-allocator.hpp"

namespace sld {

    struct stack_alloc_t {
        stack_alloc_t* next;
        stack_alloc_t* prev;
        u64                 size;
    };

    struct stack_alctr_t {
        struct {
            stack_alloc_t* first;
        } allocation_list;
    };

};
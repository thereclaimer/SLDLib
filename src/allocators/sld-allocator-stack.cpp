#pragma once

#include "sld-allocator.hpp"

namespace sld {

    struct stack_allocation_t {
        stack_allocation_t* next;
        stack_allocation_t* prev;
        u64                 size;
    };

    struct stack_allocator_t {
        struct {
            stack_allocation_t* first;
        } allocation_list;
    };

};
#pragma once

#include "sld-memory.hpp"
#include "sld-os.hpp"
#include "sld-stack.hpp"

namespace sld {

    struct reservation_list_t {
        reservation_t* reserved;
        reservation_t* released;
    };

    struct arena_list_t {
        arena_t* released;
    };

    struct global_stack_t : stack_t {
        reservation_list_t reservation_list;
        arena_list_t       arena_list;
        memory_error_t     last_error;
    };

    SLD_FUNC global_stack_t*     global_stack_instance                     (void);
    SLD_FUNC reservation_list_t& global_stack_get_reservation_list         (void);
    SLD_FUNC arena_list_t&       global_stack_get_arena_list               (void);
};    
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

    SLD_FUNC global_stack_t&     global_stack_instance                     (void);
    SLD_FUNC reservation_list_t& global_stack_get_reservation_list         (void);
    SLD_FUNC arena_list_t&       global_stack_get_arena_list               (void);

    SLD_FUNC arena_t*            reservation_remove_next_decommitted_arena (reservation_t*      reservation);
    SLD_FUNC void                reservation_remove_committed_arena        (reservation_t*      reservation, arena_t*      arena);
    SLD_FUNC void                reservation_insert_committed_arena        (reservation_t*      reservation, arena_t*      arena);
    SLD_FUNC void                reservation_insert_decommitted_arena      (reservation_t*      reservation, arena_t*      arena);
    SLD_FUNC arena_t*            reservation_remove_all_arenas             (reservation_t*      reservation, arena_list_t& arena_list);
    SLD_FUNC reservation_t*      reservation_list_remove_next_released     (reservation_list_t& reservation_list);
    SLD_FUNC bool                reservation_list_remove_reserved          (reservation_list_t& reservation_list, reservation_t* reservation);
    SLD_FUNC void                reservation_list_insert_released          (reservation_list_t& reservation_list, reservation_t* reservation);
    SLD_FUNC void                reservation_list_insert_reserved          (reservation_list_t& reservation_list, reservation_t* reservation);
    
    SLD_FUNC arena_t*            arena_list_remove_next_released           (arena_list_t& arena_list);
    SLD_FUNC void                arena_list_insert_all_released            (arena_list_t& arena_list, reservation_t* reservation);
    SLD_FUNC void                arena_list_insert_released                (arena_list_t& arena_list, arena_t* arena);
};    
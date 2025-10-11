#pragma once

#include "sld-memory.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API bool
    arena_t::is_valid(
        void) const {

        bool is_valid = true;
        is_valid &= reservation->is_valid();
        is_valid &= (this->array    != NULL); 
        is_valid &= (this->capacity != 0); 
        is_valid &= (this->position <  this->capacity); 
        is_valid &= (this->save     <= this->position); 
        return(is_valid);
    }

    SLD_API void
    arena_t::assert_valid(
        void) const {
        
        assert(is_valid());
    }

    SLD_API bool
    arena_t::decommit(
        void) {

        assert_valid();

        reservation_t* res          = reservation;
        const arena_t* first_arena  = reservation->get_first_arena();
        const bool     is_first     = (this == first_arena); 
        arena_t*       tmp_next     = this->next;
        arena_t*       tmp_prev     = this->prev;
        void*          arena_memory = (void*)this;

        // attempt to decommit the memory
        const bool is_decommitted = os_memory_decommit(arena_memory, res->get_arena_size());
        if (!is_decommitted) {
            last_error.val = memory_error_e_os_failed_to_decommit;
            return(is_decommitted);
        }

        // remove the arena from the list
        if (tmp_next) tmp_next->prev = tmp_prev;
        if (tmp_prev) tmp_prev->next = tmp_next;
        if (is_first) res->arenas    = tmp_next;
        return(is_decommitted);
    }
};
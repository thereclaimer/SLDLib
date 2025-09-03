#pragma once

#include "sld-memory.hpp"
#include "sld-os.hpp"

namespace sld {

    SLD_FUNC bool
    arena_validate(
        arena_t* arena) {

        bool is_valid = (arena != NULL);
        if (is_valid) {
            is_valid &= stack_validate       (arena->stack);
            is_valid &= reservation_validate (arena->reservation);
        };
        return(is_valid);
    }


    SLD_FUNC arena_t*
    arena_commit(
        reservation_t* reservation) {

        arena_t* committed_arena = NULL;                 

        bool is_valid = reservation_validate(reservation);
        if (!is_valid) return(committed_arena);

        if (reservation->arena_list.decommitted)

    }

    SLD_FUNC bool
    arena_decommit(
        arena_t* arena) {

    }

    SLD_FUNC byte*
    arena_push_bytes(
        arena_t*  arena,
        const u64 size,
        const u64 alignment = 0) {

    }

    SLD_FUNC bool
    arena_pull_bytes(
        arena_t*  arena,
        const u64 size,
        const u64 alignment = 0) {

    }

    SLD_FUNC bool
    arena_can_push_bytes(
        arena_t*  arena,
        const u64 size,
        const u64 alignment = 0) {

    }

    SLD_FUNC bool
    arena_can_pull_bytes(
        arena_t*  arena,
        const u64 size,
        const u64 alignment = 0) {

    }

    SLD_FUNC u64
    arena_space_remaining(
        const arena_t* arena) {

    }
};
#ifndef SLD_ARENA_HPP
#define SLD_ARENA_HPP

#include "sld-memory.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // ARENA API
    //-------------------------------------------------------------------

    struct arena_t {
        u64 size;
        u64 position;
        u64 save;
    };

    constexpr u32 ARENA_HEADER_SIZE = sizeof(arena_t);

    SLD_API_INLINE          bool     arena_is_valid     (const arena_t*  arena);
    SLD_API_INLINE          void     arena_assert_valid (const arena_t*  arena);
    SLD_API_INLINE          memory_t arena_to_memory    (const arena_t*  arena);
    SLD_API_INLINE          void*    arena_get_header   (const arena_t*  arena);
    SLD_API_INLINE          void*    arena_get_start    (const arena_t*  arena);
    SLD_API_INLINE          void*    arena_get_position (const arena_t*  arena);
    SLD_API_INLINE          arena_t* arena_from_memory  (const memory_t& memory);
    SLD_API_INLINE          void     arena_reset        (arena_t*        arena);
    SLD_API_INLINE          void     arena_save         (arena_t*        arena);
    SLD_API_INLINE          void     arena_roll_back    (arena_t*        arena);
    SLD_API_INLINE          void*    arena_push_bytes   (arena_t*        arena, const u64 size, const u64 alignment = 0);
    SLD_API_INLINE_TEMPLATE type*    arena_push_struct  (arena_t*        arena, const u32 count = 1);

    //-------------------------------------------------------------------
    // ARENA INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE arena_t*
    arena_from_memory(
        const memory_t& memory) {

        memory_assert_valid(memory);

        arena_t* arena = (arena_t*)memory.ptr;
        return(arena);
    }

    SLD_API_INLINE bool
    arena_is_valid(
        const arena_t* arena) {

        bool is_valid = (arena != NULL);
        if (is_valid) {

            is_valid &= (arena->size     != 0); 
            is_valid &= (arena->position <  arena->size); 
            is_valid &= (arena->save     <= arena->position); 
        }
        return(is_valid);
    }

    SLD_API_INLINE void
    arena_assert_valid(
        const arena_t* arena) {

        assert(arena_is_valid(arena));
    }

    SLD_API_INLINE void*
    arena_get_header(
        const arena_t* arena) {

        arena_assert_valid(arena);
        void* arena_header = (void*)arena;
        return(arena_header);
    }

    SLD_API_INLINE memory_t
    arena_to_memory(
        const arena_t* arena) {

        arena_assert_valid(arena);

        memory_t arena_memory;
        arena_memory.ptr  = (void*)arena;
        arena_memory.size = arena->size + sizeof(arena_t);
        return(arena_memory);
    }

    SLD_API_INLINE void*
    arena_get_start(
        const arena_t* arena) {

        arena_assert_valid(arena);

        const memory_t memory_arena = arena_to_memory(arena);
        memory_t       memory_start = memory_add_offset(memory_arena, sizeof(arena_t));
        return(memory_start.ptr);
    }

    SLD_API_INLINE void*
    arena_get_position(
        const arena_t* arena) {

        arena_assert_valid(arena);
        void* arena_start  = arena_get_start  (arena);
        void* position     = (void*)((addr)arena_start + arena->position); 
        return(position);
    }

    SLD_API_INLINE void
    arena_reset(
        arena_t* arena) {

        arena_assert_valid(arena);
        arena->position = 0;
        arena->save     = 0;
    }

    SLD_API_INLINE void
    arena_save(
        arena_t* arena) {

        arena_assert_valid(arena);
        arena->save = arena->position;
    }

    SLD_API_INLINE void
    arena_roll_back(
        arena_t* arena) {

        arena_assert_valid(arena);
        arena->position = arena->save;
    }

    SLD_API_INLINE void*
    arena_push_bytes(
        arena_t* arena,
        const u64       size,
        const u64       alignment) {

        arena_assert_valid(arena);

        const u64 alignment_pow_2 = size_round_up_pow2 (alignment);
        const u64 size_aligned    = size_align_pow_2   (size, alignment);
        const u64 new_position    = arena->position + size_aligned;
        
        void* ptr = NULL;
        if (new_position <= arena->position) {

            ptr = arena_get_position(arena);
            assert(ptr);
            
            arena->position = new_position;
        } 
        return(ptr);
    }
    
    SLD_API_INLINE_TEMPLATE type*
    arena_push_struct(
        arena_t*  arena,
        const u32 count) {

        arena_assert_valid(arena);
        assert(count != 0);

        const u64 struct_alignment  = alignof (type);
        const u64 struct_size       = sizeof  (type);
        const u64 struct_array_size = (struct_size * count); 
         
        type* ptr = (type*)arena_push_bytes(
            arena,
            struct_array_size,
            struct_alignment);

        return(ptr);
    }

};

#endif //SLD_ARENA_HPP
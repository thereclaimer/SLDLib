#ifndef SLD_ARENA_ALLOCATOR_HPP
#define SLD_ARENA_ALLOCATOR_HPP

#include "sld-memory.hpp"
#include "sld-arena.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    struct arena_allocator_t {
        memory_t memory;
        u32      arena_size;
        u32      arena_count;
    };

    SLD_API_INLINE void     arena_allocator_reserve_os_memory (arena_allocator_t*       alctr, const u32 size_total, const u32 size_arena);
    SLD_API_INLINE void     arena_allocator_release_os_memory (arena_allocator_t*       alctr);
    SLD_API_INLINE bool     arena_allocator_is_valid          (const arena_allocator_t* alctr);
    SLD_API_INLINE bool     arena_allocator_is_arena_valid    (const arena_allocator_t* alctr, const arena_t* arena);
    SLD_API_INLINE void     arena_allocator_assert_valid      (const arena_allocator_t* alctr);
    SLD_API_INLINE arena_t* arena_allocator_commit            (const arena_allocator_t* alctr);
    SLD_API_INLINE void     arena_allocator_decommit          (const arena_allocator_t* alctr, arena_t* arena);
    SLD_API_INLINE u32      arena_allocator_get_size_total    (const arena_allocator_t* alctr);
    SLD_API_INLINE u32      arena_allocator_get_size_free     (const arena_allocator_t* alctr);
    SLD_API_INLINE u32      arena_allocator_get_size_used     (const arena_allocator_t* alctr);
    SLD_API_INLINE u32      arena_allocator_get_arenas_free   (const arena_allocator_t* alctr);
    SLD_API_INLINE u32      arena_allocator_get_arenas_used   (const arena_allocator_t* alctr);

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE void 
    arena_allocator_reserve_os_memory(
        arena_allocator_t* alctr,
        const u32          size_total,
        const u32          size_arena) {

        // check args
        bool can_acquire = true;
        can_acquire &= (alctr      != NULL);
        can_acquire &= (size_total != 0);
        can_acquire &= (size_arena <  size_total);
        assert(can_acquire);

        // calculate the aligned sizes
        const u32 size_arena_aligned = os_memory_align_to_page (size_arena);
        const u32 size_total_aligned = size_align_pow_2        (size_total, size_arena_aligned);

        // reserve memory
        memory_t memory;
        memory.addr = 0;
        memory.size = size_total_aligned;
        memory_os_reserve(memory);

        // initialize the allocator
        alctr->memory      = memory;
        alctr->arena_size  = size_arena_aligned;
        alctr->arena_count = alctr->memory.size / size_arena_aligned;
        arena_allocator_assert_valid(alctr);
    }

    SLD_API_INLINE void
    arena_allocator_release_os_memory(
        arena_allocator_t* alctr) {

        arena_allocator_assert_valid(alctr);
        memory_os_release(alctr->memory);
    }

    SLD_API_INLINE bool
    arena_allocator_is_valid(
        const arena_allocator_t* alctr) {

        bool is_valid = (alctr != NULL);
        if (is_valid) {

            const bool is_reserved  = os_memory_is_reserved  (alctr->memory.ptr);
            const bool is_committed = os_memory_is_committed (alctr->memory.ptr);

            is_valid &= (is_reserved || is_committed);
            is_valid &= (alctr->memory.addr != 0);  
            is_valid &= (alctr->arena_count != 0);  
            is_valid &= (alctr->arena_size  != 0);
        }
        return(is_valid);
    }

    SLD_API_INLINE bool
    arena_allocator_is_arena_valid(
        const arena_allocator_t* alctr,
        const arena_t*           arena) {

        bool is_valid = true;
        is_valid &= arena_allocator_is_valid (alctr); 
        is_valid &= arena_is_valid    (arena); 

        if (is_valid) {

            const memory_t arena_memory = arena_to_memory(arena);
            memory_assert_valid(arena_memory);

            // make sure this arena is managed by the allocator;
            const addr last_arena_addr = (alctr->memory.addr + alctr->memory.size) - alctr->arena_size; 
            const u32  arena_offset    = (arena_memory.addr  - alctr->memory.addr);
            is_valid &= (arena->size       == alctr->arena_size);
            is_valid &= (arena_memory.addr <  last_arena_addr); 
            is_valid &= ((arena_offset % alctr->arena_size) == 0);
        }

        return(is_valid);
    }

    SLD_API_INLINE void
    arena_allocator_assert_valid(
        const arena_allocator_t* alctr) {

        assert(arena_allocator_is_valid(alctr));
    }

    SLD_API_INLINE arena_t*
    arena_allocator_commit(
        const arena_allocator_t* alctr) {

        arena_allocator_assert_valid(alctr);

        memory_t arena_memory;
        arena_memory.size = alctr->arena_size;
        arena_memory.addr = alctr->memory.addr;

        for (
            u32 arena_index = 0;
                arena_index < alctr->arena_count;
              ++arena_index) {

            const bool is_free = memory_is_os_reserved(arena_memory);
            if (is_free) {
                memory_os_commit(arena_memory);

                arena_t* arena  = arena_from_memory(arena_memory);
                arena->position = 0;
                arena->save     = 0;
                arena->size     = (arena_memory.size - sizeof(arena_t));
                arena_assert_valid(arena);
                return(arena);
            }
            arena_memory.addr += alctr->arena_size;
        }

        return(NULL);
    }
    
    SLD_API_INLINE void
    arena_allocator_decommit(
        arena_allocator_t* alctr,
        arena_t*    arena) {

        bool can_free = arena_allocator_is_arena_valid (alctr, arena); 
        assert(can_free);

        // free the arena
        memory_t arena_memory = arena_to_memory(arena);
        arena->size     = 0;
        arena->position = 0;
        arena->save     = 0;
        memory_os_decommit(arena_memory);
    }
    
    SLD_API_INLINE u32
    arena_allocator_get_size_total(
        const arena_allocator_t* alctr) {

        arena_allocator_assert_valid(alctr);

        const u32 size_total = (alctr->arena_size * alctr->arena_count);
        return(size_total);
    }
    
    SLD_API_INLINE u32
    arena_allocator_get_size_free(
        const arena_allocator_t* alctr) {

        arena_allocator_assert_valid(alctr);

        u32 size_free    = 0;
        memory_t arena_memory;
        arena_memory.size = alctr->arena_size;
        arena_memory.ptr  = alctr->memory.ptr; 
        for (
            u32 arena = 0;
            arena < alctr->arena_count;
            ++arena) {
            
            const bool is_free = memory_is_os_reserved(arena_memory); 
            if (is_free) {
                size_free += arena_memory.size;
            } 
            arena_memory.addr += arena_memory.size;
        }

        assert(size_free <= (arena_allocator_get_size_total(alctr)));
        return(size_free);
    }
    
    SLD_API_INLINE u32
    arena_allocator_get_size_used(
        const arena_allocator_t* alctr) {

        arena_allocator_assert_valid(alctr);

        u32 size_used    = 0;
        memory_t arena_memory;
        arena_memory.size = alctr->arena_size;
        arena_memory.ptr  = alctr->memory.ptr; 
        for (
            u32 arena = 0;
                arena < alctr->arena_count;
              ++arena) {
            
            const bool is_used = memory_is_os_committed(arena_memory); 
            if (is_used) {
                size_used += arena_memory.size;
            } 
            arena_memory.addr += arena_memory.size;
        }


        assert(size_used <= (arena_allocator_get_size_total(alctr)));
        return(size_used);
    }
    
    SLD_API_INLINE u32
    arena_allocator_get_arenas_free(
        const arena_allocator_t* alctr) {
        
        const u32 free_size  = arena_allocator_get_size_free(alctr); 
        const u32 free_count = free_size / alctr->arena_size;
        assert(free_count <= alctr->arena_count);
        return(free_count);
    }
    
    SLD_API_INLINE u32
    arena_allocator_get_arenas_used(
        const arena_allocator_t* alctr) {

        const u32 used_size  = arena_allocator_get_size_free(alctr); 
        const u32 used_count = used_size / alctr->arena_size;
        assert(used_count <= alctr->arena_count);
        return(used_count);
    }
};

#endif //SLD_ARENA_ALLOCATOR_HPP
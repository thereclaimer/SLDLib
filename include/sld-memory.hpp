#ifndef SLD_MEMORY_HPP
#define SLD_MEMORY_HPP

#include "sld.hpp"
#include "sld-stack.hpp"

#ifndef    SLD_MEMORY_INTERNAL_STACK_SIZE_KB
#   define SLD_MEMORY_INTERNAL_STACK_SIZE_KB 128
#endif 
#ifndef    SLD_MEMORY_DEFAULT_ALIGNMENT
#   define SLD_MEMORY_DEFAULT_ALIGNMENT 4
#endif

#define SLD_MEMORY_BLOCK_ALCTR_IMPL_INLINE inline auto block_allocator_t::
namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    struct memory_error_t : s32_t { };
    struct memory_t;
    
    struct reservation_t;
    struct arena_t;

    struct allocation_t;
    struct allocator_base_t;

    struct block_allocator_t;
    struct stack_allocator_t;
    struct block_arena_allocator_t;

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API void  memory_zero    (byte*       start,     const u64   size);
    SLD_API byte* memory_advance (const byte* start,     const u64   size,      const u64 stride, u64& offset);
    SLD_API void  memory_copy    (byte*       start_dst, const byte* start_src, const u64 size);

    //-------------------------------------------------------------------
    // ARENA ALLOCATOR
    //-------------------------------------------------------------------

    struct arena_allocator_t {
        addr start;
        u32  arena_size;
        u32  arena_count;
    };

    struct arena_t {
        arena_allocator_t* allocator;
        u32                position;
        u32                save;
    };

    SLD_API void     arena_allocator_reserve_os_memory (arena_allocator_t* alctr);
    SLD_API arena_t* arena_allocator_release_os_memory (arena_allocator_t* alctr);
    SLD_API arena_t* arena_allocator_commit_arena      (arena_allocator_t* alctr);
    SLD_API void     arena_allocator_decommit_arena    (arena_allocator_t* alctr, arena_t* arena);

    SLD_API void  arena_reset     (arena_t* arena);
    SLD_API void  arena_save      (arena_t* arena);
    SLD_API void  arena_roll_back (arena_t* arena);
    SLD_API void* arena_alloc     (arena_t* arena, const u32 size, const u32 alignment = 0);
    
    SLD_API template <typename t>
    void* arena_push_struct(arena_t* arena);

    struct reservation_t {
        addr           start;
        u64            size_reserved;
        u64            size_arena;
        arena_t*       arenas;
        memory_error_t last_error;
    
        SLD_API bool                  acquire_system_memory (const u64 size_min_reservation, const u64 size_min_arena = 0);
        SLD_API bool                  release_system_memory (void);
        SLD_API bool                  is_valid              (void);
        SLD_API void                  assert_valid          (void);
        SLD_API bool                  reset                 (void);
        SLD_API u64                   size_committed        (void);
        SLD_API u64                   size_decommitted      (void);
        SLD_API arena_t*              commit_arena          (void);
        SLD_API inline arena_t*       get_first_arena       (void) { return(arenas);     }
        SLD_API inline memory_error_t get_last_error        (void) { return(last_error); }
        SLD_API inline u32            get_arena_size        (void) { return(size_arena); }
    }; 

    //-------------------------------------------------------------------
    // ARENA
    //-------------------------------------------------------------------

    struct arena_t {
        reservation_t* reservation;
        u32 position;
        u32 last_error;

    };


        SLD_API bool                  is_valid             (void) const;
        SLD_API void                  assert_valid         (void) const;
        SLD_API bool                  decommit             (void);
        SLD_API inline memory_error_t get_last_error       (void) { return(last_error); }

    //-------------------------------------------------------------------
    // BLOCK ALLOCATOR
    //-------------------------------------------------------------------
    
    struct block_allocator_t {
        addr  start;
        u32   block_count;
        u32   block_size;
    };

    SLD_API void  block_allocator_reserve_os_memory (block_allocator_t* alctr, const u32 size, const u32 granularity);
    SLD_API void  block_allocator_release_os_memory (block_allocator_t* alctr);
    SLD_API void* block_allocator_alloc             (const block_allocator_t* alctr);
    SLD_API void  block_allocator_free              (const block_allocator_t* alctr, void* block);
    SLD_API bool  block_allocator_is_valid          (const block_allocator_t* alctr);
    SLD_API void  block_allocator_assert_valid      (const block_allocator_t* alctr);
    SLD_API u32   block_allocator_get_size_total    (const block_allocator_t* alctr);
    SLD_API u32   block_allocator_get_size_free     (const block_allocator_t* alctr);
    SLD_API u32   block_allocator_get_size_used     (const block_allocator_t* alctr);
    SLD_API u32   block_allocator_get_blocks_free   (const block_allocator_t* alctr);
    SLD_API u32   block_allocator_get_blocks_used   (const block_allocator_t* alctr);

    //-------------------------------------------------------------------
    // POOL ALLOCATOR
    //-------------------------------------------------------------------

    
    //-------------------------------------------------------------------
    // ENUMS
    //-------------------------------------------------------------------

    enum memory_error_e {
        memory_error_e_success                   =  1,
        memory_error_e_unknown                   = -1,
        memory_error_e_invalid_args              = -2,
        memory_error_e_invalid_reservation       = -4,
        memory_error_e_invalid_arena             = -5,
        memory_error_e_stack_not_enough_memory   = -7,
        memory_error_e_reservation_out_of_memory = -8,
        memory_error_e_arena_not_enough_memory   = -9,
        memory_error_e_os_failed_to_reserve      = -10,
        memory_error_e_os_failed_to_release      = -11,
        memory_error_e_os_failed_to_commit       = -12,
        memory_error_e_os_failed_to_decommit     = -13,
        memory_error_e_critical                  = -14
    };

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct memory_t {
        addr start;
        u64  size;
    };
};

#endif //SLD_MEMORY_HPP
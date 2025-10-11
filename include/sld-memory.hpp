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
    // RESERVATION
    //-------------------------------------------------------------------

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

    struct arena_t : stack_t {

        reservation_t* reservation;
        arena_t*       next;
        arena_t*       prev;
        memory_error_t last_error;

        SLD_API bool                  is_valid             (void) const;
        SLD_API void                  assert_valid         (void) const;
        SLD_API bool                  decommit             (void);
        SLD_API inline memory_error_t get_last_error       (void) { return(last_error); }
    };

    //-------------------------------------------------------------------
    // BLOCK ALLOCATOR
    //-------------------------------------------------------------------
    struct block_allocator_t {

        addr  memory_start;
        bool* block_array;
        u32   block_count;
        u32   block_size;

        void  init                  (void* memory, const u32 memory_size, const u32 block_size);
        void  assert_valid          (void);
        void* alloc                 (void);
        void  free                  (void*);
        void  reset                 (void);

        SLD_API inline u32 get_size_total        (void);
        SLD_API inline u32 get_size_used         (void);
        SLD_API inline u32 get_size_free         (void);
        SLD_API inline u32 get_block_count_used  (void);
        SLD_API inline u32 get_block_count_free  (void);
    };

    SLD_MEMORY_BLOCK_ALCTR_IMPL_INLINE
    get_size_total(void) -> u32 {

        return(block_count * block_size);
    }

    //-------------------------------------------------------------------
    // POOL ALLOCATOR
    //-------------------------------------------------------------------


    SLD_API stack_allocator_t*  stack_allocator_init          (const void*               memory,    const u32   size, const u32   granularity = SLD_MEMORY_DEFAULT_ALIGNMENT);
    SLD_API bool                stack_allocator_validate      (stack_allocator_t*  const allocator);
    SLD_API void*               stack_allocator_alloc_abs     (stack_allocator_t*  const allocator, const u32   size);
    SLD_API u32                 stack_allocator_alloc_rel     (stack_allocator_t*  const allocator, const u32   size);
    SLD_API bool                stack_allocator_free_abs      (stack_allocator_t*  const allocator, void* const memory);
    SLD_API bool                stack_allocator_free_rel      (stack_allocator_t*  const allocator, const u32   offset);
    
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
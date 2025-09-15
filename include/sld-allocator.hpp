#ifndef SLD_ALLOCATOR_HPP
#define SLD_ALLOCATOR_HPP

#include "sld.hpp"
#include "sld-memory.hpp"
#include "sld-stack.hpp"
#include "sld-heap.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    struct block_allocator_t;
    struct block_allocation_t;

    struct heap_allocator_t;
    struct heap_allocation_t;
    
    struct stack_allocator_t;
    struct stack_allocation_t;

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API const u64          stack_allocator_get_required_memory_size (const u64       stack_size);
    SLD_API stack_allocator_t* stack_allocator_init_from_memory         (const memory_t& memory);
    SLD_API stack_allocator_t* stack_allocator_init_from_arena          (arena_t*        arena);
    SLD_API void*              stack_alloc                              (stack_allocator_t* stack_allocator, const u64 size);
    SLD_API bool               stack_free                               (stack_allocator_t* stack_allocator, const u64 size);

    SLD_API const u64          block_allocator_get_required_memory_size (const u64   block_memory_size, const u64 block_size);
    SLD_API block_allocator_t* block_allocator_init_from_memory         (const void* block_memory,      const u64 block_memory_size, const u64 block_size);
    SLD_API block_allocator_t* block_allocator_init_from_arena          (arena_t*    arena,             const u64 block_size,        const u64 block_count);
    SLD_API bool               block_allocator_validate                 (block_allocator_t* block_allocator);
    SLD_API void*              block_alloc                              (block_allocator_t* block_allocator);
    SLD_API bool               block_free                               (block_allocator_t* block_allocator, const void* block);
    
    SLD_API const u64          heap_allocator_get_required_memory_size  (const u64   heap_size,  const u64 heap_granularity);
    SLD_API heap_allocator_t*  heap_allocator_init_from_memory          (const void* heap_start, const u64 heap_size, const u64 heap_granularity);
    SLD_API heap_allocator_t*  heap_allocator_init_from_arena           (arena_t*    arena,      const u64 heap_size, const u64 heap_granularity);
    SLD_API            void*   heap_allocator_validate                  (heap_allocator_t*  heap_allocator,  const u64   size);
    SLD_API            void*   heap_alloc                               (heap_allocator_t*  heap_allocator,  const u64   size);
    SLD_API            bool    heap_free                                (heap_allocator_t*  heap_allocator,  const void* memory);
};


#endif //SLD_ALLOCATOR_HPP
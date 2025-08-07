#ifndef SLD_ALLOCATOR_HPP
#define SLD_ALLOCATOR_HPP

#include "sld.hpp"
#include "sld-memory.hpp"

namespace sld {

    struct stack_allocator_t;
    struct block_allocator_t;
    struct heap_allocator_t;

    const u32                stack_allocator_get_required_memory_size (const u32 stack_size);
    const stack_allocator_t* stack_allocator_init_from_memory         (const memory_t& memory);
    const stack_allocator_t* stack_allocator_init_from_arena          (memory_arena_t* arena);
    void*                    stack_alloc                              (stack_allocator_t* stack_allocator, const u32 size);
    bool                     stack_free                               (stack_allocator_t* stack_allocator, const u32 size);


    const u32                block_allocator_get_required_memory_size (const u32 block_size,   const u32 block_count);
    const block_allocator_t* block_allocator_init_from_memory         (const memory_t& memory, const u32 block_size);
    const block_allocator_t* block_allocator_init_from_arena          (memory_arena_t* arena,  const u32 block_size, const u32 block_count);
    
    const u32               heap_allocator_get_required_memory_size  (const u32 heap_size,    const u32 heap_granularity);
    const heap_allocator_t* heap_allocator_init_from_memory          (const memory_t& memory, const u32 heap_granularity);
    const heap_allocator_t* heap_allocator_init_from_arena           (memory_arena_t* arena,  const u32 heap_size, const u32 heap_granularity);
                      void* heap_alloc  (heap_allocator_t*  heap_allocator,  const u32   size);
                      bool  heap_free   (heap_allocator_t*  heap_allocator,  const void* memory);
    
    void* block_alloc (block_allocator_t* block_allocator);
    bool  block_free  (block_allocator_t* block_allocator, const void* block);

};

#endif //SLD_ALLOCATOR_HPP
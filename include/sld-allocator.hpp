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

    struct block_alctr_t;
    struct block_alloc_t;

    struct heap_alctr_t;
    struct heap_alloc_t;

    struct stack_alctr_t;
    struct stack_alloc_t;

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API stack_alctr_t* stack_alctr_init_from_memory (const memory_t& memory);
    SLD_API stack_alctr_t* stack_alctr_init_from_arena  (arena_t*        arena);
    SLD_API bool           stack_alctr_validate         (stack_alctr_t*  alctr);
    SLD_API void*          stack_alctr_alloc            (stack_alctr_t*  alctr, const u64 size);
    SLD_API bool           stack_alctr_free             (stack_alctr_t*  alctr, const u64 size);

    SLD_API block_alctr_t* block_alctr_init_from_memory (const void*    memory, const u64 size, const u64 granularity);
    SLD_API block_alctr_t* block_alctr_init_from_arena  (arena_t*       arena,  const u64 size, const u64 granularity);
    SLD_API bool           block_alctr_validate         (block_alctr_t* alctr);
    SLD_API void*          block_alctr_alloc            (block_alctr_t* alctr);
    SLD_API bool           block_alctr_free             (block_alctr_t* alctr, const void* memory);
    
    SLD_API heap_alctr_t*  heap_alctr_init_from_memory  (const void*    memory, const u64   size, const u64 granularity);
    SLD_API heap_alctr_t*  heap_alctr_init_from_arena   (arena_t*       arena,  const u64   size, const u64 granularity);
    SLD_API         void*  heap_alctr_validate          (heap_alctr_t*  alctr,  const u64   size);
    SLD_API         void*  heap_alctr_alloc             (heap_alctr_t*  alctr,  const u64   size);
    SLD_API         bool   heap_alctr_free              (heap_alctr_t*  alctr,  const void* memory);
    SLD_API         bool   heap_alctr_reset             (heap_alctr_t*  alctr);

};


#endif //SLD_ALLOCATOR_HPP
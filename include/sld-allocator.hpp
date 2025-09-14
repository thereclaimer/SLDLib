#ifndef SLD_ALLOCATOR_HPP
#define SLD_ALLOCATOR_HPP

#include "sld.hpp"
#include "sld-memory.hpp"
#include "sld-stack.hpp"
#include "sld-heap.hpp"

namespace sld {

    struct stack_allocator_t;
    struct stack_allocation_t;
    struct block_allocator_t;
    struct block_allocation_t;
    struct heap_allocator_t;
    struct heap_allocation_t;

    const u64                stack_allocator_get_required_memory_size (const u64       stack_size);
    const stack_allocator_t* stack_allocator_init_from_memory         (const memory_t& memory);
    const stack_allocator_t* stack_allocator_init_from_arena          (arena_t*        arena);
    void*                    stack_alloc                              (stack_allocator_t* stack_allocator, const u64 size);
    bool                     stack_free                               (stack_allocator_t* stack_allocator, const u64 size);

    const u64                block_allocator_get_required_memory_size (const u64       block_size, const u64 block_count);
    const block_allocator_t* block_allocator_init_from_memory         (const memory_t& memory,     const u64 block_size);
    const block_allocator_t* block_allocator_init_from_arena          (arena_t* arena,  const u64 block_size, const u64 block_count);
    void*                    block_alloc                              (block_allocator_t* block_allocator);
    bool                     block_free                               (block_allocator_t* block_allocator, const void* block);
    
    const u64                heap_allocator_get_required_memory_size  (const u64       heap_size, const u64 node_size_min, const u64 node_size_max);
    const heap_allocator_t*  heap_allocator_init_from_memory          (const memory_t& memory,    const u64 heap_granularity);
    const heap_allocator_t*  heap_allocator_init_from_arena           (arena_t*        arena,     const u64 heap_size, const u64 heap_granularity);
                      void*  heap_alloc                               (heap_allocator_t*  heap_allocator,  const u64   size);
                      bool   heap_free                                (heap_allocator_t*  heap_allocator,  const void* memory);

    struct stack_allocation_t {
        stack_allocation_t* next;
        stack_allocation_t* prev;
        u64                 size;
    };

    struct block_allocation_t {
        block_allocation_t* next;
        block_allocation_t* prev;
    };

    struct heap_allocation_t {
        heap_allocation_t* next;
        heap_allocation_t* prev;
        heap_node_t        node;
    };
    struct stack_allocator_t : memory_t {
        stack_t;
        struct {
            stack_allocation_t* first;
        } allocation_list;
    };
    struct block_allocator_t : memory_t {
        u64 block_size;
        struct {
            block_allocation_t* free;
            block_allocation_t* used;
        } allocation_list;
    };
    struct heap_allocator_t : memory_t {
        heap_t heap;
        struct {
            heap_allocation_t* first;
        } allocation_list;
    };

};


#endif //SLD_ALLOCATOR_HPP
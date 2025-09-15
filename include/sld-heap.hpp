#ifndef SLD_HEAP_HPP
#define SLD_HEAP_HPP

#include "sld.hpp"
#include "sld-memory.hpp"

#define SLD_HEAP_DEFAULT_GRANULARITY sizeof(u32)

namespace sld {

    struct heap_t;
    struct heap_root_t;
    struct heap_node_t;

    SLD_API const u64    heap_memory_size (const u64       heap_size_min);
    SLD_API bool         heap_validate    (const heap_t*   heap);
    SLD_API heap_t*      heap_init        (const memory_t& memory, const u64 granularity);
    SLD_API bool         heap_reset       (heap_t*         heap);
    SLD_API heap_node_t* heap_insert      (heap_t*         heap, const u32    size);
    SLD_API bool         heap_remove      (heap_t*         heap, heap_node_t* node);
};

#endif //SLD_HEAP_HPP
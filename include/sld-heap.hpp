#ifndef SLD_HEAP_HPP
#define SLD_HEAP_HPP

#include "sld.hpp"

#define SLD_HEAP_DEFAULT_GRANULARITY sizeof(u32)

namespace sld {

    struct heap_t;
    struct heap_node_t;

    bool  heap_validate (const heap_t& heap);
    bool  heap_init     (heap_t&       heap);
    bool  heap_reset    (heap_t&       heap);
    void* heap_add      (heap_t&       heap, const u32   size);
    bool  heap_remove   (heap_t&       heap, const void* data);

    struct heap_t {
        addr start;
        struct  {
            heap_node_t* free;
            heap_node_t* reserved;
        } nodes;
        u32 size;
        u32 granularity;
    };

    struct heap_node_t {
        heap_node_t* parent;
        heap_node_t* next;
        heap_node_t* prev;
        u32          size;
    };

};

#endif //SLD_HEAP_HPP
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

namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    struct memory_error_t : s32_t { };
    struct reservation_t;
    struct reservation_list_t;
    struct arena_t;
    struct arena_list_t;
    struct memory_t;

    struct allocation_t;
    struct allocation_list_t;

    class allocator_base_t;
    template<typename t> class block_allocator_t;
    template<typename t> class stack_allocator_t;
    template<typename t> class heap_allocator_t;

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API void                memory_zero                   (byte*       start,     const u64 size);
    SLD_API byte*               memory_advance                (const byte* start,     const u64 size,      const u64 stride, u64& offset);
    SLD_API void                memory_copy                   (byte*       start_dst, byte*     start_src, const u64 size);

    SLD_API byte*               global_stack_push_bytes       (const u64 size, const u64 alignment = 0);
    SLD_API memory_error_t      global_stack_last_error       (void);
    SLD_API u64                 global_stack_size_total       (void);
    SLD_API u64                 global_stack_size_used        (void);
    SLD_API u64                 global_stack_size_free        (void);
    SLD_API addr                global_stack_start            (void);

    SLD_API bool                reservation_validate          (reservation_t* reservation);
    SLD_API reservation_t*      reservation_acquire           (const u64 size_min_reservation = 0, const u64 size_min_arena = 0);
    SLD_API bool                reservation_release           (reservation_t* reservation);
    SLD_API bool                reservation_reset             (reservation_t* reservation);
    SLD_API u64                 reservation_size_committed    (reservation_t* reservation);
    SLD_API u64                 reservation_size_decommitted  (reservation_t* reservation);

    SLD_API bool                arena_validate                (arena_t*       arena);
    SLD_API arena_t*            arena_commit                  (reservation_t* reservation);
    SLD_API bool                arena_decommit                (arena_t*       arena);
    SLD_API byte*               arena_push_bytes              (arena_t*       arena, const u64 size, const u64 alignment = SLD_MEMORY_DEFAULT_ALIGNMENT);
    SLD_API bool                arena_pull_bytes              (arena_t*       arena, const u64 size, const u64 alignment = SLD_MEMORY_DEFAULT_ALIGNMENT);
    SLD_API bool                arena_reset                   (arena_t*       arena);
    SLD_API bool                arena_roll_back               (arena_t*       arena);
    SLD_API bool                arena_save_position           (arena_t*       arena);
    SLD_API u64                 arena_size_total              (arena_t*       arena);
    SLD_API u64                 arena_size_free               (arena_t*       arena);
    SLD_API u64                 arena_size_used               (arena_t*       arena);

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
        const addr start;
        const u64  size;
    };

    struct reservation_t {
        addr start;
        struct {
            u64 reserved;
            u64 arena;
        } size;
        struct {
            arena_t* committed;
            arena_t* decommitted;
        } arena_list;
        reservation_t* next;
        reservation_t* prev;
        memory_error_t last_error;
    }; 

    struct arena_t : stack_t {
        arena_t*       next;
        arena_t*       prev;
        reservation_t* reservation;
        memory_error_t last_error;
    };

    struct allocation_t {
        allocation_list_t* list;
        allocation_t*      next;
        allocation_t*      prev;
        u32                size;
        u32                handle;
    };  

    struct allocation_list_t {
        allocator_base_t*  alctr;
        allocation_t*      free;
        allocation_t*      used;
    };

    allocation_t* allocation_list_split                                (allocation_list_t* const list, allocation_t* const allocation, const u32 size,     const u32 alignment = SLD_MEMORY_DEFAULT_ALIGNMENT);
    allocation_t* allocation_list_recycle_free_allocation              (allocation_list_t* const list, const u32           size = 0,   const u32 alignment = SLD_MEMORY_DEFAULT_ALIGNMENT);
    allocation_t* allocation_list_recycle_and_split_free_allocation    (allocation_list_t* const list, const u32           size = 0,   const u32 alignment = SLD_MEMORY_DEFAULT_ALIGNMENT);
    allocation_t* allocation_list_get_allocation_from_memory           (allocation_list_t* const list, const void*         memory);
    void          allocation_list_free_used_allocation                 (allocation_list_t* const list, allocation_t*       allocation);
    void          allocation_list_free_and_consolidate_used_allocation (allocation_list_t* const list, allocation_t*       allocation);
    void*         allocation_list_get_memory_from_allocation           (allocation_list_t* const list, const allocation_t* allocation);
    u32           allocation_list_get_count_free                       (allocation_list_t* const list);
    u32           allocation_list_get_count_used                       (allocation_list_t* const list);
    u32           allocation_list_get_size_free                        (allocation_list_t* const list);
    u32           allocation_list_get_size_used                        (allocation_list_t* const list);

    class allocator_base_t {

    private:

        addr              _start;
        u32               _size;
        allocation_t*     _free_allocations;
        allocation_t*     _used_allocations;

        allocation_t* get_allocation_from_memory (const void*         memory);
        void*         get_memory_from_allocation (const allocation_t* allocation);
        allocation_t* recycle_free_allocation    (void);
        void          free_used_allocation       (allocation_t* allocation);

    public:

        allocator_base_t (const void* memory, const u32 size);
        allocator_base_t (arena_t*    arena,  const u32 size);

        u32  size_total  (void);

        void* alloc_abs         (const u32   size = 0, const u32 alignment = 0);
        u32   alloc_rel         (const u32   size = 0, const u32 alignment = 0);
        void  free_abs          (const void* pointer);
        void  free_rel          (const u32   handle);
        void* alloc_get_pointer (const void* memory);
        void* alloc_get_handle  (const void* memory);
    };

    template<typename t>
    class block_allocator_t {

    private:
        memory_t _memory;
        u64      _block_size;
        

    public:


        virtual t* alloc() = 0;
    };
};

#endif //SLD_MEMORY_HPP
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
    struct allocator_base_t;

    struct block_allocator_t;
    struct stack_allocator_t;
    struct block_arena_allocator_t;

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API void                memory_zero                   (byte*       start,     const u64   size);
    SLD_API byte*               memory_advance                (const byte* start,     const u64   size,      const u64 stride, u64& offset);
    SLD_API void                memory_copy                   (byte*       start_dst, const byte* start_src, const u64 size);

    //-------------------------------------------------------------------
    // RESERVATION
    //-------------------------------------------------------------------

    struct reservation_t {
        addr start;
        struct {
            u64 reserved;
            u64 arena_memory;
            u64 arena_header;
        } size;
        arena_t*       arenas;
        memory_error_t last_error;
    
        SLD_API bool     acquire_system_memory (const u64 size_min_reservation, const u64 size_min_arena = 0, const u64 size_arena_header = 0);
        SLD_API bool     release_system_memory (void);
        SLD_API bool     is_valid              (void);
        SLD_API void     assert_valid          (void);
        SLD_API bool     reset                 (void);
        SLD_API u64      size_committed        (void);
        SLD_API u64      size_decommitted      (void);
        SLD_API arena_t* commit_arena          (void);
    }; 

    //-------------------------------------------------------------------
    // ARENA
    //-------------------------------------------------------------------

    struct arena_t {
        reservation_t* reservation;
        arena_t*       next;
        arena_t*       prev;
        data_stack_t*  stack;
        memory_error_t last_error;

        SLD_API bool               is_valid             (void);
        SLD_API void               assert_valid         (void);
        SLD_API bool               decommit             (void);
        SLD_API byte*              push_bytes           (const u64 size, const u64 alignment   = SLD_MEMORY_DEFAULT_ALIGNMENT);
        SLD_API block_allocator_t* push_block_allocator (const u32 size, const u32 granularity = SLD_MEMORY_DEFAULT_ALIGNMENT);
        SLD_API stack_allocator_t* push_stack_allocator (const u32 size, const u32 granularity = SLD_MEMORY_DEFAULT_ALIGNMENT);
        SLD_API bool               pull_bytes           (const u64 size, const u64 alignment   = SLD_MEMORY_DEFAULT_ALIGNMENT);
        SLD_API void               reset                (void);
        SLD_API void               roll_back            (void);
        SLD_API void               save_position        (void);
        SLD_API u64                size_total           (void);
        SLD_API u64                size_free            (void);
        SLD_API u64                size_used            (void);

        template<typename t>
        SLD_API t* push_struct(void);
    };

    SLD_API block_allocator_t*  block_allocator_init          (const void*              memory,    const u32   size, const u32 granularity = SLD_MEMORY_DEFAULT_ALIGNMENT);
    SLD_API void*               block_allocator_alloc_abs     (block_allocator_t* const allocator, const u32   size, const u32 alignment   = SLD_MEMORY_DEFAULT_ALIGNMENT);
    SLD_API u32                 block_allocator_alloc_rel     (block_allocator_t* const allocator, const u32   size, const u32 alignment   = SLD_MEMORY_DEFAULT_ALIGNMENT);
    SLD_API bool                block_allocator_free_abs      (block_allocator_t* const allocator, void* const memory);
    SLD_API bool                block_allocator_free_rel      (block_allocator_t* const allocator, const u32   offset);
    SLD_API bool                block_allocator_validate      (block_allocator_t* const allocator);
    SLD_API bool                block_allocator_reset         (block_allocator_t* const allocator);

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





    struct allocation_t {
        allocator_base_t* alctr;
        allocation_t*     next;
        allocation_t*     prev;
        u32               size;
        u32               offset;
    };

    struct allocator_base_t {
        struct {
            allocation_t* used;
            allocation_t* free;
        } allocation_list;
        u32 size;
        u32 granularity;
    };

    struct block_allocator_t : allocator_base_t {
        u32 block_count;
    };

    struct stack_allocator_t : allocator_base_t {
        data_stack_t  stack;
        allocation_t* head_used;
        allocation_t* head_free;
    };

    ///////////////////////////////////////////////    
    // REASONING:
    // the allocators are 32 bit, meaning they can
    // handle 4GB of memory max
    //
    // in theory, if we take 4GB as a number,
    // subtract the size of the allocator header,
    // then divide by the minimum allocation size,
    // that should give us a reasonable max number of
    // times we can iterate a single loop when transforming
    // memory allocations
    ///////////////////////////////////////////////    
    constexpr u32 MEMORY_MAX_LOOP_ITERATIONS = (
        (0xFFFFFF - sizeof(allocator_base_t)) / 
        (sizeof(allocation_t) + SLD_MEMORY_DEFAULT_ALIGNMENT)
    );

    //-------------------------------------------------------------------
    // INLINE
    //-------------------------------------------------------------------

    SLD_INLINE bool
    allocator_validate(
        allocator_base_t* const allocator) {

        bool is_valid = (allocator != NULL);
        if (is_valid) {

            is_valid &= (allocator->allocation_list.free != NULL || allocator->allocation_list.used != NULL);
            is_valid &= (allocator->size        != 0);
            is_valid &= (allocator->granularity != 0);
            is_valid &= (allocator->granularity < allocator->size);
        }
        return(is_valid);
    }

    SLD_INLINE u32
    allocator_aligned_allocation_size(
        allocator_base_t* allocator,
        const u32         size_requested) {

        const u32 aligned    = size_align_pow_2(size_requested, allocator->granularity);
        const u32 size_total = aligned + sizeof(allocation_t);
        return(size_total);
    } 

    SLD_INLINE u32
    allocator_minimum_allocation_size(
        allocator_base_t* allocator) {

        bool      is_valid = (allocator != NULL) && (allocator->granularity != 0);
        const u32 size_min = (is_valid == true)
            ? sizeof(allocation_t) + allocator->granularity
            : 0;

        return(size_min);
    }

    SLD_INLINE u32
    allocator_max_possible_allocations(
        allocator_base_t* allocator) {

        const u32 min_alloc_size  = allocator_minimum_allocation_size(allocator);
        const u32 max_alloc_count = allocator != NULL ? allocator->size / min_alloc_size : 0;

        return(
            (max_alloc_count < MEMORY_MAX_LOOP_ITERATIONS) 
                ? max_alloc_count
                : MEMORY_MAX_LOOP_ITERATIONS
        );
    }

    SLD_INLINE bool
    allocation_validate(
        allocation_t* const allocation) {
        
        bool is_valid = true;
        is_valid &= (allocation != NULL);
        is_valid &= allocator_validate(allocation->alctr); 
        if (is_valid) {
            
            const allocator_base_t* alctr       = allocation->alctr;
            const addr              alctr_start = (addr)alctr;
            const addr              alctr_end   = alctr_start + alctr->size;
            const addr              alloc_start = (addr)allocation;
            const u32               alloc_mod   = (allocation->size % alctr->granularity); 

            is_valid &= (alloc_mod          == 0);          // the size should be aligned to the granularity
            is_valid &= (allocation->size   != 0);          // the allocation size shouldn't be 0
            is_valid &= (allocation->offset != 0);          // the allocation offset shouldn't be 0
            is_valid &= (allocation->size   < alctr->size); // the allocation shouldn't exceed the allocator size
            is_valid &= (alloc_start        > alctr_start); // the allocation should be after the allocator start            
            is_valid &= (alloc_start        < alctr_end);   // the allocation should be before the allocator end         
        }
        return(is_valid);
    }


    SLD_INLINE void*
    allocation_get_memory(
        allocation_t* const allocation) { 

        void* memory = (allocation != NULL)
            ? (void*)(((addr)allocation) + sizeof(allocation_t))
            : NULL;
        return(memory);
    }

    SLD_INLINE allocation_t*
    allocation_from_memory(void* const memory) {

        if (!memory) return(NULL);

        allocation_t* alloc = (allocation_t*)(((addr)memory) - sizeof(allocation_t));

        const bool is_valid = allocation_validate(alloc);
        return(is_valid ? alloc : NULL);
    };

    SLD_INLINE allocation_t*
    allocation_from_offset(
        allocator_base_t* const allocator,
        const u32               offset) {

        bool is_valid = true;
        is_valid &= (allocator != NULL);
        is_valid &= (offset    != 0);
        if (!is_valid) return(NULL);

        const addr addr_allocator  = (addr)allocator;
        const addr addr_allocation = (addr_allocator + offset);
        allocation_t* allocation   = (allocation_t*)addr_allocation; 

        return(allocation->alctr == allocator ? allocation : NULL);
    }

    SLD_INLINE u32
    allocation_size_total(
        allocation_t* const allocation) {

        const u32 size_total = (allocation != NULL)
            ? sizeof(allocation_t) + allocation->size
            : 0;

        return(size_total);
    }

    SLD_INLINE addr
    allocation_get_ending_address(
        allocation_t* const allocation) {

        constexpr u32 struct_size = sizeof(allocation_t); 
        const addr    alloc_start = (addr)allocation;
        const addr    alloc_end   = (alloc_start + struct_size + allocation->size); 

        return(alloc_end);

    }

};

#endif //SLD_MEMORY_HPP
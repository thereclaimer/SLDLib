#ifndef SLD_MEMORY_HPP
#define SLD_MEMORY_HPP

#include "sld.hpp"
#include "sld-stack.hpp"
#include "sld-os.hpp"

#ifndef    SLD_MEMORY_DEFAULT_ALIGNMENT
#   define SLD_MEMORY_DEFAULT_ALIGNMENT 4
#endif

namespace sld {

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    constexpr u32 MEMORY_DEFAULT_ALIGNMENT = SLD_MEMORY_DEFAULT_ALIGNMENT;

    struct memory_t;
    struct memory_os_reservation_t;
    struct memory_os_commit_t;
    struct memory_arena_t;
    using  memory_error_t = s32;

    SLD_API_INLINE          bool            memory_is_valid           (const memory_t& memory);
    SLD_API_INLINE          void            memory_assert_valid       (const memory_t& memory);
    SLD_API_INLINE          void*           memory_add_offset         (const void*     memory, const u32 offset);
    SLD_API_INLINE          bool            memory_is_os_committed    (const memory_t& memory);
    SLD_API_INLINE          bool            memory_is_os_reserved     (const memory_t& memory);
    SLD_API_INLINE          memory_arena_t* memory_to_arena           (const memory_t& memory);

    SLD_API_INLINE          void            memory_os_reserve         (memory_t& memory);
    SLD_API_INLINE          void            memory_os_release         (memory_t& memory);
    SLD_API_INLINE          void            memory_os_commit          (memory_t& memory);
    SLD_API_INLINE          void            memory_os_decommit        (memory_t& memory);
    SLD_API_INLINE          void            memory_zero               (memory_t& memory);
    SLD_API_INLINE_TEMPLATE void            memory_zero_struct        (type*     ptr_type);
    
    SLD_API_INLINE          bool            memory_arena_is_valid     (const memory_arena_t* arena);
    SLD_API_INLINE          void            memory_arena_assert_valid (const memory_arena_t* arena);
    SLD_API_INLINE          memory_t        memory_arena_to_memory    (const memory_arena_t* arena);
    SLD_API_INLINE          void*           memory_arena_get_header   (const memory_arena_t* arena);
    SLD_API_INLINE          void*           memory_arena_get_start    (const memory_arena_t* arena);
    SLD_API_INLINE          void*           memory_arena_get_position (const memory_arena_t* arena);
    SLD_API_INLINE          void            memory_arena_reset        (memory_arena_t*       arena);
    SLD_API_INLINE          void            memory_arena_save         (memory_arena_t*       arena);
    SLD_API_INLINE          void            memory_arena_roll_back    (memory_arena_t*       arena);
    SLD_API_INLINE          void*           memory_arena_push_bytes   (memory_arena_t*       arena, const u32 size, const u32 alignment = MEMORY_DEFAULT_ALIGNMENT);
    SLD_API_INLINE_TEMPLATE type*           memory_arena_push_struct  (memory_arena_t*       arena);

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct memory_t {
        union {
            addr  addr;
            void* ptr;
            byte* bytes;
        };
        u64   size;
    };

    struct memory_arena_t {
        u32 size;
        u32 position;
        u32 save;
    };

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE bool
    memory_is_valid(
        const memory_t& memory) {

        const bool is_valid = (memory.addr != 0 && memory.size != 0);
    }

    SLD_API_INLINE void
    memory_assert_valid(
        const memory_t& memory) {

        assert(memory_is_valid(memory));
    }

    SLD_API_INLINE void*
    memory_add_offset(
        const void* memory,
        const u32   offset) {

        void* ptr = (void*)((addr)memory + offset);
        return(ptr);
    }

    SLD_API_INLINE void
    memory_os_reserve(memory_t& memory) {

        const u64 requested_size = memory.size;
        memory.size = os_memory_align_to_granularity (requested_size);
        memory.ptr  = os_memory_reserve              (NULL, memory.size);

        bool is_valid = true; 
        is_valid &= (requested_size != 0);
        is_valid &= (memory.addr    != 0);
        is_valid &= (memory.size    != 0);
        is_valid &= (memory.size    >= requested_size);
        assert(is_valid);
    }

    SLD_API_INLINE void
    memory_os_commit(
        memory_t& memory) {

        const u64 requested_size = memory.size;
        void*     requested_ptr  = memory.ptr;  
        memory.size = os_memory_align_to_page (requested_size);
        memory.ptr  = os_memory_commit        (requested_ptr, memory.size); 
    
        bool is_valid = true;
        is_valid &= (requested_size != 0);
        is_valid &= (requested_ptr  != NULL);
        is_valid &= (memory.addr    != 0);
        is_valid &= (memory.size    != 0);
        is_valid &= (memory.ptr     == requested_ptr);
        is_valid &= (memory.size    >= requested_size);
        assert(is_valid);
    }

    SLD_API_INLINE void
    memory_zero(memory_t& memory) {

        assert(memory.addr != 0 && memory.size != 0);
        (void)memset(memory.ptr, 0, memory.size);
    }

    SLD_API_INLINE_TEMPLATE void
    memory_zero_struct(type* ptr_struct) {

        (void)memset((void*)ptr_struct, 0, sizeof(t));
    }

    SLD_API_INLINE bool
    memory_is_committed(
        const memory_t& memory) {

        memory_assert_valid(memory);
        const bool is_committed = os_memory_is_committed(memory.ptr);
        return(is_committed);
    }

    SLD_API_INLINE bool
    memory_is_os_reserved(
        const memory_t& memory) {

        memory_assert_valid(memory);
        const bool is_reserved = os_memory_is_reserved(memory.ptr);
        return(is_reserved);
    }
    
    SLD_API_INLINE memory_arena_t*
    memory_to_arena(
        const memory_t& memory) {

        memory_assert_valid(memory);

        memory_arena_t* arena = (memory_arena_t*)memory.ptr;
        return(arena);
    }

    SLD_API_INLINE bool
    memory_arena_is_valid(
        const memory_arena_t* arena) {

        bool is_valid = (arena != NULL);
        if (is_valid) {

            is_valid &= os_memory_is_committed((void*)arena);
            is_valid &= (arena->size     != 0); 
            is_valid &= (arena->position != 0); 
            is_valid &= (arena->save     <= arena->position); 
        }
        return(is_valid);
    }

    SLD_API_INLINE void
    memory_arena_assert_valid(
        const memory_arena_t* arena) {

        assert(memory_arena_is_valid(arena));
    }

    SLD_API_INLINE void*
    memory_arena_get_header(
        const memory_arena_t* arena) {

        memory_arena_assert_valid(arena);
        void* arena_header = (void*)arena;
        return(arena_header);
    }

    SLD_API_INLINE memory_t
    memory_arena_to_memory(
        const memory_arena_t* arena) {

        memory_arena_assert_valid(arena);

        memory_t arena_memory;
        arena_memory.ptr  = (void*)arena;
        arena_memory.size = arena->size + sizeof(memory_arena_t);
        return(arena_memory);
    }

    SLD_API_INLINE void*
    memory_arena_get_start(
        const memory_arena_t* arena) {

        memory_arena_assert_valid(arena);
        constexpr u32 size_arena_header = sizeof(memory_arena_t);
        void*         arena_start       = memory_add_offset((void*)arena, size_arena_header); 
        return(arena_start);
    }

    SLD_API_INLINE void*
    memory_arena_get_position(
        const memory_arena_t* arena) {

        memory_arena_assert_valid(arena);
        const void* arena_start    = memory_arena_get_start(arena);
        void*       arena_position = memory_add_offset(arena_start, arena->position);
        return(arena_position);
    }

    SLD_API_INLINE void
    memory_arena_reset(
        memory_arena_t* arena) {

        memory_arena_assert_valid(arena);
        arena->position = 0;
        arena->save     = 0;
    }

    SLD_API_INLINE void
    memory_arena_save(
        memory_arena_t* arena) {

        memory_arena_assert_valid(arena);
        arena->save = arena->position;
    }

    SLD_API_INLINE void
    memory_arena_roll_back(
        memory_arena_t* arena) {

        memory_arena_assert_valid(arena);
        arena->position = arena->save;
    }

    SLD_API_INLINE void*
    memory_arena_push_bytes(
        memory_arena_t* arena,
        const u32       size,
        const u32       alignment) {

        memory_arena_assert_valid(arena);

        const u32 alignment_pow_2 = size_round_up_pow2 (alignment);
        const u32 size_aligned    = size_align_pow_2   (size, alignment);
        const u32 new_position    = arena->position + size_aligned;
        
        void* ptr = NULL;
        if (new_position <= arena->position) {

            ptr = memory_arena_get_position(arena);
            assert(ptr);
            
            arena->position = new_position;
        } 
        return(ptr);
    }
    
    SLD_API_INLINE_TEMPLATE type*
    memory_arena_push_struct(
        memory_arena_t* arena) {

        memory_arena_assert_valid(arena);

        const u32 struct_size      = sizeof  (type);
        const u32 struct_alignment = alignof (type);
         
        type* ptr = (type*)memory_arena_push_bytes(
            arena,
            struct_size,
            struct_alignment);

        return(ptr);
    }


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
};

#endif //SLD_MEMORY_HPP
#ifndef SLD_MEMORY_HPP
#define SLD_MEMORY_HPP

#include "sld.hpp"
#include "sld-os.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // MEMORY API
    //-------------------------------------------------------------------

    struct memory_t {
        union {
            addr  addr;
            void* ptr;
            byte* bytes;
        };
        u64 size;
    };

    SLD_API_INLINE          bool     memory_is_valid           (const memory_t& memory);
    SLD_API_INLINE          void     memory_assert_valid       (const memory_t& memory);
    SLD_API_INLINE          memory_t memory_add_offset         (const memory_t& memory, const u32 offset);
    SLD_API_INLINE          bool     memory_is_os_committed    (const memory_t& memory);
    SLD_API_INLINE          bool     memory_is_os_reserved     (const memory_t& memory);
    SLD_API_INLINE          void     memory_os_reserve         (memory_t& memory);
    SLD_API_INLINE          void     memory_os_release         (memory_t& memory);
    SLD_API_INLINE          void     memory_os_commit          (memory_t& memory);
    SLD_API_INLINE          void     memory_os_decommit        (memory_t& memory);
    SLD_API_INLINE          u32      memory_copy               (memory_t& memory_dst, const memory_t& memory_src);        
    SLD_API_INLINE          void     memory_zero               (memory_t& memory);
    SLD_API_INLINE_TEMPLATE void     memory_zero_struct        (type*     ptr_type);

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE bool
    memory_is_valid(
        const memory_t& memory) {

        const bool is_valid = (memory.addr != 0 && memory.size != 0);
        return(is_valid);
    }

    SLD_API_INLINE void
    memory_assert_valid(
        const memory_t& memory) {

        assert(memory_is_valid(memory));
    }

    SLD_API_INLINE memory_t
    memory_add_offset(
        const memory_t& memory,
        const u32       offset) {

        memory_t offset_memory;
        offset_memory.addr = memory.addr + offset;
        offset_memory.size = memory.size - offset;
        return(offset_memory);
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
    memory_os_release(
        memory_t& memory) {

        assert(memory.ptr != NULL && memory.size != 0);
        const bool did_release = os_memory_release(memory.ptr, memory.size);
        assert(did_release);
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

    SLD_API_INLINE u32 
    memory_copy(
        memory_t&       memory_dst,
        const memory_t& memory_src) {

        memory_assert_valid(memory_dst);
        memory_assert_valid(memory_src);

        const u32 copy_length = (memory_dst.size >= memory_src.size)
            ? memory_src.size
            : memory_dst.size;

        const errno_t error = memmove_s(
            memory_dst.ptr, memory_dst.size,
            memory_src.ptr, memory_src.size
            );

        assert(error == 0);

        return(copy_length);
    }

    SLD_API_INLINE void
    memory_zero(memory_t& memory) {

        assert(memory.addr != 0 && memory.size != 0);
        (void)memset(memory.ptr, 0, memory.size);
    }

    SLD_API_INLINE_TEMPLATE void
    memory_zero_struct(type* ptr_struct) {

        (void)memset((void*)ptr_struct, 0, sizeof(type));
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
#ifndef SLD_BUFFER_HPP
#define SLD_BUFFER_HPP

#include "sld.hpp"
#include "sld-memory.hpp"


namespace sld {


    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    struct buffer_t {
        byte* data;
        u32   size;
        u32   length;
    };

    SLD_API_INLINE buffer_t* buffer_init_from_arena (memory_arena_t* arena, const u32 size);
    SLD_API_INLINE bool      buffer_is_valid        (buffer_t* buffer);
    SLD_API_INLINE void      buffer_assert_valid    (buffer_t* buffer);
    SLD_API_INLINE void      buffer_reset           (buffer_t* buffer);
    SLD_API_INLINE void      buffer_zero            (buffer_t* buffer);
    SLD_API_INLINE byte_t*   buffer_index           (buffer_t* buffer, const u32   index);
    SLD_API_INLINE u32       buffer_append          (buffer_t* buffer, const byte* src_data, const u32 src_length);
    SLD_API_INLINE u32       buffer_copy            (buffer_t* buffer, const byte* src_data, const u32 src_length);  

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    inline buffer_t*
    buffer_init_from_memory(
        const void* memory,
        const u32   size) {

        bool can_init = true;
        can_init &= (memory != NULL);
        can_init &= (size   != 0);
        assert(can_init);

        buffer_t* buffer = (buffer_t*)memory;

        constexpr u32 struct_size = sizeof(buffer_t);
        
        buffer->data   = (byte*)(((addr)memory) + struct_size);
        buffer->size   = (size - struct_size);
        buffer->length = 0;
        buffer->assert_valid();

        return(buffer);
    }

    SLD_BUFFER_IMPL_CONSTEXPR
    is_valid(
        void) -> bool {

        bool is_valid = true;
        is_valid &= (data   != NULL);
        is_valid &= (size   != 0);
        is_valid &= (length <= size);
        return(is_valid);
    }   

    SLD_BUFFER_IMPL_CONSTEXPR
    assert_valid(void) -> void {
        assert(this->is_valid());
    }

    SLD_BUFFER_IMPL_CONSTEXPR
    reset(void) -> void {

        assert_valid();
        length = 0;
    }

    SLD_BUFFER_IMPL_INLINE
    zero(void) -> void {

        assert_valid();
        memset(data, 0, size);
        length = 0;
    }

    SLD_BUFFER_IMPL_INLINE    
    append(
        const byte* src_data,
        const u32   src_length) -> u32 {

        assert_valid();

        u32 bytes_appended = 0;

        bool can_append = true;
        can_append &= (src_data   != NULL);
        can_append &= (src_length != NULL);
        if (!can_append) return(bytes_appended);


        for (
            bytes_appended = 0;
            (
                bytes_appended < src_length &&
                length         < size
            );
            ++bytes_appended) {

            data[length] = src_data[bytes_appended];
            ++length;
        }

        return(bytes_appended);
    }

    SLD_BUFFER_IMPL_INLINE  
    copy(
        const byte* src_data,
        const u32   src_length) -> u32 {

        assert_valid();

        bool result = true;
        result &= (src_data   != NULL);
        result &= (src_length != 0);
        if (!result) return(0);

        const u32 bytes_to_copy = (src_length < size)
            ? src_length
            : size;

        for (
            length = 0;
            length < bytes_to_copy;
            ++length) {

            data[length] = src_data[length];
        }

        return(length);
    }
};

#endif //SLD_BUFFER_HPP
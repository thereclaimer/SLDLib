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

    SLD_API_INLINE bool     buffer_is_valid        (buffer_t* buffer);
    SLD_API_INLINE void     buffer_assert_valid    (buffer_t* buffer);
    SLD_API_INLINE void     buffer_reset           (buffer_t* buffer);
    SLD_API_INLINE void     buffer_zero            (buffer_t* buffer);
    SLD_API_INLINE u32      buffer_append          (buffer_t* buffer, const byte* src_data, const u32 src_length);
    SLD_API_INLINE u32      buffer_copy            (buffer_t* buffer, const byte* src_data, const u32 src_length);  

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE buffer_t*
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
        buffer_assert_valid(buffer);

        return(buffer);
    }

    SLD_API_INLINE bool
    buffer_is_valid(
        const buffer_t* buffer) {

        bool is_valid = (buffer != NULL);
        if (is_valid) {
            is_valid &= (buffer->data   != NULL);
            is_valid &= (buffer->size   != 0);
            is_valid &= (buffer->length <= buffer->size);
        }
        return(is_valid);
    }   

    SLD_API_INLINE void
    buffer_assert_valid(
        const buffer_t* buffer) {
        assert(buffer_is_valid(buffer));
    }


    SLD_API_INLINE void
    buffer_reset(
        buffer_t* buffer) {

        buffer_assert_valid(buffer);
        buffer->length = 0;
    }

    SLD_API_INLINE void
    buffer_zero(
        buffer_t* buffer) {

        buffer_assert_valid(buffer);
        memset(buffer->data, 0, buffer->size);
        buffer->length = 0;
    }

    SLD_API_INLINE u32    
    buffer_append(
        buffer_t*   buffer,
        const byte* src_data,
        const u32   src_length) {

        buffer_assert_valid(buffer);

        u32 bytes_appended = 0;

        bool can_append = true;
        can_append &= (src_data   != NULL);
        can_append &= (src_length != NULL);
        if (!can_append) return(bytes_appended);


        for (
            bytes_appended = 0;
            (
                bytes_appended < src_length &&
                buffer->length < buffer->size
            );
            ++bytes_appended) {

            buffer->data[buffer->length] = src_data[bytes_appended];
            ++buffer->length;
        }

        return(bytes_appended);
    }

    SLD_API_INLINE u32  
    buffer_copy(
        buffer_t*   buffer,
        const byte* src_data,
        const u32   src_length) {

        buffer_assert_valid(buffer);

        bool result = true;
        result &= (src_data   != NULL);
        result &= (src_length != 0);
        if (!result) return(0);

        const u32 bytes_to_copy = (src_length < buffer->size)
            ? src_length
            : buffer->size;

        for (
            buffer->length = 0;
            buffer->length < bytes_to_copy;
            ++buffer->length) {

            buffer->data[buffer->length] = src_data[buffer->length];
        }

        return(buffer->length);
    }
};

#endif //SLD_BUFFER_HPP
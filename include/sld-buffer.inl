#ifndef SLD_BUFFER_INL
#define SLD_BUFFER_INL

#include "sld.hpp"

#define SLD_BUFFER_IMPL_INLINE    SLD_COLLECTION_IMPL_INLINE    (buffer_t)
#define SLD_BUFFER_IMPL_STATIC    SLD_COLLECTION_IMPL_STATIC    (buffer_t)
#define SLD_BUFFER_IMPL_CONSTEXPR SLD_COLLECTION_IMPL_CONSTEXPR (buffer_t)

namespace sld {

    SLD_BUFFER_IMPL_STATIC
    init_from_memory(
        const void* memory,
        const u32   size) -> void {

        bool can_init = true;
        can_init &= (memory != NULL);
        can_init &= (size   != 0);
        assert(can_init);

        buffer_t<t>* buffer = (buffer_t<t>*)memory;

        constexpr u32 struct_size = sizeof(buffer_t<t>);
        
        buffer->data   = (t*)(((addr)memory) + struct_size);
        buffer->size   = (size - struct_size);
        buffer->length = 0;
        buffer->assert_valid();

        return(buffer);
    }

    SLD_BUFFER_IMPL_INLINE
    init_from_data(
        const t*  data,
        const u32 size,
        const u32 length) -> void {

        bool can_init = true;
        can_init &= (data != NULL);
        can_init &= (size   != 0);
        can_init &= (length <= size);
        assert(can_init);

        buffer->data   = data;
        buffer->size   = size;
        buffer->length = length
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
    assert_valid(
        void) -> void {

    }

    SLD_BUFFER_IMPL_CONSTEXPR
    reset(
        void) -> void {

        assert_valid();
        length = 0;
    }

    SLD_BUFFER_IMPL_CONSTEXPR
    zero(
        void) -> void {

        assert_valid();
        memset(data, 0, size);
        length = 0;
    }

    SLD_BUFFER_IMPL_INLINE    
    append(
        const t*  src_data,
        const u32 src_length) -> u32 {

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
        const t*  src_data,
        const u32 src_length) -> u32 {

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

#undef SLD_BUFFER_IMPL_INLINE
#undef SLD_BUFFER_IMPL_STATIC
#undef SLD_BUFFER_IMPL_CONSTEXPR

#endif //SLD_BUFFER_INL
#ifndef SLD_STRING_HPP
#define SLD_STRING_HPP

#include "sld.hpp"

#define SLD_ARRAY_LIST_IMPL_INLINE template<typename t> inline auto str_t<t>::

namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    template<typename t> struct str_t;
    using cstr_t = str_t<cchar>; // STRING 8  | C-STRING | ASCII   | UTF-8
    using wstr_t = str_t<wchar>; // STRING 16 | W-STRING | UNICODE | UTF-16

    //-------------------------------------------------------------------
    // STRING
    //-------------------------------------------------------------------

    template<typename t>
    struct str_t {

        t*  chars;
        u32 size;

        SLD_API inline bool is_valid     (void)                const;
        SLD_API inline bool is_empty     (void)                const;
        SLD_API inline void assert_valid (void)                const;
        SLD_API inline u32  length       (void)                const;
        SLD_API inline u32  append       (const str_t<t>* src);
        SLD_API inline u32  copy_to      (str_t<t>*       dst) const;
        SLD_API inline u32  copy_from    (const str_t<t>* src);
        SLD_API inline u32  copy_to      (t*              dst_chars, const u32 dst_size) const;
        SLD_API inline u32  copy_from    (const t*        src_chars, const u32 src_size);
        SLD_API inline void reset        (void);
        SLD_API inline void zero         (void);
    };

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    SLD_ARRAY_LIST_IMPL_INLINE    
    is_valid(void) const -> bool {

        bool result = true;
        result &= (size  != 0);
        result &= (chars != NULL);
        return(result);
    }

    SLD_ARRAY_LIST_IMPL_INLINE    
    assert_valid(void) const -> void {
        assert(is_valid());
    }

    SLD_ARRAY_LIST_IMPL_INLINE
    is_empty(void) const -> bool {

        assert_valid();
        return(chars[0] == 0);
    }

    SLD_ARRAY_LIST_IMPL_INLINE
    length(void) const -> u32 {

        assert_valid();

        u32 length = 0;

        for (
            length;
            length < string.size;
            ++length) {

            if (string.chars[length] == 0) break;
        }

        return(length);
    }

    SLD_ARRAY_LIST_IMPL_INLINE
    copy_to(str_t<t>* dst) const -> u32 {

        this->assert_valid();
        assert(dst != NULL && dst->assert_valid());

        u32 src_size_copied = 0;

        // calculate how much we can copy
        const u32 src_size_to_copy = (this->size <  dst->size)
            ? this->size
            : dst->size;

        // copy till we hit the size we can copy
        // or until we hit a null terminator
        for (
            u32& char_index = src_size_copied;
                 char_index < src_size_to_copy;
                ++char_index) {

            const cchar src_char = this->chars[char_index];
            dst->chars[char_index] = src_char;
            if (src_char == 0) break;
        }

        // ensure the string is terminated
        dst->chars[dst->size - 1] = 0;
        return(src_size_copied);

    }

    SLD_ARRAY_LIST_IMPL_INLINE
    copy_to(
        t*        dst_chars,
        const u32 dst_size) const -> u32 {

        bool is_valid = true;
        is_valid &= this->is_valid();
        is_valid &= (dst_chars != NULL);
        is_valid &= (dst_size  != 0);
        assert(is_valid);

        u32 src_size_copied = 0;

        // calculate how much we can copy
        const u32 src_size_to_copy = (this->size <  dst_size)
            ? this->size
            : dst_size;

        // copy till we hit the size we can copy
        // or until we hit a null terminator
        for (
            u32& char_index = src_size_copied;
                 char_index < src_size_to_copy;
                ++char_index) {

            const cchar src_char = this->chars[char_index];
            dst_chars[char_index] = src_char;
            if (src_char == 0) break;
        }

        // ensure the string is terminated
        dst_chars[dst_size - 1] = 0;
        return(src_size_copied);
    }

    SLD_ARRAY_LIST_IMPL_INLINE
    copy_from(const str_t<t>* src) -> u32  {

        bool is_valid = true;
        is_valid &= this->is_valid();
        is_valid &= (src != NULL && src->is_valid());
        assert(is_valid);

        u32 src_size_copied = 0;

        // calculate how much we can copy
        const u32 src_size_to_copy = (src->size <  this->size)
            ? src->size
            : this->size;

        // copy till we hit the size we can copy
        // or until we hit a null terminator
        for (
            u32& char_index = src_size_copied;
                 char_index < src_size_to_copy;
                ++char_index) {

            const cchar src_char = src->chars[char_index];
            this->chars[char_index] = src_char;
            if (src_char == 0) break;
        }

        // ensure the string is terminated
        this->chars[this->size - 1] = 0;
        return(src_size_copied);
    }

    SLD_ARRAY_LIST_IMPL_INLINE
    copy_from(
        const t*  src_chars,
        const u32 src_size) -> u32 {
        
        bool is_valid = true;
        is_valid &= this->is_valid();
        is_valid &= (src_chars != NULL);
        is_valid &= (src_size  != 0);
        assert(is_valid);

        u32 src_size_copied = 0;

        // calculate how much we can copy
        const u32 src_size_to_copy = (src_size <  this->size)
            ? src_size
            : this->size;

        // copy till we hit the size we can copy
        // or until we hit a null terminator
        for (
            u32& char_index = src_size_copied;
                 char_index < src_size_to_copy;
                ++char_index) {

            const cchar src_char = src_chars[char_index];
            this->chars[char_index] = src_char;
            if (src_char == 0) break;
        }

        // ensure the string is terminated
        this->chars[this->size - 1] = 0;
        return(src_size_copied);
    }

    SLD_ARRAY_LIST_IMPL_INLINE
    append(const str_t<t>* src) -> u32  {

        bool is_valid = true; 
        is_valid &= (this->is_valid());
        is_valid &= (src != NULL && src->is_valid());
        assert(is_valid);

        u32 bytes_appended = 0;

        // get the length of the destination string
        u32 dst_length = this->length();

        // calculate how many bytes we can append at max
        // and where we are starting to append to
        // based on if/where the null terminator exists
        const u32 dst_length      = (this->size - str_dst_length);
        const u32 bytes_to_append = (dst_length < this->size) ? dst_length : this->size;   
        const u32 dst_start       = (this->chars[dst_length - 1] == 0)  ? (dst_length - 1) : dst_length;

        // append until we hit a null terminator in the source
        // or we hit max length
        for (
            u32& src_index = bytes_appended;
                 src_index < str_src.size;
               ++src_index) {

            const cchar src_char  = str_src.chars[src_index];
            const u32   dst_index = dst_start + src_index;

            this->chars[dst_index] = src_char;
            if (src_char == 0) break;
        }

        this->chars[this->size - 1] = 0;
        return(bytes_appended);
    }

    SLD_ARRAY_LIST_IMPL_INLINE
    reset(void) -> void {

        assert_valid();

        chars[0]        = 0;
        chars[size - 1] = 0;

    }

    SLD_ARRAY_LIST_IMPL_INLINE
    zero(void) -> void {

        assert_valid();

        for (
            u32 index = 0;
            index < string.size;
            ++index) {

            chars[index] = 0;
        }

        return(result);
    }
};

#endif  //SLD_STRING_HPP
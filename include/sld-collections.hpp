#ifndef SLD_COLLECTIONS_HPP
#define SLD_COLLECTIONS_HPP

#include "sld.hpp"

#define SLD_COLLECTION_IMPL_INLINE(collection)    template<typename t> inline           auto collection<t>::
#define SLD_COLLECTION_IMPL_STATIC(collection)    template<typename t> inline static    auto collection<t>::
#define SLD_COLLECTION_IMPL_CONSTEXPR(collection) template<typename t> inline constexpr auto collection<t>::


namespace sld {

    //-------------------------------------------------------------------
    // TYPES
    //-------------------------------------------------------------------

    template<typename t> struct array_list_t;
    template<typename t> struct buffer_t_;

    //-------------------------------------------------------------------
    // BUFFER
    //-------------------------------------------------------------------

    template<typename t>
    struct buffer_t_ {

        t*  data;
        u32 size;
        u32 length;

        inline static void    init_from_memory (const void* memory, const u32 size);
        inline void           init_from_buffer (const t*    buffer, const u32 size);
        inline constexpr bool is_valid         (void);
        inline constexpr void assert_valid     (void);
        inline constexpr void reset            (void);
        inline constexpr void zero             (void);
        inline t*             index            (const u32   index);
        inline u32            append           (const byte* src_data, const u32 src_length);
        inline u32            copy             (const byte* src_data, const u32 src_length);        
    };

    using buffer_byte_t = buffer_t_<byte>;

    //-------------------------------------------------------------------
    // ARRAY LIST
    //-------------------------------------------------------------------

    constexpr u32 ARRAY_LIST_INDEX_INVALID = 0xFFFFFFFF;

    template<typename t>
    struct array_list_t {

        t*   start;
        u32  capacity;
        u32  count;

        inline static array_list_t<t>* init_from_memory  (const void* memory, const u32 size);
        inline bool                    init_from_array   (t* const    array,  const u32 count);
        inline u32                     get_index_of      (const t*    element);
        inline bool                    add_element       (const t*    element);
        inline bool                    insert_at         (const t*    element, const u32 index);
        inline bool                    remove_at         (const u32   index);
        inline constexpr void          assert_valid      (void);
        inline constexpr bool          is_valid          (void);
        inline constexpr bool          is_empty          (void); 
        inline constexpr bool          is_full           (void);
        inline constexpr bool          is_not_full       (void);
        inline constexpr t*            get_first_element (void);
        inline constexpr t*            get_last_element  (void);
        inline constexpr t*            get_element_at    (const u32 index);
        inline constexpr t*            get_next_element  (const t*  current);
    };

    using array_list_u8_t    = array_list_t<u8>;
    using array_list_u16_t   = array_list_t<u16>;
    using array_list_u32_t   = array_list_t<u32>;
    using array_list_u64_t   = array_list_t<u64>;
    using array_list_s8_t    = array_list_t<s8>;
    using array_list_s16_t   = array_list_t<s16>;
    using array_list_s32_t   = array_list_t<s32>;
    using array_list_s64_t   = array_list_t<s64>;
    using array_list_cchar_t = array_list_t<cchar>;
    using array_list_wchar_t = array_list_t<wchar>;
    using array_list_f32_t   = array_list_t<f32>;
    using array_list_f64_t   = array_list_t<f64>;
};

#include "sld-array-list.inl"


#endif  //SLD_COLLECTIONS_HPP
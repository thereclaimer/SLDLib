#ifndef SLD_STACK_HPP
#define SLD_STACK_HPP

#include "sld.hpp"

#define SLD_DATA_STACK_IMPL_INLINE    inline           auto stack_t::
#define SLD_DATA_STACK_IMPL_STATIC    inline static    auto
#define SLD_DATA_STACK_IMPL_CONSTEXPR inline constexpr auto stack_t::


namespace sld {

    //-------------------------------------------------------------------
    // STACK
    //-------------------------------------------------------------------

    constexpr u32 STACK_DEFAULT_ALIGNMENT = 4;

    struct stack_t {
        
        byte* array;
        u32   capacity;
        u32   position;
        u32   save;
    
        SLD_API inline           void  init           (byte* array, const u32 capacity);
        SLD_API inline constexpr bool  is_valid       (void) const;
        SLD_API inline constexpr void  assert_valid   (void) const;
        SLD_API inline constexpr void  reset          (void);
        SLD_API inline constexpr void  reset_to_save  (void);
        SLD_API inline constexpr void  save_position  (void);
        SLD_API inline           byte* push_bytes     (const u32 size, const u32 alignment = STACK_DEFAULT_ALIGNMENT);
        SLD_API inline           bool  pull_bytes     (const u32 size, const u32 alignment = STACK_DEFAULT_ALIGNMENT);

        template<typename t> SLD_API inline t*   push_struct (const u32 count = 1);
        template<typename t> SLD_API inline bool pull_struct (const u32 count = 1);
    };

    SLD_API inline static stack_t* stack_init_from_memory(const void* memory, const u32 size);

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    SLD_DATA_STACK_IMPL_STATIC
    stack_init_from_memory(
        const void* memory,
        const u32   size) ->  stack_t* 
        {

        constexpr u32 struct_size = sizeof(stack_t); 
        stack_t* stack = (stack_t*)memory;



    }



    SLD_DATA_STACK_IMPL_CONSTRUCTOR
    stack_t(
        byte*     stack_data,
        const u32 stack_size) {

        array    = stack_data;
        capacity = stack_size;
        position = 0;
        save     = 0;
    }

    SLD_DATA_STACK_IMPL_INLINE
    init_from_data(
        byte*     stack_data,
        const u32 stack_size) -> void {

        array    = stack_data;
        capacity = stack_size;
        position = 0;
        save     = 0;
    }

    SLD_DATA_STACK_IMPL_CONSTEXPR
    is_valid(void) const -> bool { 

        bool is_valid = true;
        is_valid &= (array    != NULL); 
        is_valid &= (capacity != 0); 
        is_valid &= (position <  capacity); 
        is_valid &= (save     <= position); 
        return(is_valid);
    }

    SLD_DATA_STACK_IMPL_CONSTEXPR
    assert_valid(void) const -> void {
        assert(this->is_valid());
    }

    SLD_DATA_STACK_IMPL_CONSTEXPR 
    reset (void) -> void {

        this->assert_valid();
        position = 0;
        save     = 0;
    }

    SLD_DATA_STACK_IMPL_CONSTEXPR
    reset_to_save(void) -> void {
        
        this->assert_valid();

        position = save;
    }

    SLD_DATA_STACK_IMPL_CONSTEXPR
    save_position(void) -> void {
        
        this->assert_valid();
    
        save = position;
    }

    SLD_DATA_STACK_IMPL_INLINE
    push_bytes(
        const u32 size,
        const u32 alignment) -> byte* {
        
        this->assert_valid();

        const u32 align_pow_2  = size_round_up_pow2 (alignment);
        const u32 size_aligned = size_align_pow_2   (size, align_pow_2); 
        const u32 newposition = position + size_aligned;
        
        bool can_push = (newposition <= capacity);
        if (!can_push) return(NULL);

        byte* ptr = &array[position];
        position = newposition;

        return(ptr);
    }

    SLD_DATA_STACK_IMPL_INLINE
    pull_bytes(
        const u32 size,
        const u32 alignment) -> bool {

        assert_valid();

        const u32 align_pow_2  = size_round_up_pow2 (alignment);
        const u32 size_aligned = size_align_pow_2   (size, align_pow_2); 

        bool can_pull = (size_aligned <= position);
        if (can_pull) {

            position -= size_aligned;
            if (save > position) {
                save = 0;
            }
        }
        
        return(can_pull);
    }


    template<typename t>
    SLD_API inline t* push_struct(const u32 count) {
        
        this->assert_valid();

        const u32 size_struct = sizeof(t);
        const u32 size_push   = size_struct * count;

        // push bytes
        addr array_start  = (addr)this->push_bytes(push_size);
        u32  array_offset = 0;
        if (array_start != 0) {

            for (
                u32 index = 0;
                index < count;
                ++index) {

                // initialize the struct at this location
                void* struct_memory   = (void*)(array_start + array_offset);
                t*    struct_instance = new (struct_memory) t();
                assert(struct_instance);

                // update the offset
                array_offset += size_struct;
            }
        }
        return((t*)array_start);
    }

   
};

#undef SLD_STACK_IMPL_INLINE
#undef SLD_STACK_IMPL_STATIC
#undef SLD_STACK_IMPL_CONSTEXPR

#endif //SLD_STACK_HPP
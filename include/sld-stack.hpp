#ifndef SLD_STACK_HPP
#define SLD_STACK_HPP

#include "sld.hpp"

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
    };

    SLD_API_INLINE stack_t* stack_init_from_memory (const void* memory, const u32 size);
    SLD_API_INLINE void     stack_init_from_array  (stack_t* stack, byte* array, const u32 capacity);
    SLD_API_INLINE byte*    stack_push_bytes       (stack_t* stack, const u32 size, const u32 alignment = STACK_DEFAULT_ALIGNMENT);
    SLD_API_INLINE bool     stack_pull_bytes       (stack_t* stack, const u32 size, const u32 alignment = STACK_DEFAULT_ALIGNMENT);
    SLD_API_INLINE bool     stack_is_valid         (const stack_t* stack);
    SLD_API_INLINE void     stack_assert_valid     (const stack_t* stack);
    SLD_API_INLINE void     stack_reset            (const stack_t* stack);
    SLD_API_INLINE void     stack_reset_to_save    (const stack_t* stack);
    SLD_API_INLINE void     stack_save_position    (const stack_t* stack);

    template<typename t> SLD_API_INLINE t*   stack_push_struct (stack_t* stack, const u32 count = 1);
    template<typename t> SLD_API_INLINE bool stack_pull_struct (stack_t* stack, const u32 count = 1);

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE stack_t*
    stack_init_from_memory(
        const void* memory,
        const u32   size) {

        constexpr u32 struct_size = sizeof(stack_t); 
        
        bool can_init = true;
        can_init &= (memory != NULL);
        can_init &= (size   != 0);
        can_init &= (size   >  struct_size);

        stack_t* stack  = (stack_t*)memory;
        stack->array    = (byte*)((addr)memory + struct_size);
        stack->capacity = size - struct_size;
        stack->position = 0;
        stack->save     = 0;

        return(stack);
    }

    SLD_API_INLINE void
    stack_init_from_array(
        stack_t*  stack,
        byte*     array,
        const u32 capacity) {

        if (stack != NULL) {
            stack->array    = array;
            stack->capacity = capacity;
            stack->position = 0;
            stack->save     = 0;
        }
        stack_assert_valid(stack);
    }

    SLD_API_INLINE bool
    stack_is_valid(const stack_t* stack)  { 

        bool test = true;

        bool is_valid = (stack != NULL);
        if (is_valid) {
            is_valid &= (stack->array    != NULL); 
            is_valid &= (stack->capacity != 0); 
            is_valid &= (stack->position <  stack->capacity); 
            is_valid &= (stack->save     <= stack->position); 
        }
        return(is_valid);
    }

    SLD_API_INLINE void
    stack_assert_valid(const stack_t* stack) {
        assert(stack_is_valid(stack));
    }

    SLD_API_INLINE void 
    stack_reset (stack_t* stack) {

        stack_assert_valid(stack);
        stack->position = 0;
        stack->save     = 0;
    }

    SLD_API_INLINE void
    stack_reset_to_save(stack_t* stack) {
        
        stack_assert_valid(stack);
        stack->position = stack->save;
    }

    SLD_API_INLINE void
    save_position(stack_t* stack) {
        
        stack_assert_valid(stack);
    
        stack->save = stack->position;
    }

    SLD_API_INLINE byte*
    stack_push_bytes(
        stack_t*  stack,
        const u32 size,
        const u32 alignment) {
        
        
        const u32 align_pow_2  = size_round_up_pow2 (alignment);
        const u32 size_aligned = size_align_pow_2   (size, align_pow_2); 
        const u32 new_position = stack->position + size_aligned;
        
        bool can_push = (new_position <= stack->capacity);
        if (!can_push) return(NULL);

        byte* ptr = &stack->array[stack->position];
        stack->position = new_position;

        return(ptr);
    }

    SLD_API_INLINE bool
    stack_pull_bytes(
        stack_t*  stack,
        const u32 size,
        const u32 alignment) {

        stack_assert_valid(stack);

        const u32 align_pow_2  = size_round_up_pow2 (alignment);
        const u32 size_aligned = size_align_pow_2   (size, align_pow_2); 

        bool can_pull = (size_aligned <= stack->position);
        if (can_pull) {

            stack->position -= size_aligned;
            if (stack->save > stack->position) {
                stack->save = 0;
            }
        }
        
        return(can_pull);
    }

    template<typename t>
    SLD_API_INLINE t*
    stack_push_struct(
        stack_t*  stack,
        const u32 count) {
        
        this->assert_valid();

        const u32 size_struct = sizeof(t);
        const u32 size_push   = size_struct * count;

        // push bytes
        addr array_start  = (addr)stack_push_bytes(stack, push_size);
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
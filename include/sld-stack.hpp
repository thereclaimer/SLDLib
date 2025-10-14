#ifndef SLD_STACK_HPP
#define SLD_STACK_HPP

#include "sld.hpp"
#include "sld-arena.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // STACK
    //-------------------------------------------------------------------

    struct stack_t {
        byte* array;
        u64   capacity;
        u64   position;
        u64   save;
    };

    constexpr u64 STACK_DEFAULT_ALIGNMENT = 4;
    constexpr u64 STACK_HEADER_SIZE       = sizeof(stack_t);

    SLD_API_INLINE          stack_t* stack_init_from_memory (memory_t& memory);
    SLD_API_INLINE          stack_t* stack_init_from_arena  (arena_t*  arena, const u64 capacity);
    SLD_API_INLINE          void     stack_init_from_array  (stack_t*  stack, const u64 capacity, byte* array);
    SLD_API_INLINE          byte*    stack_push_bytes       (stack_t*  stack, const u64 size, const u64 alignment = STACK_DEFAULT_ALIGNMENT);
    SLD_API_INLINE          bool     stack_pull_bytes       (stack_t*  stack, const u64 size, const u64 alignment = STACK_DEFAULT_ALIGNMENT);
    SLD_API_INLINE          bool     stack_is_valid         (const stack_t* stack);
    SLD_API_INLINE          void     stack_assert_valid     (const stack_t* stack);
    SLD_API_INLINE          void     stack_reset            (const stack_t* stack);
    SLD_API_INLINE          void     stack_reset_to_save    (const stack_t* stack);
    SLD_API_INLINE          void     stack_save_position    (const stack_t* stack);
    SLD_API_INLINE_TEMPLATE type*    stack_push_struct (stack_t* stack, const u64 count = 1);
    SLD_API_INLINE_TEMPLATE bool     stack_pull_struct (stack_t* stack, const u64 count = 1);



    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    SLD_API_INLINE stack_t*
    stack_init_from_memory(
        memory_t& memory) {
        
        bool can_init = true;
        can_init &= memory_is_valid(memory);
        can_init &= (memory.size > STACK_HEADER_SIZE);

        stack_t* stack  = (stack_t*)memory.ptr;
        stack->array    = (byte*)(memory.addr + STACK_HEADER_SIZE);
        stack->capacity = memory.size - STACK_HEADER_SIZE;
        stack->position = 0;
        stack->save     = 0;

        return(stack);
    }

    SLD_API_INLINE void
    stack_init_from_array(
        stack_t*  stack,
        const u64 capacity,
        byte*     array) {

        if (stack != NULL) {
            stack->array    = array;
            stack->capacity = capacity;
            stack->position = 0;
            stack->save     = 0;
        }
        stack_assert_valid(stack);
    }

    SLD_API_INLINE stack_t*
    stack_init_from_arena(
        arena_t*  arena,
        const u64 capacity) {

        bool can_init = true;
        can_init &= arena_is_valid(arena);
        can_init &= (capacity > 0);
        assert(can_init);

        stack_t* stack = (stack_t*)arena_push_bytes(arena, STACK_HEADER_SIZE);
        byte*    array =    (byte*)arena_push_bytes(arena, capacity);

        can_init &= (stack != NULL);
        can_init &= (array != NULL);
        assert(can_init);

        stack->array    = array;
        stack->capacity = capacity;
        stack->position = 0;
        stack->save     = 0;
        stack_assert_valid(stack);

        return(stack);
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
        const u64 size,
        const u64 alignment) {
        
        
        const u64 align_pow_2  = size_round_up_pow2 (alignment);
        const u64 size_aligned = size_align_pow_2   (size, align_pow_2); 
        const u64 new_position = stack->position + size_aligned;
        
        bool can_push = (new_position <= stack->capacity);
        if (!can_push) return(NULL);

        byte* ptr = &stack->array[stack->position];
        stack->position = new_position;

        return(ptr);
    }

    SLD_API_INLINE bool
    stack_pull_bytes(
        stack_t*  stack,
        const u64 size,
        const u64 alignment) {

        stack_assert_valid(stack);

        const u64 align_pow_2  = size_round_up_pow2 (alignment);
        const u64 size_aligned = size_align_pow_2   (size, align_pow_2); 

        bool can_pull = (size_aligned <= stack->position);
        if (can_pull) {

            stack->position -= size_aligned;
            if (stack->save > stack->position) {
                stack->save = 0;
            }
        }
        
        return(can_pull);
    }

    SLD_API_INLINE_TEMPLATE type*
    stack_push_struct(
        stack_t*  stack,
        const u64 count) {
        
        stack_assert_valid(stack);

        const u64 size_struct = sizeof(type);
        const u64 size_push   = size_struct * count;

        // push bytes
        addr array_start  = (addr)stack_push_bytes(stack, size_push);
        u64  array_offset = 0;
        if (array_start != 0) {

            for (
                u64 index = 0;
                index < count;
                ++index) {

                // initialize the struct at this location
                void* struct_memory   = (void*)(array_start + array_offset);
                type* struct_instance = new (struct_memory) type();
                assert(struct_instance);

                // update the offset
                array_offset += size_struct;
            }
        }
        return((type*)array_start);
    }
   
};

#endif //SLD_STACK_HPP
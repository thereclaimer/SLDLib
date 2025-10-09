#ifndef SLD_QUEUE_HPP
#define SLD_QUEUE_HPP

#include "sld.hpp"
#include "sld-memory.hpp"

#define SLD_QUEUE_IMPL_STATIC    template<typename t> inline static    auto
#define SLD_QUEUE_IMPL_INLINE    template<typename t> inline           auto queue_t<t>::
#define SLD_QUEUE_IMPL_CONSTEXPR template<typename t> inline constexpr auto queue_t<t>::

namespace sld {

    //-------------------------------------------------------------------
    // QUEUE API
    //-------------------------------------------------------------------

    constexpr u32 QUEUE_START_HEAD = 0xFFFFFFFF;
    constexpr u32 QUEUE_START_TAIL = 0;
    
    template<typename t>
    class queue_t {

    private:
        t*   array;
        u32  capacity;
        u32  head;
        u32  tail;

    public:
        SLD_API inline bool     enqueue_element     (const t& element);
        SLD_API inline bool     dequeue_element     (t&       element);
        SLD_API inline const t* peek_element        (const u32 index = 0) const;
        SLD_API inline bool     is_valid            (void) const;
        SLD_API inline bool     is_empty            (void) const;
        SLD_API inline void     assert_valid        (void) const;
        SLD_API inline bool     is_full             (void) const;
        SLD_API inline void     reset               (void);
        SLD_API inline u32      get_element_size    (void) const;
        SLD_API inline u32      get_size_total      (void) const;
        SLD_API inline u32      get_size_remaining  (void) const;
        SLD_API inline u32      get_size_used       (void) const;
        SLD_API inline u32      get_count_total     (void) const;
        SLD_API inline u32      get_count_remaining (void) const;
        SLD_API inline u32      get_count_used      (void) const;
    };

    template<typename t> SLD_API inline queue_t<t>* queue_init_from_arena  (arena_t* const arena,  const u32 capacity);
    template<typename t> SLD_API inline queue_t<t>* queue_init_from_memory (void*    const memory, const u32 capacity);

    //-------------------------------------------------------------------
    // STATIC METHODS
    //-------------------------------------------------------------------

    SLD_QUEUE_IMPL_STATIC 
    queue_init_from_arena(
        arena_t* const arena,
        const u32      capacity) -> queue_t<t>* {

        // check args
        bool can_init = (arena != NULL && capacity != 0);
        assert(can_init);

        // allocate memory
        queue_t<t>* queue     = arena->push_struct<queue_t<t>>();
        t*          array     = arena->push_struct<t>(capacity);
        bool        is_mem_ok = (queue != NULL && array != NULL);
        assert(can_init);

        // initialize and return
        queue->array    = array;
        queue->capacity = capacity;
        queue->head     = QUEUE_START_HEAD;
        queue->tail     = QUEUE_START_TAIL;
        queue->assert_valid();
        return(queue); 
    }

    SLD_QUEUE_IMPL_STATIC 
    queue_init_from_memory(
        void* const memory,
        const u32   size) -> queue_t<t>* {

        const u32 size_struct  = sizeof(queue_t<t>);
        const u32 size_element = sizeof(t);
        const u32 size_min     = struct_size + element_size

        // check args        
        bool can_init = (memory != NULL && size > size_min);
        assert(can_init);

        // set pointers
        queue_t<t>* queue = new (memory) queue_t<>();
        t*          array = (t*)(((addr)memory) + struct_size);
        assert(queue != NULL && array != NULL);

        // initialize the queue and return
        queue->array    = array;
        queue->capacity = size / size_element;
        queue->head     = QUEUE_START_HEAD;
        queue->tail     = QUEUE_START_TAIL;
        queue->assert_valid();
        return(queue);
    }

    //-------------------------------------------------------------------
    // INLINE METHODS
    //-------------------------------------------------------------------

    SLD_QUEUE_IMPL_INLINE
    enqueue_element(
        const t& element) -> bool {

        this->assert_valid();
        if (this->is_full()) {
            return(false);
        }

        // copy the data
        const u32 index = (this->start + this->tail);
        this->array[index] = element;

        // update the head and tail
        this->tail = (this->tail + sizeof(t)) % this->size;
        if (this->head == QUEUE_START_HEAD) this->head = 0;
        assert(this->head != this->tail);
        return(true);
    }

    SLD_QUEUE_IMPL_INLINE
    dequeue_element(t& element) -> bool {

        assert_valid();
        const bool can_dequeue = !this->is_empty(); 
        if (can_dequeue) {
            
            element = this->array[this->head];

            const u32 element_size = sizeof(t);
            const u32 new_head     = (queue->head == queue->tail)
                ? (this->tail - element_size)
                : (this->head + element_size) % this->size;
            queue->head = new_head;

            if (this->head == this->tail) {
                this->head = QUEUE_START_HEAD;
                this->tail = QUEUE_START_TAIL;
            }
        }
        return(can_dequeue);
    }

    SLD_QUEUE_IMPL_INLINE    
    peek_element(const u32 index) const -> const t* {

        assert_valid();


        const bool can_peek = (index > this->head);
        if (can_peek) {

        }
        return(can_peek);


        const u32 space_to_index = (space_used  - queue->stride);
        const u32 queue_offset   = (queue->head + space_to_index); 

        void* data = (void*)(queue->start + queue_offset);
        return(data);
    }

    SLD_QUEUE_IMPL_INLINE
    is_valid(void) const -> bool {

        bool is_valid = true;

        is_valid &= (this->array    != 0);
        is_valid &= (this->capacity != 0);
        is_valid &= (this->head     != queue->tail);
        is_valid &= (this->tail     == 0) ? (queue->head == QUEUE_START_HEAD) : (queue->head < queue->tail); 

        // the head should be no greater than the size, or equal to the start
        is_valid &= ((queue->head <= queue->capacity) || queue->head == QUEUE_START_HEAD);

        // if the head is at the start, the tail should be as well
        is_valid &= (queue->head == QUEUE_START_HEAD) ? (queue->tail == QUEUE_START_TAIL) : ( queue->tail<= queue->capacity);

        return(is_valid);
    }

    SLD_QUEUE_IMPL_INLINE
    assert_valid(void) const -> void {
        assert(this->is_valid);
    }

    SLD_QUEUE_IMPL_INLINE
    reset(void) -> void {

        this->assert_valid();
        this->head = QUEUE_START_HEAD;
        this->tail = QUEUE_START_TAIL;
    }

    SLD_QUEUE_IMPL_INLINE
    get_element_size (void) const -> u32 {

        assert_valid();
        return(sizeof(t));
    }

    SLD_QUEUE_IMPL_INLINE
    is_empty(void) const -> bool {

        this->assert_valid();
        return(this->head == QUEUE_START_HEAD);
    }

    SLD_QUEUE_IMPL_INLINE
    is_full(void) const -> bool {

        this->assert_valid();

        const u32  next_tail = (this->tail + sizeof(t)) % this->capacity;
        const bool is_full   = (next_tail == queue->head);
        return(is_full);
    }
    
    SLD_QUEUE_IMPL_INLINE
    get_size_total (void) const -> u32 {
        assert_valid();
        
        const u32 total_size = sizeof(t) * this->capacity;
        return(total_size);
    }


};


#endif //SLD_QUEUE_HPP
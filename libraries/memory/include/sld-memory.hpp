#ifndef SLD_MEMORY_HPP
#define SLD_MEMORY_HPP

#include "sld.hpp"

#ifndef  SLD_MEM_STACK_SIZE_KB
#define  SLD_MEM_STACK_SIZE_KB 64
#endif //SLD_MEM_STACK_SIZE_KB

/**********************************************************************************/
/* HANDLES                                                                        */
/**********************************************************************************/

namespace sld {
    
    typedef u32 mem_u32;

    enum mem_error_number_e {
        mem_error_number_e_none                      = 0,
        mem_error_number_e_context_init_fail         = 1,
        mem_error_number_e_context_out_of_memory     = 2,
        mem_error_number_e_reservation_invalid       = 3,
        mem_error_number_e_reservation_out_of_memory = 4,
        mem_error_number_e_os_mem_reserve_fail       = 5,
        mem_error_number_e_os_mem_release_fail       = 6,
        mem_error_number_e_os_mem_commit_fail        = 7,
        mem_error_number_e_os_mem_decommit_fail      = 8,
        mem_error_number_e_arena_invalid             = 9,     
        mem_error_number_e_arena_out_of_memory       = 10      
    };

    enum mem_allocator_type_e {
        mem_allocator_type_e_linear   = 0,
        mem_allocator_type_e_block    = 1,
        mem_allocator_type_e_circular = 2,
    };
};

/**********************************************************************************/
/* CONTEXT                                                                        */
/**********************************************************************************/

namespace sld {

    struct mem_context_info_t {
        mem_u32 last_error;
        u32     size_total;
        u32     size_used;
        u32     count_reservations;
        u32     count_arenas;
    };

    bool          mem_context_init           (void);
    void          mem_context_info           (mem_context_info_t& info);
    const ptr     mem_context_push_bytes_abs (const u32 size);
    const addr    mem_context_push_bytes_abs (const u32 size);
    const mem_u32 mem_context_push_bytes_rel (const u32 size);
}

#define sld_mem_context_push_struct(type) (type*)mem_context_push_bytes(sizeof(type))

/**********************************************************************************/
/* RESERVATION                                                                    */
/**********************************************************************************/

namespace sld {
     
    struct mem_reservation_info_t {
        u32 size_page;
        u32 size_arena;
        u32 count_arenas_total;
        u32 count_arenas_used;
    };

    const mem_u32 mem_reservation_acquire        (const u64 size_min_reservation, const u32 size_min_arena);
    bool          mem_reservation_release        (const mem_u32 reservation_id);
    bool          mem_reservation_get_info       (const mem_u32 reservation_id, mem_reservation_info_t& info);
    const mem_u32 mem_reservation_commit_arena   (const mem_u32 reservation_id);
    const mem_u32 mem_reservation_decommit_arena (const mem_u32 reservation_id, const mem_u32 arena_id);
};

/**********************************************************************************/
/* ARENA                                                                          */
/**********************************************************************************/

namespace sld {


    struct mem_arena_info_t {
        mem_u32 reservation_id;
        u32     page_number;
        u32     size_used;
    };

    const ptr     mem_arena_push_bytes_abs          (const mem_u32 arena_id, const u32 size);
    const addr    mem_arena_push_bytes_abs          (const mem_u32 arena_id, const u32 size);
    const u32     mem_arena_push_bytes_rel          (const mem_u32 arena_id, const u32 size);
    const ptr     mem_arena_get_ptr                 (const mem_u32 arena_id, const u32 offset);
    const mem_u32 mem_arena_push_allocator_linear   (const mem_u32 arena_id, const u32 size);
    const mem_u32 mem_arena_push_allocator_circular (const mem_u32 arena_id, const u32 size);
    const mem_u32 mem_arena_push_allocator_block    (const mem_u32 arena_id, const u32 size, const u32 stride);
    bool          mem_arena_pull_bytes              (const mem_u32 arena_id, const u32 size);
    bool          mem_arena_reset                   (const mem_u32 arena_id);
    bool          mem_arena_get_info                (const mem_u32 arena_id, mem_arena_info_t& info);
};

#define sld_mem_arena_push_struct(arena,type) (type*)mem_arena_push_bytes(arena,sizeof(type))
#define sld_mem_arena_pull_struct(arena,type)        mem_arena_pull_bytes(arena,sizeof(type))

/**********************************************************************************/
/* LINEAR ALLOCATOR                                                               */
/**********************************************************************************/

namespace sld {

    struct mem_linear_alloc_info_t {
        mem_u32 arena_id;
        u32     size_total;
        u32     size_used_a;
        u32     size_used_b;
    };

    bool       mem_linear_get_info    (const mem_u32 l_alloc, mem_linear_alloc_info_t& info);
    bool       mem_linear_reset_all   (const mem_u32 l_alloc);

    const ptr  mem_linear_a_alloc_abs (const mem_u32 l_alloc, const u32 size);
    const addr mem_linear_a_alloc_abs (const mem_u32 l_alloc, const u32 size);
    const u32  mem_linear_a_alloc_rel (const mem_u32 l_alloc, const u32 size);
    const ptr  mem_linear_a_get_ptr   (const mem_u32 l_alloc. const u32 offset);
    bool       mem_linear_a_free      (const mem_u32 l_alloc, const u32 size);
    bool       mem_linear_a_reset     (const mem_u32 l_alloc);
    
    const ptr  mem_linear_b_alloc_abs (const mem_u32 l_alloc, const u32 size);
    const addr mem_linear_b_alloc_abs (const mem_u32 l_alloc, const u32 size);
    const u32  mem_linear_b_alloc_rel (const mem_u32 l_alloc, const u32 size);
    const ptr  mem_linear_b_get_ptr   (const mem_u32 l_alloc. const u32 offset);
    bool       mem_linear_b_free      (const mem_u32 l_alloc, const u32 size);
    bool       mem_linear_b_reset     (const mem_u32 l_alloc);

};

/**********************************************************************************/
/* BLOCK ALLOCATOR                                                                */
/**********************************************************************************/

namespace sld {

    struct mem_block_alloc_info_t {
        mem_u32 arena_id;
        u32     block_size;
        u32     block_count_total;
        u32     block_count_used;
    };

    const ptr  mem_block_alloc_abs (const mem_u32 b_alloc);
    const u32  mem_block_alloc_rel (const mem_u32 b_alloc);
    bool       mem_block_free_abs  (const mem_u32 b_alloc, const ptr block_ptr);
    bool       mem_block_free_rel  (const mem_u32 b_alloc, const u32 block_index);
    bool       mem_block_get_info  (const mem_u32 b_alloc, mem_block_alloc_info_t& info);
    bool       mem_block_reset     (const mem_u32 b_alloc);
};

#endif //SLD_MEMORY_HPP
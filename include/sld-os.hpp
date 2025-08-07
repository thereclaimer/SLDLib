#ifndef SLD_OS_HPP
#define SLD_OS_HPP

#include "sld.hpp"
#include "sld-bitwise.hpp"

/**********************************************************************************/
/* WINDOW                                                                         */
/**********************************************************************************/

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    struct os_window_t;
    struct os_window_size_t;
    struct os_window_position_t;

    typedef u32   os_window_flags_t;
    typedef void* os_window_handle_t;

    const os_window_handle_t os_window_create       (const os_window_t&       window);
    bool                     os_window_destroy      (const os_window_handle_t window_handle);
    bool                     os_window_show         (const os_window_handle_t window_handle);
    bool                     os_window_get_size     (const os_window_handle_t window_handle, os_window_size_t&     window_size);
    bool                     os_window_get_position (const os_window_handle_t window_handle, os_window_position_t& window_position);

    //-------------------------------------------------------------------
    // ENUMS
    //-------------------------------------------------------------------

    enum os_window_flag_e {
        os_window_flag_e_none    = 0,
        os_window_flag_e_visible = bit_value(0),
        os_window_flag_e_closed  = bit_value(1),
        os_window_flag_e_moved   = bit_value(2),
        os_window_flag_e_resized = bit_value(3)
    };

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct os_window_size_t {
        u32 width;
        u32 height;
    };

    struct os_window_position_t {
        u32 screen_x;
        u32 screen_y;
    };

    struct os_window_t {
        const c8*            title;
        os_window_size_t     size;
        os_window_position_t position;
    };
};

/**********************************************************************************/
/* SYSTEM                                                                         */
/**********************************************************************************/

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    struct os_system_info_memory_t;
    struct os_system_info_cpu_cache_t;
    struct os_system_info_cpu_t;

    typedef const b8 (*os_system_get_info_cpu_f)    (os_system_info_cpu_t*    cpu_info);
    typedef const b8 (*os_system_get_info_memory_f) (os_system_info_memory_t* memory_info);
    typedef const ms (*os_system_time_ms_f)         (void);
    typedef void     (*os_system_sleep_f)           (const u32 ms);
    typedef void     (*os_system_debug_print_f)     (const c8* debug_string);

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------

    struct os_system_info_cpu_cache_t {
        u32 total_size;
        u32 line_size;
    };

    struct os_system_info_cpu_t {
        u32                        parent_core_number;
        u32                        speed_mhz;
        u32                        core_count_physical;
        u32                        core_count_logical;
        os_system_info_cpu_cache_t cache_l1;
        os_system_info_cpu_cache_t cache_l2;
        os_system_info_cpu_cache_t cache_l3;
    };

    struct os_system_info_memory_t {
        u32 page_size;
        u32 allocation_granularity;
    };
};

/**********************************************************************************/
/* MEMORY                                                                         */
/**********************************************************************************/

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    typedef void* (*os_memory_reserve)  (const void* start, const u64 size);
    typedef void* (*os_memory_release)  (const void* start, const u64 size);
    typedef void* (*os_memory_commit)   (const void* start, const u64 size);
    typedef void* (*os_memory_decommit) (const void* start, const u64 size);


    typedef void* os_file_handle_t;

    ////////////////////
    // system
    ////////////////////
    



    ////////////////////
    // memory
    ////////////////////

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------



};

/**********************************************************************************/
/* FILES                                                                           */
/**********************************************************************************/


#endif //SLD_OS_HPP
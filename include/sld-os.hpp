#ifndef SLD_OS_HPP
#define SLD_OS_HPP

#include "sld.hpp"
#include "sld-buffer.hpp"

/**********************************************************************************/
/* OS                                                                             */
/**********************************************************************************/

#define SLD_OS_WINDOW_TABLE_COUNT 8
#define SLD_OS_FILE_TABLE_COUNT   32
#define SLD_OS_THREAD_TABLE_COUNT 32

namespace sld {

    //-------------------------------------------------------------------
    // SYSTEM
    //-------------------------------------------------------------------

    struct os_system_cpu_info_t;
    struct os_system_cpu_cache_info_t;
    struct os_system_memory_info_t;

    using os_system_get_cpu_info_f              = const b8  (*) (os_system_cpu_info_t&       cpu_info);
    using os_system_get_cpu_cache_info_f        = bool      (*) (os_system_cpu_cache_info_t& cpu_cache_info);
    using os_system_get_memory_info_f           = const b8  (*) (os_system_info_memory_t* memory_info);
    using os_system_time_ms_f                   = const u64 (*) (void);
    using os_system_sleep_f                     = void      (*) (const u32 ms);
    using os_system_debug_print_f               = void      (*) (const c8* debug_string);

    struct os_system_cpu_cache_info_t {
        u32 level;
        u32 total_size;
        u32 line_size;
    };

    struct os_system_info_cpu_t {
        u32 parent_core_number;
        u32 speed_mhz;
        u32 core_count_physical;
        u32 core_count_logical;
        u32 cache_levels;
    };

    struct os_system_info_memory_t {
        u32 page_size;
        u32 allocation_granularity;
    };

    //-------------------------------------------------------------------
    // MONITORS
    //-------------------------------------------------------------------

    struct os_monitor_screen_size_t;
    struct os_monitor_info_t;

    using os_monitor_count_f       = const u32 (*) (void);
    using os_monitor_screen_size_f = void      (*) (os_monitor_screen_size_t& monitor_screen_size);
    using os_monitor_info_f        = bool      (*) (os_monitor_info_t&        monitor_info);

    struct os_monitor_screen_size_t {
        u32 pixel_width;
        u32 pixel_height;
    };

    struct os_monitor_info_t {
        u32 index;
        u32 pixel_width;
        u32 pixel_height;
        u32 position_x;
        u32 position_y;
    };

    //-------------------------------------------------------------------
    // WINDOW
    //-------------------------------------------------------------------

    struct os_window_t;
    struct os_window_size_t;
    struct os_window_position_t;

    typedef u32   os_window_flags_t;
    typedef void* os_window_handle_t;

    using os_window_create_f       = bool (*) (os_window_t& window);
    using os_window_destroy_f      = bool (*) (const os_window_handle_t window_handle);
    using os_window_show_f         = bool (*) (const os_window_handle_t window_handle);
    using os_window_get_size_f     = bool (*) (const os_window_handle_t window_handle, os_window_size_t&     window_size);
    using os_window_get_position_f = bool (*) (const os_window_handle_t window_handle, os_window_position_t& window_position);

    enum os_window_flag_e {
        os_window_flag_e_none    = 0,
        os_window_flag_e_visible = bit_value(0),
        os_window_flag_e_closed  = bit_value(1),
        os_window_flag_e_moved   = bit_value(2),
        os_window_flag_e_resized = bit_value(3)
    };

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
        os_window_handle_t   handle;
    };

    //-------------------------------------------------------------------
    // MEMORY
    //-------------------------------------------------------------------

    using os_memory_reserve_f  = void* (*) (const void* start, const u64 size);
    using os_memory_release_f  = void* (*) (const void* start, const u64 size);
    using os_memory_commit_f   = void* (*) (const void* start, const u64 size);
    using os_memory_decommit_f = void* (*) (const void* start, const u64 size);

    //-------------------------------------------------------------------
    // FILES
    //-------------------------------------------------------------------

    struct os_file_t;
    struct os_file_io_t;

    typedef void* os_file_handle_t;
    typedef byte  os_file_flags_t;

    enum os_file_flag_e {
        os_file_flag_e_none         = 0,
        os_file_flag_e_async        = bit_value(0),
        os_file_flag_e_read         = bit_value(1),
        os_file_flag_e_write        = bit_value(2),
        os_file_flag_e_share_read   = bit_value(3),
        os_file_flag_e_share_write  = bit_value(4),
        os_file_flag_e_share_delete = bit_value(5),
    };

    using os_file_io_callback_f = void (*) (const os_file_handle_t handle, const u32 bytes_transferred); 
    using os_file_open_f        = bool (*) (const u32 count,  os_file_t*    file);
    using os_file_size_f        = bool (*) (const u32 count,  os_file_io_t* file_io);
    using os_file_read_f        = bool (*) (const u32 count,  os_file_io_t* file_io);    
    using os_file_write_f       = bool (*) (const u32 count,  os_file_io_t* file_io);    

    struct os_file_t {
        const c8*        path;
        os_file_flags_t  flags;
        os_file_handle_t handle;
    };

    struct os_file_io_t {
        os_file_handle_t      handle;
        os_file_io_callback_f callback;
        byte*                 data;
        u32                   offset;
        u32                   size;
        u32                   length;        
    };

    //-------------------------------------------------------------------
    // THREADS
    //-------------------------------------------------------------------

    typedef void* os_thread_handle_t;

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    extern os_system_get_cpu_info_f       os_system_get_cpu_info;
    extern os_system_get_cpu_cache_info_f os_system_get_cpu_cache_info;
    extern os_system_get_memory_info_f    os_system_get_memory_info;
    extern os_system_time_ms_f            os_system_time_ms;
    extern os_system_sleep_f              os_system_sleep;
    extern os_system_debug_print_f        os_system_debug_print;

    extern os_monitor_count_f             os_monitor_count;
    extern os_monitor_screen_size_f       os_monitor_screen_size;
    extern os_monitor_info_f              os_monitor_info;

    extern os_window_create_f             os_window_create;
    extern os_window_destroy_f            os_window_destroy;
    extern os_window_show_f               os_window_show;
    extern os_window_get_size_f           os_window_get_size;
    extern os_window_get_position_f       os_window_get_position;

    extern os_memory_reserve_f            os_memory_reserve;
    extern os_memory_release_f            os_memory_release;
    extern os_memory_commit_f             os_memory_commit;
    extern os_memory_decommit_f           os_memory_decommit;

    extern os_file_size_f                 os_file_size;
    extern os_file_read_f                 os_file_read;
    extern os_file_write_f                os_file_read;
    extern os_file_open_f                 os_file_open;

};

#endif //SLD_OS_HPP
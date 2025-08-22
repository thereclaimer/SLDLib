#ifndef SLD_OS_HPP
#define SLD_OS_HPP

#include "sld.hpp"
#include "sld-buffer.hpp"

/**********************************************************************************/
/* OS                                                                             */
/**********************************************************************************/

#ifndef    SLD_OS_FILE_ASYNC_CONTEXT_SIZE
#   define SLD_OS_FILE_ASYNC_CONTEXT_SIZE      1024
#endif
#ifndef    SLD_OS_INPUT_KEYCODE_QUEUE_CAPACITY
#   define SLD_OS_INPUT_KEYCODE_QUEUE_CAPACITY 8
#endif

namespace sld {

    //-------------------------------------------------------------------
    // CONTEXT
    //-------------------------------------------------------------------

    typedef u32 os_context_graphics_t; 

    enum os_context_graphics_e {
        os_context_graphics_e_none   = 0,
        os_context_graphics_e_opengl = 1,
        os_context_graphics_e_dx12   = 2      
    };

    using os_context_init_f = bool (*) (const os_context_graphics_t graphics);

    //-------------------------------------------------------------------
    // SYSTEM
    //-------------------------------------------------------------------

    struct os_system_cpu_info_t;
    struct os_system_cpu_cache_info_t;
    struct os_system_memory_info_t;

    using os_system_get_cpu_info_f       = void      (*) (os_system_cpu_info_t&       cpu_info);
    using os_system_get_cpu_cache_info_f = void      (*) (os_system_cpu_cache_info_t& cpu_cache_info);
    using os_system_get_memory_info_f    = void      (*) (os_system_memory_info_t&    memory_info);
    using os_system_time_ms_f            = const u64 (*) (void);
    using os_system_sleep_f              = void      (*) (const u32 ms);
    using os_system_debug_print_f        = void      (*) (const c8* debug_string);

    struct os_system_cpu_cache_info_t {
        u32 level;
        u32 total_size;
        u32 line_size;
    };

    struct os_system_cpu_info_t {
        u32 parent_core_number;
        u32 speed_mhz;
        u32 core_count_physical;
        u32 core_count_logical;
        u32 cache_levels;
    };

    struct os_system_memory_info_t {
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
    // INPUT
    //-------------------------------------------------------------------

    struct os_input_t;
    struct os_input_mouse_t;
    struct os_input_keyboard_t;

    typedef byte os_input_keycode_t;



    //-------------------------------------------------------------------
    // WINDOW
    //-------------------------------------------------------------------

    struct os_window_size_t;
    struct os_window_position_t;

    typedef void* os_window_handle_t;
    typedef u32   os_window_events_t;

    using os_window_create_f         = os_window_handle_t (*) (const c8* title, const os_window_size_t& size, const os_window_position_t& position);
    using os_window_frame_start_f    = bool               (*) (const os_window_handle_t handle);
    using os_window_frame_render_f   = bool               (*) (const os_window_handle_t handle);
    using os_window_process_events_f = bool               (*) (const os_window_handle_t handle, os_window_events_t&   events);
    using os_window_destroy_f        = bool               (*) (const os_window_handle_t handle);
    using os_window_show_f           = bool               (*) (const os_window_handle_t handle);
    using os_window_get_size_f       = bool               (*) (const os_window_handle_t handle, os_window_size_t&     size);
    using os_window_get_position_f   = bool               (*) (const os_window_handle_t handle, os_window_position_t& position);

    enum os_window_event_e {
        os_window_event_e_none                  = 0,
        os_window_event_e_quit                  = bit_value(0),
        os_window_event_e_destroyed             = bit_value(1),
        os_window_event_e_moved                 = bit_value(2),
        os_window_event_e_resized               = bit_value(3),
        os_window_event_e_key_down              = bit_value(4),
        os_window_event_e_key_up                = bit_value(5),
        os_window_event_e_mouse_move            = bit_value(6),
        os_window_event_e_mouse_left_down       = bit_value(7),
        os_window_event_e_mouse_left_up         = bit_value(8),
        os_window_event_e_mouse_left_dbl_click  = bit_value(9),
        os_window_event_e_mouse_right_down      = bit_value(10),
        os_window_event_e_mouse_right_up        = bit_value(11),
        os_window_event_e_mouse_right_dbl_click = bit_value(12),
        os_window_event_e_mouse_wheel           = bit_value(13)
    };

    struct os_window_size_t {
        u32 width;
        u32 height;
    };

    struct os_window_position_t {
        u32 screen_x;
        u32 screen_y;
    };

    //-------------------------------------------------------------------
    // MEMORY
    //-------------------------------------------------------------------

    using os_memory_reserve_f              = void* (*) (void* start, const u64 size);
    using os_memory_release_f              = bool  (*) (void* start, const u64 size);
    using os_memory_commit_f               = void* (*) (void* start, const u64 size);
    using os_memory_decommit_f             = bool  (*) (void* start, const u64 size);
    using os_memory_align_to_page_f        = u64   (*) (const u64 size);
    using os_memory_align_to_granularity_f = u64   (*) (const u64 size);

    //-------------------------------------------------------------------
    // FILES
    //-------------------------------------------------------------------

    typedef void* os_file_handle_t;
    typedef byte  os_file_flags_t;
    typedef u32   os_file_error_t;

    struct os_file_buffer_t;
    struct os_file_async_context_t;

    using os_file_last_error_f     = const os_file_error_t  (*) (void); 
    using os_file_open_f           = const os_file_handle_t (*) (const c8* path, const os_file_flags_t flags);
    using os_file_size_f           = bool                   (*) (const os_file_handle_t handle, u64& size);
    using os_file_async_callback_f = void                   (*) (const os_file_async_context_t* async_context); 
    using os_file_read_f           = const u64              (*) (const os_file_handle_t handle, os_file_buffer_t& buffer);    
    using os_file_write_f          = const u64              (*) (const os_file_handle_t handle, os_file_buffer_t& buffer);    
    using os_file_read_async_f     = bool                   (*) (const os_file_handle_t handle, os_file_buffer_t& buffer, os_file_async_context_t& async_context);    
    using os_file_write_async_f    = bool                   (*) (const os_file_handle_t handle, os_file_buffer_t& buffer, os_file_async_context_t& async_context);    

    struct os_file_buffer_t {
        byte* data;
        u64   offset;
        u64   size;
        u64   length;
    };

    struct os_file_async_context_t {
        os_file_handle_t      handle;
        os_file_async_callback_f callback;
        os_file_error_t       error;
        u32                   bytes_transferred;
        byte                  os_data[SLD_OS_FILE_ASYNC_CONTEXT_SIZE];                 
    };


    enum os_file_flag_e {
        os_file_flag_e_none          = 0,
        os_file_flag_e_async         = bit_value(0),
        os_file_flag_e_read          = bit_value(1),
        os_file_flag_e_write         = bit_value(2),
        os_file_flag_e_share_read    = bit_value(3), 
        os_file_flag_e_share_write   = bit_value(4),
        os_file_flag_e_share_delete  = bit_value(5),
        os_file_flag_e_open_existing = bit_value(6),
        os_file_flag_e_overwrite     = bit_value(7),
    };

    enum os_file_error_e {
        os_file_error_e_success             =  0,
        os_file_error_e_unknown             = -1,
        os_file_error_e_invalid_args        = -2,
        os_file_error_e_invalid_handle      = -3,
        os_file_error_e_invalid_disk        = -4,
        os_file_error_e_invalid_device      = -5,
        os_file_error_e_invalid_buffer      = -6,
        os_file_error_e_invalid_file        = -7,
        os_file_error_e_sharing_violation   = -8,
        os_file_error_e_already_exists      = -9,
        os_file_error_e_not_found           = -10,
        os_file_error_e_access_denied       = -11,
        os_file_error_e_pipe_busy           = -12,
        os_file_error_e_reached_end_of_file = -13,
        os_file_error_e_broken_pipe         = -14,
        os_file_error_e_no_data             = -15,
        os_file_error_e_more_data           = -16,
        os_file_error_e_io_incomplete       = -17,
        os_file_error_e_io_pending          = -18,
        os_file_error_e_operation_aborted   = -19,
        os_file_error_e_disk_io_failure     = -20,
        os_file_error_e_disk_corrupt        = -21,
        os_file_error_e_device_not_ready    = -22,
        os_file_error_e_out_of_memory       = -23,
        os_file_error_e_device_failure      = -24
    };

    static inline bool
    os_file_buffer_validate(
        const os_file_buffer_t& buffer) {
        
        bool is_valid = true;
        is_valid &= (buffer.data   != NULL);
        is_valid &= (buffer.size   != 0);
        is_valid &= (buffer.length <= buffer.size);
        return(is_valid);
    }

    //-------------------------------------------------------------------
    // THREADS
    //-------------------------------------------------------------------

    typedef void* os_thread_handle_t;

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    sld_os_api os_system_get_cpu_info_f         os_system_get_cpu_info;
    sld_os_api os_system_get_cpu_cache_info_f   os_system_get_cpu_cache_info;
    sld_os_api os_system_get_memory_info_f      os_system_get_memory_info;
    sld_os_api os_system_time_ms_f              os_system_time_ms;
    sld_os_api os_system_sleep_f                os_system_sleep;
    sld_os_api os_system_debug_print_f          os_system_debug_print;

    sld_os_api os_monitor_count_f               os_monitor_count;
    sld_os_api os_monitor_screen_size_f         os_monitor_screen_size;
    sld_os_api os_monitor_info_f                os_monitor_info;

    sld_os_api os_window_create_f               os_window_create; 
    sld_os_api os_window_frame_start_f          os_window_frame_start; 
    sld_os_api os_window_frame_render_f         os_window_frame_render; 
    sld_os_api os_window_process_events_f       os_window_process_events; 
    sld_os_api os_window_destroy_f              os_window_destroy; 
    sld_os_api os_window_show_f                 os_window_show; 
    sld_os_api os_window_get_size_f             os_window_get_size; 
    sld_os_api os_window_get_position_f         os_window_get_position; 

    sld_os_api os_memory_reserve_f              os_memory_reserve;
    sld_os_api os_memory_release_f              os_memory_release;
    sld_os_api os_memory_commit_f               os_memory_commit;
    sld_os_api os_memory_decommit_f             os_memory_decommit;
    sld_os_api os_memory_align_to_page_f        os_memory_align_to_page;
    sld_os_api os_memory_align_to_granularity_f os_memory_align_to_granularity;

    sld_os_api os_file_last_error_f             os_file_last_error;
    sld_os_api os_file_open_f                   os_file_open;
    sld_os_api os_file_size_f                   os_file_size;
    sld_os_api os_file_read_f                   os_file_read;
    sld_os_api os_file_write_f                  os_file_write;
    sld_os_api os_file_read_async_f             os_file_read_async;
    sld_os_api os_file_write_async_f            os_file_write_async;
};

#endif //SLD_OS_HPP
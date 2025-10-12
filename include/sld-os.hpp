#ifndef SLD_OS_HPP
#define SLD_OS_HPP

#include "sld.hpp"
#include "sld-buffer.hpp"
#include "sld-geometry.hpp"
#include "sld-graphics.hpp"
#include "sld-queue.hpp"
#include "sld-input.hpp"

/**********************************************************************************/
/* OS                                                                             */
/**********************************************************************************/

#define    SLD_API_OS          extern        // os api declaration
#define    SLD_API_OS_FUNC     static        // os api function
#define    SLD_API_OS_INTERNAL inline static // internal os function

#ifndef    SLD_OS_FILE_ASYNC_CONTEXT_SIZE
#   define SLD_OS_FILE_ASYNC_CONTEXT_SIZE      1024
#endif
#define    SLD_OS_GRAPHICS_CONTEXT_OPENGL      0
#define    SLD_OS_GRAPHICS_CONTEXT_DIRECTX12   1
#ifndef    SLD_OS_GRAPHICS_CONTEXT_TYPE
#   define SLD_OS_GRAPHICS_CONTEXT_TYPE        SLD_OS_GRAPHICS_CONTEXT_OPENGL
#endif

namespace sld {

    struct os_handle_t : vptr_t { };
    struct os_error_t  : s32_t  { };
    struct os_flags_t  : u32_t  { };

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

    struct os_monitor_handle_t : os_handle_t { };
    struct os_monitor_info_t;
    struct os_monitor_working_area_t;
    
    using os_monitor_count_f        = u32                 (*) (void);
    using os_monitor_working_area_f = void                (*) (os_monitor_working_area_t& monitor_working_area);
    using os_monitor_info_f         = void                (*) (os_monitor_info_t*         monitor_info);
    using os_monitor_primary_f      = os_monitor_handle_t (*) (void);
    using os_monitor_from_point_f   = os_monitor_handle_t (*) (const u32 x, const u32 y);

    struct os_monitor_working_area_t {
        u32 virtual_pixel_width;
        u32 virtual_pixel_height;
    };

    struct os_monitor_screen_size_t {
        u32 pixel_width;
        u32 pixel_height;
    };

    constexpr u32 OS_MONITOR_NAME_WIDTH = 32;
    struct os_monitor_info_t {
        os_monitor_handle_t handle;
        u32                 index;
        u32                 pixel_width;
        u32                 pixel_height;
        u32                 position_x;
        u32                 position_y;
        cchar               name_cstr[OS_MONITOR_NAME_WIDTH];
    };

    //-------------------------------------------------------------------
    // INPUT
    //-------------------------------------------------------------------

    struct os_input_t;
    struct os_input_mouse_t;
    struct os_input_keyboard_t;
    struct os_input_keycode_t;
    struct os_input_keycode_array_t;
    struct os_input_gamepad_t;
    struct os_input_gamepad_stick_t;
    struct os_input_gamepad_triggers_t;

    struct os_input_gamepad_button_flags_t : u16_t { };

    void os_input_keyboard_add_key_down (os_input_keyboard_t* keyboard, const os_input_keycode_t keycode);
    void os_input_keyboard_add_key_up   (os_input_keyboard_t* keyboard, const os_input_keycode_t keycode);
    void os_input_keyboard_reset        (os_input_keyboard_t* keyboard);

    struct os_input_t {
        os_input_mouse_t*    mouse;
        os_input_keyboard_t* keyboard;
        os_input_gamepad_t*  gamepad;    
    };


    struct os_input_mouse_t {
        f32 x;
        f32 y;
        f32 wheel;
    };

    struct os_input_keycode_t {
        u16 val;
    };

    struct os_input_keycode_array_t {
        u8                  capacity;
        u8                  count;
        os_input_keycode_t* array;        
    };

    struct os_input_keyboard_t {
        os_input_keycode_array_t keys_down;
        os_input_keycode_array_t keys_up;
    };

    struct os_input_gamepad_sticks_t {
        f32 left_x;
        f32 left_y;
        f32 right_x;
        f32 right_y;
    };

    struct os_input_gamepad_triggers_t {
        f32 left;
        f32 right;
    };

    struct os_input_gamepad_t {
        os_input_gamepad_sticks_t       sticks;
        os_input_gamepad_triggers_t     triggers;
        os_input_gamepad_button_flags_t button_flags;
    };


    enum os_input_keycode_e {
        
        // control
        os_input_keycode_e_null         = 0x00,
        os_input_keycode_e_escape       = 0x01,  // esc
        os_input_keycode_e_tab          = 0x0F,
        os_input_keycode_e_enter        = 0x1C,
        os_input_keycode_e_backspace    = 0x0E,
        os_input_keycode_e_space        = 0x39,

        // modifiers
        os_input_keycode_e_shift_left   = 0x2A,
        os_input_keycode_e_shift_right  = 0x36,
        os_input_keycode_e_ctrl_left    = 0x1D,
        os_input_keycode_e_ctrl_right   = 0x11D,
        os_input_keycode_e_alt_left     = 0x38,
        os_input_keycode_e_alt_right    = 0x138,
        os_input_keycode_e_caps_lock    = 0x3A,
        os_input_keycode_e_num_lock     = 0x45,
        os_input_keycode_e_scroll_lock  = 0x46,
        os_input_keycode_e_super_left   = 0x15B, // win / command
        os_input_keycode_e_super_right  = 0x15C,
        os_input_keycode_e_menu         = 0x15D,

        // function keys
        os_input_keycode_e_f1           = 0x3B,
        os_input_keycode_e_f2           = 0x3C,
        os_input_keycode_e_f3           = 0x3D,
        os_input_keycode_e_f4           = 0x3E,
        os_input_keycode_e_f5           = 0x3F,
        os_input_keycode_e_f6           = 0x40,
        os_input_keycode_e_f7           = 0x41,
        os_input_keycode_e_f8           = 0x42,
        os_input_keycode_e_f9           = 0x43,
        os_input_keycode_e_f10          = 0x44,
        os_input_keycode_e_f11          = 0x57,
        os_input_keycode_e_f12          = 0x58,

        // navigation cluster
        os_input_keycode_e_insert       = 0x152,
        os_input_keycode_e_delete       = 0x153,
        os_input_keycode_e_home         = 0x147,
        os_input_keycode_e_end          = 0x14F,
        os_input_keycode_e_page_up      = 0x149,
        os_input_keycode_e_page_down    = 0x151,
        os_input_keycode_e_arrow_up     = 0x148,
        os_input_keycode_e_arrow_down   = 0x150,
        os_input_keycode_e_arrow_left   = 0x14B,
        os_input_keycode_e_arrow_right  = 0x14D,

        // number row (shifted versions are symbols)
        os_input_keycode_e_1            = 0x02,
        os_input_keycode_e_2            = 0x03,
        os_input_keycode_e_3            = 0x04,
        os_input_keycode_e_4            = 0x05,
        os_input_keycode_e_5            = 0x06,
        os_input_keycode_e_6            = 0x07,
        os_input_keycode_e_7            = 0x08,
        os_input_keycode_e_8            = 0x09,
        os_input_keycode_e_9            = 0x0A,
        os_input_keycode_e_0            = 0x0B,
        os_input_keycode_e_minus        = 0x0C, // -
        os_input_keycode_e_equals       = 0x0D, // =

        // letters
        os_input_keycode_e_a            = 0x1E,
        os_input_keycode_e_b            = 0x30,
        os_input_keycode_e_c            = 0x2E,
        os_input_keycode_e_d            = 0x20,
        os_input_keycode_e_e            = 0x12,
        os_input_keycode_e_f            = 0x21,
        os_input_keycode_e_g            = 0x22,
        os_input_keycode_e_h            = 0x23,
        os_input_keycode_e_i            = 0x17,
        os_input_keycode_e_j            = 0x24,
        os_input_keycode_e_k            = 0x25,
        os_input_keycode_e_l            = 0x26,
        os_input_keycode_e_m            = 0x32,
        os_input_keycode_e_n            = 0x31,
        os_input_keycode_e_o            = 0x18,
        os_input_keycode_e_p            = 0x19,
        os_input_keycode_e_q            = 0x10,
        os_input_keycode_e_r            = 0x13,
        os_input_keycode_e_s            = 0x1F,
        os_input_keycode_e_t            = 0x14,
        os_input_keycode_e_u            = 0x16,
        os_input_keycode_e_v            = 0x2F,
        os_input_keycode_e_w            = 0x11,
        os_input_keycode_e_x            = 0x2D,
        os_input_keycode_e_y            = 0x15,
        os_input_keycode_e_z            = 0x2C,
        
        // punctuation
        os_input_keycode_e_grave        = 0x29, /*  `  */
        os_input_keycode_e_bracket_left = 0x1A, /*  [  */
        os_input_keycode_e_bracket_right= 0x1B, /*  ]  */
        os_input_keycode_e_backslash    = 0x2B, /*  \  */
        os_input_keycode_e_semicolon    = 0x27, /*  ;  */
        os_input_keycode_e_apostrophe   = 0x28, /*  '  */
        os_input_keycode_e_comma        = 0x33, /*  ,  */
        os_input_keycode_e_period       = 0x34, /*  .  */
        os_input_keycode_e_slash        = 0x35, /*  /  */

        // keypad
        os_input_keycode_e_kp_divide    = 0x135,
        os_input_keycode_e_kp_multiply  = 0x37,
        os_input_keycode_e_kp_subtract  = 0x4A,
        os_input_keycode_e_kp_add       = 0x4E,
        os_input_keycode_e_kp_enter     = 0x11C,
        os_input_keycode_e_kp_decimal   = 0x53,
        os_input_keycode_e_kp_0         = 0x52,
        os_input_keycode_e_kp_1         = 0x4F,
        os_input_keycode_e_kp_2         = 0x50,
        os_input_keycode_e_kp_3         = 0x51,
        os_input_keycode_e_kp_4         = 0x4B,
        os_input_keycode_e_kp_5         = 0x4C,
        os_input_keycode_e_kp_6         = 0x4D,
        os_input_keycode_e_kp_7         = 0x47,
        os_input_keycode_e_kp_8         = 0x48,
        os_input_keycode_e_kp_9         = 0x49,

        // multimedia
        os_input_keycode_e_volume_mute  = 0x120,
        os_input_keycode_e_volume_down  = 0x12E,
        os_input_keycode_e_volume_up    = 0x130,
        os_input_keycode_e_media_next   = 0x119,
        os_input_keycode_e_media_prev   = 0x110,
        os_input_keycode_e_media_stop   = 0x124,
        os_input_keycode_e_media_play   = 0x122,
    };

    enum os_input_gamepad_button_flag_e {
        os_input_gamepad_button_flag_e_start        = bit_value(0),
        os_input_gamepad_button_flag_e_select       = bit_value(1),
        os_input_gamepad_button_flag_e_bumper_left  = bit_value(2),
        os_input_gamepad_button_flag_e_bumper_right = bit_value(3),
        os_input_gamepad_button_flag_e_stick_left   = bit_value(4),
        os_input_gamepad_button_flag_e_stick_right  = bit_value(5),
        os_input_gamepad_button_flag_e_dpad_up      = bit_value(6),
        os_input_gamepad_button_flag_e_dpad_down    = bit_value(7),
        os_input_gamepad_button_flag_e_dpad_left    = bit_value(8),
        os_input_gamepad_button_flag_e_dpad_right   = bit_value(9),
        os_input_gamepad_button_flag_e_bpad_up      = bit_value(10),
        os_input_gamepad_button_flag_e_bpad_down    = bit_value(11),
        os_input_gamepad_button_flag_e_bpad_left    = bit_value(12),
        os_input_gamepad_button_flag_e_bpad_right   = bit_value(13)
    };

    //-------------------------------------------------------------------
    // WINDOW
    //-------------------------------------------------------------------

    struct os_window_handle_t               : os_handle_t { };
    struct os_window_error_t                : os_error_t  { };
    struct os_window_event_flags_t          : os_flags_t  { }; 

    struct os_window_update_t;

    typedef  dims_u32_size_t os_window_size_t;
    typedef  dims_u32_pos_t  os_window_pos_t ;
    typedef  color_u32_t     os_window_color_t;

    using os_window_create_f          = const os_window_error_t (*) (os_window_handle_t&      window_handle, const c8* title, const os_window_size_t& size, const os_window_pos_t& position);
    using os_window_set_viewport_f    = const os_window_error_t (*) (const os_window_handle_t window_handle, const os_window_size_t& size, const os_window_pos_t& position); 
    using os_window_update_f          = const os_window_error_t (*) (const os_window_handle_t window_handle, os_window_update_t&           update);
    using os_window_get_size_f        = const os_window_error_t (*) (const os_window_handle_t window_handle, os_window_size_t&             size);
    using os_window_get_position_f    = const os_window_error_t (*) (const os_window_handle_t window_handle, os_window_pos_t&              position);
    using os_window_set_clear_color_f = const os_window_error_t (*) (const os_window_handle_t window_handle, const os_window_color_t&      color);
    using os_window_destroy_f         = const os_window_error_t (*) (const os_window_handle_t window_handle);
    using os_window_swap_buffers_f    = const os_window_error_t (*) (const os_window_handle_t window_handle);
    using os_window_show_f            = const os_window_error_t (*) (const os_window_handle_t window_handle);

    struct os_window_update_t {
        input_keyboard_t*       keyboard;
        os_window_event_flags_t events;
    };

    enum os_window_event_flag_e {
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

    enum os_window_error_e {
        os_window_error_e_success                = 1,
        os_window_error_e_unknown                = -1,
        os_window_error_e_resource_not_found     = -2,
        os_window_error_e_access_denied          = -3,
        os_window_error_e_system_out_of_memory   = -4,
        os_window_error_e_general_out_of_memory  = -5,
        os_window_error_e_invalid_args           = -6,
        os_window_error_e_invalid_handle         = -7,
        os_window_error_e_invalid_class          = -8,
        os_window_error_e_class_already_exists   = -9,
        os_window_error_e_invalid_device_context = -10,
        os_window_error_e_invalid_thread         = -11,
        os_window_error_e_invalid_resource       = -12,
        os_window_error_e_quota_exceeded         = -13
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
    using os_memory_is_reserved_f          = bool  (*) (void* start);
    using os_memory_is_committed_f         = bool  (*) (void* start);

    //-------------------------------------------------------------------
    // FILES
    //-------------------------------------------------------------------

    struct os_file_handle_t : os_handle_t { };
    struct os_file_flags_t  : os_flags_t  { };
    struct os_file_error_t  : os_error_t  { };

    struct os_file_access_flags_t : u8_t { };
    struct os_file_share_flags_t  : u8_t { };
    struct os_file_mode_t         : u8_t { };
    struct os_file_config_t;

    struct os_file_buffer_t;
    struct os_file_callback_context_t;
    struct os_file_os_context_t;
    struct os_file_async_context_t;

    using os_file_async_callback_f = void                  (*) (const void* data, const os_file_error_t error, const u32 bytes_transferred);
    using os_file_open_f           = const os_file_error_t (*) (os_file_handle_t&      file_handle, const c8* path, const os_file_config_t& config);
    using os_file_size_f           = const os_file_error_t (*) (const os_file_handle_t file_handle, u64& size);
    using os_file_read_f           = const os_file_error_t (*) (const os_file_handle_t file_handle, os_file_buffer_t& buffer);    
    using os_file_write_f          = const os_file_error_t (*) (const os_file_handle_t file_handle, os_file_buffer_t& buffer);    
    using os_file_read_async_f     = const os_file_error_t (*) (const os_file_handle_t file_handle, os_file_buffer_t& buffer, os_file_async_context_t& context);    
    using os_file_write_async_f    = const os_file_error_t (*) (const os_file_handle_t file_handle, os_file_buffer_t& buffer, os_file_async_context_t& context);    

    struct os_file_buffer_t : buffer_t {
        u64 cursor;
        u64 transferred;
    };

    struct os_file_os_context_t {
        byte data[SLD_OS_FILE_ASYNC_CONTEXT_SIZE];                 
    };

    struct os_file_callback_context_t {
        os_file_async_callback_f func;
        void*                    data;
    };

    struct os_file_async_context_t {
        os_file_os_context_t*       os;
        os_file_callback_context_t* callback;
    };

    struct os_file_context_t {
        os_file_handle_t handle;
        u32              bytes_transferred;
    };

    enum os_file_access_flag_e {
        os_file_access_flag_e_none           = 0,        
        os_file_access_flag_e_read           = bit_value(0),        
        os_file_access_flag_e_write          = bit_value(1)
    };

    enum os_file_share_flag_e {
        os_file_share_flag_e_none   = 0 ,
        os_file_share_flag_e_read   = bit_value(0),
        os_file_share_flag_e_write  = bit_value(1),
        os_file_share_flag_e_delete = bit_value(2)
    };

    enum os_file_mode_e {
        os_file_mode_e_create_new         = 0, 
        os_file_mode_e_open_existing      = 1, 
        os_file_mode_e_open_always        = 2, 
        os_file_mode_e_overwrite_existing = 3 
    };

    struct os_file_config_t {
        os_file_access_flags_t access_flags;
        os_file_share_flags_t  share_flags;
        os_file_mode_t         mode;
        bool                   is_async;
    };

    enum os_file_error_e {
        os_file_error_e_success             = 0,
        os_file_error_e_unknown             = 1,
        os_file_error_e_invalid_args        = 2,
        os_file_error_e_invalid_handle      = 3,
        os_file_error_e_invalid_disk        = 4,
        os_file_error_e_invalid_device      = 5,
        os_file_error_e_invalid_buffer      = 6,
        os_file_error_e_invalid_file        = 7,
        os_file_error_e_sharing_violation   = 8,
        os_file_error_e_already_exists      = 9,
        os_file_error_e_not_found           = 10,
        os_file_error_e_access_denied       = 11,
        os_file_error_e_pipe_busy           = 12,
        os_file_error_e_reached_end_of_file = 13,
        os_file_error_e_broken_pipe         = 14,
        os_file_error_e_no_data             = 15,
        os_file_error_e_more_data           = 16,
        os_file_error_e_io_incomplete       = 17,
        os_file_error_e_io_pending          = 18,
        os_file_error_e_operation_aborted   = 19,
        os_file_error_e_disk_io_failure     = 20,
        os_file_error_e_disk_corrupt        = 21,
        os_file_error_e_device_not_ready    = 22,
        os_file_error_e_out_of_memory       = 23,
        os_file_error_e_device_failure      = 24
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

    struct os_thread_handle_t           : os_handle_t { };
    struct os_thread_mutex_handle_t     : os_handle_t { };
    struct os_thread_condition_handle_t : os_handle_t { };
    struct os_thread_error_t            : os_error_t  { };

    struct os_thread_callback_data_t;
    struct os_thread_context_t;

    using os_thread_callback_function_f   = void (*) (os_thread_context_t& context);

    using os_thread_create_f              = const os_thread_error_t (*) (os_thread_handle_t&            thread_handle);
    using os_thread_destroy_f             = const os_thread_error_t (*) (const os_thread_handle_t       thread_handle);
    using os_thread_exit_f                = const os_thread_error_t (*) (const os_thread_handle_t       thread_handle);
    using os_thread_sleep_f               = const os_thread_error_t (*) (const os_thread_handle_t       thread_handle);     
    using os_thread_yield_f               = const os_thread_error_t (*) (const os_thread_handle_t       thread_handle);
    using os_thread_join_f                = const os_thread_error_t (*) (const os_thread_handle_t       thread_handle);
    using os_thread_mutex_create_f        = const os_thread_error_t (*) (os_thread_mutex_handle_t&      mutex_handle);
    using os_thread_mutex_destroy_f       = const os_thread_error_t (*) (const os_thread_mutex_handle_t mutex_handle);
    using os_thread_mutex_lock_f          = const os_thread_error_t (*) (const os_thread_mutex_handle_t mutex_handle);
    using os_thread_mutex_unlock_f        = const os_thread_error_t (*) (const os_thread_mutex_handle_t mutex_handle);
    using os_thread_mutex_try_lock_f      = const os_thread_error_t (*) (const os_thread_mutex_handle_t mutex_handle);
    using os_thread_condition_create_f    = const os_thread_error_t (*) (void);
    using os_thread_condition_destroy_f   = const os_thread_error_t (*) (void);
    using os_thread_condition_wait_f      = const os_thread_error_t (*) (void);
    using os_thread_condition_signal_f    = const os_thread_error_t (*) (void);
    using os_thread_condition_broadcast_f = const os_thread_error_t (*) (void);

    struct os_thread_callback_data_t {
        void* ptr;
        u64   size;
    };

    struct os_thread_context_t {
        os_thread_callback_function_f function;
        os_thread_callback_data_t     data;
    };

    enum os_thread_error_e {
        os_thread_error_e_success = 1,
        os_thread_error_e_unknown = -1
    };

    //-------------------------------------------------------------------
    // API
    //-------------------------------------------------------------------

    SLD_API_OS os_system_get_cpu_info_f         os_system_get_cpu_info;
    SLD_API_OS os_system_get_cpu_cache_info_f   os_system_get_cpu_cache_info;
    SLD_API_OS os_system_get_memory_info_f      os_system_get_memory_info;
    SLD_API_OS os_system_time_ms_f              os_system_time_ms;
    SLD_API_OS os_system_sleep_f                os_system_sleep;
    SLD_API_OS os_system_debug_print_f          os_system_debug_print;

    SLD_API_OS os_monitor_count_f               os_monitor_count;
    SLD_API_OS os_monitor_working_area_f        os_monitor_working_area;
    SLD_API_OS os_monitor_info_f                os_monitor_info;
    SLD_API_OS os_monitor_primary_f             os_monitor_primary;
    SLD_API_OS os_monitor_from_point_f          os_monitor_from_point; 

    SLD_API_OS os_window_create_f               os_window_create; 
    SLD_API_OS os_window_update_f               os_window_update; 
    SLD_API_OS os_window_swap_buffers_f         os_window_swap_buffers; 
    SLD_API_OS os_window_destroy_f              os_window_destroy; 
    SLD_API_OS os_window_show_f                 os_window_show; 
    SLD_API_OS os_window_get_size_f             os_window_get_size; 
    SLD_API_OS os_window_get_position_f         os_window_get_position; 
    SLD_API_OS os_window_set_viewport_f         os_window_set_viewport;  
    SLD_API_OS os_window_set_clear_color_f      os_window_set_clear_color;  

    SLD_API_OS os_memory_reserve_f              os_memory_reserve;
    SLD_API_OS os_memory_release_f              os_memory_release;
    SLD_API_OS os_memory_commit_f               os_memory_commit;
    SLD_API_OS os_memory_decommit_f             os_memory_decommit;
    SLD_API_OS os_memory_align_to_page_f        os_memory_align_to_page;
    SLD_API_OS os_memory_align_to_granularity_f os_memory_align_to_granularity;
    SLD_API_OS os_memory_is_reserved_f          os_memory_is_reserved;
    SLD_API_OS os_memory_is_committed_f         os_memory_is_committed;

    SLD_API_OS os_file_open_f                   os_file_open;
    SLD_API_OS os_file_size_f                   os_file_size;
    SLD_API_OS os_file_read_f                   os_file_read;
    SLD_API_OS os_file_write_f                  os_file_write;
    SLD_API_OS os_file_read_async_f             os_file_read_async;
    SLD_API_OS os_file_write_async_f            os_file_write_async;

    SLD_API_OS os_thread_create_f               os_thread_create;
    SLD_API_OS os_thread_destroy_f              os_thread_destroy;
    SLD_API_OS os_thread_exit_f                 os_thread_exit;
    SLD_API_OS os_thread_sleep_f                os_thread_sleep;
    SLD_API_OS os_thread_yield_f                os_thread_yield;
    SLD_API_OS os_thread_join_f                 os_thread_join;
    SLD_API_OS os_thread_mutex_create_f         os_thread_mutex_create;
    SLD_API_OS os_thread_mutex_destroy_f        os_thread_mutex_destroy;
    SLD_API_OS os_thread_mutex_lock_f           os_thread_mutex_lock;
    SLD_API_OS os_thread_mutex_unlock_f         os_thread_mutex_unlock;
    SLD_API_OS os_thread_mutex_try_lock_f       os_thread_mutex_try_lock;
    SLD_API_OS os_thread_condition_create_f     os_thread_condition_create;
    SLD_API_OS os_thread_condition_destroy_f    os_thread_condition_destroy;
    SLD_API_OS os_thread_condition_wait_f       os_thread_condition_wait;
    SLD_API_OS os_thread_condition_signal_f     os_thread_condition_signal;
    SLD_API_OS os_thread_condition_broadcast_f  os_thread_condition_broadcast;
};

#endif //SLD_OS_HPP
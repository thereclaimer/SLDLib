#pragma once

#include "sld-os.hpp"
#include "sld-win32-system.cpp"
#include "sld-win32-memory.cpp"
#include "sld-win32-file.cpp"

namespace sld {

    sld_os_api os_system_get_cpu_info_f         os_system_get_cpu_info         = win32_system_get_cpu_info;
    sld_os_api os_system_get_cpu_cache_info_f   os_system_get_cpu_cache_info   = win32_system_get_cpu_cache_info;
    sld_os_api os_system_get_memory_info_f      os_system_get_memory_info      = win32_system_get_memory_info;
    sld_os_api os_system_time_ms_f              os_system_time_ms              = win32_system_time_ms;
    sld_os_api os_system_sleep_f                os_system_sleep                = win32_system_sleep;
    sld_os_api os_system_debug_print_f          os_system_debug_print          = win32_system_debug_print;

    // os_monitor_count_f               os_monitor_count               = win32_monitor_count;
    // os_monitor_screen_size_f         os_monitor_screen_size         = win32_monitor_screen_size;
    // os_monitor_info_f                os_monitor_info                = win32_monitor_info;

    // sld_os_api os_window_create_f       os_window_create_opengl3       = win32_window_create_opengl3; 
    // sld_os_api os_window_frame_start_f  os_window_frame_start_opengl3  = win32_window_frame_start_opengl3; 
    // sld_os_api os_window_frame_render_f os_window_frame_render_opengl3 = NULL; 
    // sld_os_api os_window_destroy_f              os_window_destroy              = win32_window_destroy; 
    // sld_os_api os_window_show_f                 os_window_show                 = win32_window_show; 
    // sld_os_api os_window_get_size_f             os_window_get_size             = win32_window_get_size; 
    // sld_os_api os_window_get_position_f         os_window_get_position         = win32_window_get_position; 
    // sld_os_api os_window_process_events_f       os_window_process_events       = win32_window_process_events; 

    sld_os_api os_memory_reserve_f              os_memory_reserve              = win32_memory_reserve;
    sld_os_api os_memory_release_f              os_memory_release              = win32_memory_release;
    sld_os_api os_memory_commit_f               os_memory_commit               = win32_memory_commit;
    sld_os_api os_memory_decommit_f             os_memory_decommit             = win32_memory_decommit;
    sld_os_api os_memory_align_to_page_f        os_memory_align_to_page        = win32_memory_align_to_page;
    sld_os_api os_memory_align_to_granularity_f os_memory_align_to_granularity = win32_memory_align_to_granularity;

    // os_file_size_f                   os_file_size                   = win32_file_size;
    // os_file_read_f                   os_file_read                   = win32_file_read;
    // os_file_write_f                  os_file_write                  = win32_file_write;
    // os_file_open_f                   os_file_open                   = win32_file_open;

    struct win32_context_t {
        LPWNDCLASSA window_classs;
        HGLRC       gl_context;
        HDC         device_context;
    };

    static win32_context_t& win32_context_instance    (void);
    static LPWNDCLASSA      win32_create_window_class (void);

    static bool
    win32_os_context_init(
        const os_context_graphics_t graphics) {

        

    }

    static win32_context_t&
    win32_context_instance(void) {

        static win32_context_t win32_context = {0};
        return(win32_context);
    }

    static LPWNDCLASSA 
    win32_create_window_class(void) {

        static WNDCLASSA window_class = {0};        

        window_class.hInstance     = GetModuleHandle(NULL);  
        window_class.lpszClassName = "sld::os_window_t | dx12";
        window_class.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

        return(&window_class);
    }

};
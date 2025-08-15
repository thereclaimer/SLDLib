#pragma once

#include "sld-os.hpp"
#include "sld-win32-file.cpp"
#include "sld-win32-memory.cpp"
#include "sld-win32-monitor.cpp"
#include "sld-win32-registry.cpp"
#include "sld-win32-system.cpp"
#include "sld-win32-thread.cpp"
#include "sld-win32-window.cpp"

namespace sld {

    os_system_get_cpu_info_f         os_system_get_cpu_info         = win32_system_get_cpu_info;
    os_system_get_cpu_cache_info_f   os_system_get_cpu_cache_info   = win32_system_get_cpu_cache_info;
    os_system_get_memory_info_f      os_system_get_memory_info      = win32_system_get_memory_info;
    os_system_time_ms_f              os_system_time_ms              = win32_system_time_ms;
    os_system_sleep_f                os_system_sleep                = win32_system_sleep;
    os_system_debug_print_f          os_system_debug_print          = win32_system_debug_print;

    os_monitor_count_f               os_monitor_count               = win32_monitor_count;
    os_monitor_screen_size_f         os_monitor_screen_size         = win32_monitor_screen_size;
    os_monitor_info_f                os_monitor_info                = win32_monitor_info;

    os_window_create_opengl3_f       os_window_create_opengl3       = win32_window_create_opengl3; 
    os_window_create_dx12_f          os_window_create_dx12          = win32_window_create_dx12; 
    os_window_frame_start_opengl3_f  os_window_frame_start_opengl3  = win32_window_frame_start_opengl3; 
    os_window_frame_start_dx12_f     os_window_frame_start_dx12     = win32_window_frame_start_dx12; 
    os_window_frame_render_opengl3_f os_window_frame_render_opengl3 = NULL; 
    os_window_frame_render_dx12_f    os_window_frame_render_dx12    = NULL; 
    os_window_destroy_f              os_window_destroy              = win32_window_destroy; 
    os_window_show_f                 os_window_show                 = win32_window_show; 
    os_window_get_size_f             os_window_get_size             = win32_window_get_size; 
    os_window_get_position_f         os_window_get_position         = win32_window_get_position; 
    os_window_process_events_f       os_window_process_events       = win32_window_process_events; 

    os_memory_reserve_page_table_f   os_memory_reserve_page_table   = win32_memory_reserve_page_table;
    os_memory_release_page_table_f   os_memory_release_page_table   = win32_memory_release_page_table;
    os_memory_commit_pages_f         os_memory_commit_pages         = win32_memory_commit_pages;
    os_memory_decommit_pages_f       os_memory_decommit_pages       = win32_memory_decommit_pages;

    os_file_size_f                   os_file_size                   = win32_file_size;
    os_file_read_f                   os_file_read                   = win32_file_read;
    os_file_write_f                  os_file_write                  = win32_file_write;
    os_file_open_f                   os_file_open                   = win32_file_open;
};
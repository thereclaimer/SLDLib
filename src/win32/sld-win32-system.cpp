#pragma once

#include <Windows.h>
#include "sld-os.hpp"

namespace sld {

    static bool
    win32_system_get_cpu_info(
        os_system_cpu_info_t& cpu_info) {

        const u32 parent_core_number = 
        

        cpu_info.parent_core_number  = GetCurrentProcessorNumber();
        cpu_info.speed_mhz           = ;
        cpu_info.core_count_physical = ;
        cpu_info.core_count_logical  = ;
        cpu_info.cache_levels        = ;

    }

    static bool
    win32_system_get_cpu_cache_info(
        os_system_cpu_cache_info_t& cpu_cache_info) {

    }
    
    static bool
    win32_system_get_memory_info(
        os_system_info_memory_t* memory_info) {

    }
    
    static const u64
    win32_system_time_ms(
        void) {

    }
    
    static void
    win32_system_sleep(
        const u32 ms) {

    }
    
    static void
    win32_system_debug_print(
        const c8* debug_string) {

    }
};
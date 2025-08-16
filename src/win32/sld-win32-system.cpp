#pragma once

#include <Windows.h>
#include "sld-os.hpp"

namespace sld {

    static void
    win32_system_get_cpu_info(
        os_system_cpu_info_t& cpu_info) {


    }

    static void
    win32_system_get_cpu_cache_info(
        os_system_cpu_cache_info_t& cpu_cache_info) {

    }
    
    static void
    win32_system_get_memory_info(
        os_system_memory_info_t& memory_info) {

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
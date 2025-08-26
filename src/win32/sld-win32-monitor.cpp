#pragma once

#include <Windows.h>
#include "sld-os.hpp"

namespace sld {

    static const u32
    win32_monitor_count(
        void) {

        const u32 monitor_count = GetSystemMetrics(SM_CMONITORS);

        return(monitor_count);
    }

    static void
    win32_monitor_screen_size(
        os_monitor_screen_size_t& monitor_screen_size) {

    }

    static bool
    win32_monitor_info(
        os_monitor_info_t& monitor_info) {
        
        return(false);
    }
};
#pragma once

#include "sld-win32.hpp"

namespace sld {

    bool
    win32_system_get_info(
        os_system_info_t& system_info) {

        SYSTEM_INFO win32_sys_info;
        GetSystemInfo(&win32_sys_info);

        system_info.alloc_granularity  = win32_sys_info.dwAllocationGranularity;
        system_info.page_size          = win32_sys_info.dwPageSize;
        system_info.logical_core_count = win32_sys_info.dwNumberOfProcessors;

        return(true);
    }
};
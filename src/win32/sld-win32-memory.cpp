#pragma once

#include <Windows.h>
#include "sld-os.hpp"

namespace sld {
    
    static bool
    win32_memory_reserve_page_table(
        os_memory_page_table_t& page_table) {

        void* reserve_start = NULL;

        SYSTEM_INFO sys_info;
        GetSystemInfo(&sys_info);

        page_table.granularity = sys_info.dwAllocationGranularity;
        page_table.size_actual = size_align_pow_2(page_table.size_requested, page_table.granularity);
        page_table.page_size   = sys_info.dwPageSize;
        page_table.page_count  = page_table.size_actual / page_table.page_size; 
        page_table.start       = (addr)VirtualAlloc(
            reserve_start,
            page_table.size_actual,
            MEM_RESERVE,
            PAGE_NOACCESS
        );

        const bool result = (page_table.start != 0); 
        return(result);
    }

    static bool
    win32_memory_release_page_table(
        os_memory_page_table_t& page_table) {

        const bool result = VirtualFree(
            (LPVOID)page_table.start,
            page_table.size_actual);
     
        return(result);
    }

    static bool
    win32_memory_commit_pages(
        const os_memory_page_table_t& page_table,
        os_memory_page_commit_t&      page_commit) {

        const u64  commit_size   = page_table.page_size * page_commit.page_stride;
        const u64  commit_offset = page_table.page_size * page_commit.page_number;
        const addr commit_start  = page_table.start     + commit_offset;

        page_commit.start = (addr)VirtualAlloc(
            (LPVOID)commit_start,
            commit_size,
            MEM_COMMIT,
            PAGE_READWRITE
        );

        return(page_commit.start != 0);
    }

    static bool
    win32_memory_decommit_pages(
        const os_memory_page_table_t& page_table,
        os_memory_page_commit_t&      page_commit) {

        const u64  commit_size = page_table.page_size * page_commit.page_stride;
        
        const bool result = VirtualFree(
            (LPVOID)page_commit.start,
            commit_size,
            MEM_DECOMMIT
        );

        return(result);
    }
};
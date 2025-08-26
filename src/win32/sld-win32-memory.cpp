#pragma once

#include <Windows.h>
#include "sld-os.hpp"

namespace sld {
    
    SLD_OS_API_FUNC void*
    win32_memory_reserve(
        void*     start,
        const u64 size) {

        void* memory = (void*)VirtualAlloc(
            start,
            size,
            MEM_RESERVE,
            PAGE_NOACCESS
        );

        return(memory);
    }

    SLD_OS_API_FUNC bool
    win32_memory_release(
        void*     start,
        const u64 size) {

        bool result = VirtualFree(
            start,
            size,
            MEM_RELEASE
        );

        return(result);
    }

    SLD_OS_API_FUNC void*
    win32_memory_commit(
        void*     start,
        const u64 size) {

        void* memory = (void*)VirtualAlloc(
            start,
            size,
            MEM_COMMIT,
            PAGE_READWRITE
        );

        return(memory);
    }

    SLD_OS_API_FUNC bool
    win32_memory_decommit(
        void*     start,
        const u64 size) {

        bool result = VirtualFree(
            start,
            size,
            MEM_DECOMMIT
        );

        return(result);
    }

    SLD_OS_API_FUNC u64
    win32_memory_align_to_page(
        const u64 size) {

        SLD_OS_API_FUNC SYSTEM_INFO sys_info;
        GetSystemInfo(&sys_info);

        const u64 size_aligned = size_align_pow_2(size, sys_info.dwPageSize);
        return(size_aligned);
    }

    SLD_OS_API_FUNC u64
    win32_memory_align_to_granularity(
        const u64 size) {

        SLD_OS_API_FUNC SYSTEM_INFO sys_info;
        GetSystemInfo(&sys_info);

        const u64 size_aligned = size_align_pow_2(size, sys_info.dwAllocationGranularity);
        return(size_aligned);
    }
};
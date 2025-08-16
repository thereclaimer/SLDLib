#pragma once

#include <Windows.h>
#include "sld-os.hpp"

namespace sld {
    
    static void*
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

    static bool
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

    static void*
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

    static bool
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

};
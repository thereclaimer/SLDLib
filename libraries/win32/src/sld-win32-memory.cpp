#pragma once

#include "sld-win32.hpp"

namespace sld {

    bool
    win32_memory_reserve(
        os_memory_t& memory) {

        memory.start = VirtualAlloc(
            NULL,
            memory.size,
            MEM_RESERVE,
            PAGE_NOACCESS
        );

        return(memory.start != NULL);
    }

    bool
    win32_memory_release(
        os_memory_t& memory) {

        const bool result = VirtualFree(
            memory.start,
            memory.size,
            MEM_RELEASE
        );

        return(result);
    }

    bool
    win32_memory_commit(
        os_memory_t& memory) {

        const ptr result = VirtualAlloc(
            memory.start,
            memory.size,
            MEM_COMMIT,
            PAGE_READWRITE
        );

        return(result == memory.start);
    }

    bool
    win32_memory_decommit(
        os_memory_t& memory) {

        const bool result = VirtualFree(
            memory.start,
            memory.size,
            MEM_DECOMMIT
        );

        return(result);
    }
};
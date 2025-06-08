#ifndef SLD_WIN32_HPP
#define SLD_WIN32_HPP

#include <Windows.h>

#include "sld.hpp"
#include "sld-os.hpp"

namespace sld {

    bool win32_system_info (os_system_info_t& system_info);

    //-------------------------
    // memory
    //-------------------------

    bool win32_memory_reserve  (os_memory_t& memory);
    bool win32_memory_release  (os_memory_t& memory);
    bool win32_memory_commit   (os_memory_t& memory);
    bool win32_memory_decommit (os_memory_t& memory);

};

#endif //SLD_WIN32_HPP
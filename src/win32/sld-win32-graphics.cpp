#pragma once

#include <Windows.h>
#include "sld.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    const HGLRC   win32_graphics_init_opengl (const HDC device_context);
    ImGuiContext* win32_graphics_init_imgui  (void);

    //-------------------------------------------------------------------
    // DEFINITIONS
    //-------------------------------------------------------------------


    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    static const HGLRC
    win32_graphics_init_opengl(
        const HDC device_context) {
            

    }
};
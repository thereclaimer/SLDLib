#pragma once

#include <Windows.h>

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    const HGLRC win32_opengl_init (const HDC device_context);

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    static const HGLRC
    win32_opengl_init(
        const HDC device_context) {

        //set our preferred format descriptor
        PIXELFORMATDESCRIPTOR preferred_format_descriptor = {0};
        preferred_format_descriptor.nSize      = sizeof(preferred_format_descriptor);
        preferred_format_descriptor.nVersion   = 1;
        preferred_format_descriptor.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        preferred_format_descriptor.iPixelType = PFD_TYPE_RGBA;
        preferred_format_descriptor.cColorBits = 32;
        
        // set the pixel format and initialize the context
        const s32   chosen_pixel_format   = ChoosePixelFormat (device_context, &preferred_format_descriptor);
        const bool  is_pixel_format_set   = SetPixelFormat    (device_context, chosen_pixel_format, &preferred_format_descriptor);
        const HGLRC gl_context            = wglCreateContext  (device_context);
        const bool  is_gl_context_current = wglMakeCurrent    (device_context, gl_context);

        // if initialized, return the context
        bool result = true;
        result &= is_pixel_format_set; 
        result &= is_gl_context_current; 
        return(result ? gl_context : NULL);
    };
};
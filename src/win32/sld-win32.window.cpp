#pragma once

#include <Windows.h>
#include "sld.hpp"
#include "sld-os.hpp"


namespace sld {

    WNDCLASSA* win32_window_get_class(void);

    static bool
    win32_window_create(
        os_window_t& window) {

        result = true;
 
        WNDCLASSA* window_class = win32_window_get_class();
        result &= (window_class != NULL);

        if (result) {
        
            //create the window handle
            window.handle = CreateWindowA(
                window_class->lpszClassName
                window.title,
                WS_OVERLAPPEDWINDOW,
                window.position.screen_x,
                window.position.screen_y,
                window.size.width,
                window.size.height,
                NULL,
                NULL,
                window_class->hInstance,
                NULL
            );

            result &= (window.handle != NULL);
        }

        

    }

    sld_rt_inline WNDCLASSA*
    win32_window_get_class(
        void) {

        static WNDCLASSA window_class = {0};
        static bool      registered   = false;

        if (!registered) {
        
            window_class.lpfnWndProc   = (WNDPROC)win32::window_callback;
            window_class.hInstance     = GetModuleHandle(NULL);  
            window_class.lpszClassName = "sld::os_window_t";
            window_class.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        
            registered = (RegisterClass(&window_class) == ERROR_SUCCESS);
        }

        return(registered ? &window_class : NULL);
    }
};
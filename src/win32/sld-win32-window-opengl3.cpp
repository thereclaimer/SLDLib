#pragma once

#include <Windows.h>

#include "sld-os.hpp"
#include "sld-win32-window-error.cpp"

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------


    //-------------------------------------------------------------------
    // OS API METHODS
    //-------------------------------------------------------------------

    static const os_window_error_t
    win32_window_opengl3_create(
        os_window_handle_t&         window_handle,
        const c8*                   title,
        const os_window_size_t&     size,
        const os_window_position_t& position) {

        os_window_error_t error = { os_window_error_e_success };

        // get the class
        LPWNDCLASSA window_class = win32_window_opengl3_get_class();
        if (!window_class) {
            error.val = win32_window_error_get_last();
            return(error);
        }


    }

    static const os_window_error_t
    win32_window_opengl3_update(
        const os_window_handle_t window_handle,
        os_window_update_t&      update) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

    }
    
    static inline const os_window_error_t
    win32_window_opengl3_swap_buffers(
        const os_window_handle_t window_handle) {

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
        HDC device_context = GetDC((HWND)handle);
        const bool result  = SwapBuffers(device_context);

        return(result);
    }

    //-------------------------------------------------------------------
    // INTERNAL METHODS
    //-------------------------------------------------------------------

    static inline LPWNDCLASSA 
    win32_window_opengl3_get_class(
        void) {

        static WNDCLASSA   window_class;
        static LPWNDCLASSA window_class_ptr = NULL;

        if (!window_class_ptr) {

            // register the window class
            window_class.lpfnWndProc   = (WNDPROC)win32_window_callback_opengl3;
            window_class.hInstance     = GetModuleHandle(NULL);  
            window_class.lpszClassName = "SLD Win32 DirectX12 Window";
            window_class.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

            const DWORD result = RegisterClass(&window_class);
            window_class_ptr   = (result == ERROR_SUCCESS)
                ? &window_class 
                : NULL;                
            }
        }

        return(window_class_ptr);
    }
    
    static inline LRESULT CALLBACK
    win32_window_opengl3_callback(
        HWND   handle,
        UINT   message,
        WPARAM w_param,
        LPARAM l_param) {

        return(0);
    }
};
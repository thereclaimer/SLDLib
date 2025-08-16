#pragma once

#include <Windows.h>

#include "sld.hpp"
#include "sld-os.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    WNDCLASSA*       win32_window_get_class_opengl3 (void);
    WNDCLASSA*       win32_window_get_class_dx12    (void);
    LRESULT CALLBACK win32_window_callback_opengl3  (HWND handle, UINT message, WPARAM w_param, LPARAM l_param);
    LRESULT CALLBACK win32_window_callback_dx12     (HWND handle, UINT message, WPARAM w_param, LPARAM l_param);

    //-------------------------------------------------------------------
    // OS API METHODS
    //-------------------------------------------------------------------

    static os_window_handle_t
    win32_window_create_opengl3(
        const c8*                   title,
        const os_window_size_t&     size,
        const os_window_position_t& position) {
 
        WNDCLASSA* window_class  = win32_window_get_class_opengl3();
        if (!window_class) return(NULL);
        
        os_window_handle_t window_handle = CreateWindowA(
            window_class->lpszClassName,
            title,
            WS_OVERLAPPEDWINDOW,
            position.screen_x,
            position.screen_y,
            size.width,
            size.height,
            NULL,
            NULL,
            window_class->hInstance,
            NULL
        );

        return(window_handle);
    }

    static os_window_handle_t
    win32_window_create_dx12(
        const c8*                   title,
        const os_window_size_t&     size,
        const os_window_position_t& position) {

        WNDCLASSA* window_class  = win32_window_get_class_dx12();
        if (!window_class) return(NULL);
        
        os_window_handle_t window_handle = CreateWindowA(
            window_class->lpszClassName,
            title,
            WS_OVERLAPPEDWINDOW,
            position.screen_x,
            position.screen_y,
            size.width,
            size.height,
            NULL,
            NULL,
            window_class->hInstance,
            NULL
        );

        return(window_handle);

    }

    static bool
    win32_window_destroy(
        const os_window_handle_t handle) {

    }

    static bool
    win32_window_show(
        const os_window_handle_t handle) {

        static const s32 cmd_show_true  = 1;
        const bool       result         = ShowWindow((HWND)handle, cmd_show_true);

        return(result);
    }

    static bool
    win32_window_frame_start_opengl3(
        const os_window_handle_t handle) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        return(true);
    }

    static bool
    win32_window_frame_start_dx12(
        const os_window_handle_t handle) {

        //TODO

        return(false);
    }

    static bool
    win32_window_render_opengl3(
        const os_window_handle_t handle) {

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
        HDC device_context = GetDC((HWND)handle);
        const bool result = SwapBuffers(device_context);
        return(result);
    }

    static bool
    win32_window_render_dx12(
        const os_window_handle_t handle) {
    }

    static bool
    win32_window_get_size(
        const os_window_handle_t handle,
        os_window_size_t&        size) {
        
        RECT window_rect;

        const bool result = GetWindowRect((HWND)handle, &window_rect);
        size.width  = (window_rect.right  - window_rect.left);  
        size.height = (window_rect.bottom - window_rect.top);  

        return(result);
    }

    static bool
    win32_window_get_position(
        const os_window_handle_t handle,
        os_window_position_t&    position) {

        RECT window_rect;

        const bool result = GetWindowRect((HWND)handle, &window_rect);
        position.screen_x = window_rect.left;
        position.screen_y = window_rect.top;

        return(result);
    }

    static bool
    win32_window_process_events(
        const os_window_handle_t handle,
        os_window_events_t&      events) {

        if (!handle) return(false);

        events = os_window_event_e_none;

        static const u32 filter_min = 0;
        static const u32 filter_max = 0;

        // peek first message
        MSG  window_message;
        bool process_message = PeekMessage(
                &window_message,
                (HWND)handle,
                filter_min,
                filter_max,
                PM_REMOVE);


        bool result = true;
        while (process_message) {

            // toggle os event flags
            switch (window_message.message) {
                
                case WM_KEYDOWN:       { events |= os_window_event_e_key_down;              break; } 
                case WM_SYSKEYDOWN:    { events |= os_window_event_e_key_down;              break; } 
                case WM_KEYUP:         { events |= os_window_event_e_key_up;                break; } 
                case WM_SYSKEYUP:      { events |= os_window_event_e_key_up;                break; } 
                case WM_QUIT:          { events |= os_window_event_e_quit;                  break; }
                case WM_CLOSE:         { events |= os_window_event_e_quit;                  break; }
                case WM_DESTROY:       { events |= os_window_event_e_destroyed;             break; }
                case WM_MOVE:          { events |= os_window_event_e_moved;                 break; }
                case WM_SIZE:          { events |= os_window_event_e_resized;               break; }
                case WM_MOUSEMOVE:     { events |= os_window_event_e_mouse_move;            break; }
                case WM_LBUTTONDOWN:   { events |= os_window_event_e_mouse_left_down;       break; }
                case WM_LBUTTONUP:     { events |= os_window_event_e_mouse_left_up;         break; }
                case WM_LBUTTONDBLCLK: { events |= os_window_event_e_mouse_left_dbl_click;  break; }
                case WM_RBUTTONDOWN:   { events |= os_window_event_e_mouse_right_down;      break; }
                case WM_RBUTTONUP:     { events |= os_window_event_e_mouse_right_up;        break; }
                case WM_RBUTTONDBLCLK: { events |= os_window_event_e_mouse_right_dbl_click; break; }
                default: break;
            }

            //handle the message
            result &= (bool)TranslateMessage (&window_message);
            (void)DispatchMessage      (&window_message);

            // peek next message
            process_message = PeekMessage(
                &window_message,
                (HWND)handle,
                filter_min,
                filter_max,
                PM_REMOVE);
        }
    }

    //-------------------------------------------------------------------
    // INTERNAL METHODS
    //-------------------------------------------------------------------

    sld_rt_inline WNDCLASSA*
    win32_window_get_class_opengl3(
        void) {

        static WNDCLASSA window_class = {0};
        static bool      registered   = false;

        if (!registered) {
        
            window_class.lpfnWndProc   = (WNDPROC)win32_window_callback_opengl3;
            window_class.hInstance     = GetModuleHandle(NULL);  
            window_class.lpszClassName = "sld::os_window_t | opengl3";
            window_class.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

            //set our preferred format descriptor
            PIXELFORMATDESCRIPTOR preferred_format_descriptor = {0};
            preferred_format_descriptor.nSize      = sizeof(preferred_format_descriptor);
            preferred_format_descriptor.nVersion   = 1;
            preferred_format_descriptor.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            preferred_format_descriptor.iPixelType = PFD_TYPE_RGBA;
            preferred_format_descriptor.cColorBits = 32;

            // //get the window
            // IFBWin32Window* window_ptr = ifb_win32::context_get_window();
            
            // //query for the closest format descriptor
            // const IFBS32 chosen_format_descriptor = 
            //     ChoosePixelFormat(
            //         window_ptr->device_context,
            //         &preferred_format_descriptor);

            // //set the chosen pixel format
            // const IFBB8 pixel_format_is_set = 
            //     SetPixelFormat(
            //         window_ptr->device_context,
            //         chosen_format_descriptor,
            //         &preferred_format_descriptor);

            // //create the opengl context
            // window_ptr->opengl_context = wglCreateContext(window_ptr->device_context);

            // //make the context current
            // const IFBB8 context_active = wglMakeCurrent(
            //     window_ptr->device_context,
            //     window_ptr->opengl_context);


            registered = (RegisterClass(&window_class) == ERROR_SUCCESS);
        }

        return(registered ? &window_class : NULL);
    }

    sld_rt_inline WNDCLASSA*
    win32_window_get_class_dx12(
        void) {

        static WNDCLASSA window_class = {0};
        static bool      registered   = false;

        if (!registered) {
        
            window_class.lpfnWndProc   = (WNDPROC)win32_window_callback_dx12;
            window_class.hInstance     = GetModuleHandle(NULL);  
            window_class.lpszClassName = "sld::os_window_t | dx12";
            window_class.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
        
            registered = (RegisterClass(&window_class) == ERROR_SUCCESS);
        }

        return(registered ? &window_class : NULL);
    }

    LRESULT CALLBACK
    win32_window_callback_opengl3(
        HWND   handle,
        UINT   message,
        WPARAM w_param,
        LPARAM l_param) {


    }

    LRESULT CALLBACK
    win32_window_callback_dx12(
        HWND   handle,
        UINT   message,
        WPARAM w_param,
        LPARAM l_param) {

    }
};
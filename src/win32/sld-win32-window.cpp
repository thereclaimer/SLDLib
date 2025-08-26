#pragma once

#include <Windows.h>

#include "sld-os.hpp"

#if SLD_OS_GRAPHICS_CONTEXT_TYPE == SLD_OS_GRAPHICS_CONTEXT_OPENGL
#   define win32_window_get_class    win32_window_opengl3_get_class
#   define win32_window_update       win32_window_opengl3_update
#   define win32_window_swap_buffers win32_window_opengl3_swap_buffers
#   define win32_window_get_class    win32_window_opengl3_get_class
#   define win32_window_callback     win32_window_opengl3_callback
#elif SLD_OS_GRAPHICS_CONTEXT_TYPE == SLD_OS_GRAPHICS_CONTEXT_DIRECTX12
#   define win32_window_get_class    win32_window_directx12_get_class
#   define win32_window_update       win32_window_directx12_update
#   define win32_window_swap_buffers win32_window_directx12_swap_buffers
#   define win32_window_get_class    win32_window_directx12_get_class
#   define win32_window_callback     win32_window_directx12_callback
#else
#   define win32_window_get_class    win32_window_opengl3_get_class
#   define win32_window_update       win32_window_opengl3_update
#   define win32_window_swap_buffers win32_window_opengl3_swap_buffers
#   define win32_window_get_class    win32_window_opengl3_get_class
#   define win32_window_callback     win32_window_opengl3_callback
#endif

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    // common
    const os_window_error_t  win32_window_error_get_last         (void);
    const os_window_error_t& win32_window_error_success          (void);
    const os_window_error_t  win32_window_process_events         (const os_window_handle_t handle, os_window_update_t& update);

    // directx12
    const os_window_error_t  win32_window_directx12_update       (const os_window_handle_t window_handle, os_window_update_t&   update);
    const os_window_error_t  win32_window_directx12_swap_buffers (const os_window_handle_t window_handle);
    LPWNDCLASSA              win32_window_directx12_get_class    (void);
    LRESULT CALLBACK         win32_window_directx12_callback     (HWND handle, UINT message, WPARAM w_param, LPARAM l_param);

    // opengl
    const os_window_error_t  win32_window_opengl3_update         (const os_window_handle_t window_handle, os_window_update_t&   update);
    const os_window_error_t  win32_window_opengl3_swap_buffers   (const os_window_handle_t window_handle);
    LPWNDCLASSA              win32_window_opengl3_get_class      (void);
    LRESULT CALLBACK         win32_window_opengl3_callback       (HWND handle, UINT message, WPARAM w_param, LPARAM l_param);

    //-------------------------------------------------------------------
    // OS API METHODS
    //-------------------------------------------------------------------

    static const os_window_error_t
    win32_window_create(
        os_window_handle_t&         window_handle,
        const c8*                   title,
        const os_window_size_t&     size,
        const os_window_position_t& position) {

        os_window_error_t error = {os_window_error_e_success};

        // get the glass
        LPWNDCLASSA window_class = win32_window_get_class();
        if (!window_class) {
            error = win32_window_error_get_last();
            return(error);
        }

        // create the window
        window_handle.val = CreateWindowA(
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

        // return the error code
        error = (window_handle.val == INVALID_HANDLE_VALUE)
            ? win32_window_error_get_last ()
            : win32_window_error_success  ();
        return(error);
    }

    static bool
    win32_window_destroy(
        const os_window_handle_t handle) {

        return(false);
    }

    static bool
    win32_window_show(
        const os_window_handle_t handle) {

        static const s32 cmd_show_true  = 1;
        const bool       result         = ShowWindow((HWND)handle.val, cmd_show_true);

        return(result);
    }

    static bool
    win32_window_get_size(
        const os_window_handle_t handle,
        os_window_size_t&        size) {
        
        RECT window_rect;

        const bool result = GetWindowRect((HWND)handle.val, &window_rect);
        size.width  = (window_rect.right  - window_rect.left);  
        size.height = (window_rect.bottom - window_rect.top);  

        return(result);
    }

    static bool
    win32_window_get_position(
        const os_window_handle_t handle,
        os_window_position_t&    position) {

        RECT window_rect;

        const bool result = GetWindowRect((HWND)handle.val, &window_rect);
        position.screen_x = window_rect.left;
        position.screen_y = window_rect.top;

        return(result);
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    static inline const os_window_error_t
    win32_window_error_get_last(
        void) {
        
        os_window_error_t error;

        const DWORD win32_error = GetLastError();
        switch (win32_error) {

            case (ERROR_FILE_NOT_FOUND):          { error.val = os_window_error_e_resource_not_found;     break; } // Resource file (icon/cursor/etc.) not found.
            case (ERROR_ACCESS_DENIED):           { error.val = os_window_error_e_access_denied;          break; } // Operation not permitted (e.g., cross-process window manipulation).
            case (ERROR_NOT_ENOUGH_MEMORY):       { error.val = os_window_error_e_system_out_of_memory;   break; } // System ran out of memory.
            case (ERROR_OUTOFMEMORY):             { error.val = os_window_error_e_general_out_of_memory;  break; } // Out of memory (general resource exhaustion).
            case (ERROR_INVALID_PARAMETER):       { error.val = os_window_error_e_invalid_args;           break; } // Invalid argument/flag passed to an API.
            case (ERROR_INVALID_WINDOW_HANDLE):   { error.val = os_window_error_e_invalid_handle;         break; } // The window handle (HWND) is invalid.
            case (ERROR_CANNOT_FIND_WND_CLASS):   { error.val = os_window_error_e_invalid_class;          break; } // Window class not registered.
            case (ERROR_CLASS_ALREADY_EXISTS):    { error.val = os_window_error_e_class_already_exists;   break; } // Attempted to register an already-registered window class.
            case (ERROR_DC_NOT_FOUND):            { error.val = os_window_error_e_invalid_device_context; break; } // Invalid or already released device context (DC).
            case (ERROR_INVALID_THREAD_ID):       { error.val = os_window_error_e_invalid_thread;         break; } // Thread ID is invalid / has no message queue.
            case (ERROR_RESOURCE_NAME_NOT_FOUND): { error.val = os_window_error_e_invalid_resource;       break; } // Resource (cursor, icon, etc.) not found.
            case (ERROR_NOT_ENOUGH_QUOTA):        { error.val = os_window_error_e_quota_exceeded;         break; } // System/user quota limit exceeded.
            default:                              { error.val = os_window_error_e_unknown;                break; } // General/unknown error
        }

        return(error);
    }

    static inline const os_window_error_t&
    win32_window_error_success(
        void) {

        static const os_window_error_t error = { os_window_error_e_success };
        return(error);
    }

    static inline const os_window_error_t
    win32_window_process_events(
        const os_window_handle_t handle,
        os_window_update_t&      update) {

        static const u32 filter_min = 0;
        static const u32 filter_max = 0;

        os_input_keyboard_reset(update.input.keyboard);

        update.events = os_window_event_e_none;

        // peek first message
        MSG  window_message;
        bool can_process_message = PeekMessage(
                &window_message,
                (HWND)handle,
                filter_min,
                filter_max,
                PM_REMOVE);


        bool result = true;
        while (can_process_message) {

            // toggle os event flags
            switch (window_message.message) {
                
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN: {

                    events |= os_window_event_e_key_down; 
                    
                    const os_input_keycode_t keycode = win32_input_translate_keycode(
                        window_message.wParam,
                        window_message.lParam);

                    os_input_keyboard_add_key_down(update.input.keyboard);

                } break;

                case WM_KEYUP:
                case WM_SYSKEYUP: {
                
                    events |= os_window_event_e_key_up;
                    
                    const os_input_keycode_t keycode = win32_input_translate_keycode(
                        window_message.wParam,
                        window_message.lParam);

                    os_input_keyboard_add_key_up(update.input.keyboard);

                } break;

                case WM_QUIT:
                case WM_CLOSE: {
                    events |= os_window_event_e_quit;
                } break;

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
            can_process_message = PeekMessage(
                &window_message,
                (HWND)handle,
                filter_min,
                filter_max,
                PM_REMOVE);
        }  
    }
};
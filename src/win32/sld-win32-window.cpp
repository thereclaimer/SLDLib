#pragma once

#include <Windows.h>
#include "sld-os.hpp"

#if SLD_OS_GRAPHICS_CONTEXT_TYPE == SLD_OS_GRAPHICS_CONTEXT_OPENGL
#   define win32_window_create       win32_window_opengl3_create
#   define win32_window_update       win32_window_opengl3_update
#   define win32_window_swap_buffers win32_window_opengl3_swap_buffers
#elif SLD_OS_GRAPHICS_CONTEXT_TYPE == SLD_OS_GRAPHICS_CONTEXT_DIRECTX12
#   define win32_window_create       win32_window_directx12_create
#   define win32_window_update       win32_window_directx12_update
#   define win32_window_swap_buffers win32_window_directx12_swap_buffers
#else
#   define win32_window_create       win32_window_opengl3_create
#   define win32_window_update       win32_window_opengl3_update
#   define win32_window_swap_buffers win32_window_opengl3_swap_buffers
#endif

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    struct win32_window_message_peek_args_t {
        HWND window_handle;
        MSG  message;
        u32  filter_min;
        u32  filter_max;
    };

    // common
    const os_window_error_t  win32_window_error_get_last (void);
    const os_window_error_t  win32_window_error_success  (void);
    const os_window_error_t  win32_window_process_events (const os_window_handle_t handle, os_window_update_t& update);
    bool                     win32_window_peek_message   (win32_window_message_peek_args_t& peek_args);

    //-------------------------------------------------------------------
    // OS API
    //-------------------------------------------------------------------

    SLD_OS_API_FUNC const os_window_error_t
    win32_window_destroy(
        const os_window_handle_t handle) {

        const os_window_error_t error = win32_window_error_success();

        return(error);
    }

    SLD_OS_API_FUNC const os_window_error_t
    win32_window_show(
        const os_window_handle_t handle) {

        static const s32 cmd_show_true  = 1;
        const bool       result         = ShowWindow((HWND)handle.val, cmd_show_true);

        const os_window_error_t error = (result == true) 
            ? win32_window_error_success  ()
            : win32_window_error_get_last ();

        return(error);
    }

    SLD_OS_API_FUNC const os_window_error_t
    win32_window_get_size(
        const os_window_handle_t handle,
        os_window_size_t&        size) {
        
        RECT window_rect;

        const bool result = GetWindowRect((HWND)handle.val, &window_rect);
        size.width  = (window_rect.right  - window_rect.left);  
        size.height = (window_rect.bottom - window_rect.top);  

        const os_window_error_t error = (result == true) 
            ? win32_window_error_success  ()
            : win32_window_error_get_last ();

        return(error);
    }

    SLD_OS_API_FUNC const os_window_error_t
    win32_window_get_position(
        const os_window_handle_t handle,
        os_window_position_t&    position) {

        RECT window_rect;

        const bool result = GetWindowRect((HWND)handle.val, &window_rect);
        position.screen_x = window_rect.left;
        position.screen_y = window_rect.top;

        const os_window_error_t error = (result == true) 
            ? win32_window_error_success  ()
            : win32_window_error_get_last ();

        return(error);
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_OS_API_INTERNAL const os_window_error_t
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

    SLD_OS_API_INTERNAL const os_window_error_t
    win32_window_error_success(
        void) {

        SLD_OS_API_FUNC const os_window_error_t error = { os_window_error_e_success };
        return(error);
    }

    SLD_OS_API_INTERNAL const os_window_error_t
    win32_window_process_events(
        const os_window_handle_t handle,
        os_window_update_t&      update) {

        os_window_error_t error = win32_window_error_success();

        os_input_keyboard_reset(update.input.keyboard);

        update.events.val = os_window_event_e_none;

        win32_window_message_peek_args_t msg_peek_args;
        msg_peek_args.filter_min    = 0;
        msg_peek_args.filter_max    = 0;
        msg_peek_args.window_handle = (HWND)handle.val;

        for (
            bool has_message = win32_window_peek_message(msg_peek_args);
            has_message == true;
            has_message = win32_window_peek_message(msg_peek_args)) {

            // toggle os event flags
            switch (msg_peek_args.message.message) {
                
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN: {

                    update.events.val |= os_window_event_e_key_down; 
                    
                    const os_input_keycode_t keycode = win32_input_translate_keycode(
                        msg_peek_args.message.wParam,
                        msg_peek_args.message.lParam);

                    os_input_keyboard_add_key_down(update.input.keyboard, keycode);

                } break;

                case WM_KEYUP:
                case WM_SYSKEYUP: {
                
                    update.events.val |= os_window_event_e_key_up;
                    
                    const os_input_keycode_t keycode = win32_input_translate_keycode(
                        msg_peek_args.message.wParam,
                        msg_peek_args.message.lParam);

                    os_input_keyboard_add_key_up(update.input.keyboard, keycode);

                } break;

                case WM_QUIT:
                case WM_CLOSE: {
                    update.events.val |= os_window_event_e_quit;
                } break;

                case WM_DESTROY:       { update.events.val |= os_window_event_e_destroyed;             break; }
                case WM_MOVE:          { update.events.val |= os_window_event_e_moved;                 break; }
                case WM_SIZE:          { update.events.val |= os_window_event_e_resized;               break; }
                case WM_MOUSEMOVE:     { update.events.val |= os_window_event_e_mouse_move;            break; }
                case WM_LBUTTONDOWN:   { update.events.val |= os_window_event_e_mouse_left_down;       break; }
                case WM_LBUTTONUP:     { update.events.val |= os_window_event_e_mouse_left_up;         break; }
                case WM_LBUTTONDBLCLK: { update.events.val |= os_window_event_e_mouse_left_dbl_click;  break; }
                case WM_RBUTTONDOWN:   { update.events.val |= os_window_event_e_mouse_right_down;      break; }
                case WM_RBUTTONUP:     { update.events.val |= os_window_event_e_mouse_right_up;        break; }
                case WM_RBUTTONDBLCLK: { update.events.val |= os_window_event_e_mouse_right_dbl_click; break; }
                default: break;
            }

            // handle the message
            (void)TranslateMessage (&msg_peek_args.message);
            (void)DispatchMessage  (&msg_peek_args.message);
        }

        return(error);
    }

    SLD_OS_API_INTERNAL bool
    win32_window_peek_message(
        win32_window_message_peek_args_t& peek_args) {

        const bool result = PeekMessage(
            &peek_args.message,
            peek_args.window_handle,
            peek_args.filter_min,
            peek_args.filter_min,
            PM_REMOVE
        );

        return(result);
    }
};
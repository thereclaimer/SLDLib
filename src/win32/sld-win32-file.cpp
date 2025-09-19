#pragma once

#include <Windows.h>
#include "sld-os.hpp"

#ifdef     SLD_OS_FILE_ASYNC_CONTEXT_SIZE
#   undef  SLD_OS_FILE_ASYNC_CONTEXT_SIZE
#   define SLD_OS_FILE_ASYNC_CONTEXT_SIZE sizeof(OVERLAPPED)
#endif

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    struct win32_file_args_t {
        LPSECURITY_ATTRIBUTES security;
        HANDLE                template_handle;
        DWORD                 access;
        DWORD                 share;
        DWORD                 mode;
        DWORD                 flags;
    };

    void                   win32_file_async_callback (DWORD error_code, DWORD bytes_transferred, LPOVERLAPPED overlapped);
    const os_file_error_t  win32_file_get_error_code (const DWORD error_code);
    const os_file_error_t& win32_file_error_success  (void);

    //-------------------------------------------------------------------
    // OS API
    //-------------------------------------------------------------------

    SLD_OS_API_FUNC const os_file_error_t
    win32_file_open(
        os_file_handle_t&       file_handle,
        const c8*               path,
        const os_file_config_t& config) {

        win32_file_args_t file_args = {0};
        file_args.access          = 0;
        file_args.security        = NULL;
        file_args.template_handle = NULL;
        file_args.flags           = FILE_ATTRIBUTE_NORMAL; 

        // access
        const bool access_read  = (config.access_flags.val & os_file_access_flag_e_read);
        const bool access_write = (config.access_flags.val & os_file_access_flag_e_write);
        if (access_read)  file_args.access |= GENERIC_READ;
        if (access_write) file_args.access |= GENERIC_WRITE;

        // share        
        const bool share_read   = (config.share_flags.val & os_file_share_flag_e_read);
        const bool share_write  = (config.share_flags.val & os_file_share_flag_e_write);
        const bool share_delete = (config.share_flags.val & os_file_share_flag_e_delete);
        if (share_read)   file_args.share  |= FILE_SHARE_READ;
        if (share_write)  file_args.share  |= FILE_SHARE_WRITE;
        if (share_delete) file_args.share  |= FILE_SHARE_DELETE;

        // mode
        const bool mode_create_new         = config.mode.val & os_file_mode_e_create_new;
        const bool mode_open_existing      = config.mode.val & os_file_mode_e_open_existing;
        const bool mode_open_always        = config.mode.val & os_file_mode_e_open_always;
        const bool mode_overwrite_existing = config.mode.val & os_file_mode_e_overwrite_existing;

        switch (config.mode.val) {
            case(os_file_mode_e_create_new):          file_args.mode = CREATE_NEW;    break;
            case(os_file_mode_e_open_existing):       file_args.mode = OPEN_EXISTING; break;
            case(os_file_mode_e_open_always):         file_args.mode = OPEN_ALWAYS;   break;
            case(os_file_mode_e_overwrite_existing):  file_args.mode = CREATE_ALWAYS; break;
            default:                                  file_args.mode = CREATE_NEW;    break;
        }

        // async
        if (config.is_async) file_args.flags |= FILE_FLAG_OVERLAPPED;

        // create file
        const HANDLE win32_handle = CreateFile(
            path,
            file_args.access,
            file_args.share,
            file_args.security,
            file_args.mode,
            file_args.flags,
            file_args.template_handle
        );
        file_handle.val = win32_handle;

        const DWORD win32_error = GetLastError();

        const os_file_error_t error = (file_handle.val != INVALID_HANDLE_VALUE)
            ? win32_file_error_success  ()
            : win32_file_get_error_code(win32_error);
        return(error);
    }

    SLD_OS_API_FUNC const os_file_error_t
    win32_file_size(
        const os_file_handle_t handle,
        u64&                   size) {

        HANDLE         win32_handle         = handle.val;
        static LPDWORD win32_file_size_high = NULL;
        
        size = GetFileSize(
            win32_handle,
            win32_file_size_high);

        const DWORD win32_error = GetLastError();

        const os_file_error_t error = (size != INVALID_FILE_SIZE)
            ? win32_file_error_success()
            : win32_file_get_error_code(win32_error);
        return(error);
    }

    SLD_OS_API_FUNC const os_file_error_t
    win32_file_read(
        const os_file_handle_t handle,
        os_file_buffer_t&      buffer) {

        OVERLAPPED overlapped;
        overlapped.Offset = buffer.cursor;

        LPVOID    lp_buffer     = (LPVOID)&buffer.data[buffer.length];
        const u32 bytes_to_read = buffer.size - buffer.length;

        BOOL result = ReadFile(
            (HANDLE)handle.val,           // hFile
            (LPVOID)lp_buffer,            // lpBuffer
            bytes_to_read,                // nNumberOfBytesToRead
            (LPDWORD)&buffer.transferred, // lpNumberOfBytesRead
            &overlapped                   // lpOverlapped
        );

        buffer.length += buffer.transferred;

        const DWORD win32_error = GetLastError();

        const os_file_error_t error = (result)
            ? win32_file_error_success()
            : win32_file_get_error_code(win32_error);
        return(error);   
    }

    SLD_OS_API_FUNC const os_file_error_t
    win32_file_write(
        const os_file_handle_t handle,
        os_file_buffer_t&      buffer) {

        OVERLAPPED overlapped;
        ZeroMemory(&overlapped, sizeof(overlapped));
        overlapped.Offset = buffer.cursor;

        const HANDLE win32_handle = (HANDLE)handle.val;

        // write the file
        const BOOL result = WriteFile(
            win32_handle,                 // hFile,
            (LPVOID)buffer.data,          // lpBuffer,
            buffer.length,                // nNumberOfBytesToWrite,
            (LPDWORD)&buffer.transferred, // lpNumberOfBytesWritten,
            &overlapped                   // lpOverlapped
        );

        // get the last error
        DWORD      win32_error   = GetLastError();
        const bool io_is_pending = !result && (win32_error == ERROR_IO_PENDING); 
        
        // if its pending, try waiting
        if (io_is_pending) {
            const DWORD wait_result = WaitForSingleObject((HANDLE)handle.val, INFINITE);
            win32_error = (wait_result == WAIT_OBJECT_0) 
                ? ERROR_SUCCESS
                : GetLastError();
        }
        // otherwise, get the last error
        else {
            win32_error = result ? ERROR_SUCCESS : GetLastError(); 
        }

        // translate the error
        const os_file_error_t error = win32_file_get_error_code(win32_error);
        return(error);
    }

    SLD_OS_API_FUNC const os_file_error_t
    win32_file_read_async(
        const os_file_handle_t   handle,
        os_file_buffer_t&        buffer,
        os_file_async_context_t& context) {

        LPOVERLAPPED overlapped = (LPOVERLAPPED)context.os->data;
        overlapped->Pointer     = (PVOID)context.callback;
        overlapped->Offset      = buffer.cursor;

        const bool result = ReadFileEx(
            handle.val,               // hFile 
            (LPVOID)buffer.data,      // lpBuffer 
            buffer.length,            // nNumberOfBytesToRead 
            overlapped,               // lpOverlapped 
            win32_file_async_callback // lpCompletionRoutine 
        );

        const DWORD win32_error = GetLastError();

        const os_file_error_t error = (result)
            ? win32_file_error_success()
            : win32_file_get_error_code(win32_error);
        return(error);
    }

    SLD_OS_API_FUNC const os_file_error_t
    win32_file_write_async(
        const os_file_handle_t   handle,
        os_file_buffer_t&        buffer,
        os_file_async_context_t& context) {

        LPOVERLAPPED overlapped = (LPOVERLAPPED)context.os->data;
        overlapped->Pointer     = (PVOID)&context.callback;
        overlapped->Offset      = buffer.cursor;

        const bool result = WriteFileEx(
            handle.val,               // hFile 
            (LPCVOID)buffer.data,     // lpBuffer 
            buffer.length,            // nNumberOfBytesToWrite 
            overlapped,               // lpOverlapped 
            win32_file_async_callback // lpCompletionRoutine 
        );

        const DWORD win32_error = GetLastError();

        const os_file_error_t error = (result)
            ? win32_file_error_success()
            : win32_file_get_error_code(win32_error);
        return(error);
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_OS_API_INTERNAL const os_file_error_t
    win32_file_get_error_code(
        const DWORD win32_error) {

        os_file_error_t error;

        switch (win32_error) {
            case (ERROR_SUCCESS):              { error.val = os_file_error_e_success;             return(error); }
            case (ERROR_INVALID_PARAMETER):    { error.val = os_file_error_e_invalid_args;        return(error); }
            case (ERROR_INVALID_HANDLE):       { error.val = os_file_error_e_invalid_handle;      return(error); }
            case (ERROR_SECTOR_NOT_FOUND):     { error.val = os_file_error_e_invalid_disk;        return(error); }
            case (ERROR_DEVICE_NOT_CONNECTED): { error.val = os_file_error_e_invalid_device;      return(error); }
            case (ERROR_INVALID_USER_BUFFER):  { error.val = os_file_error_e_invalid_buffer;      return(error); }
            case (ERROR_FILE_INVALID):         { error.val = os_file_error_e_invalid_file;        return(error); }
            case (ERROR_SHARING_VIOLATION):    { error.val = os_file_error_e_sharing_violation;   return(error); }
            case (ERROR_ALREADY_EXISTS):       { error.val = os_file_error_e_already_exists;      return(error); }
            case (ERROR_FILE_EXISTS):          { error.val = os_file_error_e_already_exists;      return(error); }
            case (ERROR_FILE_NOT_FOUND):       { error.val = os_file_error_e_not_found;           return(error); }
            case (ERROR_ACCESS_DENIED):        { error.val = os_file_error_e_access_denied;       return(error); }
            case (ERROR_PIPE_BUSY):            { error.val = os_file_error_e_pipe_busy;           return(error); }
            case (ERROR_HANDLE_EOF):           { error.val = os_file_error_e_reached_end_of_file; return(error); }
            case (ERROR_BROKEN_PIPE):          { error.val = os_file_error_e_broken_pipe;         return(error); }
            case (ERROR_NO_DATA):              { error.val = os_file_error_e_no_data;             return(error); }
            case (ERROR_MORE_DATA):            { error.val = os_file_error_e_more_data;           return(error); }
            case (ERROR_IO_INCOMPLETE):        { error.val = os_file_error_e_io_incomplete;       return(error); }
            case (ERROR_IO_PENDING):           { error.val = os_file_error_e_io_pending;          return(error); }
            case (ERROR_OPERATION_ABORTED):    { error.val = os_file_error_e_operation_aborted;   return(error); }
            case (ERROR_CRC):                  { error.val = os_file_error_e_disk_io_failure;     return(error); }
            case (ERROR_DISK_CORRUPT):         { error.val = os_file_error_e_disk_corrupt;        return(error); }
            case (ERROR_NOT_READY):            { error.val = os_file_error_e_device_not_ready;    return(error); }
            case (ERROR_GEN_FAILURE):          { error.val = os_file_error_e_device_failure;      return(error); }
            case (ERROR_NOT_ENOUGH_MEMORY):    { error.val = os_file_error_e_out_of_memory;       return(error); }
            default:                           { error.val = os_file_error_e_unknown;             return(error); }
        }

        return(error);
    }

    SLD_OS_API_INTERNAL const os_file_error_t&
    win32_file_error_success(
        void) {

        static const os_file_error_t error = { os_file_error_e_success };
        return(error);
    }

    SLD_OS_API_INTERNAL void
    win32_file_async_callback(
        DWORD        error_code,
        DWORD        bytes_transferred,
        LPOVERLAPPED overlapped) {

        if (!overlapped) return;

        auto context = (os_file_callback_context_t*)overlapped->Pointer;

        const os_file_error_t error = win32_file_get_error_code(error_code);

        context->func(context->data, error, bytes_transferred);
    }
};
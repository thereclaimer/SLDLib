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

    static DWORD _win32_file_last_error = ERROR_SUCCESS; 

    struct win32_file_args_t {
        LPSECURITY_ATTRIBUTES security;
        HANDLE                template_handle;
        DWORD                 access;
        DWORD                 share;
        DWORD                 on_create;
        DWORD                 flags;
    };

    void                  win32_file_async_callback (DWORD error_code, DWORD bytes_transferred, LPOVERLAPPED overlapped);
    const os_file_error_t win32_file_get_error_code (const DWORD win32_error);    

    //-------------------------------------------------------------------
    // OS API
    //-------------------------------------------------------------------

    const os_file_error_t
    win32_file_last_error(
        void) {

        const os_file_error_t error = win32_file_get_error_code(_win32_file_last_error);
        return(error);
    }

    const os_file_handle_t
    win32_file_open(
        const c8*             path,
        const os_file_flags_t flags) {

        win32_file_args_t file_args = {0};
        file_args.security  = NULL;
        file_args.template_handle  = NULL;
        file_args.on_create = CREATE_NEW;
        file_args.flags     = FILE_ATTRIBUTE_NORMAL; 

        // access
        if (flags & os_file_flag_e_read)  file_args.access |= GENERIC_READ;
        if (flags & os_file_flag_e_write) file_args.access |= GENERIC_WRITE;

        // share        
        if (flags & os_file_flag_e_share_read)   file_args.share  |= FILE_SHARE_READ;
        if (flags & os_file_flag_e_share_write)  file_args.share  |= FILE_SHARE_WRITE;
        if (flags & os_file_flag_e_share_delete) file_args.share  |= FILE_SHARE_DELETE;

        // on create
        const bool existing      = (flags & os_file_flag_e_open_existing);
        const bool overwrite     = (flags & os_file_flag_e_overwrite);
        const bool create_always = (overwrite  && !existing);
        const bool open_existing = (!overwrite && existing);
        const bool open_always   = (overwrite  && existing); 
        if      (create_always)  file_args.on_create = CREATE_ALWAYS;
        else if (open_existing)  file_args.on_create = OPEN_EXISTING;
        else if (open_always)    file_args.on_create = OPEN_ALWAYS;

        // flags
        if (flags & os_file_flag_e_async) file_args.flags |= FILE_FLAG_OVERLAPPED;

        // create file
        const os_file_handle_t handle = (os_file_handle_t)CreateFile(
            path,
            file_args.access,
            file_args.share,
            file_args.security,
            file_args.on_create,
            file_args.flags,
            file_args.template_handle
        );

        _win32_file_last_error = GetLastError();

        return(handle);
    }

    bool
    win32_file_size(
        const os_file_handle_t handle,
        u64&                   size) {

        HANDLE         win32_handle         = (os_file_handle_t)handle;
        static LPDWORD win32_file_size_high = NULL;
        
        size = GetFileSize(
            win32_handle,
            win32_file_size_high);

        _win32_file_last_error = GetLastError();

        const bool result = (size != INVALID_FILE_SIZE);        
        return(result);
    }

    const u64
    win32_file_read(
        const os_file_handle_t handle,
        os_file_buffer_t&      buffer) {

        DWORD bytes_read = 0;

        OVERLAPPED overlapped;
        overlapped.Offset = buffer.offset;

        BOOL result = ReadFile(
            (HANDLE)handle,      // hFile
            (LPVOID)buffer.data, // lpBuffer
            buffer.length,       // nNumberOfBytesToRead
            &bytes_read,         // lpNumberOfBytesRead
            &overlapped          // lpOverlapped
        );

        if (!result) {
            bytes_read = 0;
        }

        _win32_file_last_error = GetLastError();

        return(bytes_read);    
    }

    const u64
    win32_file_write(
        const os_file_handle_t handle,
        os_file_buffer_t&      buffer) {

        DWORD bytes_written = 0;

        OVERLAPPED overlapped;
        overlapped.Offset = buffer.offset;

        const BOOL result = WriteFile(
            (HANDLE)handle,       // hFile,
            (LPCVOID)buffer.data, // lpBuffer,
            buffer.length,        // nNumberOfBytesToWrite,
            &bytes_written,       // lpNumberOfBytesWritten,
            &overlapped           // lpOverlapped
        );

        if (!result) {
            bytes_written = 0;
        }

        _win32_file_last_error = GetLastError();

        return(bytes_written);
    }

    bool
    win32_file_read_async(
        const os_file_handle_t   handle,
        os_file_buffer_t&        buffer,
        os_file_async_context_t& async_context) {

        LPOVERLAPPED overlapped = (LPOVERLAPPED)async_context.os_data;
        overlapped->Pointer     = (PVOID)&async_context;
        overlapped->Offset      = buffer.offset;
        async_context.handle    = handle;

        const bool result = ReadFileEx(
            handle,                   // hFile 
            (LPVOID)buffer.data,      // lpBuffer 
            buffer.length,            // nNumberOfBytesToRead 
            overlapped,               // lpOverlapped 
            win32_file_async_callback // lpCompletionRoutine 
        );

        _win32_file_last_error = GetLastError();

        return(result);
    }

    bool
    win32_file_write_async(
        const os_file_handle_t   handle,
        os_file_buffer_t&        buffer,
        os_file_async_context_t& async_context) {

        LPOVERLAPPED overlapped = (LPOVERLAPPED)async_context.os_data;
        overlapped->Pointer     = (PVOID)&async_context;
        overlapped->Offset      = buffer.offset;
        async_context.handle    = handle;

        const bool result = WriteFileEx(
            handle,                   // hFile 
            (LPCVOID)buffer.data,     // lpBuffer 
            buffer.length,            // nNumberOfBytesToWrite 
            overlapped,               // lpOverlapped 
            win32_file_async_callback // lpCompletionRoutine 
        );

        _win32_file_last_error = GetLastError();

        return(result);

    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    sld_rt_inline const os_file_error_t
    win32_file_get_error_code(
        const DWORD win32_error) {

        switch (_win32_file_last_error) {
            case (ERROR_SUCCESS):              return(os_file_error_e_success);
            case (ERROR_INVALID_PARAMETER):    return(os_file_error_e_invalid_args);
            case (ERROR_INVALID_HANDLE):       return(os_file_error_e_invalid_handle);
            case (ERROR_SECTOR_NOT_FOUND):     return(os_file_error_e_invalid_disk);
            case (ERROR_DEVICE_NOT_CONNECTED): return(os_file_error_e_invalid_device);
            case (ERROR_INVALID_USER_BUFFER):  return(os_file_error_e_invalid_buffer);
            case (ERROR_FILE_INVALID):         return(os_file_error_e_invalid_file);
            case (ERROR_SHARING_VIOLATION):    return(os_file_error_e_sharing_violation);
            case (ERROR_ALREADY_EXISTS):       return(os_file_error_e_already_exists);
            case (ERROR_FILE_EXISTS):          return(os_file_error_e_already_exists);
            case (ERROR_FILE_NOT_FOUND):       return(os_file_error_e_not_found);
            case (ERROR_ACCESS_DENIED):        return(os_file_error_e_access_denied);
            case (ERROR_PIPE_BUSY):            return(os_file_error_e_pipe_busy);
            case (ERROR_HANDLE_EOF):           return(os_file_error_e_reached_end_of_file);
            case (ERROR_BROKEN_PIPE):          return(os_file_error_e_broken_pipe);
            case (ERROR_NO_DATA):              return(os_file_error_e_no_data);
            case (ERROR_MORE_DATA):            return(os_file_error_e_more_data);
            case (ERROR_IO_INCOMPLETE):        return(os_file_error_e_io_incomplete);
            case (ERROR_IO_PENDING):           return(os_file_error_e_io_pending);
            case (ERROR_OPERATION_ABORTED):    return(os_file_error_e_operation_aborted);
            case (ERROR_CRC):                  return(os_file_error_e_disk_io_failure);
            case (ERROR_DISK_CORRUPT):         return(os_file_error_e_disk_corrupt);
            case (ERROR_NOT_READY):            return(os_file_error_e_device_not_ready);
            case (ERROR_GEN_FAILURE):          return(os_file_error_e_device_failure);
            case (ERROR_NOT_ENOUGH_MEMORY):    return(os_file_error_e_out_of_memory);
            default:                           return(os_file_error_e_unknown);
        }

    }

    sld_rt_inline void
    win32_file_async_callback(
        DWORD        error_code,
        DWORD        bytes_transferred,
        LPOVERLAPPED overlapped) {

        if (!overlapped) return;

        auto context = (os_file_async_context_t*)overlapped->Pointer;

        context->bytes_transferred = bytes_transferred;
        context->error             = win32_file_get_error_code(error_code);

        context->callback(context);
    }
};
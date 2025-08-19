#pragma once

#include <Windows.h>
#include "sld-os.hpp"

namespace sld {

    static bool
    win32_file_open(
        const u32  count,
        os_file_t* file) {

        const bool result = true;
        result &= (count != 0);
        result &= (file  != NULL);
        if (!result) return(false);

        LPSECURITY_ATTRIBUTES file_security   = NULL;
        HANDLE                file_template   = NULL;

        for (
            u32 index = 0;
                index < count;
              ++index) {

            os_file_t& f = file[index];

            DWORD file_access = 0;
            if (f.flags & os_file_flag_e_read)         file_access |= GENERIC_READ;
            if (f.flags & os_file_flag_e_write)        file_access |= GENERIC_WRITE;
            
            DWORD file_share = 0;
            if (f.flags & os_file_flag_e_share_read)   file_share  |= FILE_SHARE_READ;
            if (f.flags & os_file_flag_e_share_write)  file_share  |= FILE_SHARE_WRITE;
            if (f.flags & os_file_flag_e_share_delete) file_share  |= FILE_SHARE_DELETE;

            const bool existing      = (f.flags & os_file_flag_e_open_existing);
            const bool overwrite     = (f.flags & os_file_flag_e_overwrite);
            const bool create_always = (overwrite  && !existing);
            const bool open_existing = (!overwrite && existing);
            const bool open_always   = (overwrite  && existing); 

            DWORD file_on_create = CREATE_NEW;
            if      (create_always)  file_on_create = CREATE_ALWAYS;
            else if (open_existing)  file_on_create = OPEN_EXISTING
            else if (open_always)    file_on_create = OPEN_ALWAYS

            DWORD file_attributes = FILE_ATTRIBUTE_NORMAL;
            if (f.flags & os_file_flag_e_async) file_attributes |= FILE_FLAG_OVERLAPPED;

            f.handle = CreateFile(
                f.path,
                file_access,
                file_share,
                file_security,
                file_on_create,
                file_attributes,
                file_template
            );
        }
    }

    static bool
    win32_file_size(
        const u32     count,
        os_file_io_t* file_io) {

        bool result = true;
        result &= (count   != 0);
        result &= (file_io != NULL); 
        if (!result) return(false);

        for (
            u32 index = 0;
                index < count;
              ++index) {
        
            os_file_io_t& io = file_io[index];

            io.size = GetFileSize(io.handle, NULL);
        }

        return(result);
    }

    static bool
    win32_file_read(
        const u32     count,
        os_file_io_t* file_io) {

        bool result = true;
        result &= (count   != 0);
        result &= (file_io != NULL); 
        if (!result) return(false);

        for (

        )
    }

    static bool
    win32_file_write(
        const u32     count,
        os_file_io_t* file_io) {

    }
};
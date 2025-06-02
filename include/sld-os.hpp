#ifndef SLD_OS_HPP
#define SLD_OS_HPP

#include "sld.hpp"

namespace sld {

    typedef void* os_handle;

    struct os_system_info_t {
        u32 page_size;
        u32 alloc_granularity;
        u32 logical_core_count;
    };

    using os_system_get_info_f = void (os_system_info_t& system_info);

    struct os_memory_t {
        ptr start;
        u64 size;
    };

    using os_memory_reserve_f  = bool (os_memory_t& memory);
    using os_memory_release_f  = bool (os_memory_t& memory);
    using os_memory_commit_f   = bool (os_memory_t& memory);
    using os_memory_decommit_f = bool (os_memory_t& memory);

    struct os_file_open_t {
        os_handle handle;
        u64       error;
        c8*       path_cstr;
    };

    struct os_file_io_t {
        os_handle handle;
        u32       error;
        byte*     buffer;
        u32       size;
        u32       length;
        u32       offset;
    };

    struct os_file_close_t {
        os_handle handle;
        u64       error;
    };

    struct os_file_size_t {
        os_handle handle;
        u32       error;
        u32       size;
    };

    using os_file_ro_open_new_f             = bool (os_file_open_t&  file_open);
    using os_file_ro_open_existing_f        = bool (os_file_open_t&  file_open);
    using os_file_ro_open_new_or_existing_f = bool (os_file_open_t&  file_open);
    using os_file_rw_open_new_f             = bool (os_file_open_t&  file_open);
    using os_file_rw_open_existing_f        = bool (os_file_open_t&  file_open);
    using os_file_rw_open_new_or_existing_f = bool (os_file_open_t&  file_open);
    using os_file_close_f                   = bool (os_file_close_t& file_close);
    using os_file_size_f                    = bool (os_file_size_t&  file_size);
    using os_file_read_f                    = bool (os_file_io_t&    file_io);
    using os_file_write_f                   = bool (os_file_io_t&    file_io);

    struct os_window_create_t {
        os_handle handle;
        u64       error;
        c8*       name;
        u32       width;
        u32       height;
        u32       pos_x;
        u32       pos_y;
    };

    struct os_window_op_t {
        os_handle handle;
        u64       error;
    };

    enum os_window_event_flag_e {
        os_window_event_flag_e_closed = 0
    };

    struct os_window_process_events_t {

    };

    using os_window_create_f = bool (os_window_create_t& window_create);
    using os_window_show_f   = bool (os_window_op_t&     window_op);
    using os_window_hide_f   = bool (os_window_op_t&     window_op);


};


#endif //SLD_OS_HPP
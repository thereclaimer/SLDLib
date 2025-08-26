#pragma once

#include <Windows.h>
#include "sld-os.hpp"

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    const os_thread_error_t  win32_thread_error_success  (void);
    const os_thread_error_t  win32_thread_error_get_last (void);

    //-------------------------------------------------------------------
    // OS API
    //-------------------------------------------------------------------

    static const os_thread_error_t
    win32_thread_create(
        os_thread_handle_t& thread_handle) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);
    }

    static const os_thread_error_t 
    win32_thread_destroy(
        const os_thread_handle_t thread_handle) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);
    }

    static const os_thread_error_t 
    win32_thread_exit(
        const os_thread_handle_t thread_handle) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);
    }

    static const os_thread_error_t 
    win32_thread_sleep(
        const os_thread_handle_t thread_handle) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);
    }

    static const os_thread_error_t 
    win32_thread_yield(
        const os_thread_handle_t thread_handle) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);
    }

    static const os_thread_error_t 
    win32_thread_join(
        const os_thread_handle_t thread_handle) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);
    }

    static const os_thread_error_t
    win32_thread_mutex_create(
        os_thread_mutex_handle_t& mutex_handle) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);
    }

    static const os_thread_error_t
    win32_thread_mutex_destroy(
        const os_thread_mutex_handle_t mutex_handle) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);    
    }

    static const os_thread_error_t
    win32_thread_mutex_lock(
        const os_thread_mutex_handle_t mutex_handle) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);    
    }

    static const os_thread_error_t
    win32_thread_mutex_unlock(
        const os_thread_mutex_handle_t mutex_handle) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);    
    }

    static const os_thread_error_t
    win32_thread_mutex_try_lock(
        const os_thread_mutex_handle_t mutex_handle) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);    
    }

    static const os_thread_error_t
    win32_thread_condition_create(
        void) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);   
    }

    static const os_thread_error_t
    win32_thread_condition_destroy(
        void) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);   
    }

    static const os_thread_error_t
    win32_thread_condition_wait(
        void) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);   
    }

    static const os_thread_error_t
    win32_thread_condition_signal(
        void) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);   
    }

    static const os_thread_error_t
    win32_thread_condition_broadcast(
        void) {

        os_thread_error_t error = win32_thread_error_success();
        return(error);   
    }

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    const os_thread_error_t
    win32_thread_error_success(
        void) {

        static const os_thread_error_t error = { os_thread_error_e_success };
        return(error);
    }

    const os_thread_error_t
    win32_thread_error_get_last(
        void) {

        os_thread_error_t error;

        const DWORD win32_error = GetLastError();
        switch(win32_error) {
            case(ERROR_SUCCESS): error.val = os_thread_error_e_success; break;
            default:             error.val = os_thread_error_e_unknown; break;
        }

        return(error);
    }

};
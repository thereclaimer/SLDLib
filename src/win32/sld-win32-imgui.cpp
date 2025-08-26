#pragma once

#include <Windows.h>
#include "sld-os.hpp"

extern IMGUI_IMPL_API LRESULT 
ImGui_ImplWin32_WndProcHandler(
    HWND   hWnd,
    UINT   msg,
    WPARAM wParam,
    LPARAM lParam);

namespace sld {

    //-------------------------------------------------------------------
    // DECLARATIONS
    //-------------------------------------------------------------------

    bool win32_imgui_init_opengl3   (HWND window_handle);
    bool win32_imgui_init_directx12 (HWND window_handle);

    //-------------------------------------------------------------------
    // INTERNAL
    //-------------------------------------------------------------------

    SLD_OS_API_INTERNAL bool
    win32_imgui_init_opengl3(
        HWND window_handle) {

        bool result = true;

        // initialize context
        ImGuiContext* imgui_context = IMGUI_CHECKVERSION() ? ImGui::CreateContext() : NULL;  
        result &= (imgui_context != NULL);

        // initialize win32/opengl methods
        if (result) {
            result &= ImGui_ImplWin32_InitForOpenGL (window_handle);
            result &= ImGui_ImplOpenGL3_Init        ("#version 330");
        }

        return(result);        
    }

    SLD_OS_API_INTERNAL bool
    win32_imgui_init_directx12(
        void) {

        return(false);     
    }
};
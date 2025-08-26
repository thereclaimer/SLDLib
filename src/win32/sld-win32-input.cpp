#pragma once

#include <Windows.h>
#include "sld-os.hpp"

namespace sld {

    struct win32_input_keycode_map_t;

    sld_ct_inline win32_input_keycode_map_t& win32_input_keycode_map_instance (void);
    sld_ct_inline os_input_keycode_t         win32_input_translate_keycode    (const WPARAM w_param, const LPARAM l_param);

    struct win32_input_keycode_map_t {
        
        sld_ct_const u32 capacity           = 256;
        sld_ct_const u16 keycodes[capacity] = {

            /* 0x00-0x07 */ 
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            
            /* 0x08 = BACKSPACE */
            os_input_keycode_e_backspace,
            
            /* 0x09 = TAB */
            os_input_keycode_e_tab,os_input_keycode_e_null, os_input_keycode_e_null,
            
            /* 0x0D = ENTER */
            os_input_keycode_e_enter, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            
            /* 0x10-0x1F */
            os_input_keycode_e_ctrl_left, os_input_keycode_e_shift_left,  os_input_keycode_e_null, os_input_keycode_e_alt_left,
            os_input_keycode_e_null,  os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null,  os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null,  os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,

            /* 0x20-0x2F */
            os_input_keycode_e_space,
            os_input_keycode_e_page_up,
            os_input_keycode_e_page_down,
            os_input_keycode_e_end,
            os_input_keycode_e_home,
            os_input_keycode_e_arrow_left,
            os_input_keycode_e_arrow_up,
            os_input_keycode_e_arrow_right,
            os_input_keycode_e_arrow_down,
            os_input_keycode_e_null,
            os_input_keycode_e_null,
            os_input_keycode_e_null,
            os_input_keycode_e_null,
            os_input_keycode_e_null,
            os_input_keycode_e_null,
            os_input_keycode_e_null,

            /* 0x30-0x39 (0-9) */
            os_input_keycode_e_0, os_input_keycode_e_1, os_input_keycode_e_2, os_input_keycode_e_3, os_input_keycode_e_4,
            os_input_keycode_e_5, os_input_keycode_e_6, os_input_keycode_e_7, os_input_keycode_e_8, os_input_keycode_e_9,

            /* 0x3A-0x40 */       
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,

            /* 0x41-0x5A (A-Z) */
            os_input_keycode_e_a, os_input_keycode_e_b, os_input_keycode_e_c, os_input_keycode_e_d,
            os_input_keycode_e_e, os_input_keycode_e_f, os_input_keycode_e_g, os_input_keycode_e_h,
            os_input_keycode_e_i, os_input_keycode_e_j, os_input_keycode_e_k, os_input_keycode_e_l,
            os_input_keycode_e_m, os_input_keycode_e_n, os_input_keycode_e_o, os_input_keycode_e_p,
            os_input_keycode_e_q, os_input_keycode_e_r, os_input_keycode_e_s, os_input_keycode_e_t,
            os_input_keycode_e_u, os_input_keycode_e_v, os_input_keycode_e_w, os_input_keycode_e_x,
            os_input_keycode_e_y, os_input_keycode_e_z,

            /* 0x5B-0x5F */
            os_input_keycode_e_super_left,
            os_input_keycode_e_super_right,
            os_input_keycode_e_menu,
            os_input_keycode_e_null,
            os_input_keycode_e_null,

            /* 0x60-0x69 (numpad 0-9) */ 
            os_input_keycode_e_kp_0, os_input_keycode_e_kp_1, os_input_keycode_e_kp_2, os_input_keycode_e_kp_3,
            os_input_keycode_e_kp_4, os_input_keycode_e_kp_5, os_input_keycode_e_kp_6, os_input_keycode_e_kp_7,
            os_input_keycode_e_kp_8, os_input_keycode_e_kp_9,

            /* 0x6A-0x6F (numpad operations) */ 
            os_input_keycode_e_kp_multiply, os_input_keycode_e_kp_add,     os_input_keycode_e_null,
            os_input_keycode_e_kp_subtract, os_input_keycode_e_kp_decimal, os_input_keycode_e_kp_divide,

            /* 0x70-0x87 F1-F24 */ 
            os_input_keycode_e_f1,   os_input_keycode_e_f2,   os_input_keycode_e_f3,   os_input_keycode_e_f4,
            os_input_keycode_e_f5,   os_input_keycode_e_f6,   os_input_keycode_e_f7,   os_input_keycode_e_f8,
            os_input_keycode_e_f9,   os_input_keycode_e_f10,  os_input_keycode_e_f11,  os_input_keycode_e_f12,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,

            /* 0x88-0x8F */ 
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,

            /* 0x90-0x9F Locks & misc */
            os_input_keycode_e_num_lock, os_input_keycode_e_scroll_lock, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,

            /* 0xA0-0xA5 Left/Right modifiers (Shift/Ctrl/Alt) */
            os_input_keycode_e_shift_left, os_input_keycode_e_shift_right, os_input_keycode_e_ctrl_left, os_input_keycode_e_ctrl_right,
            os_input_keycode_e_alt_left,   os_input_keycode_e_alt_right,

            /* 0xA6-0xAF unknown */
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,

            /* 0xB0-0xFF Symbols/OEM & remaining unknowns */
            os_input_keycode_e_null,      os_input_keycode_e_null,   os_input_keycode_e_null,         os_input_keycode_e_null,
            os_input_keycode_e_null,      os_input_keycode_e_null,   os_input_keycode_e_null,         os_input_keycode_e_null,
            os_input_keycode_e_null,      os_input_keycode_e_null,   os_input_keycode_e_null,         os_input_keycode_e_null,
            os_input_keycode_e_null,      os_input_keycode_e_null,   os_input_keycode_e_null,         os_input_keycode_e_null,
            os_input_keycode_e_minus,     os_input_keycode_e_equals, os_input_keycode_e_bracket_left, os_input_keycode_e_bracket_right,
            os_input_keycode_e_backslash, os_input_keycode_e_null,   os_input_keycode_e_semicolon,    os_input_keycode_e_apostrophe,
            os_input_keycode_e_grave,     os_input_keycode_e_comma,  os_input_keycode_e_period,       os_input_keycode_e_slash,
            os_input_keycode_e_null,      os_input_keycode_e_null,   os_input_keycode_e_null,         os_input_keycode_e_null,
            
            /* remaining 0xE0-0xFF default to unknown */ 
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null,
            os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null, os_input_keycode_e_null
        };
    };

    sld_ct_inline os_input_keycode_t
    win32_input_translate_keycode(
        const WPARAM w_param,
        const LPARAM l_param) {

        constexpr win32_input_keycode_map_t keycode_map;
        os_input_keycode_t                  keycode = {os_input_keycode_e_null};
        
        if (w_param >= 256) return(keycode);

        const bool is_extended = (l_param & 0x01000000) != 0;
        const u8   scan        = (l_param >> 16) & 0xFF;

        switch(w_param) {
            
            case (VK_SHIFT):   keycode.val = (scan == 0x36) ? os_input_keycode_e_shift_right : os_input_keycode_e_shift_left;  
            case (VK_CONTROL): keycode.val = (is_extended)  ? os_input_keycode_e_ctrl_right  : os_input_keycode_e_ctrl_left;
            case (VK_MENU):    keycode.val = (is_extended)  ? os_input_keycode_e_alt_right   : os_input_keycode_e_alt_left;
            default:           keycode.val = keycode_map.keycodes[w_param];
        }

        return(keycode);
    }
}
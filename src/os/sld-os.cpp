#pragma once

#include "sld-os.hpp"

namespace sld {

    static void
    os_input_keyboard_add_key_down(
        os_input_keyboard_t&     keyboard,
        const os_input_keycode_t keycode) {

        

        if (keyboard.keys_down.count >= keyboard.keys_down.capacity) {
            keyboard.keys_down.count =  keyboard.keys_down.capacity;
            return;
        }

        const u8 index = keyboard.keys_down.count;

        keyboard.keys_down.array[index] = keycode;
        ++keyboard.keys_down.count;
    }

    static void
    os_input_keyboard_add_key_up(
        os_input_keyboard_t&     keyboard,
        const os_input_keycode_t keycode) {


        if (keyboard.keys_up.count >= keyboard.keys_up.capacity) {
            keyboard.keys_up.count =  keyboard.keys_up.capacity;
            return;
        }

        const u8 index = keyboard.keys_up.count;

        keyboard.keys_up.array[index] = keycode;
        ++keyboard.keys_up.count;
    }


    static void
    os_input_keyboard_reset(
        os_input_keyboard_t& keycode_list) {

        keycode_list.keys_up.count   = 0;
        keycode_list.keys_down.count = 0;
    }
};
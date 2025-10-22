#pragma once

#include "sld-input.hpp"

namespace sld {

    SLD_API void
    input_keyboard_validate(
        input_keyboard_t* keyboard) {

        bool is_valid = (keyboard != NULL);
        if (is_valid) {
            is_valid &= (INPUT_ARRAY_SIZE_KEYCODE != 0);
            is_valid &= (keyboard->key_count.down <= INPUT_ARRAY_SIZE_KEYCODE);
            is_valid &= (keyboard->key_count.up   <= INPUT_ARRAY_SIZE_KEYCODE);
        }
        assert(is_valid);
    }

    SLD_API void
    input_keyboard_add_key_down(
        input_keyboard_t*     keyboard,
        const input_keycode_t keycode) {

        // input_keyboard_validate(keyboard);

        // if (keyboard->key_count.down == INPUT_ARRAY_SIZE_KEYCODE) return;

        // const u8 index = keyboard->key_count.down; 

        // keyboard->keycode_array.down[index] = keycode;
        // ++keyboard->key_count.down;
    }

    SLD_API void
    input_keyboard_add_key_up(
        input_keyboard_t*     keyboard,
        const input_keycode_t keycode) {

        // input_keyboard_validate(keyboard);

        // if (keyboard->key_count.up == INPUT_ARRAY_SIZE_KEYCODE) return;

        // const u8 index = keyboard->key_count.up; 

        // keyboard->keycode_array.up[index] = keycode;
        // ++keyboard->key_count.up;
    }

    SLD_API void
    input_keyboard_reset(
        input_keyboard_t* keyboard) {

        input_keyboard_validate(keyboard);

        keyboard->key_count.down = 0;
        keyboard->key_count.up   = 0;
    }
};
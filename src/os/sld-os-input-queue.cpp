#pragma once

#include "sld-os.hpp"


namespace sld {

    SLD_API void
    os_input_queue_reset(
        os_input_queue_t& input_queue) {

        bool is_valid = true;
        is_valid &= (input_queue.keyboard_key_up   != NULL);
        is_valid &= (input_queue.keyboard_key_down != NULL);
        assert(is_valid);

        queue_reset(input_queue.keyboard_key_up);
        queue_reset(input_queue.keyboard_key_down);
    }


    SLD_API bool
    os_input_queue_key_up_push(
        os_input_queue_t&        input_queue,
        const os_input_keycode_t keycode) {

        assert(input_queue.keyboard_key_up != NULL);
        const bool did_add = queue_push(input_queue.keyboard_key_up, &keycode);
        return(did_add);
    }

    SLD_API os_input_keycode_t
    os_input_queue_key_up_pop(
        os_input_queue_t& input_queue) {

        assert(input_queue.keyboard_key_up != NULL);
        os_input_keycode_t* keycode = (os_input_keycode_t*)queue_pop(input_queue.keyboard_key_up);
        return(*keycode);
    }

    SLD_API u32
    os_input_queue_key_up_count(
        os_input_queue_t& input_queue) {

        assert(input_queue.keyboard_key_up != NULL);
        const u32 count = queue_count(input_queue.keyboard_key_up);
        return(count);
    }

    SLD_API bool
    os_input_queue_key_down_push(
        os_input_queue_t&  input_queue,
        os_input_keycode_t keycode) {

        assert(input_queue.keyboard_key_down != NULL);
        const bool did_add = queue_push(input_queue.keyboard_key_down, &keycode);
        return(did_add);
    }

    SLD_API os_input_keycode_t
    os_input_queue_key_down_pop(
        os_input_queue_t& input_queue) {

        assert(input_queue.keyboard_key_down);
        os_input_keycode_t* keycode = (os_input_keycode_t*)queue_pop(input_queue.keyboard_key_down);
        return(*keycode);
    }

    SLD_API u32
    os_input_queue_key_down_count(
        os_input_queue_t& input_queue) {

        assert(input_queue.keyboard_key_down != NULL);
        const u32 count = queue_count(input_queue.keyboard_key_down);
        return(count);
    }
};

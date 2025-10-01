#pragma once

#include "sld-os.hpp"

#define os_input_queue_keyboard_reset(queue)                               queue_reset           (queue)
#define os_input_queue_keyboard_is_empty(queue)                            queue_is_empty        (queue)
#define os_input_queue_keyboard_is_full(queue)                             queue_is_full         (queue)
#define os_input_queue_keyboard_space_remaining(queue)                     queue_space_remaining (queue)
#define os_input_queue_keyboard_space_used(queue)                          queue_space_used      (queue)
#define os_input_queue_keyboard_count(queue)                               queue_count           (queue)
#define os_input_queue_keyboard_push(queue, keycode)                       queue_push            (queue, (void*)keycode, 1)      
#define os_input_queue_keyboard_pop(queue)           (os_input_keycode_t*) queue_pop             (queue, 1)
#define os_input_queue_keyboard_peek(queue)          (os_input_keycode_t*) queue_peek            (queue, 1)

namespace sld {

    SLD_API bool
    os_input_queue_push_key_up(
        os_input_queue_t&        input_queue,
        const os_input_keycode_t keycode) {

        assert(input_queue.keyboard_key_up != NULL);
        const bool did_add = os_input_queue_keyboard_push(input_queue.keyboard_key_up, &keycode);
        return(did_add);
    }

    SLD_API bool
    os_input_queue_push_key_down(
        os_input_queue_t&  input_queue,
        os_input_keycode_t keycode) {

        assert(input_queue.keyboard_key_down != NULL);
        const bool did_add = os_input_queue_keyboard_push(input_queue.keyboard_key_down, &keycode);
        return(did_add);
    }

    SLD_API os_input_keycode_t
    os_input_queue_pop_key_up(
        os_input_queue_t& input_queue) {

        assert(input_queue.keyboard_key_up != NULL);
        os_input_keycode_t* keycode = os_input_queue_keyboard_pop(input_queue.keyboard_key_up);
        return(*keycode);
    }

    SLD_API os_input_keycode_t
    os_input_queue_pop_key_down(
        os_input_queue_t& input_queue) {

        assert(input_queue.keyboard_key_down);
        os_input_keycode_t* keycode = os_input_queue_keyboard_pop(input_queue.keyboard_key_down);
        return(*keycode);
    }
};

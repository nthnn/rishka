#ifndef LIBRISHKA_KEYBOARD_H
#define LIBRISHKA_KEYBOARD_H

#include <librishka/devices.h>
#include <librishka/types.h>

class Keyboard final {
public:
    static string layout_name();
    static string layout_desc();

    static PS2Device device_type();

    static bool is_num_lock();
    static bool is_caps_lock();
    static bool is_scroll_lock();

    static void num_lock(bool state);
    static void caps_lock(bool state);
    static void scroll_lock(bool state);

    static u32 next_scancode(i32 timeout, bool resend);
    static void lock(i32 timeout);
    static void unlock();
    static void reset();
};

#endif
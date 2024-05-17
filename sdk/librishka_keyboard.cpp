#include "librishka.h"
#include "librishka_impl.hpp"

string Keyboard::layout_name() {
    return get_rt_string(rishka_sc_0(RISHKA_SC_KB_LAYOUT_NAME));
}

string Keyboard::layout_desc() {
    return get_rt_string(rishka_sc_0(RISHKA_SC_KB_LAYOUT_DESC));
}

PS2Device Keyboard::device_type() {
    return (PS2Device) rishka_sc_0(RISHKA_SC_KB_DEVICE_TYPE);
}

bool Keyboard::is_num_lock() {
    return (bool) rishka_sc_0(RISHKA_SC_KB_LED_GET_NUM);
}

bool Keyboard::is_caps_lock() {
    return (bool) rishka_sc_0(RISHKA_SC_KB_LED_GET_CAPS);
}

bool Keyboard::is_scroll_lock() {
    return (bool) rishka_sc_0(RISHKA_SC_KB_LED_GET_SCROLL);
}

void Keyboard::num_lock(bool state) {
    rishka_sc_1(RISHKA_SC_KB_LED_SET_NUM, (i64) state);
}

void Keyboard::caps_lock(bool state) {
    rishka_sc_1(RISHKA_SC_KB_LED_SET_CAPS, (i64) state);
}

void Keyboard::scroll_lock(bool state) {
    rishka_sc_1(RISHKA_SC_KB_LED_SET_SCROLL, (i64) state);
}

u32 Keyboard::next_scancode(i32 timeout, bool resend) {
    return (u32) rishka_sc_2(RISHKA_SC_KB_NEXT_SCAN_CODE, (i64) timeout, (i64) resend);
}

void Keyboard::lock(i32 timeout) {
    rishka_sc_1(RISHKA_SC_KB_LOCK, (i64) timeout);
}

void Keyboard::unlock() {
    rishka_sc_0(RISHKA_SC_KB_UNLOCK);
}

void Keyboard::reset() {
    rishka_sc_0(RISHKA_SC_KB_RESET);
}

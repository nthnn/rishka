#include "librishka.h"
#include "librishka_impl.hpp"

i32 Display::screen_height() {
    return (i32) rishka_sc_0(RISHKA_SC_DISPLAY_SCREEN_HEIGHT);
}

i32 Display::screen_width() {
    return (i32) rishka_sc_0(RISHKA_SC_DISPLAY_SCREEN_WIDTH);
}

i32 Display::viewport_height() {
    return (i32) rishka_sc_0(RISHKA_SC_DISPLAY_VIEWPORT_HEIGHT);
}

i32 Display::viewport_width() {
    return (i32) rishka_sc_0(RISHKA_SC_DISPLAY_VIEWPORT_WIDTH);
}

i32 Display::supported_colors() {
    return (i32) rishka_sc_0(RISHKA_SC_DISPLAY_SUPPORTED_COLORS);
}
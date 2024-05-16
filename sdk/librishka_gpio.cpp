/* 
 * This file is part of the Rishka distribution (https://github.com/nthnn/rishka).
 * Copyright (c) 2024 Nathanne Isip.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "librishka.h"
#include "librishka_impl.hpp"

void Gpio::pin_mode(u8 pin, gpio_pin_mode_t mode) {
    rishka_sc_2(RISHKA_SC_GPIO_PIN_MODE, (i64) pin, (i64) mode);
}

gpio_mode_t Gpio::digital_read(u8 pin) {
    return (gpio_mode_t) rishka_sc_1(RISHKA_SC_GPIO_DIGITAL_READ, (i64) pin);
}

void Gpio::digital_write(u8 pin, gpio_mode_t mode) {
    rishka_sc_2(RISHKA_SC_GPIO_DIGITAL_WRITE, (i64) pin, (i64) mode);
}

u16 Gpio::analog_read(u8 pin) {
    return (i32) rishka_sc_1(RISHKA_SC_GPIO_ANALOG_READ, (i64) pin);
}

void Gpio::analog_write(u8 pin, u16 value) {
    rishka_sc_2(RISHKA_SC_GPIO_ANALOG_WRITE, (i64) pin, (i64) value);
}

u64 Gpio::pulse_in(u8 pin, u8 state, u64 timeout) {
    return (u64) rishka_sc_3(RISHKA_SC_GPIO_PULSE_IN, (i64) pin, (i64) state, (i64) timeout);
}

u64 Gpio::pulse_in_long(u8 pin, u8 state, u64 timeout) {
    return (u64) rishka_sc_3(RISHKA_SC_GPIO_PULSE_IN_LONG, (i64) pin, (i64) state, (i64) timeout);
}

u8 Gpio::shift_in(u8 data, u8 clock, u8 bit_order) {
    return (u8) rishka_sc_3(RISHKA_SC_GPIO_SHIFT_IN, (i64) data, (i64) clock, (i64) bit_order);
}

void Gpio::shift_out(u8 data, u8 clock, u8 bit_order, u8 value) {
    rishka_sc_4(RISHKA_SC_GPIO_SHIFT_OUT, (i64) data, (i64) clock, (i64) bit_order, (i64) value);
}

void Gpio::tone(u32 frequency, u64 duration) {
    rishka_sc_2(RISHKA_SC_GPIO_TONE, (i64) frequency, (i64) duration);
}

void Gpio::no_tone() {
    rishka_sc_0(RISHKA_SC_GPIO_NO_TONE);
}
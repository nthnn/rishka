/* 
 * This file is part of the Rishka distribution (https://github.com/rishka-esp32/rishka-sdk).
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

#ifndef LIBRISHKA_GPIO_H
#define LIBRISHKA_GPIO_H

#include <librishka/types.h>

typedef enum {
    GPIO_INPUT = 0x01,
    GPIO_OUTPUT = 0x03,
    GPIO_INPUT_PULLUP = 0x05
} gpio_pin_mode_t;

typedef enum {
    GPIO_LOW = 0x0,
    GPIO_HIGH = 0x1
} gpio_mode_t;

class Gpio {
public:
    static void pin_mode(u8 pin, gpio_pin_mode_t mode);

    static i32 digital_read(u8 pin);
    static void digital_write(u8 pin, gpio_mode_t mode);

    static i32 analog_read(u8 pin);
    static void analog_write(u8 pin, u8 value);

    static u64 pulse_in(u8 pin, u8 state, u64 timeout);
    static u64 pulse_in_long(u8 pin, u8 state, u64 timeout);

    static u8 shift_in(u8 data, u8 clock, u8 bit_order);
    static void shift_out(u8 data, u8 clock, u8 bit_order, u8 value);

    static void tone(u8 pin, u32 frequency, u64 duration);
    static void no_tone(u8 pin);
};

#endif
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

#ifndef LIBRISHKA_I2C_H
#define LIBRISHKA_I2C_H

#include <librishka/types.h>

class I2C final {
public:
    static bool begin(u8 address);
    static bool end();
    static bool pins(u8 sda, u8 scl);
    static void flush();

    static void begin_transmission(u8 address);
    static u8 end_transmission(bool stop_bit);

    static usize write(u8* data, usize size);
    static usize slave_write(u8* data, usize size);
    static usize set_buffersize(usize size);

    static i32 read();
    static i32 peek();
    static i32 available();

    static usize request(u8 address, usize size, bool stop_bit);
    static void on_receive(void (*callback)(int));
    static void on_request(void (*callback)(void));

    static void set_timeout(u16 timeout);
    static u16 get_timeout();

    static bool set_clock(u32 clock);
    static u32 get_clock();
};

#endif
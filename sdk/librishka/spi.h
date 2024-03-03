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

#ifndef LIBRISHKA_SPI_H
#define LIBRISHKA_SPI_H

#include <librishka/types.h>

class SPI final {
public:
    static void begin(u8 sck, u8 miso, u8 mosi, u8 ss);
    static void end();

    static void begin_transaction(u8 clock, u8 bit_order, u8 data_mode);
    static void end_transaction();

    static u8 transfer8(u8 data);
    static u16 transfer16(u16 data);
    static u32 transfer32(u32 data);

    static void transfer_bytes(u8* data, u8* out, u32 size);
    static void transfer_bits(u32 data, u32* out, u8 bits);

    static void set_hwcs(bool use);
    static void set_bit_order(u8 bit_order);
    static void set_data_mode(u8 data_mode);
    static void set_frequency(u32 frequency);

    static void set_clock_div(u32 clock_div);
    static u32 get_clock_div();

    static void write8(u8 data);
    static void write16(u16 data);
    static void write32(u32 data);

    static void write_bytes(u8* data, u32 size);
    static void write_pixels(void* data, u32 size);
    static void write_pattern(u8* data, u8 size, u32 pattern);
};

#endif
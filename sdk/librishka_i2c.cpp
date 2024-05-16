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

bool I2C::begin(u8 address) {
    return (bool) rishka_sc_1(RISHKA_SC_I2C_BEGIN, (i64) address);
}

bool I2C::end() {
    return (bool) rishka_sc_0(RISHKA_SC_I2C_END);
}

bool I2C::pins(u8 sda, u8 scl) {
    return (bool) rishka_sc_2(RISHKA_SC_I2C_PINS, (i64) sda, (i64) scl);
}

void I2C::flush() {
    rishka_sc_0(RISHKA_SC_I2C_FLUSH);
}

void I2C::begin_transmission(u8 address) {
    rishka_sc_1(RISHKA_SC_I2C_BEGIN_TRANSMISSION, (i64) address);
}

u8 I2C::end_transmission(bool stop_bit) {
    return (u8) rishka_sc_1(RISHKA_SC_I2C_END_TRANSMISSION, (i64) stop_bit);
}

usize I2C::write(u8* data, usize size) {
    return (usize) rishka_sc_2(RISHKA_SC_I2C_WRITE, (i64) data, (usize) size);
}

usize I2C::slave_write(u8* data, usize size) {
    return (usize) rishka_sc_2(RISHKA_SC_I2C_SLAVE_WRITE, (i64) data, (usize) size);
}

usize I2C::set_buffersize(usize size) {
    return (usize) rishka_sc_1(RISHKA_SC_I2C_BUFSIZE, (i64) size);
}

i32 I2C::read() {
    return (i32) rishka_sc_0(RISHKA_SC_I2C_READ);
}

i32 I2C::peek() {
    return (i32) rishka_sc_0(RISHKA_SC_I2C_PEEK);
}

i32 I2C::available() {
    return (i32) rishka_sc_0(RISHKA_SC_I2C_AVAILABLE);
}

usize I2C::request(u8 address, usize size, bool stop_bit) {
    return (usize) rishka_sc_3(RISHKA_SC_I2C_REQUEST, (i64) address, (i64) size, (i64) stop_bit);
}

void I2C::on_receive(void (*callback)(int)) {
    rishka_sc_1(RISHKA_SC_I2C_ON_RECEIVE, (i64) callback);
}

void I2C::on_request(void (*callback)(void)) {
    rishka_sc_1(RISHKA_SC_I2C_ON_RECEIVE, (i64) callback);
}

void I2C::set_timeout(u16 timeout) {
    rishka_sc_1(RISHKA_SC_I2C_SET_TIMEOUT, (i64) timeout);
}

u16 I2C::get_timeout() {
    return (u16) rishka_sc_0(RISHKA_SC_I2C_GET_TIMEOUT);
}

bool I2C::set_clock(u32 clock) {
    return (bool) rishka_sc_1(RISHKA_SC_I2C_SET_CLOCK, (i64) clock);
}

u32 I2C::get_clock() {
    return (u32) rishka_sc_0(RISHKA_SC_I2C_GET_CLOCK);
}
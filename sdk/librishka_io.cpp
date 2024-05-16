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

void IO::print(const string text) {
    rishka_sc_1(RISHKA_SC_IO_PRINTS, (i64) text);
}

void IO::print(
    const string text,
    const string fg,
    const string bg,
    const string style
) {
    IO::print(style);
    IO::print(fg);
    IO::print(bg);
    IO::print(text);
}

void IO::println(const string text) {
    rishka_sc_1(RISHKA_SC_IO_PRINTS, (i64) text);
    IO::println();
}

void IO::print(i64 number) {
    rishka_sc_1(RISHKA_SC_IO_PRINTN, (i64) number);
}

void IO::println(i64 number) {
    rishka_sc_1(RISHKA_SC_IO_PRINTN, (i64) number);
    IO::println();
}

void IO::print(double number) {
    rishka_sc_1(RISHKA_SC_IO_PRINTD, double_to_long(number));
}

void IO::println(double number) {
    rishka_sc_1(RISHKA_SC_IO_PRINTD, double_to_long(number));
    IO::println();
}

void IO::println() {
    IO::print(F("\r\n"));
}

rune IO::readch() {
    return (rune) rishka_sc_0(RISHKA_SC_IO_READCH);
}

string IO::readline() {
    return get_rt_string((u32) rishka_sc_0(RISHKA_SC_IO_READLINE));
}

i32 IO::available() {
    return (i32) rishka_sc_0(RISHKA_SC_IO_AVAILABLE);
}

i32 IO::peek() {
    return (i32) rishka_sc_0(RISHKA_SC_IO_PEEK);
}

bool IO::find(string target, usize size) {
    return (bool) rishka_sc_2(RISHKA_SC_IO_FIND, (i64) target, (i64) size);
}

bool IO::find_until(string target, string terminator) {
    return (bool) rishka_sc_2(RISHKA_SC_IO_FIND_UNTIL, (i64) target, (i64) terminator);
}

void IO::set_timeout(u64 timeout) {
    rishka_sc_1(RISHKA_SC_IO_SET_TIMEOUT, (i64) timeout);
}

u64 IO::get_timeout() {
    return (u64) rishka_sc_0(RISHKA_SC_IO_GET_TIMEOUT);
}
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
#include "librishka/func_args.h"
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
    IO::printf(
        F("{s}{s}{s}{s}{s}{s}"),
        style,
        fg, bg,
        text,
        TERM_STYLE_NORMAL,
        TERM_FG_HWHITE
    );
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

void IO::print(u64 number) {
    rishka_sc_1(RISHKA_SC_IO_PRINTU, (i64) number);
}

void IO::println(u64 number) {
    rishka_sc_1(RISHKA_SC_IO_PRINTU, (i64) number);
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

bool IO::printf(string format, ...) {
    func_arg_list args;
    func_arg_start(args, format);

    while(*format) {
        if(*format == '{') {
            format++;

            if(*format == 'i' && *(format + 1) == '}') {
                i64 i = func_arg_get(args, i64);
                IO::print(i);

                format += 2;
            }
            else if(*format == 'u' && *(format + 1) == '}') {
                u64 u = func_arg_get(args, u64);
                IO::print(u);

                format += 2;
            }
            else if(*format == 'd' && *(format + 1) == '}') {
                double d = func_arg_get(args, double);
                IO::print(d);

                format += 2;
            }
            else if(*format == 's' && *(format + 1) == '}') {
                string s = func_arg_get(args, char*);
                IO::print(s);

                format += 2;
            }
            else {
                func_arg_end(args);
                return 0;
            }

            continue;
        }

        rune str[2] = {*format++, '\0'};
        IO::print(str);
    }

    func_arg_end(args);
    return 1;
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
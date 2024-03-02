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

#ifndef LIBRISHKA_IO_H
#define LIBRISHKA_IO_H

#include <librishka/types.h>

class IO final {
public:
    static void print(const string text);
    static void print(i64 number);
    static void print(double number);

    static i32 read();
    static i32 available();
    static i32 peek();

    static bool find(string target, usize size);
    static bool find_until(string target, string terminator);

    static void set_timeout(u64 timeout);
    static u64 get_timeout();

    static rune readch();
    static string readline();
};

#endif
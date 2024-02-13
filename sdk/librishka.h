/* 
 * This file is part of the Rishka SDK distribution (https://github.com/rishka-esp32/rishka-sdk).
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


#ifndef LIBRISHKA_H
#define LIBRISHKA_H

typedef char                rune;
typedef char*               string;

typedef signed char         i8;
typedef signed short int    i16;
typedef signed int          i32;
typedef signed long int     i64;

typedef unsigned char       u8;
typedef unsigned short int  u16;
typedef unsigned int        u32;
typedef unsigned long int   u64;

typedef u32                 usize;
typedef void*               any;

#define F(str) ((char*) str)

class IO {
public:
    static void prints(const string text);
    static void printn(i64 number);
    static void printd(double number);
    static rune readch();
};

class Sys {
public:
    static void exit(i32 code);
};

class Memory {
public:
    static any set(any dest, i32 c, usize n);
};

#endif
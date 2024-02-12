/*
 * This file is part of the Rishka SDK.
 * Copyright (c) 2024 Nathanne Isip
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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

void io_prints(string text);
void io_printn(i64 number);
void io_printf(float number);
rune io_readch();

void sys_exit(i32 code);
any sys_memset(any dest, i32 c, usize n);

#endif
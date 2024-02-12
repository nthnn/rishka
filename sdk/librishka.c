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

#include "librishka.h"

enum rishka_syscall {
    RISHKA_SC_IO_PRi32S,
    RISHKA_SC_IO_PRi32N,
    RISHKA_SC_IO_PRi32F,
    RISHKA_SC_IO_READCH,
    RISHKA_SC_IO_READLINE,

    RISHKA_SC_SYS_DELAY_MS,
    RISHKA_SC_SYS_MICROS,
    RISHKA_SC_SYS_MILLIS,
    RISHKA_SC_SYS_SHELLEXEC,
    RISHKA_SC_SYS_EXIT,
    RISHKA_SC_SYS_INFOS,
    RISHKA_SC_SYS_INFON,
    RISHKA_SC_SYS_RANDOM,

    RISHKA_SC_MEM_ALLOC,
    RISHKA_SC_MEM_CALLOC,
    RISHKA_SC_MEM_REALLOC,
    RISHKA_SC_MEM_FREE,
    RISHKA_SC_MEM_SET,

    RISHKA_SC_GPIO_PIN_MODE,
    RISHKA_SC_GPIO_DIGITAL_READ,
    RISHKA_SC_GPIO_DIGITAL_WRITE,
    RISHKA_SC_GPIO_ANALOG_READ,
    RISHKA_SC_GPIO_ANALOG_READ_RES,
    RISHKA_SC_GPIO_ANALOG_WRITE,
    RISHKA_SC_GPIO_ANALOG_WRITE_RES,
    RISHKA_SC_GPIO_ANALOG_REF,
    RISHKA_SC_GPIO_PULSE_IN,
    RISHKA_SC_GPIO_PULSE_IN_i64,
    RISHKA_SC_GPIO_SHIFT_IN,
    RISHKA_SC_GPIO_SHIFT_OUT,
    RISHKA_SC_GPIO_TONE,
    RISHKA_SC_GPIO_NO_TONE,

    RISHKA_SC_i32_ENABLE,
    RISHKA_SC_i32_DISABLE,
    RISHKA_SC_i32_ATTACH,
    RISHKA_SC_i32_DETACH,
    RISHKA_SC_i32_DIGITAL_PIN,

    RISHKA_SC_FS_MKDIR,
    RISHKA_SC_FS_RMDIR,
    RISHKA_SC_FS_LSDIR,
    RISHKA_SC_FS_EXISTS,
    RISHKA_SC_FS_ISFILE,
    RISHKA_SC_FS_ISDIR,
    RISHKA_SC_FS_OPEN,
    RISHKA_SC_FS_CLOSE,
    RISHKA_SC_FS_AVAILABLE,
    RISHKA_SC_FS_FLUSH,
    RISHKA_SC_FS_PEEK,
    RISHKA_SC_FS_SEEK,
    RISHKA_SC_FS_SIZE,
    RISHKA_SC_FS_READ,
    RISHKA_SC_FS_WRITE,
    RISHKA_SC_FS_NEXT,

    RISHKA_SC_ARG_COUNT,
    RISHKA_SC_ARG_STR,

    RISHKA_SC_SERIAL_BEGIN,
    RISHKA_SC_SERIAL_END,
    RISHKA_SC_SERIAL_AVAILABLE,
    RISHKA_SC_SERIAL_FLUSH,

    RISHKA_SC_I2C_BEGIN,
    RISHKA_SC_I2C_END,
    RISHKA_SC_I2C_BEGIN_TRANSMISSION,
    RISHKA_SC_I2C_END_TRANSMISSION,
    RISHKA_SC_I2C_WRITE,
    RISHKA_SC_I2C_READ,
    RISHKA_SC_I2C_REQUEST,
    RISHKA_SC_I2C_AVAILABLE,
    RISHKA_SC_I2C_ON_RECEIVE,
    RISHKA_SC_I2C_ON_REQUEST,
    RISHKA_SC_I2C_GET_TIMEOUT,
    RISHKA_SC_I2C_SET_TIMEOUT,
    RISHKA_SC_I2C_SET_CLOCK,
    RISHKA_SC_I2C_CLEAR_TIMEOUT,

    RISHKA_SC_SPI_BEGIN,
    RISHKA_SC_SPI_END,
    RISHKA_SC_SPI_BEGIN_TRANSACTION,
    RISHKA_SC_SPI_END_TRANSACTION,
    RISHKA_SC_SPI_SET_BIT_ORDER,
    RISHKA_SC_SPI_SET_CLOCK_DIV,
    RISHKA_SC_SPI_DATA_MODE,
    RISHKA_SC_SPI_USE_i32,
    RISHKA_SC_SPI_TRANSFER
};

static i64 float_to_i64(float f) {
    union {
        float input;
        i64 output;
    } data;

    data.input = f;
    return data.output;
}

static float i64_to_float(i64 l) {
    union {
        float output;
        i64 input;
    } data;

    data.input = l;
    return data.output;
}

static inline i64 rishka_sc_0(i32 scallid) {
    register i64 a0 asm("a0") = 0;
    register i64 scid asm("a7") = scallid;

    asm volatile ("scall" : "+r"(a0) : "r"(scid));    
    return a0;
}

static inline i64 rishka_sc_1(i32 scallid, i64 arg0) {
    register i64 a0 asm("a0") = arg0;
    register i64 scid asm("a7") = scallid;

    asm volatile ("scall" : "+r"(a0) : "r"(scid));
    return a0;
}

static inline i64 rishka_sc_2(i32 scallid, i64 arg0, i64 arg1) {
    register i64 a0 asm("a0") = arg0;
    register i64 a1 asm("a1") = arg1;
    register i64 scid asm("a7") = scallid;

    asm volatile ("scall" : "+r"(a0) : "r"(a1), "r"(scid));
    return a0;
}

static inline i64 rishka_sc_3(i32 scallid, i64 arg0, i64 arg1, i64 arg2) {
    register i64 a0 asm("a0") = arg0;
    register i64 a1 asm("a1") = arg1;
    register i64 a2 asm("a2") = arg2;
    register i64 scid asm("a7") = scallid;

    asm volatile ("scall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(scid));
    return a0;
}

void io_prints(string text) {
    rishka_sc_1(RISHKA_SC_IO_PRi32S, (i64) text);
}

void io_printn(i64 number) {
    rishka_sc_1(RISHKA_SC_IO_PRi32N, (i64) number);
}

void io_printf(float number) {
    rishka_sc_1(RISHKA_SC_IO_PRi32F, float_to_i64(number));
}

rune io_readch() {
    return (rune) rishka_sc_0(RISHKA_SC_IO_READCH);
}

void sys_exit(i32 code) {
    rishka_sc_1(RISHKA_SC_SYS_EXIT, (i64) code);
}

any sys_memset(void* dest, i32 c, u32 n) {
    return (any) rishka_sc_3(RISHKA_SC_MEM_SET, (i64) dest, (i64) c, (i64) n);
}
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
    RISHKA_SC_IO_PRINTS,
    RISHKA_SC_IO_PRINTN,
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
    RISHKA_SC_GPIO_PULSE_IN_LONG,
    RISHKA_SC_GPIO_SHIFT_IN,
    RISHKA_SC_GPIO_SHIFT_OUT,
    RISHKA_SC_GPIO_TONE,
    RISHKA_SC_GPIO_NO_TONE,

    RISHKA_SC_INT_ENABLE,
    RISHKA_SC_INT_DISABLE,
    RISHKA_SC_INT_ATTACH,
    RISHKA_SC_INT_DETACH,
    RISHKA_SC_INT_DIGITAL_PIN,

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
    RISHKA_SC_SPI_USE_INT,
    RISHKA_SC_SPI_TRANSFER
};

#define RISHKA_SC_0(ID) \
    register long a0 asm("a0") = 0; \
    register long scid asm("a7") = ID; \
    asm volatile ("scall" : "+r"(a0) : "r"(scid))

#define RISHKA_SC_1(ID) \
    register long a0 asm("a0") = (long) arg0; \
    register long scid asm("a7") = ID; \
    asm volatile ("scall" : "+r"(a0) : "r"(scid))

#define RISHKA_SC_2(ID) \
    register long a0 asm("a0") = (long) arg0; \
    register long a1 asm("a1") = (long) arg1; \
    register long scid asm("a7") = ID; \
    asm volatile ("scall" : "+r"(a0) : "r"(a1), "r"(scid))

#define RISHKA_SC_3(ID) \
    register long a0 asm("a0") = (long) arg0; \
    register long a1 asm("a1") = (long) arg1; \
    register long a2 asm("a2") = (long) arg2; \
    register long scid asm("a7") = ID; \
    asm volatile ("scall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(scid))

void io_prints(char* arg0) {
    RISHKA_SC_1(RISHKA_SC_IO_PRINTS);
}

void io_printn(long arg0) {
    RISHKA_SC_1(RISHKA_SC_IO_PRINTN);
}

char io_readch() {
    RISHKA_SC_0(RISHKA_SC_IO_READCH);
    return (char) a0;
}

void* sys_memset(void* arg0, int arg1, unsigned int arg2) {
    RISHKA_SC_3(RISHKA_SC_MEM_SET);
    return (void*) a0;
}
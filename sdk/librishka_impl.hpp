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

#ifndef LIBRISHKA_IMPL_H
#define LIBRISHKA_IMPL_H

#include "librishka.h"

enum rishka_syscall {
    RISHKA_SC_IO_PRINTS,
    RISHKA_SC_IO_PRINTN,
    RISHKA_SC_IO_PRINTU,
    RISHKA_SC_IO_PRINTD,
    RISHKA_SC_IO_READCH,
    RISHKA_SC_IO_READLINE,
    RISHKA_SC_IO_AVAILABLE,
    RISHKA_SC_IO_PEEK,
    RISHKA_SC_IO_FIND,
    RISHKA_SC_IO_FIND_UNTIL,
    RISHKA_SC_IO_SET_TIMEOUT,
    RISHKA_SC_IO_GET_TIMEOUT,

    RISHKA_SC_SYS_DELAY_MS,
    RISHKA_SC_SYS_MICROS,
    RISHKA_SC_SYS_MILLIS,
    RISHKA_SC_SYS_SHELLEXEC,
    RISHKA_SC_SYS_EXIT,
    RISHKA_SC_SYS_INFOS,
    RISHKA_SC_SYS_INFON,
    RISHKA_SC_SYS_RANDOM,
    RISHKA_SC_SYS_CD,
    RISHKA_SC_SYS_WD,

    RISHKA_SC_MEM_ALLOC,
    RISHKA_SC_MEM_CALLOC,
    RISHKA_SC_MEM_REALLOC,
    RISHKA_SC_MEM_FREE,
    RISHKA_SC_MEM_SET,

    RISHKA_SC_GPIO_PIN_MODE,
    RISHKA_SC_GPIO_DIGITAL_READ,
    RISHKA_SC_GPIO_DIGITAL_WRITE,
    RISHKA_SC_GPIO_ANALOG_READ,
    RISHKA_SC_GPIO_ANALOG_WRITE,
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

    RISHKA_SC_FS_MKDIR,
    RISHKA_SC_FS_RMDIR,
    RISHKA_SC_FS_DELETE,
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
    RISHKA_SC_FS_WRITEB,
    RISHKA_SC_FS_WRITES,
    RISHKA_SC_FS_POS,
    RISHKA_SC_FS_PATH,
    RISHKA_SC_FS_NAME,
    RISHKA_SC_FS_IS_OK,
    RISHKA_SC_FS_NEXT,
    RISHKA_SC_FS_BUFSIZE,
    RISHKA_SC_FS_LASTWRITE,
    RISHKA_SC_FS_SEEKDIR,
    RISHKA_SC_FS_NEXT_NAME,
    RISHKA_SC_FS_REWIND,

    RISHKA_SC_ARG_COUNT,
    RISHKA_SC_ARG_STR,

    RISHKA_SC_I2C_BEGIN,
    RISHKA_SC_I2C_END,
    RISHKA_SC_I2C_BEGIN_TRANSMISSION,
    RISHKA_SC_I2C_END_TRANSMISSION,
    RISHKA_SC_I2C_WRITE,
    RISHKA_SC_I2C_SLAVE_WRITE,
    RISHKA_SC_I2C_READ,
    RISHKA_SC_I2C_PEEK,
    RISHKA_SC_I2C_REQUEST,
    RISHKA_SC_I2C_AVAILABLE,
    RISHKA_SC_I2C_FLUSH,
    RISHKA_SC_I2C_ON_RECEIVE,
    RISHKA_SC_I2C_ON_REQUEST,
    RISHKA_SC_I2C_GET_TIMEOUT,
    RISHKA_SC_I2C_SET_TIMEOUT,
    RISHKA_SC_I2C_GET_CLOCK,
    RISHKA_SC_I2C_SET_CLOCK,
    RISHKA_SC_I2C_PINS,
    RISHKA_SC_I2C_BUFSIZE,

    RISHKA_SC_KB_LAYOUT_NAME,
    RISHKA_SC_KB_LAYOUT_DESC,
    RISHKA_SC_KB_LED_GET_NUM,
    RISHKA_SC_KB_LED_GET_CAPS,
    RISHKA_SC_KB_LED_GET_SCROLL,
    RISHKA_SC_KB_LED_SET_NUM,
    RISHKA_SC_KB_LED_SET_CAPS,
    RISHKA_SC_KB_LED_SET_SCROLL,
    RISHKA_SC_KB_NEXT_SCAN_CODE,
    RISHKA_SC_KB_DEVICE_TYPE,
    RISHKA_SC_KB_AVAILABLE_VK,
    RISHKA_SC_KB_IS_AVAILABLE,
    RISHKA_SC_KB_IS_VK_DOWN,
    RISHKA_SC_KB_LOCK,
    RISHKA_SC_KB_UNLOCK,
    RISHKA_SC_KB_RESET,

    RISHKA_SC_DISPLAY_SCREEN_HEIGHT,
    RISHKA_SC_DISPLAY_SCREEN_WIDTH,
    RISHKA_SC_DISPLAY_VIEWPORT_HEIGHT,
    RISHKA_SC_DISPLAY_VIEWPORT_WIDTH,
    RISHKA_SC_DISPLAY_SUPPORTED_COLORS,

    RISHKA_SC_NVS_ERASE,
    RISHKA_SC_NVS_ERASE_ALL,
    RISHKA_SC_NVS_SET_I8,
    RISHKA_SC_NVS_SET_I16,
    RISHKA_SC_NVS_SET_I32,
    RISHKA_SC_NVS_SET_I64,
    RISHKA_SC_NVS_SET_U8,
    RISHKA_SC_NVS_SET_U16,
    RISHKA_SC_NVS_SET_U32,
    RISHKA_SC_NVS_SET_U64,
    RISHKA_SC_NVS_SET_STRING,
    RISHKA_SC_NVS_GET_I8,
    RISHKA_SC_NVS_GET_I16,
    RISHKA_SC_NVS_GET_I32,
    RISHKA_SC_NVS_GET_I64,
    RISHKA_SC_NVS_GET_U8,
    RISHKA_SC_NVS_GET_U16,
    RISHKA_SC_NVS_GET_U32,
    RISHKA_SC_NVS_GET_U64,
    RISHKA_SC_NVS_GET_STRING,

    RISHKA_SC_RT_STRPASS,
    RISHKA_SC_RT_YIELD,
    RISHKA_SC_RT_FORK_STREAM
};

static inline long long int double_to_long(double d) {
    union {
        double input;
        long long int output;
    } data;

    data.input = d;
    return data.output;
}

static inline double long_to_float(long long int l) {
    union {
        double output;
        long long int input;
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

static inline i64 rishka_sc_4(i32 scallid, i64 arg0, i64 arg1, i64 arg2, i64 arg3) {
    register i64 a0 asm("a0") = arg0;
    register i64 a1 asm("a1") = arg1;
    register i64 a2 asm("a2") = arg2;
    register i64 a3 asm("a3") = arg3;
    register i64 scid asm("a7") = scallid;

    asm volatile ("scall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a3), "r"(scid));
    return a0;
}

static inline rune rt_strpass() {
    return (rune) rishka_sc_0(RISHKA_SC_RT_STRPASS);    
}

static inline string get_rt_string(u32 len) {
    string str;
    Memory::set(str, 0, len + 1);

    for(u32 i = 0; i < len; i++)
        str[i] = rt_strpass();

    str[len] = '\0';
    return str;
}

#endif
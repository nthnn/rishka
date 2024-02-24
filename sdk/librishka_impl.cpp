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

#include "librishka.h"

enum rishka_syscall {
    RISHKA_SC_IO_PRINTS,
    RISHKA_SC_IO_PRINTN,
    RISHKA_SC_IO_PRINTD,
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
    RISHKA_SC_SPI_TRANSFER,

    RISHKA_SC_RT_STRPASS
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
    Memory::alloc(str, len + 1);

    for(u32 i = 0; i < len; i++)
        str[i] = rt_strpass();

    str[len] = '\0';
    return str;
}

void IO::prints(const string text) {
    rishka_sc_1(RISHKA_SC_IO_PRINTS, (i64) text);
}

void IO::printn(i64 number) {
    rishka_sc_1(RISHKA_SC_IO_PRINTN, (i64) number);
}

void IO::printd(double number) {
    rishka_sc_1(RISHKA_SC_IO_PRINTD, double_to_long(number));
}

rune IO::readch() {
    return (rune) rishka_sc_0(RISHKA_SC_IO_READCH);
}

string IO::readline() {
    return (string) rishka_sc_0(RISHKA_SC_IO_READLINE);
}

void Sys::delay(u64 ms) {
    rishka_sc_1(RISHKA_SC_SYS_DELAY_MS, (long) ms);
}

u64 Sys::micros() {
    return (u64) rishka_sc_0(RISHKA_SC_SYS_MICROS);
}

u64 Sys::millis() {
    return (u64) rishka_sc_0(RISHKA_SC_SYS_MILLIS);
}

i32 Sys::shellexec(string program, i32 argc, string* argv) {
    return (i32) rishka_sc_3(RISHKA_SC_SYS_SHELLEXEC, (i64) program, (i64) argc, (i64) argv);
}

void Sys::exit(i32 code) {
    rishka_sc_1(RISHKA_SC_SYS_EXIT, (i64) code);
}

string Sys::info_str(sysinfos_t key) {
    u32 len = rishka_sc_1(RISHKA_SC_SYS_INFOS, (i64) key);
    return get_rt_string(len);
}

i64 Sys::info_num(sysinfon_t key) {
    return (i64) rishka_sc_1(RISHKA_SC_SYS_INFON, (i64) key);
}

void Memory::alloc(any dest, usize size) {
    rishka_sc_2(RISHKA_SC_MEM_ALLOC, (i64) dest, (i64) size);
}

void Memory::calloc(any dest, usize num, usize size) {
    rishka_sc_3(RISHKA_SC_MEM_CALLOC, (i64) dest, (i64) num, (i64) size);
}

void Memory::realloc(any dest, any ptr, usize size) {
    rishka_sc_3(RISHKA_SC_MEM_REALLOC, (i64) dest, (i64) ptr, (i64) size);
}

void Memory::free(any ptr) {
    rishka_sc_1(RISHKA_SC_MEM_FREE, (i64) ptr);
}

any Memory::set(any dest, i32 c, u32 n) {
    return (any) rishka_sc_3(RISHKA_SC_MEM_SET, (i64) dest, (i64) c, (i64) n);
}

void Gpio::pin_mode(u8 pin, gpio_pin_mode_t mode) {
    rishka_sc_2(RISHKA_SC_GPIO_PIN_MODE, (i64) pin, (i64) mode);
}

i32 Gpio::digital_read(u8 pin) {
    return (i32) rishka_sc_1(RISHKA_SC_GPIO_DIGITAL_READ, (i64) pin);
}

void Gpio::digital_write(u8 pin, gpio_mode_t mode) {
    rishka_sc_2(RISHKA_SC_GPIO_DIGITAL_WRITE, (i64) pin, (i64) mode);
}

i32 Gpio::analog_read(u8 pin) {
    return (i32) rishka_sc_1(RISHKA_SC_GPIO_ANALOG_READ, (i64) pin);
}

void Gpio::analog_write(u8 pin, u8 value) {
    rishka_sc_2(RISHKA_SC_GPIO_ANALOG_WRITE, (i64) pin, (i64) value);
}

u64 Gpio::pulse_in(u8 pin, u8 state, u64 timeout) {
    return (u64) rishka_sc_3(RISHKA_SC_GPIO_PULSE_IN, (i64) pin, (i64) state, (i64) timeout);
}

u64 Gpio::pulse_in_long(u8 pin, u8 state, u64 timeout) {
    return (u64) rishka_sc_3(RISHKA_SC_GPIO_PULSE_IN_LONG, (i64) pin, (i64) state, (i64) timeout);
}

u8 Gpio::shift_in(u8 data, u8 clock, u8 bit_order) {
    return (u8) rishka_sc_3(RISHKA_SC_GPIO_SHIFT_IN, (i64) data, (i64) clock, (i64) bit_order);
}

void Gpio::shift_out(u8 data, u8 clock, u8 bit_order, u8 value) {
    rishka_sc_4(RISHKA_SC_GPIO_SHIFT_OUT, (i64) data, (i64) clock, (i64) bit_order, (i64) value);
}

void Gpio::tone(u8 pin, u32 frequency, u64 duration) {
    rishka_sc_3(RISHKA_SC_GPIO_TONE, (i64) pin, (i64) frequency, (i64) duration);
}

void Gpio::no_tone(u8 pin) {
    rishka_sc_1(RISHKA_SC_GPIO_NO_TONE, (i64) pin);
}

void Int::enable() {
    rishka_sc_0(RISHKA_SC_INT_ENABLE);
}

void Int::disable() {
    rishka_sc_0(RISHKA_SC_INT_DISABLE);
}

void Int::attach(u8 pin, void (*callback)(void), int_mode_t mode) {
    rishka_sc_3(RISHKA_SC_INT_ATTACH, (i64) pin, (i64) callback, (i64) mode);
}

void Int::detach(u8 pin) {
    rishka_sc_1(RISHKA_SC_INT_DETACH, (i64) pin);
}
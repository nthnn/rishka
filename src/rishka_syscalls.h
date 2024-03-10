/* 
 * This file is part of the Rishka distribution (https://github.com/rishka-esp32/rishka).
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

/**
 * @file rishka_syscalls.h
 * @brief Defines system calls for the Rishka kernel.
 *
 * This header file provides declarations for various
 * system calls used within the Rishka kernel. These
 * system calls enable communication and interaction between
 * user programs and the kernel, allowing access to hardware
 * resources, filesystem operations, and other kernel
 * functionalities.
 */

#ifndef RISHKA_SYSCALLS_H
#define RISHKA_SYSCALLS_H

#include <rishka_vm.h>

/**
 * @enum rishka_syscall
 * @brief Enumeration of system call codes for the Rishka kernel.
 *
 * This enumeration defines symbolic names for each system call supported by the Rishka kernel.
 * Each system call has a unique code associated with it, which is used to identify the specific
 * operation requested by the user program.
 */
enum rishka_syscall {
    RISHKA_SC_IO_PRINTS,
    RISHKA_SC_IO_PRINTN,
    RISHKA_SC_IO_PRINTD,
    RISHKA_SC_IO_READCH,
    RISHKA_SC_IO_READLINE,
    RISHKA_SC_IO_READ,
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

    RISHKA_SC_SPI_BEGIN,
    RISHKA_SC_SPI_END,
    RISHKA_SC_SPI_BEGIN_TRANSACTION,
    RISHKA_SC_SPI_END_TRANSACTION,
    RISHKA_SC_SPI_TRANSFER8,
    RISHKA_SC_SPI_TRANSFER16,
    RISHKA_SC_SPI_TRANSFER32,
    RISHKA_SC_SPI_TRANSFER_BYTES,
    RISHKA_SC_SPI_TRANSFER_BITS,
    RISHKA_SC_SPI_SET_HWCS,
    RISHKA_SC_SPI_SET_BIT_ORDER,
    RISHKA_SC_SPI_SET_DATA_MODE,
    RISHKA_SC_SPI_SET_FREQ,
    RISHKA_SC_SPI_SET_CLOCK_DIV,
    RISHKA_SC_SPI_GET_CLOCK_DIV,
    RISHKA_SC_SPI_WRITE8,
    RISHKA_SC_SPI_WRITE16,
    RISHKA_SC_SPI_WRITE32,
    RISHKA_SC_SPI_WRITE_BYTES,
    RISHKA_SC_SPI_WRITE_PIXELS,
    RISHKA_SC_SPI_WRITE_PATTERN,

    RISHKA_SC_RT_STRPASS,
    RISHKA_SC_RT_YIELD
};

class RishkaSyscall final {
public:

    class IO final {
    public:
        static void prints(RishkaVM* vm);
        static void printn(RishkaVM* vm);
        static void printd(RishkaVM* vm);
        static char readch(RishkaVM* vm);
        static size_t readLine(RishkaVM* vm);
        static int read(RishkaVM* vm);
        static int available(RishkaVM* vm);
        static int peek(RishkaVM* vm);
        static bool find(RishkaVM* vm);
        static bool findUntil(RishkaVM* vm);
        static void setTimeout(RishkaVM* vm);
        static uint64_t getTimeout(RishkaVM* vm);
    };

    class Sys final {
    public:
        static void delayImpl(RishkaVM* vm);
        static unsigned long microsImpl();
        static unsigned long millisImpl();
        static int64_t shellExec(RishkaVM* parent_vm);
        static void exit(RishkaVM* vm);
        static uint32_t infos(RishkaVM* vm);
        static long infon(RishkaVM* vm);
        static long randomImpl();
    };

    class Memory final {
    public:
        static void alloc(RishkaVM* vm);
        static void calloc(RishkaVM* vm);
        static void realloc(RishkaVM* vm);
        static void freeHeap(RishkaVM* vm);
        static void* set(RishkaVM* vm);
    };

    class Gpio final {
    public:
        static void pinModeImpl(RishkaVM* vm);
        static bool digitalReadImpl(RishkaVM* vm);
        static void digitalWriteImpl(RishkaVM* vm);
        static uint16_t analogReadImpl(RishkaVM* vm);
        static void analogWriteImpl(RishkaVM* vm);
        static uint64_t pulseInImpl(RishkaVM* vm);
        static uint64_t pulseInLongImpl(RishkaVM* vm);
        static uint8_t shiftInImpl(RishkaVM* vm);
        static void shiftOutImpl(RishkaVM* vm);
        static void toneImpl(RishkaVM* vm);
        static void noToneImpl(RishkaVM* vm);
    };

    class Int final {
    public:
        static void enable();
        static void disable();
        static void attach(RishkaVM* vm);
        static void detach(RishkaVM* vm);
    };

    class FS final {
    public:
        static bool mkdir(RishkaVM* vm);
        static bool rmdir(RishkaVM* vm);
        static bool remove(RishkaVM* vm);
        static bool exists(RishkaVM* vm);
        static bool isfile(RishkaVM* vm);
        static bool isdir(RishkaVM* vm);
        static uint8_t open(RishkaVM* vm);
        static void close(RishkaVM* vm);
        static int available(RishkaVM* vm);
        static void flush(RishkaVM* vm);
        static int peek(RishkaVM* vm);
        static bool seek(RishkaVM* vm);
        static uint32_t size(RishkaVM* vm);
        static int read(RishkaVM* vm);
        static size_t writeb(RishkaVM* vm);
        static size_t writes(RishkaVM* vm);
        static size_t position(RishkaVM* vm);
        static uint32_t path(RishkaVM* vm);
        static uint32_t name(RishkaVM* vm);
        static uint8_t next(RishkaVM* vm);
        static bool bufsize(RishkaVM* vm);
        static uint64_t lastwrite(RishkaVM* vm);
        static bool seekdir(RishkaVM* vm);
        static uint32_t next_name(RishkaVM* vm);
        static void rewind(RishkaVM* vm);
    };

    class Args final {
    public:
        static uint8_t count(RishkaVM* vm);
        static uint32_t value(RishkaVM* vm);
    };

    class I2C final {
    public:
        static bool begin(RishkaVM* vm);
        static bool end(RishkaVM* vm);
        static void begin_transmission(RishkaVM* vm);
        static uint8_t end_transmission(RishkaVM* vm);
        static size_t write(RishkaVM* vm);
        static size_t slave_write(RishkaVM* vm);
        static int read(RishkaVM* vm);
        static int peek(RishkaVM* vm);
        static size_t request(RishkaVM* vm);
        static int available(RishkaVM* vm);
        static void flush();
        static void on_receive(RishkaVM* vm);
        static void on_request(RishkaVM* vm);
        static uint16_t get_timeout();
        static void set_timeout(RishkaVM* vm);
        static bool set_clock(RishkaVM* vm);
        static uint32_t get_clock();
        static bool pins(RishkaVM* vm);
        static size_t bufsize(RishkaVM* vm);
    };

    class SPICall final {
    public:
        static void begin(RishkaVM* vm);
        static void end();
        static void begin_transaction(RishkaVM* vm);
        static void end_transaction();
        static uint8_t transfer8(RishkaVM* vm);
        static uint16_t transfer16(RishkaVM* vm);
        static uint32_t transfer32(RishkaVM* vm);
        static void transfer_bytes(RishkaVM* vm);
        static void transfer_bits(RishkaVM* vm);
        static void set_hwcs(RishkaVM* vm);
        static void set_bit_order(RishkaVM* vm);
        static void set_data_mode(RishkaVM* vm);
        static void set_frequency(RishkaVM* vm);
        static void set_clock_div(RishkaVM* vm);
        static uint32_t get_clock_div();
        static void write8(RishkaVM* vm);
        static void write16(RishkaVM* vm);
        static void write32(RishkaVM* vm);
        static void write_bytes(RishkaVM* vm);
        static void write_pixels(RishkaVM* vm);
        static void write_pattern(RishkaVM* vm);
    };

    class Runtime final {
    public:
        static char strpass();
        static void yield();
    };
};

#endif /* RISHKA_TYPES_H */
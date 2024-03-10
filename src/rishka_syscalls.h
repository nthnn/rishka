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

// Rishka virtual runtime system calls

void rishka_syscall_io_prints(RishkaVM* vm);
void rishka_syscall_io_printn(RishkaVM* vm);
void rishka_syscall_io_printd(RishkaVM* vm);
char rishka_syscall_io_readch(RishkaVM* vm);
size_t rishka_syscall_io_readline(RishkaVM* vm);
int rishka_syscall_io_read(RishkaVM* vm);
int rishka_syscall_io_available(RishkaVM* vm);
int rishka_syscall_io_peek(RishkaVM* vm);
bool rishka_syscall_io_find(RishkaVM* vm);
bool rishka_syscall_io_find_until(RishkaVM* vm);
void rishka_syscall_io_set_timeout(RishkaVM* vm);
uint64_t rishka_syscall_io_get_timeout(RishkaVM* vm);

void rishka_syscall_sys_delay(RishkaVM* vm);
unsigned long rishka_syscall_sys_micros();
unsigned long rishka_syscall_sys_millis();
int64_t rishka_syscall_sys_shellexec(RishkaVM* parent_vm);
void rishka_syscall_sys_exit(RishkaVM* vm);
uint32_t rishka_syscall_sys_infos(RishkaVM* vm);
long rishka_syscall_sys_infon(RishkaVM* vm);
long rishka_syscall_sys_random();

void rishka_syscall_mem_alloc(RishkaVM* vm);
void rishka_syscall_mem_calloc(RishkaVM* vm);
void rishka_syscall_mem_realloc(RishkaVM* vm);
void rishka_syscall_mem_free(RishkaVM* vm);
void* rishka_syscall_mem_set(RishkaVM* vm);

void rishka_syscall_gpio_pinmode(RishkaVM* vm);
bool rishka_syscall_gpio_digitalread(RishkaVM* vm);
void rishka_syscall_gpio_digitalwrite(RishkaVM* vm);
uint16_t rishka_syscall_gpio_analogread(RishkaVM* vm);
void rishka_syscall_gpio_analogwrite(RishkaVM* vm);
uint64_t rishka_syscall_gpio_pulse_in(RishkaVM* vm);
uint64_t rishka_syscall_gpio_pulse_in_long(RishkaVM* vm);
uint8_t rishka_syscall_gpio_shift_in(RishkaVM* vm);
void rishka_syscall_gpio_shift_out(RishkaVM* vm);
void rishka_syscall_gpio_tone(RishkaVM* vm);
void rishka_syscall_gpio_no_tone(RishkaVM* vm);

void rishka_syscall_int_enable();
void rishka_syscall_int_disable();
void rishka_syscall_int_attach(RishkaVM* vm);
void rishka_syscall_int_detach(RishkaVM* vm);

bool rishka_syscall_fs_mkdir(RishkaVM* vm);
bool rishka_syscall_fs_rmdir(RishkaVM* vm);
bool rishka_syscall_fs_delete(RishkaVM* vm);
bool rishka_syscall_fs_exists(RishkaVM* vm);
bool rishka_syscall_fs_isfile(RishkaVM* vm);
bool rishka_syscall_fs_isdir(RishkaVM* vm);
uint8_t rishka_syscall_fs_open(RishkaVM* vm);
void rishka_syscall_fs_close(RishkaVM* vm);
int rishka_syscall_fs_available(RishkaVM* vm);
void rishka_syscall_fs_flush(RishkaVM* vm);
int rishka_syscall_fs_peek(RishkaVM* vm);
bool rishka_syscall_fs_seek(RishkaVM* vm);
uint32_t rishka_syscall_fs_size(RishkaVM* vm);
int rishka_syscall_fs_read(RishkaVM* vm);
size_t rishka_syscall_fs_writeb(RishkaVM* vm);
size_t rishka_syscall_fs_writes(RishkaVM* vm);
size_t rishka_syscall_fs_position(RishkaVM* vm);
uint32_t rishka_syscall_fs_path(RishkaVM* vm);
uint32_t rishka_syscall_fs_name(RishkaVM* vm);
uint8_t rishka_syscall_fs_next(RishkaVM* vm);
bool rishka_syscall_fs_bufsize(RishkaVM* vm);
uint64_t rishka_syscall_fs_lastwrite(RishkaVM* vm);
bool rishka_syscall_fs_seekdir(RishkaVM* vm);
uint32_t rishka_syscall_fs_next_name(RishkaVM* vm);
void rishka_syscall_fs_rewind(RishkaVM* vm);

uint8_t rishka_syscall_arg_count(RishkaVM* vm);
uint32_t rishka_syscall_arg_value(RishkaVM* vm);

bool rishka_syscall_i2c_begin(RishkaVM* vm);
bool rishka_syscall_i2c_end(RishkaVM* vm);
void rishka_syscall_i2c_begin_transmission(RishkaVM* vm);
uint8_t rishka_syscall_i2c_end_transmission(RishkaVM* vm);
size_t rishka_syscall_i2c_write(RishkaVM* vm);
size_t rishka_syscall_i2c_slave_write(RishkaVM* vm);
int rishka_syscall_i2c_read(RishkaVM* vm);
int rishka_syscall_i2c_peek(RishkaVM* vm);
size_t rishka_syscall_i2c_request(RishkaVM* vm);
int rishka_syscall_i2c_available(RishkaVM* vm);
void rishka_syscall_i2c_flush();
void rishka_syscall_i2c_on_receive(RishkaVM* vm);
void rishka_syscall_i2c_on_request(RishkaVM* vm);
uint16_t rishka_syscall_i2c_get_timeout();
void rishka_syscall_i2c_set_timeout(RishkaVM* vm);
bool rishka_syscall_i2c_set_clock(RishkaVM* vm);
uint32_t rishka_syscall_i2c_get_clock();
bool rishka_syscall_i2c_pins(RishkaVM* vm);
size_t rishka_syscall_i2c_bufsize(RishkaVM* vm);

void rishka_syscall_spi_begin(RishkaVM* vm);
void rishka_syscall_spi_end();
void rishka_syscall_spi_begin_transaction(RishkaVM* vm);
void rishka_syscall_spi_end_transaction();
uint8_t rishka_syscall_spi_transfer8(RishkaVM* vm);
uint16_t rishka_syscall_spi_transfer16(RishkaVM* vm);
uint32_t rishka_syscall_spi_transfer32(RishkaVM* vm);
void rishka_syscall_spi_transfer_bytes(RishkaVM* vm);
void rishka_syscall_spi_transfer_bits(RishkaVM* vm);
void rishka_syscall_spi_set_hwcs(RishkaVM* vm);
void rishka_syscall_spi_set_bit_order(RishkaVM* vm);
void rishka_syscall_spi_set_data_mode(RishkaVM* vm);
void rishka_syscall_spi_set_frequency(RishkaVM* vm);
void rishka_syscall_spi_set_clock_div(RishkaVM* vm);
uint32_t rishka_syscall_spi_get_clock_div();
void rishka_syscall_spi_write8(RishkaVM* vm);
void rishka_syscall_spi_write16(RishkaVM* vm);
void rishka_syscall_spi_write32(RishkaVM* vm);
void rishka_syscall_spi_write_bytes(RishkaVM* vm);
void rishka_syscall_spi_write_pixels(RishkaVM* vm);
void rishka_syscall_spi_write_pattern(RishkaVM* vm);

char rishka_syscall_rt_strpass();
void rishka_syscall_rt_yield();

#endif /* RISHKA_TYPES_H */
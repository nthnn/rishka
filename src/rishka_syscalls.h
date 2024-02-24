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

#ifndef RISHKA_SYSCALLS_H
#define RISHKA_SYSCALLS_H

#include <rishka_vm.h>

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
    RISHKA_SC_SPI_TRANSFER,

    RISHKA_SC_RT_STRPASS
};

void rishka_syscall_io_prints(rishka_virtual_machine* vm);
void rishka_syscall_io_printn(rishka_virtual_machine* vm);
void rishka_syscall_io_printd(rishka_virtual_machine* vm);
char rishka_syscall_io_readch();
char* rishka_syscall_io_readline();

void rishka_syscall_sys_delay(rishka_virtual_machine* vm);
unsigned long rishka_syscall_sys_micros();
unsigned long rishka_syscall_sys_millis();
int rishka_syscall_sys_shellexec(rishka_virtual_machine* parent_vm);
void rishka_syscall_sys_exit(rishka_virtual_machine* vm);
uint32_t rishka_syscall_sys_infos(rishka_virtual_machine* vm);
long rishka_syscall_sys_infon(rishka_virtual_machine* vm);
long rishka_syscall_sys_random();

void rishka_syscall_mem_alloc(rishka_virtual_machine* vm);
void rishka_syscall_mem_calloc(rishka_virtual_machine* vm);
void rishka_syscall_mem_realloc(rishka_virtual_machine* vm);
void rishka_syscall_mem_free(rishka_virtual_machine* vm);
void* rishka_syscall_mem_set(rishka_virtual_machine* vm);

void rishka_syscall_gpio_pinmode(rishka_virtual_machine* vm);
bool rishka_syscall_gpio_digitalread(rishka_virtual_machine* vm);
void rishka_syscall_gpio_digitalwrite(rishka_virtual_machine* vm);
int rishka_syscall_gpio_analogread(rishka_virtual_machine* vm);
void rishka_syscall_gpio_analogwrite(rishka_virtual_machine* vm);
uint64_t rishka_syscall_gpio_pulse_in(rishka_virtual_machine* vm);
uint64_t rishka_syscall_gpio_pulse_in_long(rishka_virtual_machine* vm);
uint8_t rishka_syscall_gpio_shift_in(rishka_virtual_machine* vm);
void rishka_syscall_gpio_shift_out(rishka_virtual_machine* vm);
void rishka_syscall_gpio_tone(rishka_virtual_machine* vm);
void rishka_syscall_gpio_no_tone(rishka_virtual_machine* vm);

char rishka_syscall_rt_strpass();

#endif
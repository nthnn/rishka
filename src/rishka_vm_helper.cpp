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

#include <rishka_vm_helper.h>

int8_t rishka_vm_read_i8(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr) {
    return (int8_t)(*(&(((rishka_u8_arrptr*) & vm->memory)->a).v[addr]));
}

int16_t rishka_vm_read_i16(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr) {
    return (int16_t)(*(rishka_u16ptr)(&(((rishka_u8_arrptr*) & vm->memory)->a).v[addr]));
}

int32_t rishka_vm_read_i32(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr) {
    return (int32_t)(*(rishka_u32ptr)(&(((rishka_u8_arrptr*) & vm->memory)->a).v[addr]));
}

int64_t rishka_vm_read_i64(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr) {
    return (int64_t)(*(rishka_u64ptr)(&(((rishka_u8_arrptr*) & vm->memory)->a).v[addr]));
}

uint8_t rishka_vm_read_u8_arrptr(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr) {
    return (*(&(((rishka_u8_arrptr*) & vm->memory)->a).v[addr]));
}

uint16_t rishka_vm_read_u16ptr(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr) {
    return (*(rishka_u16ptr)(&(((rishka_u8_arrptr*) & vm->memory)->a).v[addr]));
}

uint32_t rishka_vm_read_u32ptr(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr) {
    return (*(rishka_u32ptr)(&(((rishka_u8_arrptr*) & vm->memory)->a).v[addr]));
}

void* rishka_vm_getptr(rishka_virtual_machine* vm, uint64_t addr) {
    return (void*)(&(((rishka_u8_arrptr*) & vm->memory)->a).v[addr]);
}

void rishka_vm_write_u8(rishka_virtual_machine* vm, uint64_t addr, uint8_t val) {
    (*(&(((rishka_u8_arrptr*) & vm->memory)->a).v[addr])) = val;
}

void rishka_vm_write_u16(rishka_virtual_machine* vm, uint64_t addr, uint16_t val) {
    (*(rishka_u16ptr)(&(((rishka_u8_arrptr*) & vm->memory)->a).v[addr])) = val;
}

void rishka_vm_write_u32(rishka_virtual_machine* vm, uint64_t addr, uint32_t val) {
    (*(rishka_u32ptr)(&(((rishka_u8_arrptr*) & vm->memory)->a).v[addr])) = val;
}

void rishka_vm_write_u64(rishka_virtual_machine* vm, uint64_t addr, uint64_t val) {
    (*(rishka_u64ptr)(&(((rishka_u8_arrptr*) & vm->memory)->a).v[addr])) = val;
}

int64_t rishka_shl_riscvi64(int64_t a, int64_t b) {
    if(b >= 0 && b < 64)
        return ((uint64_t) a) << b;
    else if(b < 0 && b > -64)
        return (uint64_t) a >> -b;

    return 0;
}

int64_t rishka_shr_riscvi64(int64_t a, int64_t b) {
    if(b >= 0 && b < 64)
        return (uint64_t) a >> b;
    else if(b < 0 && b > -64)
        return (uint64_t) a << -b;

    return 0;
}

int64_t rishka_asr_riscvi64(int64_t a, int64_t b) {
    if(b >= 0 && b < 64)
        return a >> b;
    else if(b >= 64)
        return a < 0 ? -1 : 0;
    else if(b < 0 && b > -64)
        return a << -b;

    return 0;
}

int64_t rishka_shr_riscvi128(int64_t a, int64_t b) {
    if(b >= 0 && b < 128)
        return (uint64_t) a >> b;
    else if(b < 0 && b > -128)
        return (uint64_t) a << -b;

    return 0;
}

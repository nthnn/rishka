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

#ifndef RISHKA_VM_HELPER_H
#define RISHKA_VM_HELPER_H

#include <rishka_types.h>
#include <rishka_vm.h>

int8_t rishka_vm_read_i8(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr);
int16_t rishka_vm_read_i16(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr);
int32_t rishka_vm_read_i32(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr);
int64_t rishka_vm_read_i64(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr);

uint8_t rishka_vm_read_u8_arrptr(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr);
uint16_t rishka_vm_read_u16ptr(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr);
uint32_t rishka_vm_read_u32ptr(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr);

void* rishka_vm_getptr(rishka_virtual_machine* vm, uint64_t addr);
void rishka_vm_write_u8(rishka_virtual_machine* vm, uint64_t addr, uint8_t val);
void rishka_vm_write_u16(rishka_virtual_machine* vm, uint64_t addr, uint16_t val);
void rishka_vm_write_u32(rishka_virtual_machine* vm, uint64_t addr, uint32_t val);
void rishka_vm_write_u64(rishka_virtual_machine* vm, uint64_t addr, uint64_t val);

int64_t rishka_shl_riscvi64(int64_t a, int64_t b);
int64_t rishka_shr_riscvi64(int64_t a, int64_t b);
int64_t rishka_asr_riscvi64(int64_t a, int64_t b);
int64_t rishka_shr_riscvi128(int64_t a, int64_t b);

#endif
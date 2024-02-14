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

#ifndef RISHKA_TYPES_H
#define RISHKA_TYPES_H

#include <rishka_commons.h>

typedef uint8_t* rishka_u8ptr;
typedef uint16_t* rishka_u16ptr;
typedef uint32_t* rishka_u32ptr;
typedef uint64_t* rishka_u64ptr;

typedef struct {} rishka_nil_type;
typedef rishka_nil_type rishka_type;

typedef struct {
    rishka_u8ptr data;
    uintptr_t size;
} rishka_string;

typedef struct {
    uint8_t v[RISHKA_VM_STACK_SIZE];
} rishka_u8_arr;

typedef union {
    rishka_u8_arr a;
    uint8_t p[RISHKA_VM_STACK_SIZE];
} rishka_u8_arrptr;

typedef struct {
    uint64_t v[32];
} rishka_u64_arr;

typedef union {
    rishka_u64_arr a;
    uint64_t p[32];
} rishka_u64_arrptr;

#endif
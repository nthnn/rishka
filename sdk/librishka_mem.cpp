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
#include "librishka_impl.hpp"

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
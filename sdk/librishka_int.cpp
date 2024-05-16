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
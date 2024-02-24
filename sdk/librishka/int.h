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

#ifndef LIBRISHKA_INT_H
#define LIBRISHKA_INT_H

#include <librishka/types.h>

typedef enum {
    INT_CHANGE = 0x1,
    INT_FAILING = 0x2,
    INT_RISING = 0x3
} int_mode_t;

class Int {
public:
    static void enable();
    static void disable();

    static void attach(u8 pin, void (*callback)(void), int_mode_t mode);
    static void detach(u8 pin);
};

#endif
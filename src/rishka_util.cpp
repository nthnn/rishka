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

#include <rishka_types.h>
#include <rishka_util.h>

char* rishka_string2cstring(rishka_string s) {
    return (s.size == 0) ? (char*) "": (char*) s.data;
}

rishka_string rishka_cstring2string(const char* s) {
    if(s == NULL)
        return (rishka_string) {0};

    uintptr_t size = strlen(s);
    if(size == 0)
        return (rishka_string) {0};
    return (rishka_string) {(uint8_t*)s, size};
}

int64_t rishka_double_to_long(double d) {
    union {
        double input;
        int64_t output;
    } data;

    data.input = d;
    return data.output;
}

double rishka_long_to_double(int64_t l) {
    union {
        double output;
        int64_t input;
    } data;

    data.input = l;
    return data.output;
}
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
 * @file rishka_util.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Rishka Utility Functions
 *
 * This file contains utility functions for converting value
 * between double precision floating-point and long integer values.
 */

#ifndef RISHKA_UTIL_H
#define RISHKA_UTIL_H

#include <rishka_types.h>

/**
 * @brief Converts a double value to a long integer.
 *
 * This function converts a double precision floating-point value `d` to a long integer and returns it.
 *
 * @param d The double precision floating-point value to be converted.
 * @return The converted long integer value.
 */
inline int64_t rishka_double_to_long(double d) {
    union {
        double input;
        int64_t output;
    } data;

    data.input = d;
    return data.output;
}

/**
 * @brief Converts a long integer to a double value.
 *
 * This function converts a long integer `l` to a double precision floating-point value and returns it.
 *
 * @param l The long integer value to be converted.
 * @return The converted double precision floating-point value.
 */
inline double rishka_long_to_double(int64_t l) {
    union {
        double output;
        int64_t input;
    } data;

    data.input = l;
    return data.output;
}

#endif /* RISHKA_UTIL_H */
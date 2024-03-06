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
 * @file rishka_types.h
 * @author [Nathanne Isip](https://github.com)
 * @brief Defines custom data types for the Rishka kernel.
 *
 * This header file provides definitions for various custom data
 * types used within the Rishka kernel. These data types are essential
 * for representing different kinds of data and facilitating communication
 * and manipulation within the kernel.
 */

#ifndef RISHKA_TYPES_H
#define RISHKA_TYPES_H

/**
 * @brief Pointer to an 8-bit unsigned integer.
 */
typedef uint8_t* rishka_u8ptr;

/**
 * @brief Pointer to a 16-bit unsigned integer.
 */
typedef uint16_t* rishka_u16ptr;

/**
 * @brief Pointer to a 32-bit unsigned integer.
 */
typedef uint32_t* rishka_u32ptr;

/**
 * @brief Pointer to a 64-bit unsigned integer.
 */
typedef uint64_t* rishka_u64ptr;

/**
 * @brief Represents a nil type in Rishka.
 *
 * This type is used to indicate a lack of data in certain contexts.
 */
typedef struct {} rishka_nil_type;

/**
 * @brief Alias for the nil type in Rishka.
 */
typedef rishka_nil_type rishka_type;

/**
 * @brief Represents an array of 8-bit unsigned integers in Rishka.
 */
typedef struct {
    uint8_t v[RISHKA_VM_STACK_SIZE];
} rishka_u8_arr;

/**
 * @brief Represents a pointer to an array of 8-bit unsigned integers in Rishka.
 */
typedef union {
    rishka_u8_arr a;
    uint8_t p[RISHKA_VM_STACK_SIZE];
} rishka_u8_arrptr;

/**
 * @brief Represents an array of 64-bit unsigned integers in Rishka.
 */
typedef struct {
    uint64_t v[32];
} rishka_u64_arr;

/**
 * @brief Represents a pointer to an array of 64-bit unsigned integers in Rishka.
 */
typedef union {
    rishka_u64_arr a;
    uint64_t p[32];
} rishka_u64_arrptr;

#endif /* RISHKA_TYPES_H */
/* 
 * This file is part of the Rishka distribution (https://github.com/nthnn/rishka).
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
 * @file types.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file for common data types used in Rishka applications.
 *
 * This header file defines common Rust-inspired data types and macros used throughout
 * Rishka applications, including typedefs for integer types, string type, and boolean values.
 */

#ifndef LIBRISHKA_TYPES_H
#define LIBRISHKA_TYPES_H

/**
 * @typedef rune
 * @brief Alias for the Unicode character type.
 */
typedef char                rune;

/**
 * @typedef string
 * @brief Alias for the string type.
 */
typedef char*               string;

/**
 * @typedef i8
 * @brief Alias for the signed 8-bit integer type.
 */
typedef signed char         i8;

/**
 * @typedef i16
 * @brief Alias for the signed 16-bit integer type.
 */
typedef signed short int    i16;

/**
 * @typedef i32
 * @brief Alias for the signed 32-bit integer type.
 */
typedef signed int          i32;

/**
 * @typedef i64
 * @brief Alias for the signed 64-bit integer type.
 */
typedef signed long int     i64;

/**
 * @typedef u8
 * @brief Alias for the unsigned 8-bit integer type.
 */
typedef unsigned char       u8;

/**
 * @typedef u16
 * @brief Alias for the unsigned 16-bit integer type.
 */
typedef unsigned short int  u16;

/**
 * @typedef u32
 * @brief Alias for the unsigned 32-bit integer type.
 */
typedef unsigned int        u32;

/**
 * @typedef u64
 * @brief Alias for the unsigned 64-bit integer type.
 */
typedef unsigned long int   u64;

/**
 * @typedef usize
 * @brief Alias for the unsigned integer type representing size.
 */
typedef u32                 usize;

/**
 * @typedef any
 * @brief Alias for the generic pointer type.
 */
typedef void*               any;

/**
 * @def F(str)
 * @brief Macro to convert a string literal to a char pointer.
 */
#define F(str) ((char*) str)

/**
 * @def nil
 * @brief Macro representing a null pointer.
 */
#define nil ((void*) 0)

/**
 * @def false
 * @brief Macro representing the boolean value false.
 */
#define false 0

/**
 * @def true
 * @brief Macro representing the boolean value true.
 */
#define true 1

#endif /* LIBRISHKA_TYPES_H */
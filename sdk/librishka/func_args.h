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
 * @file librishka/func_args.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file defining macros and types for handling function arguments in the Rishka SDK.
 *
 * This header file provides macros and types for handling function arguments in the Rishka SDK. It defines a type
 * `func_arg_list` representing a list of function arguments and macros for starting, accessing, and ending the
 * traversal of function arguments.
 */

#ifndef LIBRISHKA_FUNC_ARGS_H
#define LIBRISHKA_FUNC_ARGS_H

/**
 * @typedef func_arg_list
 * @brief Type representing a list of function arguments.
 *
 * This typedef represents a list of function arguments, allowing functions to accept variable numbers of arguments.
 */
typedef __builtin_va_list func_arg_list;

/**
 * @def func_arg_start(ap, param)
 * @brief Macro for starting traversal of function arguments.
 *
 * This macro initializes a `func_arg_list` object `ap` for traversing the function arguments starting from the
 * parameter `param`.
 *
 * @param ap A `func_arg_list` object to be initialized.
 * @param param The last named parameter before the variadic arguments.
 */
#define func_arg_start(ap, param)   __builtin_va_start(ap, param)

/**
 * @def func_arg_get(ap, type)
 * @brief Macro for accessing function arguments.
 *
 * This macro retrieves the next argument of type `type` from the `func_arg_list` object `ap`.
 *
 * @param ap A `func_arg_list` object representing the list of function arguments.
 * @param type The type of the argument to retrieve.
 * @return The next argument of the specified type.
 */
#define func_arg_get(ap, type)      __builtin_va_arg(ap, type)

/**
 * @def func_arg_end(ap)
 * @brief Macro for ending traversal of function arguments.
 *
 * This macro ends the traversal of function arguments represented by the `func_arg_list` object `ap`.
 *
 * @param ap A `func_arg_list` object representing the list of function arguments.
 */
#define func_arg_end(ap)            __builtin_va_end(ap)

#endif
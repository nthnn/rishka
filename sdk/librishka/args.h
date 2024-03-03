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

/**
 * @file args.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file for handling command line arguments in Rishka applications.
 *
 * This header file defines the Args class, which provides functionalities
 * for accessing and handling command line arguments passed to Rishka applications.
 */

#ifndef LIBRISHKA_ARGS_H
#define LIBRISHKA_ARGS_H

#include <librishka/types.h>

/**
 * @class Args
 * @brief Class for handling command line arguments in Rishka applications.
 *
 * The Args class provides static methods for accessing command line arguments
 * passed to the Rishka application. It allows developers to retrieve the number
 * of command line arguments and obtain the values of individual arguments.
 */
class Args final {
public:
    /**
     * @brief Get the number of command line arguments.
     *
     * This method returns the total number of command line arguments passed
     * to the Rishka application.
     *
     * @return The number of command line arguments.
     */
    static i32 count();

    /**
     * @brief Get the value of a command line argument at a specific index.
     *
     * This method retrieves the value of a command line argument at the specified
     * index. The index should be within the range [0, count() - 1].
     *
     * @param index The index of the command line argument.
     * @return The value of the command line argument at the specified index.
     */
    static string value(u8 index);
};

#endif /* LIBRISHKA_ARGS_H */
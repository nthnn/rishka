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
 * @file memory.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file for memory management in Rishka applications.
 *
 * This header file defines the Memory class, which provides functionalities
 * for dynamic memory management on ESP32-WROVER microcontrollers.
 */

#ifndef LIBRISHKA_MEM_H
#define LIBRISHKA_MEM_H

#include <librishka/types.h>

/**
 * @class Memory
 * @brief Class for handling memory management operations in Rishka applications.
 *
 * The Memory class provides static methods for allocating, reallocating, and freeing memory,
 * as well as functions for setting memory values on ESP32-WROVER microcontrollers.
 */
class Memory final {
public:
    static void initialize();

    /**
     * @brief Allocate memory.
     *
     * This method allocates a block of memory of the specified size.
     *
     * @param size The size of the memory block to allocate in bytes.
     * @returns Pointer to the memory block of the allocated memory.
     */
    static any alloc(usize size);

    /**
     * @brief Allocate and clear memory.
     *
     * This method allocates a block of memory
     * of the specified size and initializes it to zero.
     *
     * @param dest Pointer to the destination memory block.
     * @param num The number of elements in the memory block.
     * @param size The size of each element in bytes.
     */
    static any calloc(usize num, usize size);

    /**
     * @brief Reallocate memory.
     *
     * This method reallocates a block of memory to the specified size.
     *
     * @param dest Pointer to the destination memory block.
     * @param ptr Pointer to the previously allocated memory block.
     * @param size The new size of the memory block in bytes.
     */
    static any realloc(any ptr, usize size);


    /**
     * @brief Free memory.
     *
     * This method frees the memory block previously allocated by alloc, calloc, or realloc.
     *
     * @param ptr Pointer to the memory block to free.
     */
    static void free(any ptr);

    /**
     * @brief Set memory values.
     *
     * This method sets the first n bytes of the memory block pointed to by dest to the specified value.
     *
     * @param dest Pointer to the destination memory block.
     * @param c The value to set.
     * @param n The number of bytes to set.
     * @return Pointer to the memory block.
     */
    static any set(any dest, i32 c, usize n);
};

#endif /* LIBRISHKA_MEM_H */
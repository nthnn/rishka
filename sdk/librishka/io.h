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
 * @file io.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file for input/output operations in Rishka applications.
 *
 * This header file defines the IO class, which provides functionalities
 * for input/output operations on ESP32-WROVER microcontrollers.
 */

#ifndef LIBRISHKA_IO_H
#define LIBRISHKA_IO_H

#include <librishka/types.h>

/**
 * @class IO
 * @brief Class for handling input/output operations in Rishka applications.
 *
 * The IO class provides static methods for performing input/output operations,
 * such as printing text and numbers, reading input, searching for patterns,
 * and setting timeouts for input operations on ESP32-WROVER microcontrollers.
 */
class IO final {
public:
    /**
     * @brief Print text to the output stream.
     *
     * This method prints the specified text to the output stream.
     *
     * @param text The text to be printed.
     */
    static void print(const string text);

    /**
     * @brief Print an integer number to the output stream.
     *
     * This method prints the specified integer number to the output stream.
     *
     * @param number The integer number to be printed.
     */
    static void print(i64 number);

    /**
     * @brief Print a floating-point number to the output stream.
     *
     * This method prints the specified floating-point number to the output stream.
     *
     * @param number The floating-point number to be printed.
     */
    static void print(double number);

    /**
     * @brief Check if there is data available to read from the input stream.
     *
     * This method checks if there is data available to read from the input stream.
     *
     * @return The number of bytes available to read, or 0 if no data is available.
     */
    static i32 available();

    /**
     * @brief Peek at the next character in the input stream.
     *
     * This method peeks at the next character in the input stream without removing it.
     *
     * @return The next character in the input stream, or -1 if no data is available.
     */
    static i32 peek();

    /**
     * @brief Search for a target string in the input stream.
     *
     * This method searches for the specified target string in the input stream.
     *
     * @param target The target string to search for.
     * @param size The size of the target string.
     * @return True if the target string is found, false otherwise.
     */
    static bool find(string target, usize size);

    /**
     * @brief Search for a target string in the input stream until a terminator is encountered.
     *
     * This method searches for the specified target string in the input stream until
     * the specified terminator string is encountered.
     *
     * @param target The target string to search for.
     * @param terminator The terminator string.
     * @return True if the target string is found before the terminator, false otherwise.
     */
    static bool find_until(string target, string terminator);

    /**
     * @brief Set the timeout for input operations.
     *
     * This method sets the timeout for input operations in microseconds.
     *
     * @param timeout The timeout value in microseconds.
     */
    static void set_timeout(u64 timeout);

    /**
     * @brief Get the current timeout for input operations.
     *
     * This method returns the current timeout for input operations in microseconds.
     *
     * @return The current timeout value in microseconds.
     */
    static u64 get_timeout();

    /**
     * @brief Read a Unicode character from the input stream.
     *
     * This method reads a Unicode character from the input stream.
     *
     * @return The Unicode character read from the input stream.
     */
    static rune readch();

    /**
     * @brief Read a line of text from the input stream.
     *
     * This method reads a line of text from the input stream.
     *
     * @return The line of text read from the input stream.
     */
    static string readline();
};

#endif /* LIBRISHKA_IO_H */
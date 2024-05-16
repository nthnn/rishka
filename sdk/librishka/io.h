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

#define TERM_FG_BLACK       F("\e[30m")
#define TERM_FG_RED         F("\e[31m")
#define TERM_FG_GREEN       F("\e[32m")
#define TERM_FG_YELLOW      F("\e[33m")
#define TERM_FG_BLUE        F("\e[34m")
#define TERM_FG_MAGENTA     F("\e[35m")
#define TERM_FG_CYAN        F("\e[36m")
#define TERM_FG_WHITE       F("\e[37m")
#define TERM_FG_HBLACK      F("\e[90m")
#define TERM_FG_HRED        F("\e[91m")
#define TERM_FG_HGREEN      F("\e[92m")
#define TERM_FG_HYELLOW     F("\e[93m")
#define TERM_FG_HBLUE       F("\e[94m")
#define TERM_FG_HMAGENTA    F("\e[95m")
#define TERM_FG_HCYAN       F("\e[96m")
#define TERM_FG_HWHITE      F("\e[97m")

#define TERM_BG_BLACK       F("\e[40m")
#define TERM_BG_RED         F("\e[41m")
#define TERM_BG_GREEN       F("\e[42m")
#define TERM_BG_YELLOW      F("\e[43m")
#define TERM_BG_BLUE        F("\e[44m")
#define TERM_BG_MAGENTA     F("\e[45m")
#define TERM_BG_CYAN        F("\e[46m")
#define TERM_BG_WHITE       F("\e[47m")
#define TERM_BG_HBLACK      F("\e[100m")
#define TERM_BG_HRED        F("\e[101m")
#define TERM_BG_HGREEN      F("\e[102m")
#define TERM_BG_HYELLOW     F("\e[103m")
#define TERM_BG_HBLUE       F("\e[104m")
#define TERM_BG_HMAGENTA    F("\e[105m")
#define TERM_BG_HCYAN       F("\e[106m")
#define TERM_BG_HWHITE      F("\e[107m")

#define TERM_STYLE_NORMAL       F("\e[0m")
#define TERM_STYLE_BOLD         F("\e[1m")
#define TERM_STYLE_ITALIC       F("\e[3m")
#define TERM_STYLE_UNDERLINED   F("\e[4m")
#define TERM_STYLE_BLINK        F("\e[5m")
#define TERM_STYLE_INVERSE      F("\e[7m")

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
     * @brief Print text to the output stream.
     *
     * This method prints the specified text to
     * the output stream with specified foreground
     * color and style.
     *
     * @param text The text to be printed.
     * @param fg Foreground color of the output text.
     * @param style Style/format of the output.
     */
    static void print(
        const string text,
        const string fg,
        const string style = TERM_STYLE_NORMAL
    );

    /**
     * @brief Print text to the output stream.
     *
     * This method prints the specified text to
     * the output stream with specified foreground
     * color, background color, and style.
     *
     * @param text The text to be printed.
     * @param fg Foreground color of the output text.
     * @param bg Background color of the output text.
     * @param style Style/format of the output.
     */
    static void print(
        const string text,
        const string fg,
        const string bg = TERM_BG_BLACK,
        const string style = TERM_STYLE_NORMAL
    );

    /**
     * @brief Print text to the output stream.
     *
     * This method prints the specified text to the output
     * stream followed by a new line.
     *
     * @param text The text to be printed.
     */
    static void println(const string text);

    /**
     * @brief Print an integer number to the output stream.
     *
     * This method prints the specified integer number to the output stream.
     *
     * @param number The integer number to be printed.
     */
    static void print(i64 number);

    /**
     * @brief Print an integer number to the output stream.
     *
     * This method prints the specified integer number to
     * the output stream followed by a new line.
     *
     * @param number The integer number to be printed.
     */
    static void println(i64 number);

    /**
     * @brief Print a floating-point number to the output stream.
     *
     * This method prints the specified floating-point number
     * to the output stream.
     *
     * @param number The floating-point number to be printed.
     */
    static void print(double number);

    /**
     * @brief Print a floating-point number to the output stream.
     *
     * This method prints the specified floating-point number
     * to the output stream followed by a new line.
     *
     * @param number The floating-point number to be printed.
     */
    static void println(double number);

    /**
     * @brief Prints a new line to the output stream.
     * 
     * THis method prints a new line to the current
     * output stream.
    */
    static void println();

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
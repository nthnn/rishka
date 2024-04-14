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
 * @file sys.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file for system utilities in Rishka applications.
 *
 * This header file defines the Sys class, which provides functionalities
 * for accessing system information and executing system commands on ESP32-WROVER microcontrollers.
 */

#ifndef LIBRISHKA_SYS_H
#define LIBRISHKA_SYS_H

#include <librishka/types.h>

/**
 * @enum sysinfon_t
 * @brief Enumeration for system information numeric keys.
 *
 * The sysinfon_t enumeration defines numeric keys used to retrieve
 * various system information such as chip cores, CPU frequency, memory usage, etc.
 */
typedef enum {
    SYSINFO_CHIPCORES,      /**< Number of CPU cores */
    SYSINFO_CHIP_REV,       /**< Chip revision */
    SYSINFO_CPU_FREQ,       /**< CPU frequency in Hz */
    SYSINFO_CYCLE_COUNT,    /**< CPU cycle count */
    SYSINFO_EFUSE_MAC,      /**< EFUSE MAC address */
    SYSINFO_FLASH_MODE,     /**< Flash mode */
    SYSINFO_FLASH_SPEED,    /**< Flash speed */
    SYSINFO_FREE_HEAP,      /**< Free heap memory */
    SYSINFO_FREE_PSRAM,     /**< Free PSRAM memory */
    SYSINFO_HEAP_SIZE,      /**< Total heap size */
    SYSINFO_MAX_ALLOC_HEAP, /**< Maximum allocatable heap */
    SYSINFO_MIN_FREE_HEAP,  /**< Minimum free heap */
    SYSINFO_MIN_FREE_PSRAM, /**< Minimum free PSRAM */
    SYSINFO_PSRAM_SIZE,     /**< Total PSRAM size */
    SYSINFO_TEMP_VAL,       /**< Chip temperature */
    SYSINFO_CARD_TYPE,      /**< SD card type */
    SYSINFO_CARD_SIZE,      /**< SD card size */
    SYSINFO_NUM_SECTORS,    /**< Number of SD card sectors */
    SYSINFO_SECTOR_SIZE,    /**< SD card sector size */
    SYSINFO_TOTAL_STORAGE,  /**< Total storage capacity */
    SYSINFO_USED_STORAGE    /**< Used storage capacity */
} sysinfon_t;

/**
 * @enum sysinfos_t
 * @brief Enumeration for system information string keys.
 *
 * The sysinfos_t enumeration defines string keys used to retrieve
 * various system information such as chip model, SDK version, etc.
 */
typedef enum {
    SYSINFO_CHIPMODEL,      /**< Chip model */
    SYSINFO_SDK_VERSION,    /**< SDK version */
    SYSINFO_SKETCH_MD5      /**< Sketch MD5 hash */
} sysinfos_t;

/**
 * @enum sdcard_t
 * @brief Enumeration for SD card types.
 *
 * The sdcard_t enumeration defines the types of SD cards supported.
 */
typedef enum {
    SD_CARD_NONE,       /**< No SD card */
    SD_CARD_MMC,        /**< MMC SD card */
    SD_CARD_SD,         /**< Standard SD card */
    SD_CARD_SDHC,       /**< SDHC SD card */
    SD_CARD_UNKNOWN     /**< Unknown SD card type */
} sdcard_t;

/**
 * @class Sys
 * @brief Class for handling system utilities in Rishka applications.
 *
 * The Sys class provides static methods for delaying execution, accessing system time,
 * executing shell commands, retrieving system information, and exiting the application.
 */
class Sys final {
public:
    /**
     * @brief Delay execution for a specified duration.
     *
     * This method suspends program execution for the specified number of milliseconds.
     *
     * @param ms The duration to delay in milliseconds.
     */
    static void delay(u64 ms);

    /**
     * @brief Get the current system time in microseconds.
     *
     * This method returns the current system time in microseconds since boot.
     *
     * @return The current system time in microseconds.
     */
    static u64 micros();

    /**
     * @brief Get the current system time in milliseconds.
     *
     * This method returns the current system time in milliseconds since boot.
     *
     * @return The current system time in milliseconds.
     */
    static u64 millis();

    /**
     * @brief Execute a shell command.
     *
     * This method executes a shell command with the specified binary
     * program file name, argument count, and argument array.
     *
     * @param program The file name of the binary program to execute.
     * @param argc The number of arguments in the argument array.
     * @param argv The array of arguments for the shell command.
     * @return The exit status of the shell command.
     */
    static i64 shellexec(string program, i32 argc, string* argv);

    /**
     * @brief Terminate the application with an exit code.
     *
     * This method terminates the application with the specified exit code.
     *
     * @param code The exit code for the application.
     */
    static void exit(i32 code);

    /**
     * @brief Get system information as a string.
     *
     * This method retrieves system information specified by the key
     * as a string representation.
     *
     * @param key The key for the system information.
     * @return The system information as a string.
     */
    static string info_str(sysinfos_t key);

    /**
     * @brief Get system information as a numeric value.
     *
     * This method retrieves system information specified by the key
     * as a numeric value.
     *
     * @param key The key for the system information.
     * @return The system information as a numeric value.
     */
    static i64 info_num(sysinfon_t key);

    /**
     * @brief Generate a random integer value.
     *
     * The random() function generates a random integer value within the
     * range supported by the underlying hardware's random number generator.
     * The generated random value is suitable for various applications such as
     * cryptographic operations, simulation, and randomization tasks.
     *
     * @return A random integer value within the supported range.
     *
     */
    static i64 random();

    /**
     * @brief Changes the current working directory.
     *
     * This static method changes the current working directory to the specified directory.
     *
     * @param directory The directory to change to.
     * @return True if the directory change was successful, false otherwise.
     */
    static bool change_dir(char* directory);

    /**
     * @brief Retrieves the current working directory.
     *
     * This static method returns the current working directory as a string.
     *
     * @return A string representing the current working directory.
     */
    static string working_dir();
};

#endif /* LIBRISHKA_SYS_H */
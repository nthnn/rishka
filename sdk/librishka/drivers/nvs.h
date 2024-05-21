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
 * @file librishka/drivers/nvs.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Non-Volatile Storage (NVS) API for Rishka's SDK
 *
 * This header file defines the NVS class, which
 * provides methods for interacting with the
 * non-volatile storage (NVS) of the Rishka system.
 * The NVS class allows for storing, retrieving,
 * and managing key-value pairs of various data
 * types in persistent storage.
 */

#ifndef LIBRISHKA_DRIVERS_NVS_H
#define LIBRISHKA_DRIVERS_NVS_H

#include <librishka/types.h>

/**
 * @class NVS
 * @brief Provides an interface to the non-volatile
 *        storage (NVS) in the Rishka system.
 *
 * The NVS class offers a variety of static methods
 * to handle persistent storage of key-value pairs.
 * It supports storing and retrieving different types
 * of data, including integers, unsigned integers,
 * and strings. Additionally, it provides methods to
 * manage Wi-Fi configuration data.
 */
class NVS final {
public:
    /**
     * @brief Commits the changes to the NVS.
     * @return true if the commit was successful, false otherwise.
     */
    static bool commit();

    /**
     * @brief Erases all key-value pairs in the NVS.
     * @param force If true, forces the erase operation.
     * @return true if the erase was successful, false otherwise.
     */
    static bool erase_all(bool force = true);

    /**
     * @brief Erases a specific key-value pair from the NVS.
     * @param key The key to be erased.
     * @param force If true, forces the erase operation.
     * @return true if the erase was successful, false otherwise.
     */
    static bool erase(string key, bool force = true);

    /**
     * @brief Sets an 8-bit signed integer value in the NVS.
     * @param key The key associated with the value.
     * @param value The value to be stored.
     * @param force If true, forces the set operation.
     * @return true if the set operation was successful, false otherwise.
     */
    static bool set_i8(string key, i8 value, bool force = true);

    /**
     * @brief Sets a 16-bit signed integer value in the NVS.
     * @param key The key associated with the value.
     * @param value The value to be stored.
     * @param force If true, forces the set operation.
     * @return true if the set operation was successful, false otherwise.
     */
    static bool set_i16(string key, i16 value, bool force = true);

    /**
     * @brief Sets a 32-bit signed integer value in the NVS.
     * @param key The key associated with the value.
     * @param value The value to be stored.
     * @param force If true, forces the set operation.
     * @return true if the set operation was successful, false otherwise.
     */
    static bool set_i32(string key, i32 value, bool force = true);

    /**
     * @brief Sets a 64-bit signed integer value in the NVS.
     * @param key The key associated with the value.
     * @param value The value to be stored.
     * @param force If true, forces the set operation.
     * @return true if the set operation was successful, false otherwise.
     */
    static bool set_i64(string key, i64 value, bool force = true);

    /**
     * @brief Sets an 8-bit unsigned integer value in the NVS.
     * @param key The key associated with the value.
     * @param value The value to be stored.
     * @param force If true, forces the set operation.
     * @return true if the set operation was successful, false otherwise.
     */
    static bool set_u8(string key, u8 value, bool force = true);

    /**
     * @brief Sets a 16-bit unsigned integer value in the NVS.
     * @param key The key associated with the value.
     * @param value The value to be stored.
     * @param force If true, forces the set operation.
     * @return true if the set operation was successful, false otherwise.
     */
    static bool set_u16(string key, u16 value, bool force = true);

    /**
     * @brief Sets a 32-bit unsigned integer value in the NVS.
     * @param key The key associated with the value.
     * @param value The value to be stored.
     * @param force If true, forces the set operation.
     * @return true if the set operation was successful, false otherwise.
     */
    static bool set_u32(string key, u32 value, bool force = true);

    /**
     * @brief Sets a 64-bit unsigned integer value in the NVS.
     * @param key The key associated with the value.
     * @param value The value to be stored.
     * @param force If true, forces the set operation.
     * @return true if the set operation was successful, false otherwise.
     */
    static bool set_u64(string key, u64 value, bool force = true);

    /**
     * @brief Retrieves an 8-bit signed integer value from the NVS.
     * @param key The key associated with the value.
     * @param defvalue The default value if the key is not found.
     * @return The value associated with the key, or defvalue if the key is not found.
     */
    static i8 get_i8(string key, i8 defvalue);

    /**
     * @brief Retrieves a 16-bit signed integer value from the NVS.
     * @param key The key associated with the value.
     * @param defvalue The default value if the key is not found.
     * @return The value associated with the key, or defvalue if the key is not found.
     */
    static i16 get_i16(string key, i16 defvalue);

    /**
     * @brief Retrieves a 32-bit signed integer value from the NVS.
     * @param key The key associated with the value.
     * @param defvalue The default value if the key is not found.
     * @return The value associated with the key, or defvalue if the key is not found.
     */
    static i32 get_i32(string key, i32 defvalue);

    /**
     * @brief Retrieves a 64-bit signed integer value from the NVS.
     * @param key The key associated with the value.
     * @param defvalue The default value if the key is not found.
     * @return The value associated with the key, or defvalue if the key is not found.
     */
    static i64 get_i64(string key, i64 defvalue);

    /**
     * @brief Retrieves an 8-bit unsigned integer value from the NVS.
     * @param key The key associated with the value.
     * @param defvalue The default value if the key is not found.
     * @return The value associated with the key, or defvalue if the key is not found.
     */
    static u8 get_u8(string key, u8 defvalue);

    /**
     * @brief Retrieves a 16-bit unsigned integer value from the NVS.
     * @param key The key associated with the value.
     * @param defvalue The default value if the key is not found.
     * @return The value associated with the key, or defvalue if the key is not found.
     */
    static u16 get_u16(string key, u16 defvalue);

    /**
     * @brief Retrieves a 32-bit unsigned integer value from the NVS.
     * @param key The key associated with the value.
     * @param defvalue The default value if the key is not found.
     * @return The value associated with the key, or defvalue if the key is not found.
     */
    static u32 get_u32(string key, u32 defvalue);

    /**
     * @brief Retrieves a 64-bit unsigned integer value from the NVS.
     * @param key The key associated with the value.
     * @param defvalue The default value if the key is not found.
     * @return The value associated with the key, or defvalue if the key is not found.
     */
    static u64 get_u64(string key, u64 defvalue);

    /**
     * @brief Sets a string value in the NVS.
     * @param key The key associated with the value.
     * @param value The string to be stored.
     * @param force If true, forces the set operation.
     * @return true if the set operation was successful, false otherwise.
     */
    static bool set_string(string key, string value, bool force = true);

    /**
     * @brief Retrieves a string value from the NVS.
     * @param key The key associated with the value.
     * @return The string value associated with the key.
     */
    static string get_string(string key);

    /**
     * @brief Checks if a Wi-Fi configuration is available in the NVS.
     * @return true if Wi-Fi configuration is available, false otherwise.
     */
    static bool has_wifi_config();

    /**
     * @brief Sets the Wi-Fi SSID in the NVS.
     * @param ssid The SSID to be stored.
     * @return true if the set operation was successful, false otherwise.
     */
    static bool set_wifi_ssid(string ssid);

    /**
     * @brief Sets the Wi-Fi password in the NVS.
     * @param pword The password to be stored.
     * @return true if the set operation was successful, false otherwise.
     */
    static bool set_wifi_pword(string pword);
};

#endif
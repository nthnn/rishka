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
 * @file librishka/drivers/keyboard.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief This header file defines the Keyboard class, providing access to keyboard-related functionalities in the Rishka VM SDK.
 *
 * The Keyboard class allows interaction with various keyboard-related functionalities in the Rishka VM SDK. It includes methods
 * for retrieving information about the keyboard layout, checking the status of lock keys (Num Lock, Caps Lock, Scroll Lock),
 * controlling the state of these lock keys, obtaining keyboard scan codes, and performing operations such as locking, unlocking,
 * and resetting the keyboard.
 */

#ifndef LIBRISHKA_DRIVERS_KEYBOARD_H
#define LIBRISHKA_DRIVERS_KEYBOARD_H

#include <librishka/devices.h>
#include <librishka/types.h>

/**
 * @class Keyboard
 * @brief Provides access to keyboard-related
 *        functionalities in the Rishka VM SDK.
 *
 * The Keyboard class allows interaction with
 * keyboard-related functionalities in the Rishka
 * VM SDK. It provides methods to retrieve
 * information about the keyboard layout, check
 * the status of lock keys (Num Lock, Caps Lock,
 * Scroll Lock), control the state of these lock
 * keys, obtain keyboard scan codes, and perform
 * operations such as locking, unlocking, and
 * resetting the keyboard.
 */
class Keyboard final {
public:
    /**
     * @brief Get the name of the current keyboard layout.
     *
     * This method retrieves the name of the currently
     * selected keyboard layout.
     *
     * @return A string representing the name of the keyboard layout.
     */
    static string layout_name();

    /**
     * @brief Get the description of the current keyboard layout.
     *
     * This method retrieves the description of the
     * currently selected keyboard layout.
     *
     * @return String representing the description of the keyboard layout.
     */
    static string layout_desc();

    /**
     * @brief Get the type of the keyboard device.
     *
     * This method retrieves the type of the connected
     * keyboard device.
     *
     * @return The type of the keyboard device as
     *         a PS2Device enumeration value.
     */
    static PS2Device device_type();

    /**
     * @brief Check if the Num Lock key is currently active.
     *
     * This method checks whether the Num Lock
     * key is currently active.
     *
     * @return True if the Num Lock key is active, false otherwise.
     */
    static bool is_num_lock();

    /**
     * @brief Check if the Caps Lock key is currently active.
     *
     * This method checks whether the Caps Lock key
     * is currently active.
     *
     * @return True if the Caps Lock key is active, false otherwise.
     */
    static bool is_caps_lock();

    /**
     * @brief Check if the Scroll Lock key is currently active.
     *
     * This method checks whether the Scroll Lock
     * key is currently active.
     *
     * @return True if the Scroll Lock key is active, false otherwise.
     */
    static bool is_scroll_lock();

    /**
     * @brief Set the state of the Num Lock key.
     *
     * This method allows setting the state of
     * the Num Lock key.
     *
     * @param state The desired state of the Num
     *        Lock key (true for active, false
     *        for inactive).
     */
    static void num_lock(bool state);

    /**
     * @brief Set the state of the Caps Lock key.
     *
     * This method allows setting the state of
     * the Caps Lock key.
     *
     * @param state The desired state of the Caps
     *        Lock key (true for active, false
     *        for inactive).
     */
    static void caps_lock(bool state);

    /**
     * @brief Set the state of the Scroll Lock key.
     *
     * This method allows setting the state of the
     * Scroll Lock key.
     *
     * @param state The desired state of the Scroll
     *        Lock key (true for active, false
     *        for inactive).
     */
    static void scroll_lock(bool state);

    /**
     * @brief Get the next keyboard scan code.
     *
     * This method retrieves the next keyboard scan code.
     *
     * @param timeout The maximum time to wait for
     *        a scan code (in milliseconds).
     * @param resend Flag indicating whether to
     *        resend previous scan codes.
     * 
     * @return The next keyboard scan code as an unsigned 32-bit integer.
     */
    static u32 next_scancode(i32 timeout, bool resend);

    /**
     * @brief Lock the keyboard to prevent input.
     *
     * This method locks the keyboard to prevent input.
     *
     * @param timeout The duration for which to lock
     *        the keyboard (in milliseconds).
     */
    static void lock(i32 timeout);

    /**
     * @brief Unlock the keyboard to allow input.
     *
     * This method unlocks the keyboard to allow input.
     */
    static void unlock();

    /**
     * @brief Reset the keyboard to its default state.
     *
     * This method resets the keyboard to its default state.
     */
    static void reset();
};

#endif
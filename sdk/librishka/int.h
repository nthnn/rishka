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
 * @file int.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file for interrupt handling in Rishka applications.
 *
 * This header file defines the Int class, which provides functionalities
 * for managing interrupts on ESP32-WROVER microcontrollers.
 */

#ifndef LIBRISHKA_INT_H
#define LIBRISHKA_INT_H

#include <librishka/types.h>

/**
 * @brief Enum representing interrupt modes for GPIO pins.
 *
 * This enum specifies the interrupt modes available for configuring GPIO pin interrupts.
 * - INT_CHANGE: Interrupt triggered on pin state change.
 * - INT_FAILING: Interrupt triggered on falling edge.
 * - INT_RISING: Interrupt triggered on rising edge.
 */
typedef enum {
    INT_CHANGE = 0x1,   /**< Interrupt triggered on pin state change */
    INT_FAILING = 0x2,  /**< Interrupt triggered on falling edge */
    INT_RISING = 0x3    /**< Interrupt triggered on rising edge */
} int_mode_t;

/**
 * @class Int
 * @brief Class for handling interrupt operations in Rishka applications.
 *
 * The Int class provides static methods for enabling/disabling interrupts,
 * attaching/detaching interrupt handlers, and configuring interrupt modes
 * for GPIO pins on ESP32-WROVER microcontrollers.
 */
class Int final {
public:
    /**
     * @brief Enable interrupts globally.
     *
     * This method enables interrupts globally, allowing interrupt handlers to be called.
     */
    static void enable();

    /**
     * @brief Disable interrupts globally.
     *
     * This method disables interrupts globally, preventing interrupt handlers from being called.
     */
    static void disable();

    /**
     * @brief Attach an interrupt handler to a GPIO pin.
     *
     * This method attaches an interrupt handler function to the specified GPIO pin.
     * The interrupt handler will be called when the specified interrupt mode is triggered.
     *
     * @param pin The GPIO pin number.
     * @param callback Pointer to the interrupt handler function.
     * @param mode The interrupt mode (INT_CHANGE, INT_FAILING, or INT_RISING).
     */
    static void attach(u8 pin, void (*callback)(void), int_mode_t mode);

    /**
     * @brief Detach the interrupt handler from a GPIO pin.
     *
     * This method detaches the interrupt handler function from the specified GPIO pin.
     * The interrupt handler will no longer be called for interrupts on the specified pin.
     *
     * @param pin The GPIO pin number.
     */
    static void detach(u8 pin);
};

#endif /* LIBRISHKA_INT_H */
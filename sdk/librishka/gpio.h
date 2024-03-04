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
 * @file gpio.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file for GPIO operations in Rishka applications.
 *
 * This header file defines the Gpio class, which provides functionalities
 * for interacting with General Purpose Input/Output (GPIO) pins on ESP32-WROVER microcontrollers.
 */

#ifndef LIBRISHKA_GPIO_H
#define LIBRISHKA_GPIO_H

#include <librishka/types.h>

/**
 * @brief Enum representing the pin modes for GPIO pins.
 *
 * This enum specifies the available pin modes for configuring GPIO pins.
 * - GPIO_INPUT: Configures the pin as an input.
 * - GPIO_OUTPUT: Configures the pin as an output.
 * - GPIO_INPUT_PULLUP: Configures the pin as an input with a pull-up resistor enabled.
 */
typedef enum {
    GPIO_INPUT = 0x01,          /**< Input mode */
    GPIO_OUTPUT = 0x03,         /**< Output mode */
    GPIO_INPUT_PULLUP = 0x05    /**< Input mode with pull-up resistor */
} gpio_pin_mode_t;

/**
 * @brief Enum representing the digital modes for GPIO pins.
 *
 * This enum specifies the digital modes for GPIO pins.
 * - GPIO_LOW: Represents a low digital value (0).
 * - GPIO_HIGH: Represents a high digital value (1).
 */
typedef enum {
    GPIO_LOW = 0x0,     /**< Low digital value */
    GPIO_HIGH = 0x1     /**< High digital value */
} gpio_mode_t;

/**
 * @class Gpio
 * @brief Class for handling GPIO operations in Rishka applications.
 *
 * The Gpio class provides static methods for configuring and manipulating
 * General Purpose Input/Output (GPIO) pins on ESP32-WROVER microcontrollers.
 * It includes functionalities for setting pin modes, reading digital and analog
 * values, generating pulses, and controlling digital communication protocols.
 */
class Gpio final {
public:
    /**
     * @brief Set the mode of a GPIO pin.
     *
     * This method sets the mode of the specified GPIO pin to the given mode.
     *
     * @param pin The GPIO pin number.
     * @param mode The mode to set for the GPIO pin (GPIO_INPUT, GPIO_OUTPUT, or GPIO_INPUT_PULLUP).
     */
    static void pin_mode(u8 pin, gpio_pin_mode_t mode);

    /**
     * @brief Read the digital value of a GPIO pin.
     *
     * This method reads the digital value (LOW or HIGH) of the specified GPIO pin.
     *
     * @param pin The GPIO pin number.
     * @return The digital value of the GPIO pin (0 for LOW, 1 for HIGH).
     */
    static gpio_mode_t digital_read(u8 pin);

    /**
     * @brief Write a digital value to a GPIO pin.
     *
     * This method writes the digital value (LOW or HIGH) to the specified GPIO pin.
     *
     * @param pin The GPIO pin number.
     * @param mode The digital value to write to the GPIO pin (GPIO_LOW or GPIO_HIGH).
     */
    static void digital_write(u8 pin, gpio_mode_t mode);

    /**
     * @brief Read the analog value of a GPIO pin.
     *
     * This method reads the analog value (0-1023) of the specified GPIO pin.
     *
     * @param pin The GPIO pin number.
     * @return The analog value of the GPIO pin (0-1023).
     */
    static u16 analog_read(u8 pin);

    /**
     * @brief Write an analog value to a GPIO pin.
     *
     * This method writes the analog value (0-255) to the specified GPIO pin.
     *
     * @param pin The GPIO pin number.
     * @param value The analog value to write to the GPIO pin (0-255).
     */
    static void analog_write(u8 pin, u16 value);

    /**
     * @brief Measure the duration of a pulse on a GPIO pin.
     *
     * This method measures the duration of a pulse of the specified state (HIGH or LOW)
     * on the specified GPIO pin, with the specified timeout in microseconds.
     *
     * @param pin The GPIO pin number.
     * @param state The state of the pulse to measure (GPIO_LOW or GPIO_HIGH).
     * @param timeout The timeout in microseconds.
     * @return The duration of the pulse in microseconds, or 0 if the timeout occurred.
     */
    static u64 pulse_in(u8 pin, u8 state, u64 timeout);

    /**
     * @brief Measure the duration of a long pulse on a GPIO pin.
     *
     * This method measures the duration of a long pulse of the specified state (HIGH or LOW)
     * on the specified GPIO pin, with the specified timeout in microseconds.
     *
     * @param pin The GPIO pin number.
     * @param state The state of the pulse to measure (GPIO_LOW or GPIO_HIGH).
     * @param timeout The timeout in microseconds.
     * @return The duration of the long pulse in microseconds, or 0 if the timeout occurred.
     */
    static u64 pulse_in_long(u8 pin, u8 state, u64 timeout);

    /**
     * @brief Shift in data from a digital input pin.
     *
     * This method shifts in data from a digital input pin using the specified clock pin
     * and bit order (LSBFIRST or MSBFIRST).
     *
     * @param data The digital input pin number.
     * @param clock The clock pin number.
     * @param bit_order The bit order (LSBFIRST or MSBFIRST).
     * @return The shifted-in data byte.
     */
    static u8 shift_in(u8 data, u8 clock, u8 bit_order);

    /**
     * @brief Shift out data to a digital output pin.
     *
     * This method shifts out data to a digital output pin using the specified clock pin,
     * bit order (LSBFIRST or MSBFIRST), and data value.
     *
     * @param data The digital output pin number.
     * @param clock The clock pin number.
     * @param bit_order The bit order (LSBFIRST or MSBFIRST).
     * @param value The data value to shift out.
     */
    static void shift_out(u8 data, u8 clock, u8 bit_order, u8 value);

    /**
     * @brief Generate a tone of the specified frequency and duration on a GPIO pin.
     *
     * This method generates a tone of the specified frequency and duration on the
     * specified GPIO pin using pulse-width modulation (PWM).
     *
     * @param pin The GPIO pin number.
     * @param frequency The frequency of the tone in Hertz (Hz).
     * @param duration The duration of the tone in microseconds (0 for continuous).
     */
    static void tone(u8 pin, u32 frequency, u64 duration);

    /**
     * @brief Stop generating a tone on a GPIO pin.
     *
     * This method stops generating a tone on the specified GPIO pin.
     *
     * @param pin The GPIO pin number.
     */
    static void no_tone(u8 pin);
};

#endif /* LIBRISHKA_GPIO_H */
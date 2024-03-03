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
 * @file i2c.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file for I2C (Inter-Integrated Circuit) operations in Rishka applications.
 *
 * This header file defines the I2C class, which provides functionalities
 * for communicating with I2C devices on ESP32-WROVER microcontrollers.
 */

#ifndef LIBRISHKA_I2C_H
#define LIBRISHKA_I2C_H

#include <librishka/types.h>

/**
 * @class I2C
 * @brief Class for handling I2C operations in Rishka applications.
 *
 * The I2C class provides static methods for initializing, configuring, and
 * communicating with I2C devices on ESP32-WROVER microcontrollers. It includes
 * functionalities for setting up the I2C bus, beginning and ending I2C transactions,
 * reading and writing data, registering callbacks for I2C events, and setting the clock
 * frequency and timeout.
 */
class I2C final {
public:
    /**
     * @brief Initialize the I2C communication with the specified address.
     *
     * This method initializes the I2C communication with the specified 7-bit address.
     * It should be called before any other I2C communication functions are used.
     *
     * @param address The 7-bit address of the I2C device.
     * @return True if initialization was successful, false otherwise.
     */
    static bool begin(u8 address);

    /**
     * @brief End the I2C communication.
     *
     * This method ends the I2C communication and releases the associated resources.
     * It should be called to clean up the I2C interface after use.
     *
     * @return True if ending the communication was successful, false otherwise.
     */
    static bool end();

    /**
     * @brief Set the SDA and SCL pins for the I2C communication.
     *
     * This method sets the SDA (data) and SCL (clock) pins for the I2C communication.
     * It should be called to configure the GPIO pins used for I2C communication.
     *
     * @param sda The pin number for the SDA (data) line.
     * @param scl The pin number for the SCL (clock) line.
     * @return True if setting the pins was successful, false otherwise.
     */
    static bool pins(u8 sda, u8 scl);

    /**
     * @brief Flush the I2C buffer.
     *
     * This method flushes the I2C buffer, discarding any unread data.
     * It can be called to clear the buffer before initiating a new transaction.
     */
    static void flush();

    /**
     * @brief Begin an I2C transmission to the specified device address.
     *
     * This method begins an I2C transmission to the specified device address.
     * Subsequent write operations will send data to the specified device.
     *
     * @param address The 7-bit address of the I2C device.
     */
    static void begin_transmission(u8 address);

    /**
     * @brief End an I2C transmission with an optional stop bit.
     *
     * This method ends an I2C transmission with an optional stop bit.
     * If the stop_bit parameter is true, a stop condition will be generated
     * on the bus after the transmission is complete.
     *
     * @param stop_bit True to generate a stop condition, false otherwise.
     * @return The status of the transmission (0 for success, non-zero for error).
     */
    static u8 end_transmission(bool stop_bit);

    /**
     * @brief Write data to the I2C bus.
     *
     * This method writes data to the I2C bus for transmission to the device
     * specified in the begin_transmission() method.
     *
     * @param data Pointer to the data buffer.
     * @param size The number of bytes to write.
     * @return The number of bytes written.
     */
    static usize write(u8* data, usize size);

    /**
     * @brief Write data to the I2C bus as a slave device.
     *
     * This method writes data to the I2C bus as a slave device.
     * It is typically used in response to a master device's request.
     *
     * @param data Pointer to the data buffer.
     * @param size The number of bytes to write.
     * @return The number of bytes written.
     */
    static usize slave_write(u8* data, usize size);

    /**
     * @brief Set the buffer size for I2C transactions.
     *
     * This method sets the buffer size for I2C transactions.
     * It should be called before beginning an I2C transmission.
     *
     * @param size The buffer size in bytes.
     * @return True if setting the buffer size was successful, false otherwise.
     */
    static usize set_buffersize(usize size);

    /**
     * @brief Read data from the I2C bus.
     *
     * This method reads data from the I2C bus received from the device
     * specified in the begin_transmission() method.
     *
     * @return The received data byte.
     */
    static i32 read();

    /**
     * @brief Peek at the next byte on the I2C bus.
     *
     * This method peeks at the next byte on the I2C bus without removing it
     * from the buffer. It is useful for checking if data is available for reading.
     *
     * @return The next byte on the I2C bus, or -1 if no data is available.
     */
    static i32 peek();

    /**
     * @brief Get the number of bytes available for reading on the I2C bus.
     *
     * This method returns the number of bytes available for reading on the I2C bus.
     * It can be used to determine the amount of data available for reading.
     *
     * @return The number of bytes available for reading.
     */
    static i32 available();

    /**
     * @brief Request data from a remote I2C device.
     *
     * This method requests data from a remote I2C device with the specified address
     * and size, optionally generating a stop condition after the request.
     *
     * @param address The 7-bit address of the remote I2C device.
     * @param size The number of bytes to request.
     * @param stop_bit True to generate a stop condition after the request, false otherwise.
     * @return The number of bytes requested.
     */
    static usize request(u8 address, usize size, bool stop_bit);

    /**
     * @brief Register a callback function for I2C receive events.
     *
     * This method registers a callback function to be called when data is received
     * on the I2C bus as a slave device. The callback function should take an integer
     * parameter representing the number of bytes received.
     *
     * @param callback Pointer to the callback function.
     */
    static void on_receive(void (*callback)(int));

    /**
     * @brief Register a callback function for I2C request events.
     *
     * This method registers a callback function to be called when a master device
     * requests data from the I2C slave device. The callback function should not
     * take any parameters.
     *
     * @param callback Pointer to the callback function.
     */
    static void on_request(void (*callback)(void));

    /**
     * @brief Set the timeout for I2C operations.
     *
     * This method sets the timeout for I2C operations in microseconds.
     * It specifies the maximum time allowed for an I2C operation to complete.
     *
     * @param timeout The timeout value in microseconds.
     */
    static void set_timeout(u16 timeout);

    /**
     * @brief Get the current timeout for I2C operations.
     *
     * This method returns the current timeout for I2C operations in microseconds.
     *
     * @return The current timeout value in microseconds.
     */
    static u16 get_timeout();

    /**
     * @brief Set the clock frequency for I2C communication.
     *
     * This method sets the clock frequency for I2C communication in Hertz (Hz).
     *
     * @param clock The clock frequency in Hertz (Hz).
     * @return True if setting the clock frequency was successful, false otherwise.
     */
    static bool set_clock(u32 clock);

    /**
     * @brief Get the current clock frequency for I2C communication.
     *
     * This method returns the current clock frequency for I2C communication in Hertz (Hz).
     *
     * @return The current clock frequency in Hertz (Hz).
     */
    static u32 get_clock();
};

#endif /* LIBRISHKA_I2C_H */
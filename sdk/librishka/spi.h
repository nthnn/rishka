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
 * @file spi.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file for SPI (Serial Peripheral Interface) communication in Rishka applications.
 *
 * This header file defines the SPI class, which provides functionalities
 * for SPI communication on ESP32-WROVER microcontrollers.
 */

#ifndef LIBRISHKA_SPI_H
#define LIBRISHKA_SPI_H

#include <librishka/types.h>

/**
 * @enum spi_mode_t
 * @brief Enumeration for SPI data modes.
 *
 * The spi_mode_t enumeration defines different SPI data modes
 * that control the clock polarity and phase during SPI communication.
 */
typedef enum {
    SPI_MODE0 = 0x00, /**< Clock polarity (CPOL) = 0, Clock phase (CPHA) = 0 */
    SPI_MODE1 = 0x01, /**< Clock polarity (CPOL) = 0, Clock phase (CPHA) = 1 */
    SPI_MODE2 = 0x02, /**< Clock polarity (CPOL) = 1, Clock phase (CPHA) = 0 */
    SPI_MODE3 = 0x03  /**< Clock polarity (CPOL) = 1, Clock phase (CPHA) = 1 */
} spi_mode_t;

/**
 * @class SPI
 * @brief Class for handling SPI communication in Rishka applications.
 *
 * The SPI class provides static methods for initializing SPI communication,
 * transferring data over SPI, and configuring SPI parameters on ESP32-WROVER microcontrollers.
 */
class SPI final {
public:
    /**
     * @brief Initialize SPI communication.
     *
     * This method initializes SPI communication with the specified pins.
     *
     * @param sck The pin number for the SPI clock (SCK).
     * @param miso The pin number for the SPI Master In Slave Out (MISO) line.
     * @param mosi The pin number for the SPI Master Out Slave In (MOSI) line.
     * @param ss The pin number for the Slave Select (SS) line.
     */
    static void begin(u8 sck, u8 miso, u8 mosi, u8 ss);

    /**
     * @brief End SPI communication.
     *
     * This method ends SPI communication and releases the SPI resources.
     */
    static void end();

    /**
     * @brief Begin a SPI transaction.
     *
     * This method begins a SPI transaction with the specified clock frequency,
     * bit order, and data mode.
     *
     * @param clock The clock frequency for the SPI transaction.
     * @param bit_order The bit order for the SPI transaction (MSBFIRST or LSBFIRST).
     * @param data_mode The data mode for the SPI transaction (SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3).
     */
    static void begin_transaction(u8 clock, u8 bit_order, spi_mode_t data_mode);

    /**
     * @brief End a SPI transaction.
     *
     * This method ends the current SPI transaction.
     */
    static void end_transaction();

    /**
     * @brief Transfer an 8-bit data over SPI.
     *
     * This method transfers an 8-bit data over SPI and returns the received data.
     *
     * @param data The 8-bit data to be transferred.
     * @return The received 8-bit data.
     */
    static u8 transfer8(u8 data);

    /**
     * @brief Transfer a 16-bit data over SPI.
     *
     * This method transfers a 16-bit data over SPI and returns the received data.
     *
     * @param data The 16-bit data to be transferred.
     * @return The received 16-bit data.
     */
    static u16 transfer16(u16 data);

    /**
     * @brief Transfer a 32-bit data over SPI.
     *
     * This method transfers a 32-bit data over SPI and returns the received data.
     *
     * @param data The 32-bit data to be transferred.
     * @return The received 32-bit data.
     */
    static u32 transfer32(u32 data);

    /**
     * @brief Transfer data bytes over SPI.
     *
     * This method transfers a specified number of bytes of data over SPI.
     *
     * @param data Pointer to the data buffer to be transferred.
     * @param out Pointer to the output buffer to store received data.
     * @param size The number of bytes to transfer.
     */
    static void transfer_bytes(u8* data, u8* out, u32 size);

    /**
     * @brief Transfer bits over SPI.
     *
     * This method transfers a specified number of bits of data over SPI.
     *
     * @param data The data to be transferred.
     * @param out Pointer to the output buffer to store received data.
     * @param bits The number of bits to transfer.
     */
    static void transfer_bits(u32 data, u32* out, u8 bits);

    /**
     * @brief Set the hardware chip select (CS) pin usage.
     *
     * This method enables or disables the use of hardware chip select (CS) pin.
     *
     * @param use True to enable hardware CS, false to disable.
     */
    static void set_hwcs(bool use);

    /**
     * @brief Set the bit order for SPI data transfer.
     *
     * This method sets the bit order for SPI data transfer (MSBFIRST or LSBFIRST).
     *
     * @param bit_order The bit order for SPI data transfer.
     */
    static void set_bit_order(u8 bit_order);

    /**
     * @brief Set the SPI data mode.
     *
     * This method sets the SPI data mode (clock polarity and phase).
     *
     * @param data_mode The SPI data mode (SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3).
     */
    static void set_data_mode(spi_mode_t data_mode);

    /**
     * @brief Set the SPI clock frequency.
     *
     * This method sets the SPI clock frequency in Hz.
     *
     * @param frequency The SPI clock frequency in Hz.
     */
    static void set_frequency(u32 frequency);

    /**
     * @brief Set the SPI clock divider.
     *
     * This method sets the SPI clock divider.
     *
     * @param clock_div The SPI clock divider.
     */
    static void set_clock_div(u32 clock_div);

    /**
     * @brief Get the SPI clock divider.
     *
     * This method returns the current SPI clock divider.
     *
     * @return The current SPI clock divider.
     */
    static u32 get_clock_div();

    /**
     * @brief Write an 8-bit data over SPI.
     *
     * This method writes an 8-bit data over SPI.
     *
     * @param data The 8-bit data to be written.
     */
    static void write8(u8 data);

    /**
     * @brief Write a 16-bit data over SPI.
     *
     * This method writes a 16-bit data over SPI.
     *
     * @param data The 16-bit data to be written.
     */
    static void write16(u16 data);

    /**
     * @brief Write a 32-bit data over SPI.
     *
     * This method writes a 32-bit data over SPI.
     *
     * @param data The 32-bit data to be written.
     */
    static void write32(u32 data);

    /**
     * @brief Write data bytes over SPI.
     *
     * This method writes a specified number of bytes of data over SPI.
     *
     * @param data Pointer to the data buffer to be written.
     * @param size The number of bytes to write.
     */
    static void write_bytes(u8* data, u32 size);

    /**
     * @brief Write pixel data over SPI.
     *
     * This method writes pixel data over SPI. Compatible with ILI9341.
     *
     * @param data Pointer to the pixel data buffer to be written.
     * @param size The size of the pixel data buffer.
     */
    static void write_pixels(void* data, u32 size);

    /**
     * @brief Write pattern data over SPI.
     *
     * This method writes a specified pattern data over SPI.
     *
     * @param data Pointer to the pattern data buffer to be written.
     * @param size The size of the pattern data buffer.
     * @param pattern The pattern to be written.
     */
    static void write_pattern(u8* data, u8 size, u32 pattern);
};

#endif /* LIBRISHKA_SPI_H */
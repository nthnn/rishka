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
 * @file librishka.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Main header file for the Rishka SDK.
 *
 * This header file includes all the necessary headers from the Rishka SDK
 * for easy integration and usage of Rishka functionalities in your projects.
 */

#ifndef LIBRISHKA_H
#define LIBRISHKA_H

/**
 * @defgroup Rishka_SDK Rishka SDK
 * @brief Rishka SDK provides a collection of headers for accessing various
 *        functionalities on ESP32-WROVER microcontrollers.
 *
 * The Rishka SDK includes headers for GPIO, filesystem, I2C, SPI, interrupts,
 * system calls, and other essential components for developing applications
 * on the Rishka platform.
 */

#include <librishka/args.h>     /**< @ingroup Rishka_SDK */
#include <librishka/devices.h>  /**< @ingroup Rishka_SDK */
#include <librishka/fs.h>       /**< @ingroup Rishka_SDK */
#include <librishka/gpio.h>     /**< @ingroup Rishka_SDK */
#include <librishka/i2c.h>      /**< @ingroup Rishka_SDK */
#include <librishka/int.h>      /**< @ingroup Rishka_SDK */
#include <librishka/io.h>       /**< @ingroup Rishka_SDK */
#include <librishka/keyboard.h> /**< @ingroup Rishka_SDK */
#include <librishka/memory.h>   /**< @ingroup Rishka_SDK */
#include <librishka/runtime.h>  /**< @ingroup Rishka_SDK */
#include <librishka/sys.h>      /**< @ingroup Rishka_SDK */
#include <librishka/types.h>    /**< @ingroup Rishka_SDK */

#endif /* LIBRISHKA_H */
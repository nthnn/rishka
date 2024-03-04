/* 
 * This file is part of the Rishka distribution (https://github.com/rishka-esp32/rishka).
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
 * @file rishka_commons.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Common definitions used across Rishka modules.
 *
 * This file contains common definitions and includes commonly
 * used libraries for Rishka modules.
 */

#ifndef RISHKA_COMMONS_H
#define RISHKA_COMMONS_H

#include <Arduino.h>    ///< Include Arduino core library.
#include <SD.h>         ///< Include SD card library.
#include <SPI.h>        ///< Include SPI communication library.

#define  RISHKA_VM_STACK_SIZE 32768U    ///< Define the stack size for the Rishka virtual machine.

#endif
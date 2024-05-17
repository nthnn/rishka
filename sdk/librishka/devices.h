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
 * @file devices.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Definitions related to PS/2 devices for the Rishka Virtual Machine SDK.
 *
 * This header file contains definitions related to PS/2 devices for use with the Rishka Virtual
 * Machine Software Development Kit (SDK). PS/2 devices include keyboards and mice, and this file
 * provides enumerations for various types of PS/2 devices that can be connected to the Rishka VM.
 */

#ifndef LIBRISHKA_DEVICES_H
#define LIBRISHKA_DEVICES_H

/**
 * @enum PS2Device
 * @brief Enumeration of PS/2 device types.
 *
 * This enumeration defines symbolic names for
 * different types of PS/2 devices that can be
 * connected to the Rishka Virtual Machine.
 * 
 * PS/2 devices include keyboards and mice, and
 * this enumeration provides identifiers for
 * various types of keyboards and mice supported
 * by the Rishka VM.
 */
typedef enum {
    PS2_DEV_UNKNOWN,       /**< Unknown PS/2 device. */
    PS2_DEV_OLD_ATKB,      /**< Old-style AT keyboard. */
    PS2_DEV_MOUSE_STD,     /**< Standard PS/2 mouse. */
    PS2_DEV_MOUSE_WHEEL,   /**< PS/2 mouse with wheel support. */
    PS2_DEV_MOUSE_5BTN,    /**< PS/2 mouse with 5 buttons. */
    PS2_DEV_MF2KB_TRANS,   /**< MF2 keyboard with translation support. */
    PS2_DEV_MF2KB          /**< MF2 keyboard. */
} PS2Device;

#endif
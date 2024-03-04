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
 * @file rishka.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Includes all the necessary headers for Rishka kernel development.
 *
 * The rishka.h header file serves as a central point for including all the
 * necessary headers required for Rishka kernel development. By including this
 * header file in your Arduino sketch or library, you gain access to essential
 * components and utilities provided by the Rishka kernel.
 */

#ifndef RISHKA_H
#define RISHKA_H

#include <rishka_commons.h>        ///< Common definitions and macros.
#include <rishka_errors.h>         ///< Error code definitions and handling.
#include <rishka_instructions.h>   ///< Instruction set architecture definitions.
#include <rishka_syscalls.h>       ///< System call interface and implementations.
#include <rishka_types.h>          ///< Type definitions and aliases.
#include <rishka_util.h>           ///< Utility functions and macros.
#include <rishka_vm.h>             ///< Virtual machine core functionalities.
#include <rishka_vm_helper.h>      ///< Helper functions for virtual machine operations.

#endif /* RISHKA_H */
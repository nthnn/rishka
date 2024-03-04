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
 * @file rishka_errors.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file for error handling functions in Rishka.
 *
 * This file declares functions for error handling in the Rishka virtual machine.
 * It provides utilities for printing error messages and handling panics.
 */

#ifndef RISHKA_ERRORS_H
#define RISHKA_ERRORS_H

#include <rishka_commons.h>
#include <rishka_vm.h>

/**
 * @brief Print an error message to the output stream.
 *
 * This function prints the specified error message to the output stream.
 *
 * @param msg The error message to print.
 * @param len The length of the error message.
 * @param flush Whether to flush the standard error stream after printing the message.
 */
void rishka_perror(const char* msg, uintptr_t len, bool flush);

/**
 * @brief Handle a panic situation in the virtual machine.
 *
 * This function is called to handle a panic situation in the Rishka virtual machine.
 * It prints the panic message and performs any necessary cleanup before terminating the program.
 *
 * @param message The panic message to print.
 * @param vm A pointer to the virtual machine structure associated with the panic.
 */
void rishka_panic(const char* message, rishka_virtual_machine* vm);

#endif /* RISHKA_ERRORS_H */
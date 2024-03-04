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
 * @file rishka_vm_helper.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Provides helper functions for interacting with the Rishka virtual machine.
 *
 * The rishka_vm_helper.h header file defines a set of helper functions for
 * reading and writing data, manipulating pointers, and performing arithmetic
 * operations for the Rishka virtual machine. These functions assist the virtual
 * runtime in managing memory, accessing data structures, and executing specific
 * instructions in the virtual machine environment.
 */

#ifndef RISHKA_VM_HELPER_H
#define RISHKA_VM_HELPER_H

#include <rishka_types.h>   ///< Definition of Rishka data types.
#include <rishka_vm.h>      ///< Declaration of Rishka virtual machine structures.

/**
 * @brief Reads an 8-bit signed integer from the specified memory address.
 *
 * This function reads an 8-bit signed integer value from the memory address
 * `addr` within the Rishka virtual machine instance `vm`. It handles different
 * data types specified by `type` and returns the read value as an int8_t.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @param type The type of the memory address (nil, stack, heap, etc.).
 * @param addr The memory address to read from.
 * @return The 8-bit signed integer value read from the memory address.
 */
int8_t rishka_vm_read_i8(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr);

/**
 * @brief Reads a 16-bit signed integer from the specified memory address.
 *
 * This function reads a 16-bit signed integer value from the memory address
 * `addr` within the Rishka virtual machine instance `vm`. It handles different
 * data types specified by `type` and returns the read value as an int16_t.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @param type The type of the memory address (nil, stack, heap, etc.).
 * @param addr The memory address to read from.
 * @return The 16-bit signed integer value read from the memory address.
 */
int16_t rishka_vm_read_i16(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr);

/**
 * @brief Reads a 32-bit signed integer from the specified memory address.
 *
 * This function reads a 32-bit signed integer value from the memory address
 * `addr` within the Rishka virtual machine instance `vm`. It handles different
 * data types specified by `type` and returns the read value as an int32_t.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @param type The type of the memory address (nil, stack, heap, etc.).
 * @param addr The memory address to read from.
 * @return The 32-bit signed integer value read from the memory address.
 */
int32_t rishka_vm_read_i32(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr);

/**
 * @brief Reads a 64-bit signed integer from the specified memory address.
 *
 * This function reads a 64-bit signed integer value from the memory address
 * `addr` within the Rishka virtual machine instance `vm`. It handles different
 * data types specified by `type` and returns the read value as an int64_t.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @param type The type of the memory address (nil, stack, heap, etc.).
 * @param addr The memory address to read from.
 * @return The 64-bit signed integer value read from the memory address.
 */
int64_t rishka_vm_read_i64(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr);

/**
 * @brief Reads an 8-bit unsigned integer from the specified memory address.
 *
 * This function reads an 8-bit unsigned integer value from the memory address
 * `addr` within the Rishka virtual machine instance `vm`. It handles different
 * data types specified by `type` and returns the read value as a uint8_t.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @param type The type of the memory address (nil, stack, heap, etc.).
 * @param addr The memory address to read from.
 * @return The 8-bit unsigned integer value read from the memory address.
 */
uint8_t rishka_vm_read_u8_arrptr(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr);

/**
 * @brief Reads a 16-bit unsigned integer from the specified memory address.
 *
 * This function reads a 16-bit unsigned integer value from the memory address
 * `addr` within the Rishka virtual machine instance `vm`. It handles different
 * data types specified by `type` and returns the read value as a uint16_t.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @param type The type of the memory address (nil, stack, heap, etc.).
 * @param addr The memory address to read from.
 * @return The 16-bit unsigned integer value read from the memory address.
 */
uint16_t rishka_vm_read_u16ptr(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr);

/**
 * @brief Reads a 32-bit unsigned integer from the specified memory address.
 *
 * This function reads a 32-bit unsigned integer value from the memory address
 * `addr` within the Rishka virtual machine instance `vm`. It handles different
 * data types specified by `type` and returns the read value as a uint32_t.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @param type The type of the memory address (nil, stack, heap, etc.).
 * @param addr The memory address to read from.
 * @return The 32-bit unsigned integer value read from the memory address.
 */
uint32_t rishka_vm_read_u32ptr(rishka_virtual_machine* vm, rishka_nil_type type, uint64_t addr);

/**
 * @brief Retrieves a pointer value from the specified memory address.
 *
 * This function retrieves a pointer value from the memory address `addr` within
 * the Rishka virtual machine instance `vm` and returns it as a void pointer.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @param addr The memory address to retrieve the pointer from.
 * @return A void pointer pointing to the memory address specified by `addr`.
 */
void* rishka_vm_getptr(rishka_virtual_machine* vm, uint64_t addr);

/**
 * @brief Writes an 8-bit unsigned integer value to the specified memory address.
 *
 * This function writes the 8-bit unsigned integer value `val` to the memory
 * address `addr` within the Rishka virtual machine instance `vm`.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @param addr The memory address to write the value to.
 * @param val The 8-bit unsigned integer value to be written.
 */
void rishka_vm_write_u8(rishka_virtual_machine* vm, uint64_t addr, uint8_t val);

/**
 * @brief Writes a 16-bit unsigned integer value to the specified memory address.
 *
 * This function writes the 16-bit unsigned integer value `val` to the memory
 * address `addr` within the Rishka virtual machine instance `vm`.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @param addr The memory address to write the value to.
 * @param val The 16-bit unsigned integer value to be written.
 */
void rishka_vm_write_u16(rishka_virtual_machine* vm, uint64_t addr, uint16_t val);

/**
 * @brief Writes a 32-bit unsigned integer value to the specified memory address.
 *
 * This function writes the 32-bit unsigned integer value `val` to the memory
 * address `addr` within the Rishka virtual machine instance `vm`.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @param addr The memory address to write the value to.
 * @param val The 32-bit unsigned integer value to be written.
 */
void rishka_vm_write_u32(rishka_virtual_machine* vm, uint64_t addr, uint32_t val);

/**
 * @brief Writes a 64-bit unsigned integer value to the specified memory address.
 *
 * This function writes the 64-bit unsigned integer value `val` to the memory
 * address `addr` within the Rishka virtual machine instance `vm`.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @param addr The memory address to write the value to.
 * @param val The 64-bit unsigned integer value to be written.
 */
void rishka_vm_write_u64(rishka_virtual_machine* vm, uint64_t addr, uint64_t val);

/**
 * @brief Performs left shift operation on a 64-bit signed integer.
 *
 * This function performs a left shift operation on the 64-bit signed integer
 * `a` by the number of bits specified by `b`. It returns the result of the
 * left shift operation.
 *
 * @param a The 64-bit signed integer value to be shifted.
 * @param b The number of bits to shift `a` by.
 * @return The result of the left shift operation.
 */
int64_t rishka_shl_riscvi64(int64_t a, int64_t b);

/**
 * @brief Performs right shift operation on a 64-bit signed integer.
 *
 * This function performs a right shift operation on the 64-bit signed integer
 * `a` by the number of bits specified by `b`. It returns the result of the
 * right shift operation.
 *
 * @param a The 64-bit signed integer value to be shifted.
 * @param b The number of bits to shift `a` by.
 * @return The result of the right shift operation.
 */
int64_t rishka_shr_riscvi64(int64_t a, int64_t b);

/**
 * @brief Performs arithmetic right shift operation on a 64-bit signed integer.
 *
 * This function performs an arithmetic right shift operation on the 64-bit
 * signed integer `a` by the number of bits specified by `b`. It returns the
 * result of the arithmetic right shift operation.
 *
 * @param a The 64-bit signed integer value to be shifted.
 * @param b The number of bits to shift `a` by.
 * @return The result of the arithmetic right shift operation.
 */
int64_t rishka_asr_riscvi64(int64_t a, int64_t b);

/**
 * @brief Performs right shift operation on a 128-bit signed integer.
 *
 * This function performs a right shift operation on the lower 64 bits of the
 * 128-bit signed integer `a` by the number of bits specified by `b`. It
 * returns the result of the right shift operation.
 *
 * @param a The 128-bit signed integer value to be shifted.
 * @param b The number of bits to shift `a` by.
 * @return The result of the right shift operation.
 */
int64_t rishka_shr_riscvi128(int64_t a, int64_t b);

#endif /* RISHKA_VM_HELPER_H */
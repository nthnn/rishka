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
 * @file rishka_vm.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Provides functionality for managing the Rishka virtual machine.
 *
 * The rishka_vm.h header file defines structures and functions related to the
 * management and operation of the Rishka virtual machine. This virtual machine
 * serves as the core execution environment for Rishka applications, providing
 * a sandboxed environment for running user-level code with controlled access
 * to system resources.
 */

#ifndef RISHKA_VM_H
#define RISHKA_VM_H

#include <List.hpp>         ///< List container for file handles.
#include <SD.h>             ///< SD card library for file operations.

#define  RISHKA_VM_STACK_SIZE 32768U    ///< Define the stack size for the Rishka virtual machine.

/**
 * @brief Structure representing a Rishka virtual machine instance.
 */
typedef struct {
    bool running;                           ///< Flag indicating whether the VM is running.

    char** argv;                            ///< Command line arguments.
    int argc;                               ///< Number of command line arguments.

    int64_t pc;                             ///< Program counter.
    int64_t exitcode;                       ///< Exit code of the VM.

    uint64_t registers[32];                 ///< Array of VM registers.
    uint8_t memory[RISHKA_VM_STACK_SIZE];   ///< VM memory space.

    Stream* stream;                         ///< Stream for input/output operations.
    List<File> file_handles;                ///< List of file handles.
} rishka_virtual_machine;

/**
 * @brief Initializes a Rishka virtual machine instance.
 *
 * This function initializes the specified Rishka virtual machine instance `vm`
 * with the provided `stream` for input/output operations. It sets up the initial
 * state of the virtual machine, including resetting registers, memory, and file
 * handles. Once initialized, the virtual machine is ready for execution.
 *
 * @param vm A pointer to the Rishka virtual machine instance to be initialized.
 * @param stream A pointer to the Stream object for input/output operations.
 */
void rishka_vm_initialize(rishka_virtual_machine* vm, Stream* stream);

/**
 * @brief Runs a Rishka virtual machine instance.
 *
 * This function starts the execution of the specified Rishka virtual machine
 * instance `vm` with the provided command line arguments `argc` and `argv`.
 * It executes the loaded program, if any, and handles any system calls or
 * instructions encountered during execution until the program exits or an
 * error occurs.
 *
 * @param vm A pointer to the Rishka virtual machine instance to be run.
 * @param argc The number of command line arguments.
 * @param argv An array of pointers to command line argument strings.
 */
void rishka_vm_run(rishka_virtual_machine* vm, int argc, char** argv);

/**
 * @brief Loads a program file into a Rishka virtual machine instance.
 *
 * This function loads the program file specified by `file_name` into the
 * specified Rishka virtual machine instance `vm`. It checks if the file
 * exists and is readable, then loads its contents into the memory of the
 * virtual machine for execution.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @param file_name The name of the program file to be loaded.
 * @return true if the file is loaded successfully, false otherwise.
 */
bool rishka_vm_loadfile(rishka_virtual_machine* vm, const char* file_name);

/**
 * @brief Executes a single instruction in a Rishka virtual machine instance.
 *
 * This function executes a single instruction specified by `inst` within
 * the specified Rishka virtual machine instance `vm`. It fetches the
 * instruction from the program counter of the virtual machine and executes
 * it according to the Rishka instruction set architecture.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @param inst The instruction to be executed.
 */
void rishka_vm_execute(rishka_virtual_machine* vm, uint32_t inst);

/**
 * @brief Resets a Rishka virtual machine instance to its initial state.
 *
 * This function resets the specified Rishka virtual machine instance `vm`
 * to its initial state, clearing registers, memory, and file handles. It
 * prepares the virtual machine for re-initialization or re-execution.
 *
 * @param vm A pointer to the Rishka virtual machine instance to be reset.
 */
void rishka_vm_reset(rishka_virtual_machine* vm);

/**
 * @brief Fetches the next instruction to be executed in a virtual machine.
 *
 * This function fetches the next instruction from the program counter of
 * the specified Rishka virtual machine instance `vm`. It returns the fetched
 * instruction for execution by the virtual machine.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @return The next instruction to be executed.
 */
uint32_t rishka_vm_fetch(rishka_virtual_machine* vm);

/**
 * @brief Handles a system call in a Rishka virtual machine instance.
 *
 * This function handles a system call specified by `code` within the
 * specified Rishka virtual machine instance `vm`. It executes the
 * corresponding system call routine and returns the result.
 *
 * @param vm A pointer to the Rishka virtual machine instance.
 * @param code The system call code to be handled.
 * @return The result of the system call execution.
 */
uint64_t rishka_vm_handle_syscall(rishka_virtual_machine* vm, uint64_t code);

/**
 * @brief Handle a panic situation in the virtual machine.
 *
 * This function is called to handle a panic situation in the Rishka virtual machine.
 * It prints the panic message and performs any necessary cleanup before terminating the program.
 *
 * @param vm A pointer to the virtual machine structure associated with the panic.
 * @param message The panic message to print.
 */
void rishka_vm_panic(rishka_virtual_machine* vm, const char* message);

#endif /* RISHKA_VM_H */
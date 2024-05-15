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
 * @brief Definition of the RishkaVM class, representing a Rishka virtual machine.
 * 
 * This header file defines the RishkaVM class, which encapsulates the functionality
 * of a Rishka virtual machine. The RishkaVM class provides methods to initialize,
 * load, and execute bytecode programs, handle system calls, manage memory and
 * registers, and interact with input/output streams.
 */
#ifndef RISHKA_VM_H
#define RISHKA_VM_H

#include <fabgl.h>
#include <List.hpp>
#include <rishka_types.h>
#include <SD.h>

/**
 * @brief RishkaVM class for simulating a Rishka virtual machine.
 * 
 * This class represents a Rishka virtual machine, which emulates the execution
 * of Rishka bytecode. It provides functionality for loading and running bytecode
 * programs, handling system calls, managing memory, and accessing CPU registers.
 */
class RishkaVM final {
private:
    uint64_t registers[32];                 ///< CPU registers
    uint8_t memory[RISHKA_VM_STACK_SIZE];   ///< Memory space for the virtual machine

    int64_t pc;                             ///< Program counter
    fabgl::Terminal* terminal;              ///< Terminal for input/output operations
    String workingDirectory;                 ///< Current directory of the virtual machine

    bool running;                           ///< Flag indicating whether the VM is running
    int64_t exitCode;                       ///< Exit code of the VM after execution

    char** argv;                            ///< Command-line arguments
    uint8_t argc;                           ///< Number of command-line arguments

    String outputStream;                    ///< Output stream from the VM system calls

    /**
     * @brief Fetches the next instruction to be executed in a virtual machine.
     *
     * This function fetches the next instruction from the program counter of
     * the specified Rishka virtual machine instance `vm`. It returns the fetched
     * instruction for execution by the virtual machine.
     *
     * @return The next instruction to be executed.
     */
    uint32_t fetch();

    /**
     * @brief Handles a system call in a Rishka virtual machine instance.
     *
     * This function handles a system call specified by `code` within the
     * specified Rishka virtual machine instance `vm`. It executes the
     * corresponding system call routine and returns the result.
     *
     * @param code The system call code to be handled.
     * @return The result of the system call execution.
     */
    uint64_t handleSyscall(uint64_t code);

    /**
     * @brief Executes the given instruction.
     * 
     * @param inst The instruction to execute.
     */
    void execute(uint32_t inst);

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
    static int64_t shiftLeftInt64(int64_t a, int64_t b);

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
    static int64_t shiftRightInt128(int64_t a, int64_t b);

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
    static int64_t shiftRightInt64(int64_t a, int64_t b);

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
    static int64_t arithmeticShiftRightInt64(int64_t a, int64_t b);

public:
    List<File> fileHandles; ///< List of file handles used by the VM system calls

    /**
     * @brief Stops the execution of the virtual machine.
     * 
     * This method halts the execution of the virtual machine.
     */
    void stopVM();

    /**
     * @brief Sets the exit code of the virtual machine.
     * 
     * @param exitCode The exit code.
     */
    void setExitCode(int64_t exitCode);

    /**
     * @brief Gets the exit code of the virtual machine.
     * 
     * @return The exit code.
     */
    int64_t getExitCode() const;

    /**
     * @brief Gets the input/output terminal used by the virtual machine.
     * 
     * @return A pointer to the input/output terminal.
     */
    fabgl::Terminal* getTerminal() const;

    /**
     * @brief Gets the number of VM arguments on execution.
     * 
     * @return The number of VM arguments on execution.
     */
    uint8_t getArgCount() const;

    /**
     * @brief Gets the value of a VM argument on execution at the specified index.
     * 
     * @param index The index of the VM argument on execution.
     * @return The value of the VM argument on execution.
     */
    char* getArgValue(const uint8_t index) const;

    /**
     * @brief Initializes the virtual machine instance.
     *
     * This function initializes the Rishka virtual machine instance with the
     * provided `stream` for input/output operations. It sets up the initial
     * state of the virtual machine, including resetting registers, memory, and file
     * handles. Once initialized, the virtual machine is ready for execution.
     *
     * @param stream A pointer to the Terminal object for input/output operations.
     * @param directory The path to the new working directory.
     */
    void initialize(fabgl::Terminal* terminal, String workingDirectory = "/");

    /**
     * @brief Resets the Rishka virtual machine instance to its initial state.
     *
     * This function resets the specified Rishka VM instance to its initial
     * state, clearing registers, memory, and file handles. It prepares the
     * virtual machine for re-initialization or re-execution.
     */
    void reset();

    /**
     * @brief Loads a program file into a Rishka virtual machine instance.
     *
     * This function loads the program file specified by `fileName` into the
     * Rishka virtual machine instance. It checks if the file exists and is
     * readable, then loads its contents into the memory of the virtual machine
     * for execution.
     *
     * @param fileName The name of the program file to be loaded.
     * @return true if the file is loaded successfully, false otherwise.
     */
    bool loadFile(const char* fileName);

    /**
     * @brief Runs the Rishka virtual machine instance.
     *
     * This function starts the execution of the Rishka virtual machine
     * instance with the provided command line arguments `argc` and `argv`.
     * It executes the loaded program, if any, and handles any system calls or
     * instructions encountered during execution until the program exits or an
     * error occurs.
     *
     * @param argc The number of command line arguments.
     * @param argv An array of pointers to command line argument strings.
     */
    void run(int argc, char** argv);

    /**
     * @brief Checks if the virtual machine is running.
     *
     * This method returns a boolean value indicating whether the virtual machine is currently running
     * or not.
     *
     * @return true if the virtual machine is running, false otherwise.
     */
    bool isRunning();

    /**
     * @brief Handle a panic situation in the virtual machine.
     *
     * This function is called to handle a panic situation in the Rishka virtual machine.
     * It prints the panic message and performs any necessary cleanup before terminating the program.
     *
     * @param message The panic message to print.
     */
    void panic(const char* message);

    /**
     * @brief Sets the working directory for the virtual machine.
     *
     * This function sets the working directory for the virtual machine instance.
     * The working directory is the base directory for file operations performed
     * by the virtual machine.
     *
     * @param directory The path to the new working directory.
     */
    void setWorkingDirectory(String directory);

    /**
     * @brief Retrieves the current working directory of the virtual machine.
     *
     * This function retrieves the current working directory of the virtual machine instance.
     * The working directory is the base directory for file operations performed
     * by the virtual machine.
     *
     * @return A pointer to the current working directory string.
     */
    String getWorkingDirectory() const;

    /**
     * @brief Retrieves the current output stream of the virtual machine.
     *
     * This method returns the entire content of the virtual machine's output stream as a String.
     * The output stream contains all the text that has been written to the virtual machine's 
     * output during its execution.
     *
     * @return A String containing the content of the output stream.
     */
    String getOutputStream() const;

    /**
     * @brief Appends a text string to the virtual machine's output stream.
     *
     * This method allows you to add a specified text string to the end of the virtual machine's
     * output stream. This can be useful for logging or for any output generation during the
     * virtual machine's execution.
     *
     * @param text The text string to be appended to the output stream.
     */
    void appendToOutputStream(String text);

    /**
     * @brief Appends a single character to the virtual machine's output stream.
     *
     * This method allows you to add a single character to the end of the virtual machine's
     * output stream. This can be useful for logging or for any output generation during the
     * virtual machine's execution.
     *
     * @param ch The character to be appended to the output stream.
     */
    void appendToOutputStream(char ch);

    /**
     * @brief Template function to retrieve a parameter from the registers.
     * 
     * @tparam T The type of the parameter.
     * @param pos The position of the parameter.
     * @return The value of the parameter.
     */
    template<typename T>
    inline constexpr T getParam(const uint8_t pos) const {
        return (T)(((rishka_u64_arrptr*) &this->registers)->a).v[10 + pos];
    }

    /**
     * @brief Template function to retrieve a pointer parameter from memory.
     * 
     * @tparam T The type of the pointer parameter.
     * @param pos The position of the pointer parameter.
     * @return The pointer value.
     */
    template<typename T>
    inline constexpr T getPointerParam(const uint8_t pos) const {
        return (T)(&(((rishka_u8_arrptr*) this->memory)->a).v[
            (((rishka_u64_arrptr*) &this->registers)->a).v[10 + pos]
        ]);
    }
};

#endif
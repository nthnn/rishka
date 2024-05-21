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
 * @file rishka_syscalls.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Definition of system calls for the Rishka kernel.
 *
 * This header file defines the system calls supported by the Rishka kernel, providing
 * a set of functions for performing various operations such as input/output, file
 * management, hardware interaction, and runtime control.
 *
 * The system calls are organized into different categories based on their functionality,
 * and each system call is associated with a unique code that identifies the specific
 * operation it performs. These system calls are invoked by user programs running on
 * the Rishka virtual machine to interact with the underlying system and peripherals.
 */

#ifndef RISHKA_SYSCALLS_H
#define RISHKA_SYSCALLS_H

#include <rishka_vm.h>

/**
 * @enum rishka_syscall
 * @brief Enumeration of system call codes for the Rishka kernel.
 *
 * This enumeration defines symbolic names for each system call supported by the Rishka kernel.
 * Each system call has a unique code associated with it, which is used to identify the specific
 * operation requested by the user program.
 */
enum rishka_syscall {
    // Input/Output System Calls
    RISHKA_SC_IO_PRINTS, ///< Print a string
    RISHKA_SC_IO_PRINTN, ///< Print a signed number
    RISHKA_SC_IO_PRINTU, ///< Print an unsigned number
    RISHKA_SC_IO_PRINTD, ///< Print a double
    RISHKA_SC_IO_READCH, ///< Read a single character
    RISHKA_SC_IO_READLINE, ///< Read a line of text
    RISHKA_SC_IO_AVAILABLE, ///< Check if data is available in the stream
    RISHKA_SC_IO_PEEK, ///< Peek the next character from the stream
    RISHKA_SC_IO_FIND, ///< Find a character in the stream
    RISHKA_SC_IO_FIND_UNTIL, ///< Find a character until a delimiter in the stream
    RISHKA_SC_IO_SET_TIMEOUT, ///< Set the timeout for stream operations
    RISHKA_SC_IO_GET_TIMEOUT, ///< Get the current timeout for stream operations

    // System Utility System Calls
    RISHKA_SC_SYS_DELAY_MS, ///< Delay execution by milliseconds
    RISHKA_SC_SYS_MICROS, ///< Get the current microsecond count
    RISHKA_SC_SYS_MILLIS, ///< Get the current millisecond count
    RISHKA_SC_SYS_SHELLEXEC, ///< Execute a shell command
    RISHKA_SC_SYS_EXIT, ///< Terminate the program
    RISHKA_SC_SYS_INFOS, ///< Get system information as string
    RISHKA_SC_SYS_INFON, ///< Get system information as number
    RISHKA_SC_SYS_RANDOM, ///< Generate a random number
    RISHKA_SC_SYS_CD, ///< Change working directory
    RISHKA_SC_SYS_WD, ///< Current working directory

    // General Purpose Input/Output System Calls
    RISHKA_SC_GPIO_PIN_MODE, ///< Set pin mode (input/output)
    RISHKA_SC_GPIO_DIGITAL_READ, ///< Read digital input value
    RISHKA_SC_GPIO_DIGITAL_WRITE, ///< Write digital output value
    RISHKA_SC_GPIO_ANALOG_READ, ///< Read analog input value
    RISHKA_SC_GPIO_ANALOG_WRITE, ///< Write analog output value
    RISHKA_SC_GPIO_PULSE_IN, ///< Measure pulse duration in microseconds
    RISHKA_SC_GPIO_PULSE_IN_LONG, ///< Measure pulse duration in microseconds (long)
    RISHKA_SC_GPIO_SHIFT_IN, ///< Shift in data from a parallel to serial converter
    RISHKA_SC_GPIO_SHIFT_OUT, ///< Shift out data to a parallel to serial converter
    RISHKA_SC_GPIO_TONE, ///< Generate a tone of a given frequency
    RISHKA_SC_GPIO_NO_TONE, ///< Stop generating a tone

    // Interrupt Handling System Calls
    RISHKA_SC_INT_ENABLE, ///< Enable interrupts
    RISHKA_SC_INT_DISABLE, ///< Disable interrupts
    RISHKA_SC_INT_ATTACH, ///< Attach interrupt handler
    RISHKA_SC_INT_DETACH, ///< Detach interrupt handler

    // File System System Calls
    RISHKA_SC_FS_MKDIR, ///< Create a directory
    RISHKA_SC_FS_RMDIR, ///< Remove a directory
    RISHKA_SC_FS_DELETE, ///< Delete a file
    RISHKA_SC_FS_EXISTS, ///< Check if a file or directory exists
    RISHKA_SC_FS_ISFILE, ///< Check if it is a file
    RISHKA_SC_FS_ISDIR, ///< Check if it is a directory
    RISHKA_SC_FS_OPEN, ///< Open a file
    RISHKA_SC_FS_CLOSE, ///< Close a file
    RISHKA_SC_FS_AVAILABLE, ///< Check if data is available to read from file
    RISHKA_SC_FS_FLUSH, ///< Flush file output buffer
    RISHKA_SC_FS_PEEK, ///< Peek the next character from the file
    RISHKA_SC_FS_SEEK, ///< Move file position to a specified location
    RISHKA_SC_FS_SIZE, ///< Get the size of a file
    RISHKA_SC_FS_READ, ///< Read data from a file
    RISHKA_SC_FS_WRITEB, ///< Write a byte to a file
    RISHKA_SC_FS_WRITES, ///< Write a string to a file
    RISHKA_SC_FS_POS, ///< Get the current position in the file
    RISHKA_SC_FS_PATH, ///< Get the path of a file
    RISHKA_SC_FS_NAME, ///< Get the name of a file or directory
    RISHKA_SC_FS_IS_OK, ///< Checks if there are more items in the file
    RISHKA_SC_FS_NEXT, ///< Get the next file or directory in a directory
    RISHKA_SC_FS_BUFSIZE, ///< Get the file buffer size
    RISHKA_SC_FS_LASTWRITE, ///< Get the last write time of a file
    RISHKA_SC_FS_SEEKDIR, ///< Seek to a position in a directory
    RISHKA_SC_FS_NEXT_NAME, ///< Get the name of the next file or directory in a directory
    RISHKA_SC_FS_REWIND, ///< Rewind the file position to the beginning

    // Command-Line Argument System Calls
    RISHKA_SC_ARG_COUNT, ///< Get the number of command-line arguments
    RISHKA_SC_ARG_STR, ///< Get the value of a command-line argument

    // Inter-Integrated Circuit (I2C) System Calls
    RISHKA_SC_I2C_BEGIN, ///< Begin I2C communication
    RISHKA_SC_I2C_END, ///< End I2C communication
    RISHKA_SC_I2C_BEGIN_TRANSMISSION, ///< Begin transmission to I2C device
    RISHKA_SC_I2C_END_TRANSMISSION, ///< End transmission to I2C device
    RISHKA_SC_I2C_WRITE, ///< Write data to I2C device
    RISHKA_SC_I2C_SLAVE_WRITE, ///< Write data to I2C slave device
    RISHKA_SC_I2C_READ, ///< Read data from I2C device
    RISHKA_SC_I2C_PEEK, ///< Peek the next byte from the I2C buffer
    RISHKA_SC_I2C_REQUEST, ///< Send a request to an I2C device
    RISHKA_SC_I2C_AVAILABLE, ///< Check if data is available from the I2C device
    RISHKA_SC_I2C_FLUSH, ///< Flush the I2C buffer
    RISHKA_SC_I2C_ON_RECEIVE, ///< Register a callback for I2C receive event
    RISHKA_SC_I2C_ON_REQUEST, ///< Register a callback for I2C request event
    RISHKA_SC_I2C_GET_TIMEOUT, ///< Get the I2C timeout value
    RISHKA_SC_I2C_SET_TIMEOUT, ///< Set the I2C timeout value
    RISHKA_SC_I2C_GET_CLOCK, ///< Get the I2C clock frequency
    RISHKA_SC_I2C_SET_CLOCK, ///< Set the I2C clock frequency
    RISHKA_SC_I2C_PINS, ///< Get the I2C pins
    RISHKA_SC_I2C_BUFSIZE, ///< Get the I2C buffer size

    // Keyboard Driver System Calls
    RISHKA_SC_KB_LAYOUT_NAME, ///< Get the name of the current keyboard layout
    RISHKA_SC_KB_LAYOUT_DESC, ///< Get the description of the current keyboard layout
    RISHKA_SC_KB_LED_GET_NUM, ///< Get the state of the Num Lock LED
    RISHKA_SC_KB_LED_GET_CAPS, ///< Get the state of the Caps Lock LED
    RISHKA_SC_KB_LED_GET_SCROLL, ///< Get the state of the Scroll Lock LED
    RISHKA_SC_KB_LED_SET_NUM, ///< Set the state of the Num Lock LED
    RISHKA_SC_KB_LED_SET_CAPS, ///< Set the state of the Caps Lock LED
    RISHKA_SC_KB_LED_SET_SCROLL, ///< Set the state of the Scroll Lock LED
    RISHKA_SC_KB_NEXT_SCAN_CODE, ///< Get the next keyboard scan code
    RISHKA_SC_KB_DEVICE_TYPE, ///< Get the type of the keyboard device
    RISHKA_SC_KB_AVAILABLE_VK, ///< Get the available virtual key codes
    RISHKA_SC_KB_IS_AVAILABLE, ///< Check if the keyboard is available
    RISHKA_SC_KB_IS_VK_DOWN, ///< Check if a virtual key is currently pressed
    RISHKA_SC_KB_LOCK, ///< Lock the keyboard to prevent input
    RISHKA_SC_KB_UNLOCK, ///< Unlock the keyboard to allow input
    RISHKA_SC_KB_RESET, ///< Reset the keyboard to its default state

    // Display Interface Driver System Calls
    RISHKA_SC_DISPLAY_SCREEN_HEIGHT, ///< Get display screen height
    RISHKA_SC_DISPLAY_SCREEN_WIDTH, ///< Get display screen width
    RISHKA_SC_DISPLAY_VIEWPORT_HEIGHT, ///< Get display vertical size of viewport
    RISHKA_SC_DISPLAY_VIEWPORT_WIDTH, ///< Get display horizontal size of viewport
    RISHKA_SC_DISPLAY_SUPPORTED_COLORS, ///< Get numbers of color the display can provide

    // Non-Volatile Storage System Calls
    RISHKA_SC_NVS_ERASE, ///< Erase an NVS content by key
    RISHKA_SC_NVS_ERASE_ALL, ///< Erase all the contents of NVS
    RISHKA_SC_NVS_SET_I8, ///< Set i8 value by key
    RISHKA_SC_NVS_SET_I16, ///< Set i16 value by key
    RISHKA_SC_NVS_SET_I32, ///< Set i32 value by key
    RISHKA_SC_NVS_SET_I64, ///< Set i64 value by key
    RISHKA_SC_NVS_SET_U8, ///< Set u8 value by key
    RISHKA_SC_NVS_SET_U16, ///< Set u16 value by key
    RISHKA_SC_NVS_SET_U32, ///< Set u32 value by key
    RISHKA_SC_NVS_SET_U64, ///< Set u64 value by key
    RISHKA_SC_NVS_SET_STRING, ///< Set string value by key
    RISHKA_SC_NVS_GET_I8, ///< Get i8 value by key
    RISHKA_SC_NVS_GET_I16, ///< Get i16 value by key
    RISHKA_SC_NVS_GET_I32, ///< Get i32 value by key
    RISHKA_SC_NVS_GET_I64, ///< Get i64 value by key
    RISHKA_SC_NVS_GET_U8, ///< Get u8 value by key
    RISHKA_SC_NVS_GET_U16, ///< Get u16 value by key
    RISHKA_SC_NVS_GET_U32, ///< Get u32 value by key
    RISHKA_SC_NVS_GET_U64, ///< Get u64 value by key
    RISHKA_SC_NVS_GET_STRING, ///< Get string value by key

    RISHKA_SC_NVS_HAS_WIFI_CONFIG, ///< Check for existing WiFi configurations
    RISHKA_SC_NVS_SET_WIFI_SSID, ///< Set the WiFi service set identifier
    RISHKA_SC_NVS_SET_WIFI_PWORD, ///< Set the WiFi password

    // Runtime System Calls
    RISHKA_SC_RT_STRPASS, ///< Pass string from runtime to syscalls
    RISHKA_SC_RT_YIELD, ///< Yield execution to other tasks
    RISHKA_SC_RT_FORK_STREAM ///< Passes the program fork output stream
};

/**
 * @class RishkaSyscall
 * @brief Class containing implementations of Rishka kernel system calls.
 *
 * The RishkaSyscall class provides static member functions to implement various system calls
 * supported by the Rishka kernel. Each system call is implemented as a static member function
 * within the appropriate nested class corresponding to its category (e.g., IO, Sys, Memory, Gpio, etc.).
 * These functions are invoked by the Rishka virtual machine to perform the requested operations.
 */
class RishkaSyscall final {
public:
    /**
     * @class IO
     * @brief Class containing implementations of input/output system calls.
     *
     * The IO class provides static member functions to implement input/output system calls,
     * such as printing text, reading input, and manipulating streams.
     */
    class IO final {
    public:
        static void prints(RishkaVM* vm);
        static void printn(RishkaVM* vm);
        static void printu(RishkaVM* vm);
        static void printd(RishkaVM* vm);

        static char readch(RishkaVM* vm);
        static size_t readLine(RishkaVM* vm);
        static int read(RishkaVM* vm);

        static int available(RishkaVM* vm);
        static int peek(RishkaVM* vm);

        static bool find(RishkaVM* vm);
        static bool findUntil(RishkaVM* vm);

        static void setTimeout(RishkaVM* vm);
        static uint64_t getTimeout(RishkaVM* vm);
    };

    /**
     * @class Sys
     * @brief Class containing implementations of system utility system calls.
     *
     * The Sys class provides static member functions to implement system utility system calls,
     * such as delaying execution, obtaining system information, and terminating the program.
     */
    class Sys final {
    public:
        static void delayImpl(RishkaVM* vm);
        static unsigned long microsImpl();
        static unsigned long millisImpl();
        static int64_t shellExec(RishkaVM* parent_vm);
        static void exit(RishkaVM* vm);
        static uint32_t infos(RishkaVM* vm);
        static long infon(RishkaVM* vm);
        static long randomImpl();
        static bool changeDir(RishkaVM* vm);
        static uint32_t workingDirectory(RishkaVM* vm);
    };

    /**
     * @class Gpio
     * @brief Class containing implementations of general purpose input/output system calls.
     *
     * The Gpio class provides static member functions to implement general purpose input/output
     * system calls, such as configuring pins, reading/writing digital/analog values, and generating tones.
     */
    class Gpio final {
    public:
        static void pinModeImpl(RishkaVM* vm);
        static bool digitalReadImpl(RishkaVM* vm);
        static void digitalWriteImpl(RishkaVM* vm);
        static uint16_t analogReadImpl(RishkaVM* vm);
        static void analogWriteImpl(RishkaVM* vm);
        static uint64_t pulseInImpl(RishkaVM* vm);
        static uint64_t pulseInLongImpl(RishkaVM* vm);
        static uint8_t shiftInImpl(RishkaVM* vm);
        static void shiftOutImpl(RishkaVM* vm);
        static void toneImpl(RishkaVM* vm);
        static void noToneImpl(RishkaVM* vm);
    };

    /**
     * @class Int
     * @brief Class containing implementations of interrupt handling system calls.
     *
     * The Int class provides static member functions to implement interrupt handling system calls,
     * such as enabling/disabling interrupts and attaching/detaching interrupt handlers.
     */
    class Int final {
    public:
        static void enable();
        static void disable();
        static void attach(RishkaVM* vm);
        static void detach(RishkaVM* vm);
    };

    /**
     * @class FS
     * @brief Class containing implementations of file system system calls.
     *
     * The FS class provides static member functions to implement file system system calls,
     * such as file/directory manipulation, file I/O operations, and file metadata retrieval.
     */
    class FS final {
    public:
        static bool mkdir(RishkaVM* vm);
        static bool rmdir(RishkaVM* vm);
        static bool remove(RishkaVM* vm);
        static bool exists(RishkaVM* vm);
        static bool isfile(RishkaVM* vm);
        static bool isdir(RishkaVM* vm);
        static uint8_t open(RishkaVM* vm);
        static void close(RishkaVM* vm);
        static int available(RishkaVM* vm);
        static void flush(RishkaVM* vm);
        static int peek(RishkaVM* vm);
        static bool seek(RishkaVM* vm);
        static uint32_t size(RishkaVM* vm);
        static int read(RishkaVM* vm);
        static size_t writeb(RishkaVM* vm);
        static size_t writes(RishkaVM* vm);
        static size_t position(RishkaVM* vm);
        static uint32_t path(RishkaVM* vm);
        static uint32_t name(RishkaVM* vm);
        static bool isOk(RishkaVM* vm);
        static uint8_t next(RishkaVM* vm);
        static bool bufsize(RishkaVM* vm);
        static uint64_t lastwrite(RishkaVM* vm);
        static bool seekdir(RishkaVM* vm);
        static uint32_t next_name(RishkaVM* vm);
        static void rewind(RishkaVM* vm);
    };

    /**
     * @class Args
     * @brief Class containing implementations of command-line argument system calls.
     *
     * The Args class provides static member functions to implement VM argument on
     * execution system calls, such as obtaining the number of arguments and retrieving
     * argument values by index.
     */
    class Args final {
    public:
        static uint8_t count(RishkaVM* vm);
        static uint32_t value(RishkaVM* vm);
    };

    /**
     * @class I2C
     * @brief Class containing implementations of I2C system calls.
     *
     * The I2C class provides static member functions to implement I2C system calls,
     * such as initiating communication, transmitting/receiving data, and configuring I2C settings.
     */
    class I2C final {
    public:
        static bool begin(RishkaVM* vm);
        static bool end(RishkaVM* vm);
        static void begin_transmission(RishkaVM* vm);
        static uint8_t end_transmission(RishkaVM* vm);
        static size_t write(RishkaVM* vm);
        static size_t slave_write(RishkaVM* vm);
        static int read(RishkaVM* vm);
        static int peek(RishkaVM* vm);
        static size_t request(RishkaVM* vm);
        static int available(RishkaVM* vm);
        static void flush();
        static void on_receive(RishkaVM* vm);
        static void on_request(RishkaVM* vm);
        static uint16_t get_timeout();
        static void set_timeout(RishkaVM* vm);
        static bool set_clock(RishkaVM* vm);
        static uint32_t get_clock();
        static bool pins(RishkaVM* vm);
        static size_t bufsize(RishkaVM* vm);
    };

    /**
     * @class SPICall
     * @brief Class containing implementations of SPI system calls.
     *
     * The SPICall class provides static member functions to implement SPI system calls,
     * such as initiating communication, transferring data, and configuring SPI settings.
     */
    class SPICall final {
    public:
        static void begin(RishkaVM* vm);
        static void end();
        static void begin_transaction(RishkaVM* vm);
        static void end_transaction();
        static uint8_t transfer8(RishkaVM* vm);
        static uint16_t transfer16(RishkaVM* vm);
        static uint32_t transfer32(RishkaVM* vm);
        static void transfer_bytes(RishkaVM* vm);
        static void transfer_bits(RishkaVM* vm);
        static void set_hwcs(RishkaVM* vm);
        static void set_bit_order(RishkaVM* vm);
        static void set_data_mode(RishkaVM* vm);
        static void set_frequency(RishkaVM* vm);
        static void set_clock_div(RishkaVM* vm);
        static uint32_t get_clock_div();
        static void write8(RishkaVM* vm);
        static void write16(RishkaVM* vm);
        static void write32(RishkaVM* vm);
        static void write_bytes(RishkaVM* vm);
        static void write_pixels(RishkaVM* vm);
        static void write_pattern(RishkaVM* vm);
    };

    /**
     * @class Keyboard
     * @brief Class containing system call operations
     *        related to keyboard input.
     *
     * This class provides static member functions for
     * performing keyboard-related system calls in the
     * Rishka runtime environment. These functions allow
     * Rishka programs to read input from the keyboard
     * and perform operations based on user input.
     */
    class Keyboard final {
    public:
        static uint32_t layout_name();
        static uint32_t layout_desc();
        static uint8_t device_type();

        static bool is_num_lock();
        static bool is_caps_lock();
        static bool is_scroll_lock();

        static void num_lock(RishkaVM* vm);
        static void caps_lock(RishkaVM* vm);
        static void scroll_lock(RishkaVM* vm);

        static uint32_t next_scan_code(RishkaVM* vm);
        static void lock(RishkaVM* vm);
        static void unlock();
        static void reset();
    };

    /**
     * @class Display
     * @brief Static class for display-related system calls.
     *
     * The Display class provides static methods for querying
     * and setting display properties such as screen dimensions,
     * viewport dimensions, supported colors, and screen
     * orientation. This class is essential for managing and
     * interacting with the display hardware, allowing applications
     * to adapt to different screen configurations and capabilities.
     */
    class Display final {
    public:
        static int screen_height(RishkaVM* vm);
        static int screen_width(RishkaVM* vm);

        static int viewport_height(RishkaVM* vm);
        static int viewport_width(RishkaVM* vm);

        static int supported_colors(RishkaVM* vm);
    };

    /**
     * @class NVS
     * @brief Static class for non-volatile
     *        storage (NVS) related system calls.
     *
     * The NVS class provides static methods for
     * managing non-volatile storage, allowing
     * applications to store and retrieve persistent
     * data. Non-volatile storage is essential for
     * maintaining state across reboots or power
     * cycles, and this class facilitates the
     * interaction with such storage.
     */
    class NVS final {
    public:
        static bool erase_all();
        static bool erase(RishkaVM* vm);

        static bool set_i8(RishkaVM* vm);
        static bool set_i16(RishkaVM* vm);
        static bool set_i32(RishkaVM* vm);
        static bool set_i64(RishkaVM* vm);

        static bool set_u8(RishkaVM* vm);
        static bool set_u16(RishkaVM* vm);
        static bool set_u32(RishkaVM* vm);
        static bool set_u64(RishkaVM* vm);

        static bool get_i8(RishkaVM* vm);
        static bool get_i16(RishkaVM* vm);
        static bool get_i32(RishkaVM* vm);
        static bool get_i64(RishkaVM* vm);

        static bool get_u8(RishkaVM* vm);
        static bool get_u16(RishkaVM* vm);
        static bool get_u32(RishkaVM* vm);
        static bool get_u64(RishkaVM* vm);

        static bool set_string(RishkaVM* vm);
        static bool get_string(RishkaVM* vm);
    };

    /**
     * @class Runtime
     * @brief Class containing implementations of runtime system calls.
     *
     * The Runtime class provides static member functions to implement runtime system calls,
     * such as passing strings between runtime and system call implementations, and yielding
     * execution to other tasks.
     */
    class Runtime final {
    public:
        static char strpass();
        static void yield();
        static uint32_t getForkString(RishkaVM* vm);
    };
};

#endif /* RISHKA_SYSCALLS_H */
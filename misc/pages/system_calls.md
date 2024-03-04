@page rt_syscalls Runtime System Calls
@brief Rishka kernel-level system calls foundation under the hood and comprehensive list.
@tableofcontents

Rishka system calls provide a mechanism for user-level applications to request services or functionality from the underlying operating system (Jessy OS) running on the ESP32-WROVER microcontroller. Through these system calls, applications can request various services, functionalities, or access to system resources that are otherwise restricted from user-level code. This includes operations such as file I/O, memory management, device control, and system information retrieval. By leveraging system calls, applications can harness the full capabilities of the operating system while maintaining a clear separation between user-level and kernel-level code, ensuring system integrity, security, and reliability.

1. **Invocation**

    User-level applications on the ESP32-WROVER microcontroller can request privileged operations or access system-level functionality through system calls. These requests are made by the application directly to the underlying operating system via Rishka kernel. Rather than executing system calls directly, applications rely on an abstraction layer provided by the Rishka SDK to facilitate communication with the operating system.

2. **Arguments**

    When invoking a system call, the user-level application provides any necessary parameters or data to the operating system. These parameters convey important information to the system call handler, enabling it to perform the requested operation. The specific arguments required for each system call depend on the nature of the operation being requested and are documented in the Rishka SDK.

3. **Registers**

    Before initiating a system call, the user-level application prepares the arguments and system call identifier according to the RISC-V calling convention. This convention dictates the usage of specific registers to pass function arguments and system call identifiers to the operating system. By adhering to this convention, applications ensure that the operating system can correctly interpret and process the provided information.

4. **Triggering the System Call**

    Once the necessary arguments are prepared, the user-level application initiates the system call by triggering a special instruction known as the `scall`. This instruction serves as a signal to the processor that a system call is being requested. Upon encountering the scall instruction, the processor transitions from user mode to privileged mode and transfers control to the operating system's system call handler.

5. **System Call Handler**

    The system call handler within the operating system receives control when the `scall` instruction is encountered. It is responsible for interpreting the system call identifier provided by the user-level application and executing the corresponding functionality or service. The handler interacts with kernel resources, hardware peripherals, and other system components as necessary to fulfill the requested operation.

6. **Result**

    After processing the system call, the system call handler returns the result of the operation to the user-level application. This result communicates the outcome of the system call execution, indicating whether the requested operation was successful or encountered an error. The user-level application can then proceed with its execution based on this feedback.

7. **Return to User-Level Code**

    Upon completing the system call handler's execution, control returns to the user-level application. The application resumes its execution, continuing with its tasks or responding to the outcome of the system call as needed. This seamless return to user-level code ensures that applications can interact with the operating system efficiently and access privileged functionality while maintaining a clear separation between user and kernel space.

## Complete List of System Calls

The Rishka virtual runtime kernel provides an extensive array of system calls, offering a rich set of functionalities and services for user-level applications on the ESP32-WROVER microcontroller platform.

The below table is a comprehensive listing with details of the diverse range of system calls available within the Rishka kernel. Note that this is subject to changes since Rishka is still under development.

| Address (`A7`) | Name                | `A0`               | `A1`                | `A2`             | `A3`              | Implementation            |
|----------------|---------------------|--------------------|---------------------|------------------|-------------------|---------------------------|
| 0x0000         | PRINTS              | (string) text      |                     |                  |                   | IO::print(string)         |
| 0x0001         | PRINTN              | (i64) number       |                     |                  |                   | IO::print(i64)            |
| 0x0002         | PRINTD              | (double) number    |                     |                  |                   | IO::print(double)         |
| 0x0003         | READCH              |                    |                     |                  |                   | IO::readch()              |
| 0x0004         | READLINE            |                    |                     |                  |                   | IO::readline()            |
| 0x0005         | READ                |                    |                     |                  |                   | IO::read()                |
| 0x0006         | AVAILABLE           |                    |                     |                  |                   | IO::available()           |
| 0x0007         | PEEK                |                    |                     |                  |                   | IO::peek()                |
| 0x0008         | FIND                | (string) target    | (usize) size        |                  |                   | IO::find()                |
| 0x0009         | FIND_UNTIL          | (string) target    | (string) terminator |                  |                   | IO::find_until()          |
| 0x000a         | SET_TIMEOUT         | (u64) timeout      |                     |                  |                   | IO::set_timeout()         |
| 0x000b         | GET_TIMEOUT         |                    |                     |                  |                   | IO::get_timeout()         |
| 0x000c         | DELAY_MS            | (u64) ms           |                     |                  |                   | Sys::delay()              |
| 0x000d         | MICROS              |                    |                     |                  |                   | Sys::micros()             |
| 0x000e         | MILLIS              |                    |                     |                  |                   | Sys::millis()             |
| 0x000f         | SHELLEXEC           | (string) program   | (i32) argc          | (string*) argv   |                   | Sys::shellexec()          |
| 0x0010         | EXIT                | (i32) code         |                     |                  |                   | Sys::exit()               |
| 0x0011         | INFOS               | (sysinfos_t) key   |                     |                  |                   | Sys::info_str()           |
| 0x0012         | INFON               | (sysinfon_t) key   |                     |                  |                   | Sys::info_num()           |
| 0x0013         | RANDOM              |                    |                     |                  |                   | Sys::random()             |
| 0x0014         | ALLOC               | (any) dest         | (usize) size        |                  |                   | Memory::alloc()           |
| 0x0015         | CALLOC              | (any) dest         | (usize) num         | (usize) size     |                   | Memory::calloc()          |
| 0x0016         | REALLOC             | (any) dest         | (any) ptr           | (usize) size     |                   | Memory::realloc()         |
| 0x0017         | FREE                | (any) ptr          |                     |                  |                   | Memory::free()            |
| 0x0018         | MEMSET              | (any) dest         | (i32) count         | (u32) n          |                   | Memory::set()             |
| 0x0019         | PIN_MODE            | (u8) pin           | (gpio_pin_mode_t) m |                  |                   | Gpio::pin_mode()          |
| 0x001a         | DIGITAL_READ        | (u8) pin           |                     |                  |                   | Gpio::digital_read()      |
| 0x001b         | DIGITAL_WRITE       | (u8) pin           | (gpio_mode_t) mode  |                  |                   | Gpio::digital_write()     |
| 0x001c         | ANALOG_READ         | (u8) pin           |                     |                  |                   | Gpio::analog_read()       |
| 0x001d         | ANALOG_WRITE        | (u8) pin           | (u16) value         |                  |                   | Gpio::analog_write()      |
| 0x001e         | PULSE_IN            | (u8) pin           | (u8) state          | (u64) timeout    |                   | Gpio::pulse_in()          |
| 0x001f         | PULSE_IN_LONG       | (u8) pin           | (u8) state          | (u64) timeout    |                   | Gpio::pulse_in_long()     |
| 0x0020         | SHIFT_IN            | (u8) data          | (u8) clock          | (u8) bit_order   |                   | Gpio::shift_in()          |
| 0x0021         | SHIFT_OUT           | (u8) data          | (u8) clock          | (u8) bit_order   | (u8) value        | Gpio::shift_out()         |
| 0x0022         | TONE                | (u8) pin           | (u32) frequency     | (u64) duration   |                   | Gpio::tone()              |
| 0x0023         | NO_TONE             | (u8) pin           |                     |                  |                   | Gpio::no_tone()           |
| 0x0024         | INT_ENABLE          |                    |                     |                  |                   | Int::enable()             |
| 0x0025         | INT_DISABLE         |                    |                     |                  |                   | Int::disable()            |
| 0x0026         | INT_ATTACH          | (u8) pin           | void (*callback)()  | (int_mode_t) m   |                   | Int::attach()             |
| 0x0027         | INT_DETACH          | (u8) pin           |                     |                  |                   | Int::detach()             |
| 0x0028         | MKDIR               | (string) path      |                     |                  |                   | FS::mkdir()               |
| 0x0029         | RMDIR               | (string) path      |                     |                  |                   | FS::rmdir()               |
| 0x002a         | DELETE              | (string) path      |                     |                  |                   | FS::remove()              |
| 0x002b         | EXISTS              | (string) path      |                     |                  |                   | FS::exists()              |
| 0x002c         | ISFILE              | (i32) handle       |                     |                  |                   | File::is_file()           |
| 0x002d         | ISDIR               | (i32) handle       |                     |                  |                   | File::is_dir()            |
| 0x002e         | OPEN                | (string) file      | (string) mode       |                  |                   | File::open()              |
| 0x002f         | CLOSE               | (i32) handle       |                     |                  |                   | File::close()             |
| 0x0030         | AVAILABLE           | (i32) handle       |                     |                  |                   | File::available()         |
| 0x0031         | FLUSH               | (i32) handle       |                     |                  |                   | File::flush()             |
| 0x0032         | PEEK                | (i32) handle       |                     |                  |                   | File::peek()              |
| 0x0033         | SEEK                | (i32) handle       | (u32) position      |                  |                   | File::seek()              |
| 0x0034         | SIZE                | (i32) handle       |                     |                  |                   | File::size()              |
| 0x0035         | READ                | (i32) handle       |                     |                  |                   | File::read()              |
| 0x0036         | WRITEB              | (i32) handle       | (u8) data           |                  |                   | File::write(u8)           |
| 0x0037         | WRITES              | (i32) handle       | (string) data       |                  |                   | File::write(string)       |
| 0x0038         | POS                 | (i32) handle       |                     |                  |                   | File::position()          |
| 0x0039         | PATH                | (i32) handle       |                     |                  |                   | File::path()              |
| 0x003a         | NAME                | (i32) handle       |                     |                  |                   | File::name()              |
| 0x003b         | NEXT                | (i32) handle       | (string) mode       |                  |                   | File::next()              |
| 0x003c         | BUFSIZE             | (i32) handle       | (usize) size        |                  |                   | File::bufsize()           |
| 0x003d         | LASTWRITE           | (i32) handle       |                     |                  |                   | File::lastwrite()         |
| 0x003e         | SEEKDIR             | (i32) handle       | (u64) position      |                  |                   | File::seek_dir()          |
| 0x003f         | NEXT_NAME           | (i32) handle       |                     |                  |                   | File::next_name()         |
| 0x0040         | REWIND              | (i32) handle       |                     |                  |                   | File::rewind()            |
| 0x0041         | ARGC                |                    |                     |                  |                   | Args::count()             |
| 0x0042         | ARGSTR              | (u8) index         |                     |                  |                   | Args::value()             |
| 0x0043         | I2C_BEGIN           | (u8) address       |                     |                  |                   | I2C::begin()              |
| 0x0044         | I2C_END             |                    |                     |                  |                   | I2C::end()                |
| 0x0045         | I2C_BEGIN_TRANS     | (u8) address       |                     |                  |                   | I2C::begin_transmission() |
| 0x0046         | I2C_END_TRANS       | (bool) stop_bit    |                     |                  |                   | I2C::end_transmission()   |
| 0x0047         | I2C_WRITE           | (u8*) data         | (usize) size        |                  |                   | I2C::write()              |
| 0x0048         | I2C_SLAVE_WRITE     | (u8*) data         | (usize) size        |                  |                   | I2C::slave_write()        |
| 0x0049         | I2C_READ            |                    |                     |                  |                   | I2C::read()               |
| 0x004a         | I2C_PEEK            |                    |                     |                  |                   | I2C::peek()               |
| 0x004b         | I2C_REQ             | (u8) address       | (usize) size        | (bool) stop_bit  |                   | I2C::request()            |
| 0x004c         | I2C_AVAILABLE       |                    |                     |                  |                   | I2C::available()          |
| 0x004d         | I2C_FLUSH           |                    |                     |                  |                   | I2C::flush()              |
| 0x004e         | I2C_ON_RECEIVE      | void (*cback)(int) |                     |                  |                   | I2C::on_receive()         |
| 0x004f         | I2C_ON_REQUEST      | void (*cback)()    |                     |                  |                   | I2C::on_request()         |
| 0x0050         | I2C_GET_TIMEOUT     |                    |                     |                  |                   | I2C::get_timeout()        |
| 0x0051         | I2C_SET_TIMEOUT     | (u32) timeout      |                     |                  |                   | I2C::set_timeout()        |
| 0x0052         | I2C_GET_CLOCK       |                    |                     |                  |                   | I2C::get_clock()          |
| 0x0053         | I2C_SET_CLOCK       | (u32) clock        |                     |                  |                   | I2C::set_clock()          |
| 0x0054         | I2C_PINS            | (u8) sda           | (u8) scl            |                  |                   | I2C::pins()               |
| 0x0055         | I2C_BUFSIZE         | (usize) size       |                     |                  |                   | I2C::set_buffersize()     |
| 0x0056         | SPI_BEGIN           | (u8) sck           | (u8) miso           | (u8) mosi        | (u8) ss           | SPI::begin()              |
| 0x0057         | SPI_END             |                    |                     |                  |                   | SPI::end()                |
| 0x0058         | SPI_BEGIN_TRANSAC   | (u8) clock         | (u8) bit_order      | (spi_mode_t) m   |                   | SPI::begin_transaction()  |
| 0x0059         | SPI_END_TRANSAC     |                    |                     |                  |                   | SPI::end_transaction()    |
| 0x005a         | SPI_TRANSFER8       | (u8) data          |                     |                  |                   | SPI::transfer8()          |
| 0x005b         | SPI_TRANSFER16      | (u16) data         |                     |                  |                   | SPI::transfer16()         |
| 0x005c         | SPI_TRANSFER32      | (u32) data         |                     |                  |                   | SPI::transfer32()         |
| 0x005d         | SPI_TRANSFER_BYTES  | (u8*) data         | (u8*) out           | (u8) bits        |                   | SPI::transfer_bytes()     |
| 0x005e         | SPI_TRANSFER_BITS   | (u32) data         | (u32*) out          | (u8) bits        |                   | SPI::transfer_bits()      |
| 0x005f         | SPI_SET_HWCS        | (bool) use         |                     |                  |                   | SPI::set_hwcs()           |
| 0x0060         | SPI_SET_BIT_ORDER   | (u8) bit_order     |                     |                  |                   | SPI::set_bit_order()      |
| 0x0061         | SPI_SET_DATA_MODE   | (spi_mode_t) dmode |                     |                  |                   | SPI::set_data_mode()      |
| 0x0062         | SPI_SET_FREQ        | (u32) frequency    |                     |                  |                   | SPI::set_frequency()      |
| 0x0063         | SPI_SET_CLOCK_DIV   | (u32) clock_div    |                     |                  |                   | SPI::set_clock_div()      |
| 0x0064         | SPI_GET_CLOCK_DIV   |                    |                     |                  |                   | SPI::get_clock_div()      |
| 0x0065         | SPI_WRITE8          | (u8) data          |                     |                  |                   | SPI::write8()             |
| 0x0066         | SPI_WRITE16         | (u16) data         |                     |                  |                   | SPI::write16()            |
| 0x0067         | SPI_WRITE32         | (u32) data         |                     |                  |                   | SPI::write32()            |
| 0x0068         | SPI_WRITE_BYTES     | (u8*) data         | (u32) size          |                  |                   | SPI::write_bytes()        |
| 0x0069         | SPI_WRITE_PIXELS    | (any) data         | (u32) size          |                  |                   | SPI::write_pixels()       |
| 0x006a         | SPI_WRITE_PATTERN   | (u8*) data         | (u8) size           | (u32) pattern    |                   | SPI::write_pattern()      |
| 0x006b         | RT_STRPASS          |                    |                     |                  |                   | _N/A_                     |
| 0x006c         | RT_YIELD            |                    |                     |                  |                   | Runtime::yield()          |

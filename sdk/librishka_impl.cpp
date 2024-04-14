/* 
 * This file is part of the Rishka distribution (https://github.com/rishka-esp32/rishka-sdk).
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

#include "librishka.h"

enum rishka_syscall {
    RISHKA_SC_IO_PRINTS,
    RISHKA_SC_IO_PRINTN,
    RISHKA_SC_IO_PRINTD,
    RISHKA_SC_IO_READCH,
    RISHKA_SC_IO_READLINE,
    RISHKA_SC_IO_READ,
    RISHKA_SC_IO_AVAILABLE,
    RISHKA_SC_IO_PEEK,
    RISHKA_SC_IO_FIND,
    RISHKA_SC_IO_FIND_UNTIL,
    RISHKA_SC_IO_SET_TIMEOUT,
    RISHKA_SC_IO_GET_TIMEOUT,

    RISHKA_SC_SYS_DELAY_MS,
    RISHKA_SC_SYS_MICROS,
    RISHKA_SC_SYS_MILLIS,
    RISHKA_SC_SYS_SHELLEXEC,
    RISHKA_SC_SYS_EXIT,
    RISHKA_SC_SYS_INFOS,
    RISHKA_SC_SYS_INFON,
    RISHKA_SC_SYS_RANDOM,
    RISHKA_SC_SYS_CD,
    RISHKA_SC_SYS_WD,

    RISHKA_SC_MEM_ALLOC,
    RISHKA_SC_MEM_CALLOC,
    RISHKA_SC_MEM_REALLOC,
    RISHKA_SC_MEM_FREE,
    RISHKA_SC_MEM_SET,

    RISHKA_SC_GPIO_PIN_MODE,
    RISHKA_SC_GPIO_DIGITAL_READ,
    RISHKA_SC_GPIO_DIGITAL_WRITE,
    RISHKA_SC_GPIO_ANALOG_READ,
    RISHKA_SC_GPIO_ANALOG_WRITE,
    RISHKA_SC_GPIO_PULSE_IN,
    RISHKA_SC_GPIO_PULSE_IN_LONG,
    RISHKA_SC_GPIO_SHIFT_IN,
    RISHKA_SC_GPIO_SHIFT_OUT,
    RISHKA_SC_GPIO_TONE,
    RISHKA_SC_GPIO_NO_TONE,

    RISHKA_SC_INT_ENABLE,
    RISHKA_SC_INT_DISABLE,
    RISHKA_SC_INT_ATTACH,
    RISHKA_SC_INT_DETACH,

    RISHKA_SC_FS_MKDIR,
    RISHKA_SC_FS_RMDIR,
    RISHKA_SC_FS_DELETE,
    RISHKA_SC_FS_EXISTS,
    RISHKA_SC_FS_ISFILE,
    RISHKA_SC_FS_ISDIR,
    RISHKA_SC_FS_OPEN,
    RISHKA_SC_FS_CLOSE,
    RISHKA_SC_FS_AVAILABLE,
    RISHKA_SC_FS_FLUSH,
    RISHKA_SC_FS_PEEK,
    RISHKA_SC_FS_SEEK,
    RISHKA_SC_FS_SIZE,
    RISHKA_SC_FS_READ,
    RISHKA_SC_FS_WRITEB,
    RISHKA_SC_FS_WRITES,
    RISHKA_SC_FS_POS,
    RISHKA_SC_FS_PATH,
    RISHKA_SC_FS_NAME,
    RISHKA_SC_FS_IS_OK,
    RISHKA_SC_FS_NEXT,
    RISHKA_SC_FS_BUFSIZE,
    RISHKA_SC_FS_LASTWRITE,
    RISHKA_SC_FS_SEEKDIR,
    RISHKA_SC_FS_NEXT_NAME,
    RISHKA_SC_FS_REWIND,

    RISHKA_SC_ARG_COUNT,
    RISHKA_SC_ARG_STR,

    RISHKA_SC_I2C_BEGIN,
    RISHKA_SC_I2C_END,
    RISHKA_SC_I2C_BEGIN_TRANSMISSION,
    RISHKA_SC_I2C_END_TRANSMISSION,
    RISHKA_SC_I2C_WRITE,
    RISHKA_SC_I2C_SLAVE_WRITE,
    RISHKA_SC_I2C_READ,
    RISHKA_SC_I2C_PEEK,
    RISHKA_SC_I2C_REQUEST,
    RISHKA_SC_I2C_AVAILABLE,
    RISHKA_SC_I2C_FLUSH,
    RISHKA_SC_I2C_ON_RECEIVE,
    RISHKA_SC_I2C_ON_REQUEST,
    RISHKA_SC_I2C_GET_TIMEOUT,
    RISHKA_SC_I2C_SET_TIMEOUT,
    RISHKA_SC_I2C_GET_CLOCK,
    RISHKA_SC_I2C_SET_CLOCK,
    RISHKA_SC_I2C_PINS,
    RISHKA_SC_I2C_BUFSIZE,

    RISHKA_SC_SPI_BEGIN,
    RISHKA_SC_SPI_END,
    RISHKA_SC_SPI_BEGIN_TRANSACTION,
    RISHKA_SC_SPI_END_TRANSACTION,
    RISHKA_SC_SPI_TRANSFER8,
    RISHKA_SC_SPI_TRANSFER16,
    RISHKA_SC_SPI_TRANSFER32,
    RISHKA_SC_SPI_TRANSFER_BYTES,
    RISHKA_SC_SPI_TRANSFER_BITS,
    RISHKA_SC_SPI_SET_HWCS,
    RISHKA_SC_SPI_SET_BIT_ORDER,
    RISHKA_SC_SPI_SET_DATA_MODE,
    RISHKA_SC_SPI_SET_FREQ,
    RISHKA_SC_SPI_SET_CLOCK_DIV,
    RISHKA_SC_SPI_GET_CLOCK_DIV,
    RISHKA_SC_SPI_WRITE8,
    RISHKA_SC_SPI_WRITE16,
    RISHKA_SC_SPI_WRITE32,
    RISHKA_SC_SPI_WRITE_BYTES,
    RISHKA_SC_SPI_WRITE_PIXELS,
    RISHKA_SC_SPI_WRITE_PATTERN,

    RISHKA_SC_RT_STRPASS,
    RISHKA_SC_RT_YIELD
};

static inline long long int double_to_long(double d) {
    union {
        double input;
        long long int output;
    } data;

    data.input = d;
    return data.output;
}

static inline double long_to_float(long long int l) {
    union {
        double output;
        long long int input;
    } data;

    data.input = l;
    return data.output;
}

static inline i64 rishka_sc_0(i32 scallid) {
    register i64 a0 asm("a0") = 0;
    register i64 scid asm("a7") = scallid;

    asm volatile ("scall" : "+r"(a0) : "r"(scid));    
    return a0;
}

static inline i64 rishka_sc_1(i32 scallid, i64 arg0) {
    register i64 a0 asm("a0") = arg0;
    register i64 scid asm("a7") = scallid;

    asm volatile ("scall" : "+r"(a0) : "r"(scid));
    return a0;
}

static inline i64 rishka_sc_2(i32 scallid, i64 arg0, i64 arg1) {
    register i64 a0 asm("a0") = arg0;
    register i64 a1 asm("a1") = arg1;
    register i64 scid asm("a7") = scallid;

    asm volatile ("scall" : "+r"(a0) : "r"(a1), "r"(scid));
    return a0;
}

static inline i64 rishka_sc_3(i32 scallid, i64 arg0, i64 arg1, i64 arg2) {
    register i64 a0 asm("a0") = arg0;
    register i64 a1 asm("a1") = arg1;
    register i64 a2 asm("a2") = arg2;
    register i64 scid asm("a7") = scallid;

    asm volatile ("scall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(scid));
    return a0;
}

static inline i64 rishka_sc_4(i32 scallid, i64 arg0, i64 arg1, i64 arg2, i64 arg3) {
    register i64 a0 asm("a0") = arg0;
    register i64 a1 asm("a1") = arg1;
    register i64 a2 asm("a2") = arg2;
    register i64 a3 asm("a3") = arg3;
    register i64 scid asm("a7") = scallid;

    asm volatile ("scall" : "+r"(a0) : "r"(a1), "r"(a2), "r"(a3), "r"(scid));
    return a0;
}

static inline rune rt_strpass() {
    return (rune) rishka_sc_0(RISHKA_SC_RT_STRPASS);    
}

static inline string get_rt_string(u32 len) {
    string str;
    Memory::set(str, 0, len + 1);

    for(u32 i = 0; i < len; i++)
        str[i] = rt_strpass();

    str[len] = '\0';
    return str;
}

void IO::print(const string text) {
    rishka_sc_1(RISHKA_SC_IO_PRINTS, (i64) text);
}

void IO::print(i64 number) {
    rishka_sc_1(RISHKA_SC_IO_PRINTN, (i64) number);
}

void IO::print(double number) {
    rishka_sc_1(RISHKA_SC_IO_PRINTD, double_to_long(number));
}

rune IO::readch() {
    return (rune) rishka_sc_0(RISHKA_SC_IO_READCH);
}

string IO::readline() {
    return get_rt_string((u32) rishka_sc_0(RISHKA_SC_IO_READLINE));
}

i32 IO::read() {
    return (i32) rishka_sc_0(RISHKA_SC_IO_READ);
}

i32 IO::available() {
    return (i32) rishka_sc_0(RISHKA_SC_IO_AVAILABLE);
}

i32 IO::peek() {
    return (i32) rishka_sc_0(RISHKA_SC_IO_PEEK);
}

bool IO::find(string target, usize size) {
    return (bool) rishka_sc_2(RISHKA_SC_IO_FIND, (i64) target, (i64) size);
}

bool IO::find_until(string target, string terminator) {
    return (bool) rishka_sc_2(RISHKA_SC_IO_FIND_UNTIL, (i64) target, (i64) terminator);
}

void IO::set_timeout(u64 timeout) {
    rishka_sc_1(RISHKA_SC_IO_SET_TIMEOUT, (i64) timeout);
}

u64 IO::get_timeout() {
    return (u64) rishka_sc_0(RISHKA_SC_IO_GET_TIMEOUT);
}

void Sys::delay(u64 ms) {
    rishka_sc_1(RISHKA_SC_SYS_DELAY_MS, (long) ms);
}

u64 Sys::micros() {
    return (u64) rishka_sc_0(RISHKA_SC_SYS_MICROS);
}

u64 Sys::millis() {
    return (u64) rishka_sc_0(RISHKA_SC_SYS_MILLIS);
}

i64 Sys::shellexec(string program, i32 argc, string* argv) {
    return (i64) rishka_sc_3(RISHKA_SC_SYS_SHELLEXEC, (i64) program, (i64) argc, (i64) argv);
}

void Sys::exit(i32 code) {
    rishka_sc_1(RISHKA_SC_SYS_EXIT, (i64) code);
}

string Sys::info_str(sysinfos_t key) {
    u32 len = rishka_sc_1(RISHKA_SC_SYS_INFOS, (i64) key);
    return get_rt_string(len);
}

i64 Sys::info_num(sysinfon_t key) {
    return (i64) rishka_sc_1(RISHKA_SC_SYS_INFON, (i64) key);
}

i64 Sys::random() {
    return (i64) rishka_sc_0(RISHKA_SC_SYS_RANDOM);
}

bool Sys::change_dir(char* directory) {
    return (bool) rishka_sc_1(RISHKA_SC_SYS_CD, (i64) directory);
}

string Sys::working_dir() {
    u32 len = rishka_sc_0(RISHKA_SC_SYS_WD);
    return get_rt_string(len);
}

void Memory::alloc(any dest, usize size) {
    rishka_sc_2(RISHKA_SC_MEM_ALLOC, (i64) dest, (i64) size);
}

void Memory::calloc(any dest, usize num, usize size) {
    rishka_sc_3(RISHKA_SC_MEM_CALLOC, (i64) dest, (i64) num, (i64) size);
}

void Memory::realloc(any dest, any ptr, usize size) {
    rishka_sc_3(RISHKA_SC_MEM_REALLOC, (i64) dest, (i64) ptr, (i64) size);
}

void Memory::free(any ptr) {
    rishka_sc_1(RISHKA_SC_MEM_FREE, (i64) ptr);
}

any Memory::set(any dest, i32 c, u32 n) {
    return (any) rishka_sc_3(RISHKA_SC_MEM_SET, (i64) dest, (i64) c, (i64) n);
}

void Gpio::pin_mode(u8 pin, gpio_pin_mode_t mode) {
    rishka_sc_2(RISHKA_SC_GPIO_PIN_MODE, (i64) pin, (i64) mode);
}

gpio_mode_t Gpio::digital_read(u8 pin) {
    return (gpio_mode_t) rishka_sc_1(RISHKA_SC_GPIO_DIGITAL_READ, (i64) pin);
}

void Gpio::digital_write(u8 pin, gpio_mode_t mode) {
    rishka_sc_2(RISHKA_SC_GPIO_DIGITAL_WRITE, (i64) pin, (i64) mode);
}

u16 Gpio::analog_read(u8 pin) {
    return (i32) rishka_sc_1(RISHKA_SC_GPIO_ANALOG_READ, (i64) pin);
}

void Gpio::analog_write(u8 pin, u16 value) {
    rishka_sc_2(RISHKA_SC_GPIO_ANALOG_WRITE, (i64) pin, (i64) value);
}

u64 Gpio::pulse_in(u8 pin, u8 state, u64 timeout) {
    return (u64) rishka_sc_3(RISHKA_SC_GPIO_PULSE_IN, (i64) pin, (i64) state, (i64) timeout);
}

u64 Gpio::pulse_in_long(u8 pin, u8 state, u64 timeout) {
    return (u64) rishka_sc_3(RISHKA_SC_GPIO_PULSE_IN_LONG, (i64) pin, (i64) state, (i64) timeout);
}

u8 Gpio::shift_in(u8 data, u8 clock, u8 bit_order) {
    return (u8) rishka_sc_3(RISHKA_SC_GPIO_SHIFT_IN, (i64) data, (i64) clock, (i64) bit_order);
}

void Gpio::shift_out(u8 data, u8 clock, u8 bit_order, u8 value) {
    rishka_sc_4(RISHKA_SC_GPIO_SHIFT_OUT, (i64) data, (i64) clock, (i64) bit_order, (i64) value);
}

void Gpio::tone(u32 frequency, u64 duration) {
    rishka_sc_2(RISHKA_SC_GPIO_TONE, (i64) frequency, (i64) duration);
}

void Gpio::no_tone() {
    rishka_sc_0(RISHKA_SC_GPIO_NO_TONE);
}

void Int::enable() {
    rishka_sc_0(RISHKA_SC_INT_ENABLE);
}

void Int::disable() {
    rishka_sc_0(RISHKA_SC_INT_DISABLE);
}

void Int::attach(u8 pin, void (*callback)(void), int_mode_t mode) {
    rishka_sc_3(RISHKA_SC_INT_ATTACH, (i64) pin, (i64) callback, (i64) mode);
}

void Int::detach(u8 pin) {
    rishka_sc_1(RISHKA_SC_INT_DETACH, (i64) pin);
}

File::File(i32 _handle) {
    this->handle = _handle;
}

File File::open(string file, string mode) {
    return File((i32) rishka_sc_2(RISHKA_SC_FS_OPEN, (i64) file, (i64) mode));
}

bool File::is_file() {
    return (bool) rishka_sc_1(RISHKA_SC_FS_ISFILE, (i64) this->handle);
}

bool File::is_dir() {
    return (bool) rishka_sc_1(RISHKA_SC_FS_ISDIR, (i64) this->handle);
}

i32 File::available() {
    return (i32) rishka_sc_1(RISHKA_SC_FS_AVAILABLE, (i64) this->handle);
}

i32 File::peek() {
    return (i32) rishka_sc_1(RISHKA_SC_FS_PEEK, (i64) this->handle);
}

bool File::seek(u32 pos) {
    return (bool) rishka_sc_2(RISHKA_SC_FS_SEEK, (i64) this->handle, (i64) pos);
}

usize File::size() {
    return (usize) rishka_sc_1(RISHKA_SC_FS_SIZE, (i64) this->handle);
}

usize File::position() {
    return (usize) rishka_sc_1(RISHKA_SC_FS_POS, (i64) this->handle);
}

i32 File::read() {
    return (i32) rishka_sc_1(RISHKA_SC_FS_READ, (i64) this->handle);
}

void File::write(u8 data) {
    rishka_sc_2(RISHKA_SC_FS_WRITEB, (i64) this->handle, (i64) data);
}

void File::write(string data) {
    rishka_sc_2(RISHKA_SC_FS_WRITES, (i64) this->handle, (i64) data);
}

string File::path() {
    return get_rt_string(rishka_sc_1(RISHKA_SC_FS_PATH, (i64) this->handle));
}

string File::name() {
    return get_rt_string(rishka_sc_1(RISHKA_SC_FS_NAME, (i64) this->handle));
}

File File::next() {
    return File(rishka_sc_1(RISHKA_SC_FS_NEXT, (i64) this->handle));
}

bool File::is_ok() {
    return rishka_sc_1(RISHKA_SC_FS_IS_OK, (i64) this->handle);
}

void File::flush() {
    rishka_sc_1(RISHKA_SC_FS_FLUSH, (i64) this->handle);
}

void File::close() {
    rishka_sc_1(RISHKA_SC_FS_CLOSE, (i64) this->handle);
}

bool File::bufsize(usize size) {
    return (bool) rishka_sc_2(RISHKA_SC_FS_BUFSIZE, (i64) this->handle, (i64) size);
}

u64 File::lastwrite() {
    return (u64) rishka_sc_1(RISHKA_SC_FS_LASTWRITE, (i64) this->handle);
}

bool File::seek_dir(u64 position) {
    return (bool) rishka_sc_2(RISHKA_SC_FS_SEEKDIR, (i64) this->handle, (i64) position);
}

string File::next_name() {
    return get_rt_string(rishka_sc_1(RISHKA_SC_FS_NEXT_NAME, (i64) this->handle));
}

void File::rewind() {
    rishka_sc_1(RISHKA_SC_FS_REWIND, (i64) this->handle);
}

bool FS::mkdir(const char* path) {
    return (bool) rishka_sc_1(RISHKA_SC_FS_MKDIR, (i64) path);
}

bool FS::rmdir(const char* path) {
    return (bool) rishka_sc_1(RISHKA_SC_FS_RMDIR, (i64) path);
}

bool FS::remove(const char* path) {
    return (bool) rishka_sc_1(RISHKA_SC_FS_DELETE, (i64) path);
}

bool FS::exists(const char* path) {
    return (bool) rishka_sc_1(RISHKA_SC_FS_EXISTS, (i64) path);
}

u8 Args::count() {
    return (u32) rishka_sc_0(RISHKA_SC_ARG_COUNT);
}

string Args::value(u8 index) {
    return get_rt_string(rishka_sc_1(RISHKA_SC_ARG_STR, (i64) index));
}

bool I2C::begin(u8 address) {
    return (bool) rishka_sc_1(RISHKA_SC_I2C_BEGIN, (i64) address);
}

bool I2C::end() {
    return (bool) rishka_sc_0(RISHKA_SC_I2C_END);
}

bool I2C::pins(u8 sda, u8 scl) {
    return (bool) rishka_sc_2(RISHKA_SC_I2C_PINS, (i64) sda, (i64) scl);
}

void I2C::flush() {
    rishka_sc_0(RISHKA_SC_I2C_FLUSH);
}

void I2C::begin_transmission(u8 address) {
    rishka_sc_1(RISHKA_SC_I2C_BEGIN_TRANSMISSION, (i64) address);
}

u8 I2C::end_transmission(bool stop_bit) {
    return (u8) rishka_sc_1(RISHKA_SC_I2C_END_TRANSMISSION, (i64) stop_bit);
}

usize I2C::write(u8* data, usize size) {
    return (usize) rishka_sc_2(RISHKA_SC_I2C_WRITE, (i64) data, (usize) size);
}

usize I2C::slave_write(u8* data, usize size) {
    return (usize) rishka_sc_2(RISHKA_SC_I2C_SLAVE_WRITE, (i64) data, (usize) size);
}

usize I2C::set_buffersize(usize size) {
    return (usize) rishka_sc_1(RISHKA_SC_I2C_BUFSIZE, (i64) size);
}

i32 I2C::read() {
    return (i32) rishka_sc_0(RISHKA_SC_I2C_READ);
}

i32 I2C::peek() {
    return (i32) rishka_sc_0(RISHKA_SC_I2C_PEEK);
}

i32 I2C::available() {
    return (i32) rishka_sc_0(RISHKA_SC_I2C_AVAILABLE);
}

usize I2C::request(u8 address, usize size, bool stop_bit) {
    return (usize) rishka_sc_3(RISHKA_SC_I2C_REQUEST, (i64) address, (i64) size, (i64) stop_bit);
}

void I2C::on_receive(void (*callback)(int)) {
    rishka_sc_1(RISHKA_SC_I2C_ON_RECEIVE, (i64) callback);
}

void I2C::on_request(void (*callback)(void)) {
    rishka_sc_1(RISHKA_SC_I2C_ON_RECEIVE, (i64) callback);
}

void I2C::set_timeout(u16 timeout) {
    rishka_sc_1(RISHKA_SC_I2C_SET_TIMEOUT, (i64) timeout);
}

u16 I2C::get_timeout() {
    return (u16) rishka_sc_0(RISHKA_SC_I2C_GET_TIMEOUT);
}

bool I2C::set_clock(u32 clock) {
    return (bool) rishka_sc_1(RISHKA_SC_I2C_SET_CLOCK, (i64) clock);
}

u32 I2C::get_clock() {
    return (u32) rishka_sc_0(RISHKA_SC_I2C_GET_CLOCK);
}

void SPI::begin(u8 sck, u8 miso, u8 mosi, u8 ss) {
    rishka_sc_4(RISHKA_SC_SPI_BEGIN, (i64) sck, (i64) miso, (i64) mosi, (i64) ss);
}

void SPI::end() {
    rishka_sc_0(RISHKA_SC_SPI_END);
}

void SPI::begin_transaction(u8 clock, u8 bit_order, spi_mode_t data_mode) {
    rishka_sc_3(RISHKA_SC_SPI_BEGIN_TRANSACTION, (i64) clock, (i64) bit_order, (i64) data_mode);
}

void SPI::end_transaction() {
    rishka_sc_0(RISHKA_SC_SPI_END_TRANSACTION);
}

u8 SPI::transfer8(u8 data) {
    return (u8) rishka_sc_1(RISHKA_SC_SPI_TRANSFER8, (i64) data);
}

u16 SPI::transfer16(u16 data) {
    return (u16) rishka_sc_1(RISHKA_SC_SPI_TRANSFER16, (i64) data);
}

u32 SPI::transfer32(u32 data) {
    return (u32) rishka_sc_1(RISHKA_SC_SPI_TRANSFER32, (i64) data);
}

void SPI::transfer_bytes(u8* data, u8* out, u32 size) {
    rishka_sc_3(RISHKA_SC_SPI_TRANSFER_BYTES, (i64) data, (i64) out, (i64) size);
}

void SPI::transfer_bits(u32 data, u32* out, u8 bits) {
    rishka_sc_3(RISHKA_SC_SPI_TRANSFER_BITS, (i64) data, (i64) out, (i64) bits);
}

void SPI::set_hwcs(bool use) {
    rishka_sc_1(RISHKA_SC_SPI_SET_HWCS, (i64) use);
}

void SPI::set_bit_order(u8 bit_order) {
    rishka_sc_1(RISHKA_SC_SPI_SET_BIT_ORDER, (i64) bit_order);
}

void SPI::set_data_mode(spi_mode_t data_mode) {
    rishka_sc_1(RISHKA_SC_SPI_SET_DATA_MODE, (i64) data_mode);
}

void SPI::set_frequency(u32 frequency) {
    rishka_sc_1(RISHKA_SC_SPI_SET_FREQ, (i64) frequency);
}

void SPI::set_clock_div(u32 clock_div) {
    rishka_sc_1(RISHKA_SC_SPI_SET_CLOCK_DIV, (i64) clock_div);
}

u32 SPI::get_clock_div() {
    return (u32) rishka_sc_0(RISHKA_SC_SPI_GET_CLOCK_DIV);
}

void SPI::write8(u8 data) {
    rishka_sc_1(RISHKA_SC_SPI_WRITE8, (i64) data);
}

void SPI::write16(u16 data) {
    rishka_sc_1(RISHKA_SC_SPI_WRITE16, (i64) data);
}

void SPI::write32(u32 data) {
    rishka_sc_1(RISHKA_SC_SPI_WRITE32, (i64) data);
}

void SPI::write_bytes(u8* data, u32 size) {
    rishka_sc_2(RISHKA_SC_SPI_WRITE_BYTES, (i64) data, (i64) size);
}

void SPI::write_pixels(void* data, u32 size) {
    rishka_sc_2(RISHKA_SC_SPI_WRITE_PIXELS, (i64) data, (i64) size);
}

void SPI::write_pattern(u8* data, u8 size, u32 pattern) {
    rishka_sc_3(RISHKA_SC_SPI_WRITE_PATTERN, (i64) data, (i64) size, (i64) pattern);
}

void Runtime::yield() {
    rishka_sc_0(RISHKA_SC_RT_YIELD);
}
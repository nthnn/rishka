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

#include <bootloader_random.h>
#include <Esp.h>
#include <SD.h>
#include <Wire.h>

#include <rishka_syscalls.h>
#include <rishka_types.h>
#include <rishka_util.h>
#include <rishka_vm.h>

#ifdef __cplusplus
    extern "C" {
#endif

uint8_t temprature_sens_read();

#ifdef __cplusplus
    }
#endif

enum rishka_espinfo_n {
    RISHKA_ESPINFO_CHIPCORES,
    RISHKA_ESPINFO_CHIP_REV,
    RISHKA_ESPINFO_CPU_FREQ,
    RISHKA_ESPINFO_CYCLE_COUNT,
    RISHKA_ESPINFO_EFUSE_MAC,
    RISHKA_ESPINFO_FLASH_MODE,
    RISHKA_ESPINFO_FLASH_SPEED,
    RISHKA_ESPINFO_FREE_HEAP,
    RISHKA_ESPINFO_FREE_PSRAM,
    RISHKA_ESPINFO_HEAP_SIZE,
    RISHKA_ESPINFO_MAX_ALLOC_HEAP,
    RISHKA_ESPINFO_MIN_FREE_HEAP,
    RISHKA_ESPINFO_MIN_FREE_PSRAM,
    RISHKA_ESPINFO_PSRAM_SIZE,
    RISHKA_ESPINFO_TEMP_VAL,
    RISHKA_ESPINFO_CARD_TYPE,
    RISHKA_ESPINFO_CARD_SIZE,
    RISHKA_ESPINFO_NUM_SECTORS,
    RISHKA_ESPINFO_SECTOR_SIZE,
    RISHKA_ESPINFO_TOTAL_STORAGE,
    RISHKA_ESPINFO_USED_STORAGE
};

enum rishka_espinfo_s {
    RISHKA_ESPINFO_CHIPMODEL,
    RISHKA_ESPINFO_SDK_VERSION,
    RISHKA_ESPINFO_SKETCH_MD5
};

String strpass_data;
int strpass_idx = 0;

inline void change_rt_strpass(char* data) {
    strpass_data = data;
    strpass_idx = 0;
}

void rishka_syscall_io_prints(RishkaVM* vm) {
    char* arg = vm->getPointerParam<char*>(0);
    vm->getStream()->print(arg != NULL ? arg : "(null)");
}

void rishka_syscall_io_printn(RishkaVM* vm) {
    int64_t arg = vm->getParam<int64_t>(0);
    vm->getStream()->print(arg);
}

void rishka_syscall_io_printd(RishkaVM* vm) {
    double arg = vm->getParam<double>(0);
    vm->getStream()->print(arg);
}

char rishka_syscall_io_readch(RishkaVM* vm) {
    return (char) vm->getStream()->read();
}

size_t rishka_syscall_io_readline(RishkaVM* vm) {
    char* input = (char*) vm->getStream()->readString().c_str();
    change_rt_strpass(input);

    return strlen(input);
}

int rishka_syscall_io_read(RishkaVM* vm) {
    return vm->getStream()->read();
}

int rishka_syscall_io_available(RishkaVM* vm) {
    return vm->getStream()->available();
}

int rishka_syscall_io_peek(RishkaVM* vm) {
    return vm->getStream()->peek();
}

bool rishka_syscall_io_find(RishkaVM* vm) {
    char* target = vm->getPointerParam<char*>(0);
    size_t length = vm->getParam<size_t>(1);

    return vm->getStream()->find(target, length);
}

bool rishka_syscall_io_find_until(RishkaVM* vm) {
    char* target = vm->getPointerParam<char*>(0);
    char* terminator = vm->getPointerParam<char*>(1);

    return vm->getStream()->findUntil(target, terminator);
}

void rishka_syscall_io_set_timeout(RishkaVM* vm) {
    uint64_t timeout = vm->getParam<uint64_t>(0);
    vm->getStream()->setTimeout(timeout);
}

uint64_t rishka_syscall_io_get_timeout(RishkaVM* vm) {
    return vm->getStream()->getTimeout();
}

void rishka_syscall_sys_delay(RishkaVM* vm) {
    uint64_t ms = vm->getParam<uint64_t>(0);
    delay(ms);
}

unsigned long rishka_syscall_sys_micros() {
    return micros();
}

unsigned long rishka_syscall_sys_millis() {
    return millis();
}

int64_t rishka_syscall_sys_shellexec(RishkaVM* parent_vm) {
    RishkaVM* child_vm = new RishkaVM();

    char* program = parent_vm->getPointerParam<char*>(0);
    int argc = parent_vm->getParam<int>(1);
    char** argv = parent_vm->getPointerParam<char**>(2);

    child_vm->initialize(parent_vm->getStream());
    if(!child_vm->loadFile(program)) {
        child_vm->reset();
        delete child_vm;

        return -1;
    }

    child_vm->run(argc, argv);
    child_vm->reset();

    int64_t exitCode = child_vm->getExitCode();
    delete child_vm;

    return exitCode;
}

void rishka_syscall_sys_exit(RishkaVM* vm) {
    int code = vm->getParam<int>(0);

    vm->stopVM();
    vm->setExitCode(code);
}

uint32_t rishka_syscall_sys_infos(RishkaVM* vm) {
    uint8_t key = vm->getParam<uint8_t>(0);

    char* data;
    switch(key) {
        case RISHKA_ESPINFO_CHIPMODEL:
            data = (char*) ESP.getChipModel();
            break;

        case RISHKA_ESPINFO_SDK_VERSION:
            data = (char*) ESP.getSdkVersion();
            break;

        case RISHKA_ESPINFO_SKETCH_MD5:
            data = (char*) ESP.getSketchMD5().c_str();
            break;

        default:
            data = (char*) "";
            break;
    }

    change_rt_strpass(data);
    return strlen(data);
}

long rishka_syscall_sys_infon(RishkaVM* vm) {
    uint8_t key = vm->getParam<uint8_t>(0);

    switch(key) {
        case RISHKA_ESPINFO_CHIPCORES:
            return ESP.getChipCores();

        case RISHKA_ESPINFO_CHIP_REV:
            return ESP.getChipRevision();

        case RISHKA_ESPINFO_CPU_FREQ:
            return ESP.getCpuFreqMHz();

        case RISHKA_ESPINFO_CYCLE_COUNT:
            return ESP.getCycleCount();

        case RISHKA_ESPINFO_EFUSE_MAC:
            return ESP.getEfuseMac();

        case RISHKA_ESPINFO_FLASH_MODE:
            return ESP.getFlashChipMode();

        case RISHKA_ESPINFO_FLASH_SPEED:
            return ESP.getFlashChipSpeed();

        case RISHKA_ESPINFO_FREE_HEAP:
            return ESP.getFreeHeap();

        case RISHKA_ESPINFO_FREE_PSRAM:
            return ESP.getFreePsram();

        case RISHKA_ESPINFO_HEAP_SIZE:
            return ESP.getHeapSize();

        case RISHKA_ESPINFO_MAX_ALLOC_HEAP:
            return ESP.getMaxAllocHeap();

        case RISHKA_ESPINFO_MIN_FREE_HEAP:
            return ESP.getMinFreeHeap();

        case RISHKA_ESPINFO_MIN_FREE_PSRAM:
            return ESP.getMinFreePsram();

        case RISHKA_ESPINFO_PSRAM_SIZE:
            return ESP.getPsramSize();

        case RISHKA_ESPINFO_TEMP_VAL:
            return temprature_sens_read();

        case RISHKA_ESPINFO_CARD_TYPE:
            return SD.cardType();

        case RISHKA_ESPINFO_CARD_SIZE:
            return SD.cardSize();

        case RISHKA_ESPINFO_NUM_SECTORS:
            return SD.numSectors();

        case RISHKA_ESPINFO_SECTOR_SIZE:
            return SD.sectorSize();

        case RISHKA_ESPINFO_TOTAL_STORAGE:
            return SD.totalBytes();

        case RISHKA_ESPINFO_USED_STORAGE:
            return SD.usedBytes();
    }

    return 0;
}

long rishka_syscall_sys_random() {
    long rand_num = 0;

    bootloader_random_enable();
    rand_num = esp_random();
    bootloader_random_disable();

    return rand_num;
}

void rishka_syscall_mem_alloc(RishkaVM* vm) {
    void* dest = vm->getPointerParam<void*>(0);
    size_t size = vm->getParam<size_t>(1);

    dest = ps_malloc(size);
}

void rishka_syscall_mem_calloc(RishkaVM* vm) {
    void* dest = vm->getPointerParam<void*>(0);
    size_t num = vm->getParam<size_t>(1);
    size_t size = vm->getParam<size_t>(2);

    dest = ps_calloc(num, size);
}

void rishka_syscall_mem_realloc(RishkaVM* vm) {
    void* dest = vm->getPointerParam<void*>(0);
    void* ptr = vm->getPointerParam<void*>(1);
    size_t size = vm->getParam<size_t>(2);

    dest = ps_realloc(ptr, size);
}

void rishka_syscall_mem_free(RishkaVM* vm) {
    void* ptr = vm->getPointerParam<void*>(0);
    free(ptr);
}

void* rishka_syscall_mem_set(RishkaVM* vm) {
    void* dest = vm->getPointerParam<void*>(0);
    uint64_t val = vm->getParam<uint64_t>(1);
    uint64_t len = vm->getParam<uint64_t>(2);

    return memset(dest, (int) val, (size_t) len);
}

void rishka_syscall_gpio_pinmode(RishkaVM* vm) {
    uint8_t pin = vm->getParam<uint8_t>(0);
    uint8_t mode = vm->getParam<uint8_t>(1);

    pinMode(pin, mode);
}

bool rishka_syscall_gpio_digitalread(RishkaVM* vm) {
    uint8_t pin = vm->getParam<uint8_t>(0);
    return digitalRead(pin);
}

void rishka_syscall_gpio_digitalwrite(RishkaVM* vm) {
    uint8_t pin = vm->getParam<uint8_t>(0);
    bool value = vm->getParam<bool>(1);

    digitalWrite(pin, value);
}

uint16_t rishka_syscall_gpio_analogread(RishkaVM* vm) {
    uint8_t pin = vm->getParam<uint8_t>(0);
    return analogRead(pin);
}

void rishka_syscall_gpio_analogwrite(RishkaVM* vm) {
    uint8_t pin = vm->getParam<uint8_t>(0);
    uint16_t value = vm->getParam<uint16_t>(1);

    analogWrite(pin, value);
}

uint64_t rishka_syscall_gpio_pulse_in(RishkaVM* vm) {
    uint8_t pin = vm->getParam<uint8_t>(0);
    uint8_t state = vm->getParam<uint8_t>(1);
    uint64_t timeout = vm->getParam<uint64_t>(2);

    return pulseIn(pin, state, timeout);
}

uint64_t rishka_syscall_gpio_pulse_in_long(RishkaVM* vm) {
    uint8_t pin = vm->getParam<uint8_t>(0);
    uint8_t state = vm->getParam<uint8_t>(1);
    uint64_t timeout = vm->getParam<uint64_t>(2);

    return pulseInLong(pin, state, timeout);
}

uint8_t rishka_syscall_gpio_shift_in(RishkaVM* vm) {
    uint8_t data = vm->getParam<uint8_t>(0);
    uint8_t clock = vm->getParam<uint8_t>(1);
    uint8_t bit_order = vm->getParam<uint8_t>(2);

    return shiftIn(data, clock, bit_order);
}

void rishka_syscall_gpio_shift_out(RishkaVM* vm) {
    uint8_t data = vm->getParam<uint8_t>(0);
    uint8_t clock = vm->getParam<uint8_t>(1);
    uint8_t bit_order = vm->getParam<uint8_t>(2);
    uint8_t value = vm->getParam<uint8_t>(3);

    shiftOut(data, clock, bit_order, value);
}

void rishka_syscall_gpio_tone(RishkaVM* vm) {
    uint8_t pin = vm->getParam<uint8_t>(0);
    uint32_t frequency = vm->getParam<uint32_t>(1);
    uint64_t duration = vm->getParam<uint64_t>(2);

    tone(pin, frequency, duration);
}

void rishka_syscall_gpio_no_tone(RishkaVM* vm) {
    uint8_t pin = vm->getParam<uint8_t>(0);
    noTone(pin);
}

void rishka_syscall_int_enable() {
    interrupts();
}

void rishka_syscall_int_disable() {
    noInterrupts();
}

void rishka_syscall_int_attach(RishkaVM* vm) {
    uint8_t num = vm->getParam<uint8_t>(0);
    void (*ptr)(void) = vm->getPointerParam<void (*)(void)>(1);
    int mode = vm->getPointerParam<int>(2);

    attachInterrupt(num, ptr, mode);
}

void rishka_syscall_int_detach(RishkaVM* vm) {
    uint8_t num = vm->getParam<uint8_t>(0);
    detachInterrupt(num);
}

bool rishka_syscall_fs_mkdir(RishkaVM* vm) {
    char* path = vm->getPointerParam<char*>(0);
    return SD.mkdir(path);
}

bool rishka_syscall_fs_rmdir(RishkaVM* vm) {
    char* path = vm->getPointerParam<char*>(0);
    return SD.rmdir(path);
}

bool rishka_syscall_fs_delete(RishkaVM* vm) {
    char* path = vm->getPointerParam<char*>(0);
    return SD.remove(path);
}

bool rishka_syscall_fs_exists(RishkaVM* vm) {
    char* path = vm->getPointerParam<char*>(0);
    return SD.exists(path);
}

bool rishka_syscall_fs_isfile(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    return !vm->fileHandles[handle].isDirectory();
}

bool rishka_syscall_fs_isdir(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    return vm->fileHandles[handle].isDirectory();
}

uint8_t rishka_syscall_fs_open(RishkaVM* vm) {
    char* path = vm->getPointerParam<char*>(0);
    char* mode = vm->getPointerParam<char*>(1);

    vm->fileHandles.add(SD.open(path, mode));
    return vm->fileHandles.getSize() - 1;
}

void rishka_syscall_fs_close(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);

    vm->fileHandles[handle].close();
    vm->fileHandles.remove(handle);
}

int rishka_syscall_fs_available(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    return vm->fileHandles[handle].available();
}

void rishka_syscall_fs_flush(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    vm->fileHandles[handle].flush();
}

int rishka_syscall_fs_peek(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    return vm->fileHandles[handle].peek();
}

bool rishka_syscall_fs_seek(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    uint32_t pos = vm->getParam<uint32_t>(1);

    return vm->fileHandles[handle].seek(pos);
}

uint32_t rishka_syscall_fs_size(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    return vm->fileHandles[handle].size();
}

int rishka_syscall_fs_read(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    return vm->fileHandles[handle].read();
}

size_t rishka_syscall_fs_writeb(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    uint8_t data = vm->getParam<uint8_t>(1);

    return vm->fileHandles[handle].write(data);
}

size_t rishka_syscall_fs_writes(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    char* data = vm->getPointerParam<char*>(1);

    return vm->fileHandles[handle].print(data);
}

size_t rishka_syscall_fs_position(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    return vm->fileHandles[handle].position();
}

uint32_t rishka_syscall_fs_path(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    char* path = (char*) vm->fileHandles[handle].path();

    change_rt_strpass(path);
    return strlen(path);
}

uint32_t rishka_syscall_fs_name(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    char* name = (char*) vm->fileHandles[handle].name();

    change_rt_strpass(name);
    return strlen(name);
}

uint8_t rishka_syscall_fs_next(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    char* mode = vm->getPointerParam<char*>(1);

    File file = vm->fileHandles[handle].openNextFile(mode);
    vm->fileHandles.add(file);

    return vm->fileHandles.getSize();
}

bool rishka_syscall_fs_bufsize(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    size_t size = vm->getParam<size_t>(1);

    return vm->fileHandles[handle].setBufferSize(size);
}

uint64_t rishka_syscall_fs_lastwrite(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    return (uint64_t) vm->fileHandles[handle].getLastWrite();
}

bool rishka_syscall_fs_seekdir(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    uint64_t position = vm->getParam<uint64_t>(1);

    return vm->fileHandles[handle].seekDir(position);
}

uint32_t rishka_syscall_fs_next_name(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    char* name = vm->getPointerParam<char*>(1);

    change_rt_strpass(name);
    return strlen(name);
}

void rishka_syscall_fs_rewind(RishkaVM* vm) {
    uint8_t handle = vm->getParam<uint8_t>(0);
    vm->fileHandles[handle].rewindDirectory();
}

uint8_t rishka_syscall_arg_count(RishkaVM* vm) {
    return vm->getArgCount();
}

uint32_t rishka_syscall_arg_value(RishkaVM* vm) {
    uint8_t index = vm->getParam<uint8_t>(0);
    char* argv = vm->getArgValue(index);

    change_rt_strpass(argv);
    return strlen(argv);
}

bool rishka_syscall_i2c_begin(RishkaVM* vm) {
    uint8_t address = vm->getParam<uint8_t>(0);
    return Wire.begin(address);
}

bool rishka_syscall_i2c_end(RishkaVM* vm) {
    return Wire.end();
}

void rishka_syscall_i2c_begin_transmission(RishkaVM* vm) {
    uint8_t address = vm->getParam<uint8_t>(0);
    return Wire.beginTransmission(address);
}

uint8_t rishka_syscall_i2c_end_transmission(RishkaVM* vm) {
    bool stop_bit = vm->getParam<bool>(0);
    return Wire.endTransmission(stop_bit);
}

size_t rishka_syscall_i2c_write(RishkaVM* vm) {
    uint8_t* data = vm->getPointerParam<uint8_t*>(0);
    size_t size = vm->getParam<size_t>(1);

    return Wire.write(data, size);
}

size_t rishka_syscall_i2c_slave_write(RishkaVM* vm) {
    uint8_t* data = vm->getPointerParam<uint8_t*>(0);
    size_t size = vm->getParam<size_t>(1);

    return Wire.slaveWrite(data, size);
}

int rishka_syscall_i2c_read(RishkaVM* vm) {
    return Wire.read();
}

int rishka_syscall_i2c_peek(RishkaVM* vm) {
    return Wire.peek();
}

size_t rishka_syscall_i2c_request(RishkaVM* vm) {
    uint8_t address = vm->getParam<uint8_t>(0);
    size_t size = vm->getParam<size_t>(1);
    bool stop_bit = vm->getParam<bool>(2);

    return Wire.requestFrom(address, size, stop_bit);
}

int rishka_syscall_i2c_available(RishkaVM* vm) {
    return Wire.available();
}

void rishka_syscall_i2c_flush() {
    Wire.flush();
}

void rishka_syscall_i2c_on_receive(RishkaVM* vm) {
    void(*callback)(int) = vm->getPointerParam<void(*)(int)>(0);
    Wire.onReceive(callback);
}

void rishka_syscall_i2c_on_request(RishkaVM* vm) {
    void(*callback)(void) = vm->getPointerParam<void(*)(void)>(0);
    Wire.onRequest(callback);
}

uint16_t rishka_syscall_i2c_get_timeout() {
    return Wire.getTimeout();
}

void rishka_syscall_i2c_set_timeout(RishkaVM* vm) {
    uint16_t timeout = vm->getParam<uint16_t>(0);
    Wire.setTimeout(timeout);
}

bool rishka_syscall_i2c_set_clock(RishkaVM* vm) {
    uint32_t clock = vm->getParam<uint32_t>(0);
    return Wire.setClock(clock);
}

uint32_t rishka_syscall_i2c_get_clock() {
    return Wire.getClock();
}

bool rishka_syscall_i2c_pins(RishkaVM* vm) {
    uint8_t sda = vm->getParam<uint8_t>(0);
    uint8_t scl = vm->getParam<uint8_t>(1);

    return Wire.setPins(sda, scl);
}

size_t rishka_syscall_i2c_bufsize(RishkaVM* vm) {
    size_t size = vm->getParam<size_t>(0);
    return Wire.setBufferSize(size);
}

void rishka_syscall_spi_begin(RishkaVM* vm) {
    uint8_t sck = vm->getParam<uint8_t>(0);
    uint8_t miso = vm->getParam<uint8_t>(1);
    uint8_t mosi = vm->getParam<uint8_t>(2);
    uint8_t ss = vm->getParam<uint8_t>(3);

    SPI.begin(sck, miso, mosi, ss);
}

void rishka_syscall_spi_end() {
    SPI.end();
}

void rishka_syscall_spi_begin_transaction(RishkaVM* vm) {
    uint8_t clock = vm->getParam<uint8_t>(0);
    uint8_t bit_order = vm->getParam<uint8_t>(1);
    uint8_t data_mode = vm->getParam<uint8_t>(2);

    SPI.beginTransaction(SPISettings(clock, bit_order, data_mode));
}

void rishka_syscall_spi_end_transaction() {
    SPI.endTransaction();
}

uint8_t rishka_syscall_spi_transfer8(RishkaVM* vm) {
    uint8_t data = vm->getParam<uint8_t>(0);
    return SPI.transfer(data);
}

uint16_t rishka_syscall_spi_transfer16(RishkaVM* vm) {
    uint16_t data = vm->getParam<uint16_t>(0);
    return SPI.transfer16(data);
}

uint32_t rishka_syscall_spi_transfer32(RishkaVM* vm) {
    uint32_t data = vm->getParam<uint32_t>(0);
    return SPI.transfer32(data);
}

void rishka_syscall_spi_transfer_bytes(RishkaVM* vm) {
    uint8_t* data = vm->getPointerParam<uint8_t*>(0);
    uint8_t* out = vm->getPointerParam<uint8_t*>(1);
    uint32_t size = vm->getParam<uint32_t>(2);

    SPI.transferBytes(data, out, size);
}

void rishka_syscall_spi_transfer_bits(RishkaVM* vm) {
    uint32_t data = vm->getParam<uint32_t>(0);
    uint32_t* out = vm->getPointerParam<uint32_t*>(1);
    uint8_t bits = vm->getParam<uint8_t>(2);

    SPI.transferBits(data, out, bits);
}

void rishka_syscall_spi_set_hwcs(RishkaVM* vm) {
    bool use = vm->getParam<bool>(0);
    SPI.setHwCs(use);
}

void rishka_syscall_spi_set_bit_order(RishkaVM* vm) {
    uint8_t bit_order = vm->getParam<uint8_t>(0);
    SPI.setBitOrder(bit_order);
}

void rishka_syscall_spi_set_data_mode(RishkaVM* vm) {
    uint8_t data_mode = vm->getParam<uint8_t>(0);
    SPI.setDataMode(data_mode);
}

void rishka_syscall_spi_set_frequency(RishkaVM* vm) {
    uint32_t frequency = vm->getParam<uint32_t>(0);
    SPI.setFrequency(frequency);
}

void rishka_syscall_spi_set_clock_div(RishkaVM* vm) {
    uint32_t clock_div = vm->getParam<uint32_t>(0);
    SPI.setClockDivider(clock_div);
}

uint32_t rishka_syscall_spi_get_clock_div() {
    return SPI.getClockDivider();
}

void rishka_syscall_spi_write8(RishkaVM* vm) {
    uint8_t data = vm->getParam<uint8_t>(0);
    SPI.write(data);
}

void rishka_syscall_spi_write16(RishkaVM* vm) {
    uint16_t data = vm->getParam<uint16_t>(0);
    SPI.write16(data);
}

void rishka_syscall_spi_write32(RishkaVM* vm) {
    uint32_t data = vm->getParam<uint32_t>(0);
    SPI.write32(data);
}

void rishka_syscall_spi_write_bytes(RishkaVM* vm) {
    uint8_t* data = vm->getPointerParam<uint8_t*>(0);
    uint32_t size = vm->getParam<uint32_t>(1);

    SPI.writeBytes(data, size);
}

void rishka_syscall_spi_write_pixels(RishkaVM* vm) {
    void* data = vm->getPointerParam<void*>(0);
    uint32_t size = vm->getParam<uint32_t>(1);

    SPI.writePixels(data, size);
}

void rishka_syscall_spi_write_pattern(RishkaVM* vm) {
    uint8_t* data = vm->getPointerParam<uint8_t*>(0);
    uint8_t size = vm->getParam<uint8_t>(1);
    uint32_t repeat = vm->getParam<uint32_t>(2);

    SPI.writePattern(data, size, repeat);
}

char rishka_syscall_rt_strpass() {
    return strpass_data.charAt(strpass_idx++);
}
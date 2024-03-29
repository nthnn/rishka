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

extern "C" {
    uint8_t temprature_sens_read();
}

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

static inline String strpass_data;
static inline int strpass_idx = 0;

void change_rt_strpass(char* data) {
    strpass_data = data;
    strpass_idx = 0;
}

void RishkaSyscall::IO::prints(RishkaVM* vm) {
    auto arg = vm->getPointerParam<char*>(0);
    vm->getTerminal()->print(arg != NULL ? arg : "(null)");
}

void RishkaSyscall::IO::printn(RishkaVM* vm) {
    auto arg = vm->getParam<int64_t>(0);
    vm->getTerminal()->print(arg);
}

void RishkaSyscall::IO::printd(RishkaVM* vm) {
    auto arg = vm->getParam<double>(0);
    vm->getTerminal()->print(arg);
}

char RishkaSyscall::IO::readch(RishkaVM* vm) {
    return (char) vm->getTerminal()->read();
}

size_t RishkaSyscall::IO::readLine(RishkaVM* vm) {
    char* input = (char*) vm->getTerminal()->readString().c_str();

    change_rt_strpass(input);
    return strlen(input);
}

int RishkaSyscall::IO::read(RishkaVM* vm) {
    return vm->getTerminal()->read();
}

int RishkaSyscall::IO::available(RishkaVM* vm) {
    return vm->getTerminal()->available();
}

int RishkaSyscall::IO::peek(RishkaVM* vm) {
    return vm->getTerminal()->peek();
}

bool RishkaSyscall::IO::find(RishkaVM* vm) {
    auto target = vm->getPointerParam<char*>(0);
    auto length = vm->getParam<size_t>(1);

    return vm->getTerminal()->find(target, length);
}

bool RishkaSyscall::IO::findUntil(RishkaVM* vm) {
    auto target = vm->getPointerParam<char*>(0);
    auto terminator = vm->getPointerParam<char*>(1);

    return vm->getTerminal()->findUntil(target, terminator);
}

void RishkaSyscall::IO::setTimeout(RishkaVM* vm) {
    auto timeout = vm->getParam<uint64_t>(0);
    vm->getTerminal()->setTimeout(timeout);
}

uint64_t RishkaSyscall::IO::getTimeout(RishkaVM* vm) {
    return vm->getTerminal()->getTimeout();
}

void RishkaSyscall::Sys::delayImpl(RishkaVM* vm) {
    auto ms = vm->getParam<uint64_t>(0);
    delay(ms);
}

unsigned long RishkaSyscall::Sys::microsImpl() {
    return micros();
}

unsigned long RishkaSyscall::Sys::millisImpl() {
    return millis();
}

int64_t RishkaSyscall::Sys::shellExec(RishkaVM* parent_vm) {
    auto program = parent_vm->getPointerParam<char*>(0);
    auto argc = parent_vm->getParam<int>(1);
    auto argv = parent_vm->getPointerParam<char**>(2);

    RishkaVM* child_vm = new RishkaVM();
    child_vm->initialize(parent_vm->getTerminal());

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

void RishkaSyscall::Sys::exit(RishkaVM* vm) {
    auto code = vm->getParam<int>(0);

    vm->stopVM();
    vm->setExitCode(code);
}

uint32_t RishkaSyscall::Sys::infos(RishkaVM* vm) {
    auto key = vm->getParam<uint8_t>(0);

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

long RishkaSyscall::Sys::infon(RishkaVM* vm) {
    auto key = vm->getParam<uint8_t>(0);
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

long RishkaSyscall::Sys::randomImpl() {
    long rand_num = 0;

    bootloader_random_enable();
    rand_num = esp_random();
    bootloader_random_disable();

    return rand_num;
}

void RishkaSyscall::Memory::alloc(RishkaVM* vm) {
    auto dest = vm->getPointerParam<void*>(0);
    auto size = vm->getParam<size_t>(1);

    dest = ps_malloc(size);
}

void RishkaSyscall::Memory::calloc(RishkaVM* vm) {
    auto dest = vm->getPointerParam<void*>(0);
    auto num = vm->getParam<size_t>(1);
    auto size = vm->getParam<size_t>(2);

    dest = ps_calloc(num, size);
}

void RishkaSyscall::Memory::realloc(RishkaVM* vm) {
    auto dest = vm->getPointerParam<void*>(0);
    auto ptr = vm->getPointerParam<void*>(1);
    auto size = vm->getParam<size_t>(2);

    dest = ps_realloc(ptr, size);
}

void RishkaSyscall::Memory::freeHeap(RishkaVM* vm) {
    auto ptr = vm->getPointerParam<void*>(0);
    free(ptr);
}

void* RishkaSyscall::Memory::set(RishkaVM* vm) {
    auto dest = vm->getPointerParam<void*>(0);
    auto val = vm->getParam<uint64_t>(1);
    auto len = vm->getParam<uint64_t>(2);

    return memset(dest, (int) val, (size_t) len);
}

void RishkaSyscall::Gpio::pinModeImpl(RishkaVM* vm) {
    auto pin = vm->getParam<uint8_t>(0);
    auto mode = vm->getParam<uint8_t>(1);

    pinMode(pin, mode);
}

bool RishkaSyscall::Gpio::digitalReadImpl(RishkaVM* vm) {
    auto pin = vm->getParam<uint8_t>(0);
    return digitalRead(pin);
}

void RishkaSyscall::Gpio::digitalWriteImpl(RishkaVM* vm) {
    auto pin = vm->getParam<uint8_t>(0);
    auto value = vm->getParam<bool>(1);

    digitalWrite(pin, value);
}

uint16_t RishkaSyscall::Gpio::analogReadImpl(RishkaVM* vm) {
    auto pin = vm->getParam<uint8_t>(0);
    return analogRead(pin);
}

void RishkaSyscall::Gpio::analogWriteImpl(RishkaVM* vm) {
    auto pin = vm->getParam<uint8_t>(0);
    auto value = vm->getParam<uint16_t>(1);

    analogWrite(pin, value);
}

uint64_t RishkaSyscall::Gpio::pulseInImpl(RishkaVM* vm) {
    auto pin = vm->getParam<uint8_t>(0);
    auto state = vm->getParam<uint8_t>(1);
    auto timeout = vm->getParam<uint64_t>(2);

    return pulseIn(pin, state, timeout);
}

uint64_t RishkaSyscall::Gpio::pulseInLongImpl(RishkaVM* vm) {
    auto pin = vm->getParam<uint8_t>(0);
    auto state = vm->getParam<uint8_t>(1);
    auto timeout = vm->getParam<uint64_t>(2);

    return pulseInLong(pin, state, timeout);
}

uint8_t RishkaSyscall::Gpio::shiftInImpl(RishkaVM* vm) {
    auto data = vm->getParam<uint8_t>(0);
    auto clock = vm->getParam<uint8_t>(1);
    auto bit_order = vm->getParam<uint8_t>(2);

    return shiftIn(data, clock, bit_order);
}

void RishkaSyscall::Gpio::shiftOutImpl(RishkaVM* vm) {
    auto data = vm->getParam<uint8_t>(0);
    auto clock = vm->getParam<uint8_t>(1);
    auto bit_order = vm->getParam<uint8_t>(2);
    auto value = vm->getParam<uint8_t>(3);

    shiftOut(data, clock, bit_order, value);
}

void RishkaSyscall::Gpio::toneImpl(RishkaVM* vm) {
    auto pin = vm->getParam<uint8_t>(0);
    auto frequency = vm->getParam<uint32_t>(1);
    auto duration = vm->getParam<uint64_t>(2);

    tone(pin, frequency, duration);
}

void RishkaSyscall::Gpio::noToneImpl(RishkaVM* vm) {
    auto pin = vm->getParam<uint8_t>(0);
    noTone(pin);
}

void RishkaSyscall::Int::enable() {
    interrupts();
}

void RishkaSyscall::Int::disable() {
    noInterrupts();
}

void RishkaSyscall::Int::attach(RishkaVM* vm) {
    auto num = vm->getParam<uint8_t>(0);
    auto ptr = vm->getPointerParam<void(*)(void)>(1);
    auto mode = vm->getParam<int>(2);

    attachInterrupt(num, ptr, mode);
}

void RishkaSyscall::Int::detach(RishkaVM* vm) {
    auto num = vm->getParam<uint8_t>(0);
    detachInterrupt(num);
}

bool RishkaSyscall::FS::mkdir(RishkaVM* vm) {
    auto path = vm->getPointerParam<char*>(0);
    return SD.mkdir(path);
}

bool RishkaSyscall::FS::rmdir(RishkaVM* vm) {
    auto path = vm->getPointerParam<char*>(0);
    return SD.rmdir(path);
}

bool RishkaSyscall::FS::remove(RishkaVM* vm) {
    auto path = vm->getPointerParam<char*>(0);
    return SD.remove(path);
}

bool RishkaSyscall::FS::exists(RishkaVM* vm) {
    auto path = vm->getPointerParam<char*>(0);
    return SD.exists(path);
}

bool RishkaSyscall::FS::isfile(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    return !vm->fileHandles[handle].isDirectory();
}

bool RishkaSyscall::FS::isdir(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    return vm->fileHandles[handle].isDirectory();
}

uint8_t RishkaSyscall::FS::open(RishkaVM* vm) {
    auto path = vm->getPointerParam<char*>(0);
    auto mode = vm->getPointerParam<char*>(1);

    vm->fileHandles.add(SD.open(path, mode));
    return vm->fileHandles.getSize() - 1;
}

void RishkaSyscall::FS::close(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);

    vm->fileHandles[handle].close();
    vm->fileHandles.remove(handle);
}

int RishkaSyscall::FS::available(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    return vm->fileHandles[handle].available();
}

void RishkaSyscall::FS::flush(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    vm->fileHandles[handle].flush();
}

int RishkaSyscall::FS::peek(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    return vm->fileHandles[handle].peek();
}

bool RishkaSyscall::FS::seek(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    auto pos = vm->getParam<uint32_t>(1);

    return vm->fileHandles[handle].seek(pos);
}

uint32_t RishkaSyscall::FS::size(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    return vm->fileHandles[handle].size();
}

int RishkaSyscall::FS::read(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    return vm->fileHandles[handle].read();
}

size_t RishkaSyscall::FS::writeb(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    auto data = vm->getParam<uint8_t>(1);

    return vm->fileHandles[handle].write(data);
}

size_t RishkaSyscall::FS::writes(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    auto data = vm->getPointerParam<char*>(1);

    return vm->fileHandles[handle].print(data);
}

size_t RishkaSyscall::FS::position(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    return vm->fileHandles[handle].position();
}

uint32_t RishkaSyscall::FS::path(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    char* path = (char*) vm->fileHandles[handle].path();

    change_rt_strpass(path);
    return strlen(path);
}

uint32_t RishkaSyscall::FS::name(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    char* name = (char*) vm->fileHandles[handle].name();

    change_rt_strpass(name);
    return strlen(name);
}

uint8_t RishkaSyscall::FS::next(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    auto mode = vm->getPointerParam<char*>(1);

    File file = vm->fileHandles[handle].openNextFile(mode);
    vm->fileHandles.add(file);

    return vm->fileHandles.getSize();
}

bool RishkaSyscall::FS::bufsize(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    auto size = vm->getParam<size_t>(1);

    return vm->fileHandles[handle].setBufferSize(size);
}

uint64_t RishkaSyscall::FS::lastwrite(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    return (uint64_t) vm->fileHandles[handle].getLastWrite();
}

bool RishkaSyscall::FS::seekdir(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    auto position = vm->getParam<uint64_t>(1);

    return vm->fileHandles[handle].seekDir(position);
}

uint32_t RishkaSyscall::FS::next_name(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    auto name = vm->getPointerParam<char*>(1);

    change_rt_strpass(name);
    return strlen(name);
}

void RishkaSyscall::FS::rewind(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    vm->fileHandles[handle].rewindDirectory();
}

uint8_t RishkaSyscall::Args::count(RishkaVM* vm) {
    return vm->getArgCount();
}

uint32_t RishkaSyscall::Args::value(RishkaVM* vm) {
    auto index = vm->getParam<uint8_t>(0);
    char* argv = vm->getArgValue(index);

    change_rt_strpass(argv);
    return strlen(argv);
}

bool RishkaSyscall::I2C::begin(RishkaVM* vm) {
    auto address = vm->getParam<uint8_t>(0);
    return Wire.begin(address);
}

bool RishkaSyscall::I2C::end(RishkaVM* vm) {
    return Wire.end();
}

void RishkaSyscall::I2C::begin_transmission(RishkaVM* vm) {
    auto address = vm->getParam<uint8_t>(0);
    return Wire.beginTransmission(address);
}

uint8_t RishkaSyscall::I2C::end_transmission(RishkaVM* vm) {
    auto stop_bit = vm->getParam<bool>(0);
    return Wire.endTransmission(stop_bit);
}

size_t RishkaSyscall::I2C::write(RishkaVM* vm) {
    auto data = vm->getPointerParam<uint8_t*>(0);
    auto size = vm->getParam<size_t>(1);

    return Wire.write(data, size);
}

size_t RishkaSyscall::I2C::slave_write(RishkaVM* vm) {
    auto data = vm->getPointerParam<uint8_t*>(0);
    auto size = vm->getParam<size_t>(1);

    return Wire.slaveWrite(data, size);
}

int RishkaSyscall::I2C::read(RishkaVM* vm) {
    return Wire.read();
}

int RishkaSyscall::I2C::peek(RishkaVM* vm) {
    return Wire.peek();
}

size_t RishkaSyscall::I2C::request(RishkaVM* vm) {
    auto address = vm->getParam<uint8_t>(0);
    auto size = vm->getParam<size_t>(1);
    auto stop_bit = vm->getParam<bool>(2);

    return Wire.requestFrom(address, size, stop_bit);
}

int RishkaSyscall::I2C::available(RishkaVM* vm) {
    return Wire.available();
}

void RishkaSyscall::I2C::flush() {
    Wire.flush();
}

void RishkaSyscall::I2C::on_receive(RishkaVM* vm) {
    auto callback = vm->getPointerParam<void(*)(int)>(0);
    Wire.onReceive(callback);
}

void RishkaSyscall::I2C::on_request(RishkaVM* vm) {
    auto callback = vm->getPointerParam<void(*)(void)>(0);
    Wire.onRequest(callback);
}

uint16_t RishkaSyscall::I2C::get_timeout() {
    return Wire.getTimeout();
}

void RishkaSyscall::I2C::set_timeout(RishkaVM* vm) {
    auto timeout = vm->getParam<uint16_t>(0);
    Wire.setTimeout(timeout);
}

bool RishkaSyscall::I2C::set_clock(RishkaVM* vm) {
    auto clock = vm->getParam<uint32_t>(0);
    return Wire.setClock(clock);
}

uint32_t RishkaSyscall::I2C::get_clock() {
    return Wire.getClock();
}

bool RishkaSyscall::I2C::pins(RishkaVM* vm) {
    auto sda = vm->getParam<uint8_t>(0);
    auto scl = vm->getParam<uint8_t>(1);

    return Wire.setPins(sda, scl);
}

size_t RishkaSyscall::I2C::bufsize(RishkaVM* vm) {
    auto size = vm->getParam<size_t>(0);
    return Wire.setBufferSize(size);
}

void RishkaSyscall::SPICall::begin(RishkaVM* vm) {
    auto sck = vm->getParam<uint8_t>(0);
    auto miso = vm->getParam<uint8_t>(1);
    auto mosi = vm->getParam<uint8_t>(2);
    auto ss = vm->getParam<uint8_t>(3);

    SPI.begin(sck, miso, mosi, ss);
}

void RishkaSyscall::SPICall::end() {
    SPI.end();
}

void RishkaSyscall::SPICall::begin_transaction(RishkaVM* vm) {
    auto clock = vm->getParam<uint8_t>(0);
    auto bit_order = vm->getParam<uint8_t>(1);
    auto data_mode = vm->getParam<uint8_t>(2);

    SPI.beginTransaction(SPISettings(clock, bit_order, data_mode));
}

void RishkaSyscall::SPICall::end_transaction() {
    SPI.endTransaction();
}

uint8_t RishkaSyscall::SPICall::transfer8(RishkaVM* vm) {
    auto data = vm->getParam<uint8_t>(0);
    return SPI.transfer(data);
}

uint16_t RishkaSyscall::SPICall::transfer16(RishkaVM* vm) {
    auto data = vm->getParam<uint16_t>(0);
    return SPI.transfer16(data);
}

uint32_t RishkaSyscall::SPICall::transfer32(RishkaVM* vm) {
    auto data = vm->getParam<uint32_t>(0);
    return SPI.transfer32(data);
}

void RishkaSyscall::SPICall::transfer_bytes(RishkaVM* vm) {
    auto data = vm->getPointerParam<uint8_t*>(0);
    auto out = vm->getPointerParam<uint8_t*>(1);
    auto size = vm->getParam<uint32_t>(2);

    SPI.transferBytes(data, out, size);
}

void RishkaSyscall::SPICall::transfer_bits(RishkaVM* vm) {
    auto data = vm->getParam<uint32_t>(0);
    auto out = vm->getPointerParam<uint32_t*>(1);
    auto bits = vm->getParam<uint8_t>(2);

    SPI.transferBits(data, out, bits);
}

void RishkaSyscall::SPICall::set_hwcs(RishkaVM* vm) {
    auto use = vm->getParam<bool>(0);
    SPI.setHwCs(use);
}

void RishkaSyscall::SPICall::set_bit_order(RishkaVM* vm) {
    auto bit_order = vm->getParam<uint8_t>(0);
    SPI.setBitOrder(bit_order);
}

void RishkaSyscall::SPICall::set_data_mode(RishkaVM* vm) {
    auto data_mode = vm->getParam<uint8_t>(0);
    SPI.setDataMode(data_mode);
}

void RishkaSyscall::SPICall::set_frequency(RishkaVM* vm) {
    auto frequency = vm->getParam<uint32_t>(0);
    SPI.setFrequency(frequency);
}

void RishkaSyscall::SPICall::set_clock_div(RishkaVM* vm) {
    auto clock_div = vm->getParam<uint32_t>(0);
    SPI.setClockDivider(clock_div);
}

uint32_t RishkaSyscall::SPICall::get_clock_div() {
    return SPI.getClockDivider();
}

void RishkaSyscall::SPICall::write8(RishkaVM* vm) {
    auto data = vm->getParam<uint8_t>(0);
    SPI.write(data);
}

void RishkaSyscall::SPICall::write16(RishkaVM* vm) {
    auto data = vm->getParam<uint16_t>(0);
    SPI.write16(data);
}

void RishkaSyscall::SPICall::write32(RishkaVM* vm) {
    auto data = vm->getParam<uint32_t>(0);
    SPI.write32(data);
}

void RishkaSyscall::SPICall::write_bytes(RishkaVM* vm) {
    auto data = vm->getPointerParam<uint8_t*>(0);
    auto size = vm->getParam<uint32_t>(1);

    SPI.writeBytes(data, size);
}

void RishkaSyscall::SPICall::write_pixels(RishkaVM* vm) {
    auto data = vm->getPointerParam<void*>(0);
    auto size = vm->getParam<uint32_t>(1);

    SPI.writePixels(data, size);
}

void RishkaSyscall::SPICall::write_pattern(RishkaVM* vm) {
    auto data = vm->getPointerParam<uint8_t*>(0);
    auto size = vm->getParam<uint8_t>(1);
    auto repeat = vm->getParam<uint32_t>(2);

    SPI.writePattern(data, size, repeat);
}

char RishkaSyscall::Runtime::strpass() {
    return strpass_data.charAt(strpass_idx++);
}

void RishkaSyscall::Runtime::yield() {
    yield();
}
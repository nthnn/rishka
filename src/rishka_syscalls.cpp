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
#include <fabgl.h>
#include <IPAddress.h>
#include <SD.h>
#include <WiFi.h>
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
    arg = arg != NULL ? arg : (char*) "(null)";

    vm->getTerminal()->print(arg);
    vm->appendToOutputStream(arg);
}

void RishkaSyscall::IO::printn(RishkaVM* vm) {
    auto arg = vm->getParam<int64_t>(0);

    vm->getTerminal()->print(arg);
    vm->appendToOutputStream(arg);
}

void RishkaSyscall::IO::printu(RishkaVM* vm) {
    auto arg = vm->getParam<uint64_t>(0);

    vm->getTerminal()->print(arg);
    vm->appendToOutputStream(arg);
}

void RishkaSyscall::IO::printd(RishkaVM* vm) {
    auto arg = rishka_long_to_double(
        vm->getParam<int64_t>(0)
    );

    vm->getTerminal()->print(arg);
    vm->appendToOutputStream(arg);
}

char RishkaSyscall::IO::readch(RishkaVM* vm) {
    fabgl::LineEditor line(vm->getTerminal());
    line.edit();

    char ch = (char) line.get()[0];
    vm->appendToOutputStream(ch);

    return ch;
}

size_t RishkaSyscall::IO::readLine(RishkaVM* vm) {
    fabgl::LineEditor line(vm->getTerminal());
    line.edit();

    char* input = (char*) line.get();
    change_rt_strpass(input);

    vm->appendToOutputStream(String(input));
    return strlen(input);
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
    auto cmdline = parent_vm->getPointerParam<char*>(0);

    char* tokens[10];
    int count = 0;

    for(int i = 0; i < 10; i++)
        tokens[i] = (char*) malloc(50 * sizeof(char));
    rishka_split_cmd(String(cmdline), tokens, 10, count);

    RishkaVM* child_vm = new RishkaVM();
    child_vm->initialize(
        parent_vm->getTerminal(),
        parent_vm->getDisplay(),
        parent_vm->getNvsStorage(),
        parent_vm->getWorkingDirectory()
    );

    if(!child_vm->loadFile(tokens[0])) {
        child_vm->reset();
        delete child_vm;

        return -1;
    }

    child_vm->run(count, tokens);
    parent_vm->setWorkingDirectory(child_vm->getWorkingDirectory());
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

bool RishkaSyscall::Sys::changeDir(RishkaVM* vm) {
    auto dir = vm->getPointerParam<char*>(0);
    if(strcmp(dir, "~") == 0) {
        vm->setWorkingDirectory("/");
        return true;
    }
 
    String target = rishka_sanitize_path(vm->getWorkingDirectory(), dir);
    if(!SD.exists(target))
        return false;

    vm->setWorkingDirectory(target);
    return true;
}

uint32_t RishkaSyscall::Sys::workingDirectory(RishkaVM* vm) {
    String path = vm->getWorkingDirectory();
    char* data = (char*) path.c_str();

    change_rt_strpass(data);
    return strlen(data);
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
    auto frequency = vm->getParam<uint32_t>(0);
    auto duration = vm->getParam<uint64_t>(1);

    tone(25, frequency, duration);
}

void RishkaSyscall::Gpio::noToneImpl(RishkaVM* vm) {
    noTone(25);
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
    return SD.mkdir(rishka_sanitize_path(vm->getWorkingDirectory(), path));
}

bool RishkaSyscall::FS::rmdir(RishkaVM* vm) {
    auto path = vm->getPointerParam<char*>(0);
    return SD.rmdir(rishka_sanitize_path(vm->getWorkingDirectory(), path));
}

bool RishkaSyscall::FS::remove(RishkaVM* vm) {
    auto path = vm->getPointerParam<char*>(0);
    return SD.remove(rishka_sanitize_path(vm->getWorkingDirectory(), path));
}

bool RishkaSyscall::FS::exists(RishkaVM* vm) {
    auto path = vm->getPointerParam<char*>(0);
    return SD.exists(rishka_sanitize_path(vm->getWorkingDirectory(), path));
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

    if(strcmp(mode, "n") == 0)
        vm->fileHandles.add(
            SD.open(rishka_sanitize_path(vm->getWorkingDirectory(), path))
        );
    else vm->fileHandles.add(SD.open(
        rishka_sanitize_path(vm->getWorkingDirectory(), path), mode)
    );

    return vm->fileHandles.getSize() - 1;
}

void RishkaSyscall::FS::close(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    vm->fileHandles[handle].close();
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

bool RishkaSyscall::FS::isOk(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);
    return !!vm->fileHandles[handle];
}

uint8_t RishkaSyscall::FS::next(RishkaVM* vm) {
    auto handle = vm->getParam<uint8_t>(0);

    File next = vm->fileHandles[handle].openNextFile();
    File file = SD.open(next.path());

    vm->fileHandles.add(SD.open(String(next.path()).c_str()));
    return vm->fileHandles.getSize() - 1;
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

uint32_t RishkaSyscall::Keyboard::layout_name() {
    auto name = (char*) fabgl::PS2Controller::keyboard()
        ->getLayout()->name;

    change_rt_strpass(name);
    return strlen(name);
}

uint32_t RishkaSyscall::Keyboard::layout_desc() {
    auto name = (char*) fabgl::PS2Controller::keyboard()
        ->getLayout()->desc;

    change_rt_strpass(name);
    return strlen(name);
}

uint8_t RishkaSyscall::Keyboard::device_type() {
    return (uint8_t) fabgl::PS2Controller::keyboard()
        ->identify();
}

bool RishkaSyscall::Keyboard::is_num_lock() {
    bool num, caps, scroll;
    fabgl::PS2Controller::keyboard()
        ->getLEDs(&num, &caps, &scroll);

    (void) caps, (void) scroll;
    return num;
}

bool RishkaSyscall::Keyboard::is_caps_lock() {
    bool num, caps, scroll;
    fabgl::PS2Controller::keyboard()
        ->getLEDs(&num, &caps, &scroll);

    (void) num, (void) scroll;
    return caps;
}

bool RishkaSyscall::Keyboard::is_scroll_lock() {
    bool num, caps, scroll;
    fabgl::PS2Controller::keyboard()
        ->getLEDs(&num, &caps, &scroll);

    (void) num, (void) caps;
    return scroll;
}

void RishkaSyscall::Keyboard::num_lock(RishkaVM* vm) {
    auto numVal = vm->getParam<bool>(0);

    bool num, caps, scroll;
    fabgl::PS2Controller::keyboard()
        ->getLEDs(&num, &caps, &scroll);

    (void) num;
    fabgl::PS2Controller::keyboard()
        ->setLEDs(numVal, caps, scroll);
}

void RishkaSyscall::Keyboard::caps_lock(RishkaVM* vm) {
    auto capsVal = vm->getParam<bool>(0);

    bool num, caps, scroll;
    fabgl::PS2Controller::keyboard()
        ->getLEDs(&num, &caps, &scroll);

    (void) caps;
    fabgl::PS2Controller::keyboard()
        ->setLEDs(num, capsVal, scroll);
}

void RishkaSyscall::Keyboard::scroll_lock(RishkaVM* vm) {
    auto scrollVal = vm->getParam<bool>(0);

    bool num, caps, scroll;
    fabgl::PS2Controller::keyboard()
        ->getLEDs(&num, &caps, &scroll);

    (void) scroll;
    fabgl::PS2Controller::keyboard()
        ->setLEDs(num, caps, scrollVal);
}

uint32_t RishkaSyscall::Keyboard::next_scan_code(RishkaVM* vm) {
    auto timeout = vm->getParam<int>(0);
    auto resend = vm->getParam<bool>(1);

    return fabgl::PS2Controller::keyboard()
        ->getNextScancode(timeout, resend);
}

void RishkaSyscall::Keyboard::lock(RishkaVM* vm) {
    auto timeout = vm->getParam<int>(0);

    fabgl::PS2Controller::keyboard()
        ->lock(timeout);
}

void RishkaSyscall::Keyboard::unlock() {
    fabgl::PS2Controller::keyboard()
        ->unlock();
}

void RishkaSyscall::Keyboard::reset() {
    fabgl::PS2Controller::keyboard()
        ->reset();
}

int RishkaSyscall::Display::screen_height(RishkaVM* vm) {
    return vm->getDisplay()
        ->getScreenHeight();
}

int RishkaSyscall::Display::screen_width(RishkaVM* vm) {
    return vm->getDisplay()
        ->getScreenWidth();
}

int RishkaSyscall::Display::viewport_height(RishkaVM* vm) {
    return vm->getDisplay()
        ->getViewPortHeight();
}

int RishkaSyscall::Display::viewport_width(RishkaVM* vm) {
    return vm->getDisplay()
        ->getViewPortWidth();
}

int RishkaSyscall::Display::supported_colors(RishkaVM* vm) {
    return vm->getDisplay()
        ->colorsCount();
}

bool RishkaSyscall::NVS::commit(RishkaVM* vm) {
    return vm->getNvsStorage()
        ->commit();
}

bool RishkaSyscall::NVS::erase_all(RishkaVM* vm) {
    auto forceCommit = vm->getParam<bool>(0);

    return vm->getNvsStorage()
        ->eraseAll(forceCommit);
}

bool RishkaSyscall::NVS::erase(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto forceCommit = vm->getParam<bool>(1);

    return vm->getNvsStorage()
        ->erase(key, forceCommit);
}

bool RishkaSyscall::NVS::set_i8(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto value = vm->getParam<int8_t>(1);
    auto forceCommit = vm->getParam<bool>(2);

    return vm->getNvsStorage()
        ->setInt(key, value, forceCommit);
}

bool RishkaSyscall::NVS::set_i16(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto value = vm->getParam<int16_t>(1);
    auto forceCommit = vm->getParam<bool>(2);

    return vm->getNvsStorage()
        ->setInt(key, value, forceCommit);
}

bool RishkaSyscall::NVS::set_i32(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto value = vm->getParam<int32_t>(1);
    auto forceCommit = vm->getParam<bool>(2);

    return vm->getNvsStorage()
        ->setInt(key, value, forceCommit);
}

bool RishkaSyscall::NVS::set_i64(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto value = vm->getParam<int64_t>(1);
    auto forceCommit = vm->getParam<bool>(2);

    return vm->getNvsStorage()
        ->setInt(key, value, forceCommit);
}

bool RishkaSyscall::NVS::set_u8(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto value = vm->getParam<uint8_t>(1);
    auto forceCommit = vm->getParam<bool>(2);

    return vm->getNvsStorage()
        ->setInt(key, value, forceCommit);
}

bool RishkaSyscall::NVS::set_u16(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto value = vm->getParam<uint16_t>(1);
    auto forceCommit = vm->getParam<bool>(2);

    return vm->getNvsStorage()
        ->setInt(key, value, forceCommit);
}

bool RishkaSyscall::NVS::set_u32(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto value = vm->getParam<uint32_t>(1);
    auto forceCommit = vm->getParam<bool>(2);

    return vm->getNvsStorage()
        ->setInt(key, value, forceCommit);
}

bool RishkaSyscall::NVS::set_u64(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto value = vm->getParam<uint64_t>(1);
    auto forceCommit = vm->getParam<bool>(2);

    return vm->getNvsStorage()
        ->setInt(key, value, forceCommit);
}

int8_t RishkaSyscall::NVS::get_i8(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto def = vm->getParam<int8_t>(1);

    return (int8_t) vm->getNvsStorage()
        ->getInt(key, def);
}

int16_t RishkaSyscall::NVS::get_i16(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto def = vm->getParam<int16_t>(1);

    return (int16_t) vm->getNvsStorage()
        ->getInt(key, def);
}

int32_t RishkaSyscall::NVS::get_i32(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto def = vm->getParam<int32_t>(1);

    return (int32_t) vm->getNvsStorage()
        ->getInt(key, def);
}

int64_t RishkaSyscall::NVS::get_i64(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto def = vm->getParam<int64_t>(1);

    return vm->getNvsStorage()
        ->getInt(key, def);
}

uint8_t RishkaSyscall::NVS::get_u8(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto def = vm->getParam<uint8_t>(1);

    return (uint8_t) vm->getNvsStorage()
        ->getInt(key, def);
}

uint16_t RishkaSyscall::NVS::get_u16(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto def = vm->getParam<uint16_t>(1);

    return (uint16_t) vm->getNvsStorage()
        ->getInt(key, def);
}

uint32_t RishkaSyscall::NVS::get_u32(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto def = vm->getParam<uint32_t>(1);

    return (uint32_t) vm->getNvsStorage()
        ->getInt(key, def);
}

uint64_t RishkaSyscall::NVS::get_u64(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto def = vm->getParam<uint64_t>(1);

    return (uint64_t) vm->getNvsStorage()
        ->getInt(key, def);
}

bool RishkaSyscall::NVS::set_string(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);
    auto value = vm->getPointerParam<char*>(1);
    auto forceCommit = vm->getParam<bool>(2);

    if(key == "wifi_ssid" || key == "wifi_pword")
        return false;

    return vm->getNvsStorage()
        ->setString(key, value, forceCommit);
}

uint32_t RishkaSyscall::NVS::get_string(RishkaVM* vm) {
    auto key = vm->getPointerParam<char*>(0);

    if(key == "wifi_ssid" || key == "wifi_pword") {
        char* empty = "";

        change_rt_strpass(empty);
        return strlen(empty);
    }

    char* data = (char*) vm->getNvsStorage()
        ->getString(key).c_str();
    change_rt_strpass(data);

    return strlen(data);
}

bool RishkaSyscall::NVS::has_wifi_config(RishkaVM* vm) {
    return vm->getNvsStorage()->getString("wifi_ssid") != "" &&
        vm->getNvsStorage()->getString("wifi_pword");
}

bool RishkaSyscall::NVS::set_wifi_ssid(RishkaVM* vm) {
    auto value = vm->getParam<char*>(0);
    return vm->getNvsStorage()
        ->setString("wifi_ssid", value, true);
}

bool RishkaSyscall::NVS::set_wifi_passkey(RishkaVM* vm) {
    auto value = vm->getParam<char*>(0);
    return vm->getNvsStorage()
        ->setString("wifi_pword", value, true);
}

bool RishkaSyscall::WiFiDev::connect(RishkaVM* vm) {
    auto ssid = vm->getPointerParam<char*>(0);
    auto passkey = vm->getPointerParam<char*>(1);
    auto channel = vm->getParam<int32_t>(2);
    auto bssid = vm->getPointerParam<uint8_t*>(3);
    auto connect = vm->getParam<bool>(4);

    return WiFi.begin(
        (const char*) ssid,
        (const char*) passkey,
        channel,
        (uint8_t*) bssid,
        connect
    );
}

bool RishkaSyscall::WiFiDev::reconnect() {
    return WiFi.reconnect();
}

bool RishkaSyscall::WiFiDev::disconnect(RishkaVM* vm) {
    auto wifiOff = vm->getParam<bool>(0);
    auto eraseAp = vm->getParam<bool>(1);

    return WiFi.disconnect(wifiOff, eraseAp);
}

bool RishkaSyscall::WiFiDev::erase_ap() {
    return WiFi.eraseAP();
}

bool RishkaSyscall::WiFiDev::is_connected() {
    return WiFi.isConnected();
}

bool RishkaSyscall::WiFiDev::set_autoreconnect(RishkaVM* vm) {
    auto autoRecon = vm->getParam<bool>(0);

    return WiFi.setAutoReconnect(autoRecon);
}

bool RishkaSyscall::WiFiDev::is_autoreconnect() {
    return WiFi.getAutoReconnect();
}

uint8_t RishkaSyscall::WiFiDev::wait_for_result(RishkaVM* vm) {
    auto timeout = vm->getParam<uint64_t>(0);

    return WiFi.waitForConnectResult(timeout);
}

void RishkaSyscall::WiFiDev::set_min_security(RishkaVM* vm) {
    auto authMode = vm->getParam<uint8_t>(0);

    return WiFi.setMinSecurity((wifi_auth_mode_t) authMode);
}

void RishkaSyscall::WiFiDev::set_scan_method(RishkaVM* vm) {
    auto scanMethod = vm->getParam<uint8_t>(0);

    return WiFi.setScanMethod((wifi_scan_method_t) scanMethod);
}

void RishkaSyscall::WiFiDev::set_sort_method(RishkaVM* vm) {
    auto sortMethod = vm->getParam<uint8_t>(0);

    return WiFi.setSortMethod((wifi_sort_method_t) sortMethod);
}

uint8_t RishkaSyscall::WiFiDev::status() {
    return (uint8_t) WiFi.status();
}

uint32_t RishkaSyscall::WiFiDev::ssid() {
    char* ssid = (char*) WiFi.SSID().c_str();

    change_rt_strpass(ssid);
    return strlen(ssid);
}

uint32_t RishkaSyscall::WiFiDev::psk() {
    char* psk = (char*) WiFi.psk().c_str();

    change_rt_strpass(psk);
    return strlen(psk);
}

uint32_t RishkaSyscall::WiFiDev::bssid() {
    char* bssid = (char*) WiFi.BSSIDstr().c_str();

    change_rt_strpass(bssid);
    return strlen(bssid);
}

int8_t RishkaSyscall::WiFiDev::rssi() {
    return WiFi.RSSI();
}

bool RishkaSyscall::WiFiDev::set_local_ip(RishkaVM* vm) {
    auto localIP = vm->getPointerParam<char*>(0);

    IPAddress addr;
    addr.fromString(localIP);

    IPAddress gateway = WiFi.gatewayIP(),
        subnet = WiFi.subnetMask(),
        dns1 = WiFi.dnsIP(0),
        dns2 = WiFi.dnsIP(1);

    return WiFi.config(
        addr,
        gateway,
        subnet,
        dns1,
        dns2
    );
}

bool RishkaSyscall::WiFiDev::set_gateway_ip(RishkaVM* vm) {
    auto gatewayIP = vm->getPointerParam<char*>(0);

    IPAddress addr;
    addr.fromString(gatewayIP);

    IPAddress localIP = WiFi.localIP(),
        subnet = WiFi.subnetMask(),
        dns1 = WiFi.dnsIP(0),
        dns2 = WiFi.dnsIP(1);

    return WiFi.config(
        localIP,
        addr,
        subnet,
        dns1,
        dns2
    );
}

char RishkaSyscall::Runtime::strpass() {
    return strpass_data.charAt(strpass_idx++);
}

void RishkaSyscall::Runtime::yield() {
    yield();
}

uint32_t RishkaSyscall::Runtime::getForkString(RishkaVM* vm) {
    String stream = vm->getOutputStream();
    char* data = (char*) stream.c_str();

    change_rt_strpass(data);
    return strlen(data);
}
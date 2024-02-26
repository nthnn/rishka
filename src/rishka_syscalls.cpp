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

#include <rishka_syscalls.h>
#include <rishka_types.h>
#include <rishka_util.h>
#include <rishka_vm.h>
#include <rishka_vm_helper.h>

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
    RISHKA_ESPINFO_TEMP_VAL
};

enum rishka_espinfo_s {
    RISHKA_ESPINFO_CHIPMODEL,
    RISHKA_ESPINFO_SDK_VERSION,
    RISHKA_ESPINFO_SKETCH_MD5
};

String strpass_data;
int strpass_idx = 0;

void change_rt_strpass(char* data) {
    strpass_data = data;
    strpass_idx = 0;
}

void rishka_syscall_io_prints(rishka_virtual_machine* vm) {
    char* arg = (char*) rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[10]);
    Serial.print(arg != NULL ? arg : "(null)");
}

void rishka_syscall_io_printn(rishka_virtual_machine* vm) {
    int64_t arg = (int64_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    Serial.print(arg);
}

void rishka_syscall_io_printd(rishka_virtual_machine* vm) {
    double arg = rishka_long_to_double((((rishka_u64_arrptr*) & vm->registers)->a).v[10]);
    Serial.print(arg);
}

char rishka_syscall_io_readch() {
    return (char) Serial.read();
}

char* rishka_syscall_io_readline() {
    return (char*) Serial.readString().c_str();
}

void rishka_syscall_sys_delay(rishka_virtual_machine* vm) {
    uint64_t ms = (uint64_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    delay(ms);
}

unsigned long rishka_syscall_sys_micros() {
    return micros();
}

unsigned long rishka_syscall_sys_millis() {
    return millis();
}

int rishka_syscall_sys_shellexec(rishka_virtual_machine* parent_vm) {
    static rishka_virtual_machine rishka_child_vm;

    char* program = (char*) rishka_vm_getptr(parent_vm, (((rishka_u64_arrptr*) & parent_vm->registers)->a).v[10]);
    int argc = (((rishka_u64_arrptr*) & parent_vm->registers)->a).v[11];
    char** argv = (char**) rishka_vm_getptr(parent_vm, (((rishka_u64_arrptr*) & parent_vm->registers)->a).v[12]);

    rishka_vm_initialize(&rishka_child_vm);
    if(!rishka_vm_loadfile(&rishka_child_vm, program))
        return -1;

    rishka_vm_run(&rishka_child_vm, argc, argv);
    rishka_vm_reset(&rishka_child_vm);
    
    return rishka_child_vm.exitcode;
}

void rishka_syscall_sys_exit(rishka_virtual_machine* vm) {
    int code = (int64_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];

    vm->running = false;
    vm->exitcode = code;
}

uint32_t rishka_syscall_sys_infos(rishka_virtual_machine* vm) {
    uint8_t key = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];

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

long rishka_syscall_sys_infon(rishka_virtual_machine* vm) {
    uint8_t key = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];

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

void rishka_syscall_mem_alloc(rishka_virtual_machine* vm) {
    void* dest = rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[10]);
    size_t size = (((rishka_u64_arrptr*) & vm->registers)->a).v[11];

    dest = ps_malloc(size);
}

void rishka_syscall_mem_calloc(rishka_virtual_machine* vm) {
    void* dest = rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[10]);
    size_t num = (((rishka_u64_arrptr*) & vm->registers)->a).v[11];
    size_t size = (((rishka_u64_arrptr*) & vm->registers)->a).v[12];

    dest = calloc(num, size);
}

void rishka_syscall_mem_realloc(rishka_virtual_machine* vm) {
    void* dest = rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[10]);
    void* ptr = rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[11]);
    size_t size = (((rishka_u64_arrptr*) & vm->registers)->a).v[12];

    dest = realloc(ptr, size);
}

void rishka_syscall_mem_free(rishka_virtual_machine* vm) {
    void* ptr = rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[10]);
    free(ptr);
}

void* rishka_syscall_mem_set(rishka_virtual_machine* vm) {
    void* dest = rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[10]);
    uint64_t val = (((rishka_u64_arrptr*) & vm->registers)->a).v[11];
    uint64_t len = (((rishka_u64_arrptr*) & vm->registers)->a).v[12];

    return memset(dest, (int) val, (size_t) len);
}

void rishka_syscall_gpio_pinmode(rishka_virtual_machine* vm) {
    uint8_t pin = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    uint8_t mode = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[11];

    pinMode(pin, mode);
}

bool rishka_syscall_gpio_digitalread(rishka_virtual_machine* vm) {
    uint8_t pin = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    return digitalRead(pin);
}

void rishka_syscall_gpio_digitalwrite(rishka_virtual_machine* vm) {
    uint8_t pin = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    bool value = (bool)(((rishka_u64_arrptr*) & vm->registers)->a).v[11];

    digitalWrite(pin, value);
}

int rishka_syscall_gpio_analogread(rishka_virtual_machine* vm) {
    uint8_t pin = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    return analogRead(pin);
}

void rishka_syscall_gpio_analogwrite(rishka_virtual_machine* vm) {
    uint8_t pin = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    uint8_t value = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[11];

    analogWrite(pin, value);
}

uint64_t rishka_syscall_gpio_pulse_in(rishka_virtual_machine* vm) {
    uint8_t pin = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    uint8_t state = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[11];
    uint64_t timeout = (uint64_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[12];

    return pulseIn(pin, state, timeout);
}

uint64_t rishka_syscall_gpio_pulse_in_long(rishka_virtual_machine* vm) {
    uint8_t pin = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    uint8_t state = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[11];
    uint64_t timeout = (uint64_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[12];

    return pulseInLong(pin, state, timeout);
}

uint8_t rishka_syscall_gpio_shift_in(rishka_virtual_machine* vm) {
    uint8_t data = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    uint8_t clock = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[11];
    uint8_t bit_order = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[12];

    return shiftIn(data, clock, bit_order);
}

void rishka_syscall_gpio_shift_out(rishka_virtual_machine* vm) {
    uint8_t data = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    uint8_t clock = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[11];
    uint8_t bit_order = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[12];
    uint8_t value = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[13];

    shiftOut(data, clock, bit_order, value);
}

void rishka_syscall_gpio_tone(rishka_virtual_machine* vm) {
    uint8_t pin = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    uint32_t frequency = (uint32_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    uint64_t duration = (uint64_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];

    tone(pin, frequency, duration);
}

void rishka_syscall_gpio_no_tone(rishka_virtual_machine* vm) {
    uint8_t pin = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    noTone(pin);
}

void rishka_syscall_int_enable() {
    interrupts();
}

void rishka_syscall_int_disable() {
    noInterrupts();
}

void rishka_syscall_int_attach(rishka_virtual_machine* vm) {
    uint8_t num = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    void (*ptr)(void) = (void (*)(void))rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[11]);
    int mode = (int)(((rishka_u64_arrptr*) & vm->registers)->a).v[12];

    attachInterrupt(num, ptr, mode);
}

void rishka_syscall_int_detach(rishka_virtual_machine* vm) {
    uint8_t num = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    detachInterrupt(num);
}

bool rishka_syscall_fs_mkdir(rishka_virtual_machine* vm) {
    char* path = (char*) rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[10]);
    return SD.mkdir(path);
}

bool rishka_syscall_fs_rmdir(rishka_virtual_machine* vm) {
    char* path = (char*) rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[10]);
    return SD.rmdir(path);
}

bool rishka_syscall_fs_delete(rishka_virtual_machine* vm) {
    char* path = (char*) rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[10]);
    return SD.remove(path);
}

bool rishka_syscall_fs_exists(rishka_virtual_machine* vm) {
    char* path = (char*) rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[10]);
    return SD.exists(path);
}

bool rishka_syscall_fs_isfile(rishka_virtual_machine* vm) {
    uint8_t handle = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    return !vm->file_handles[handle]->isDirectory();
}

bool rishka_syscall_fs_isdir(rishka_virtual_machine* vm) {
    uint8_t handle = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    return vm->file_handles[handle]->isDirectory();
}

uint8_t rishka_syscall_fs_open(rishka_virtual_machine* vm) {
    char* path = (char*) rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[10]);
    char* mode = (char*) rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[11]);

    uint8_t idx = 0;
    for(uint8_t i = 0; i < 255; i++)
        if(vm->file_handles[i] == NULL)
            idx = i;

    File file = SD.open(path, mode);
    vm->file_handles[idx] = &file;
    return idx;
}

void rishka_syscall_fs_close(rishka_virtual_machine* vm) {
    uint8_t handle = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    vm->file_handles[handle]->close();

    free(vm->file_handles[handle]);
    vm->file_handles[handle] = NULL;
}

int rishka_syscall_fs_available(rishka_virtual_machine* vm) {
    uint8_t handle = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    return vm->file_handles[handle]->available();
}

void rishka_syscall_fs_flush(rishka_virtual_machine* vm) {
    uint8_t handle = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    vm->file_handles[handle]->flush();
}

int rishka_syscall_fs_peek(rishka_virtual_machine* vm) {
    uint8_t handle = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    return vm->file_handles[handle]->peek();
}

bool rishka_syscall_fs_seek(rishka_virtual_machine* vm) {
    uint8_t handle = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    uint32_t pos = (uint32_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[11];

    return vm->file_handles[handle]->seek(pos);
}

uint32_t rishka_syscall_fs_size(rishka_virtual_machine* vm) {
    uint8_t handle = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    return vm->file_handles[handle]->size();
}

int rishka_syscall_fs_read(rishka_virtual_machine* vm) {
    uint8_t handle = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    return vm->file_handles[handle]->read();
}

size_t rishka_syscall_fs_write(rishka_virtual_machine* vm) {
    uint8_t handle = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    uint8_t data = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[11];

    return vm->file_handles[handle]->write(data);
}

uint8_t rishka_syscall_fs_next(rishka_virtual_machine* vm) {
    uint8_t handle = (uint8_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];
    char* mode = (char*) rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[11]);

    uint8_t idx = 0;
    for(uint8_t i = 0; i < 255; i++)
        if(vm->file_handles[i] == NULL)
            idx = i;

    File file = vm->file_handles[handle]->openNextFile(mode);
    vm->file_handles[idx] = &file;
    return idx;
}

char rishka_syscall_rt_strpass() {
    return strpass_data.charAt(strpass_idx++);
}
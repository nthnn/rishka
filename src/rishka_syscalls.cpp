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

#include <rishka_syscalls.h>
#include <rishka_types.h>
#include <rishka_util.h>
#include <rishka_vm.h>
#include <rishka_vm_helper.h>

void rishka_syscall_io_prints(rishka_virtual_machine* vm) {
    char* arg = (char*) rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[10]);

    Serial.print(arg != NULL ? arg : "(null)");
    Serial.flush();
}

void rishka_syscall_io_printn(rishka_virtual_machine* vm) {
    int64_t arg = (int64_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[10];

    Serial.print(arg);
    Serial.flush();
}

void rishka_syscall_io_printd(rishka_virtual_machine* vm) {
    double arg = rishka_long_to_double((((rishka_u64_arrptr*) & vm->registers)->a).v[10]);

    Serial.print(arg);
    Serial.flush();
}

char rishka_syscall_io_readch() {
    Serial.flush();
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

void* rishka_syscall_mem_set(rishka_virtual_machine* vm) {
    void* dest = rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[10]);
    uint64_t val = (((rishka_u64_arrptr*) & vm->registers)->a).v[11];
    uint64_t len = (((rishka_u64_arrptr*) & vm->registers)->a).v[12];

    return memset(dest, (int) val, (size_t) len);
}
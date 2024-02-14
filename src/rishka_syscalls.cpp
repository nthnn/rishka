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
#include <rishka_vm.h>
#include <rishka_vm_helper.h>

void rishka_syscall_io_prints(char* arg) {
    Serial.print(arg != NULL ? arg : "(null)");
    Serial.flush();
}

void rishka_syscall_io_prints(int64_t arg) {
    Serial.print(arg);
}

void rishka_syscall_io_printd(double arg) {
    Serial.print(arg);
}

char rishka_syscall_io_readch() {
    Serial.flush();
    return (char) Serial.read();
}

void rishka_syscall_sys_exit(rishka_virtual_machine* vm, int code) {
    vm->running = false;
    vm->exitcode = code;
}

void* rishka_syscall_mem_set(rishka_virtual_machine* vm) {
    void* dest = rishka_vm_getptr(vm, (((rishka_u64_arrptr*) & vm->registers)->a).v[10]);
    uint64_t val = (((rishka_u64_arrptr*) & vm->registers)->a).v[11];
    uint64_t len = (((rishka_u64_arrptr*) & vm->registers)->a).v[12];

    return memset(dest, (int) val, (size_t) len);
}
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

#ifndef RISHKA_VM_H
#define RISHKA_VM_H

#include <rishka_commons.h>

typedef struct {
    bool running;

    char** argv;
    int argc;

    int64_t pc;
    int64_t exitcode;

    uint64_t registers[32];
    uint8_t memory[RISHKA_VM_STACK_SIZE];
} rishka_virtual_machine;

static rishka_virtual_machine riscvm_machine;

void rishka_vm_run(rishka_virtual_machine* vm, int argc, char** argv);
bool rishka_vm_loadfile(rishka_virtual_machine* vm, const char* file_name);
void rishka_vm_execute(rishka_virtual_machine* vm, uint32_t inst);
void rishka_vm_reset(rishka_virtual_machine* vm);

uint32_t rishka_vm_fetch(rishka_virtual_machine* vm);
uint64_t rishka_vm_handle_syscall(rishka_virtual_machine* vm, uint64_t code);

#endif
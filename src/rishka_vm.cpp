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

#include <rishka_errors.h>
#include <rishka_instructions.h>
#include <rishka_syscalls.h>
#include <rishka_types.h>
#include <rishka_util.h>
#include <rishka_vm.h>
#include <rishka_vm_helper.h>

void rishka_vm_initialize(rishka_virtual_machine* vm, Stream* stream) {
    vm->running = false;
    vm->argv = NULL;
    vm->argc = 0;
    vm->pc = 0;
    vm->exitcode = 0;

    vm->stream = stream;
}

void rishka_vm_run(rishka_virtual_machine* vm, int argc, char** argv) {
    vm->running = true;
    vm->argc = argc;
    vm->argv = argv;

    while(vm->running)
        rishka_vm_execute(vm, rishka_vm_fetch(vm));
}

void rishka_vm_execute(rishka_virtual_machine* vm, uint32_t inst) {
    uint32_t opcode = ((inst >> 0) & 127);
    uint32_t rd = ((inst >> 7) & 31),
        rs1 = ((inst >> 15) & 31),
        rs2 = ((inst >> 20) & 31);

    switch(opcode) {
        case RISHKA_OPINST_LOAD: {
            uint32_t function_code_3 = ((inst >> 12) & 7);
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)((inst >> 20) & 4095) << 20)) >> 20);
            uint64_t addr = ((((rishka_u64_arrptr*) & vm->registers)->a).v[rs1] + (uint64_t) immediate);

            int64_t val;
            switch(function_code_3) {
                case RISHKA_FC3_LB:
                    val = (int64_t) rishka_vm_read_i8(vm, (rishka_nil_type) {}, addr);
                    break;

                case RISHKA_FC3_LHW:
                    val = (int64_t) rishka_vm_read_i16(vm, (rishka_nil_type) {}, addr);
                    break;

                case RISHKA_FC3_LW:
                    val = (int64_t) rishka_vm_read_i32(vm, (rishka_nil_type) {}, addr);
                    break;

                case RISHKA_FC3_LDW:
                    val = rishka_vm_read_i64(vm, (rishka_nil_type) {}, addr);
                    break;

                case RISHKA_FC3_LBU:
                    val = (int64_t) rishka_vm_read_u8_arrptr(vm, (rishka_nil_type) {}, addr);
                    break;

                case RISHKA_FC3_LHU:
                    val = (int64_t) rishka_vm_read_u16ptr(vm, (rishka_nil_type) {}, addr);
                    break;

                case RISHKA_FC3_LRES:
                    val = (int64_t) rishka_vm_read_u32ptr(vm, (rishka_nil_type) {}, addr);
                    break;

                default:
                    rishka_panic("Invalid load instruction.", vm);
                    break;
            }

            if(rd != 0)
                (((rishka_u64_arrptr*) & vm->registers)->a).v[rd] = (uint64_t) val;
            break;
        }
    
        case RISHKA_OPINST_STORE: {
            uint32_t function_code_3 = ((inst >> 12) & 7);
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)(((inst >> 20) & 4064) | ((inst >> 7) & 31)) << 20)) >> 20);
            uint64_t addr = ((((rishka_u64_arrptr*) & vm->registers)->a).v[rs1] + (uint64_t) immediate);
            uint64_t val = (((rishka_u64_arrptr*) & vm->registers)->a).v[rs2];

            switch(function_code_3) {
                case RISHKA_FC3_SB:
                    rishka_vm_write_u8(vm, addr, (uint8_t) val);
                    break;

                case RISHKA_FC3_SHW:
                    rishka_vm_write_u16(vm, addr, (uint16_t) val);
                    break;

                case RISHKA_FC3_SW:
                    rishka_vm_write_u32(vm, addr, (uint32_t) val);
                    break;

                case RISHKA_FC3_SDW:
                    rishka_vm_write_u64(vm, addr, val);
                    break;

                default:
                    rishka_panic("Invalid store instruction.", vm);
                    break;
            }
            break;
        }

        case RISHKA_OPINST_IMM: {
            uint32_t function_code_3 = ((inst >> 12) & 7);
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)((inst >> 20) & 4095) << 20)) >> 20);
            uint32_t shift_amount = ((inst >> 20) & 63);
            int64_t val = (int64_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[rs1];

            switch(function_code_3) {
                case RISHKA_FC3_ADDI:
                    val = (val + immediate);
                    break;

                case RISHKA_FC3_SLLI:
                    val = rishka_shl_riscvi64(val, shift_amount);
                    break;

                case RISHKA_FC3_SLTI:
                    if((val < immediate))
                        val = 1;
                    else val = 0;
                    break;

                case RISHKA_FC3_SLTIU:
                    if(((uint64_t) val < (uint64_t) immediate))
                        val = 1;
                    else val = 0;
                    break;

                case RISHKA_FC3_XORI:
                    val = (val ^ immediate);
                    break;

                case RISHKA_FC3_SRLI: {
                    uint32_t function_code_6 = ((inst >> 26) & 63);
                    switch((function_code_6 >> 4)) {
                        case 0x0:
                            val = rishka_shr_riscvi64(val, shift_amount);
                            break;

                        case 0x1:
                            val = rishka_asr_riscvi64(val, shift_amount);
                            break;

                        default:
                            rishka_panic("Invalid immediate shift instruction.", vm);
                            break;
                    }
                    break;
                }

                case RISHKA_FC3_ORI:
                    val = (val | immediate);
                    break;

                case RISHKA_FC3_ANDI: {
                    val = (val & immediate);
                    break;
                }

                default:
                    rishka_panic("Invalid immediate instruction.", vm);
                    break;
            }

            if(rd != 0)
                (((rishka_u64_arrptr*) & vm->registers)->a).v[rd] = (uint64_t) val;
            break;
        }

        case RISHKA_OPINST_IALU: {
            uint32_t function_code_3 = ((inst >> 12) & 7);
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)((inst >> 20) & 4095) << 20)) >> 20);
            int64_t val = (int64_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[rs1];

            switch(function_code_3) {
                case RISHKA_FC3_SLLIW:
                    val = (int64_t)(val + immediate);
                    break;

                case RISHKA_FC3_SRLIW:
                    val = (int64_t) rishka_shl_riscvi64(val, immediate);
                    break;

                case RISHKA_FC3_SRAIW: {
                    uint32_t shift_amount = rs2;
                    uint32_t function_code_7 = ((inst >> 25) & 127);

                    switch((function_code_7 >> 5)) {
                        case 0x0:
                            val = (int64_t) rishka_shr_riscvi64(val, shift_amount);
                            break;

                        case 0x1:
                            val = (int64_t) rishka_asr_riscvi64(val, shift_amount);
                            break;

                        default:
                            rishka_panic("Invalid immediate shift instruction.", vm);
                            break;
                    }

                    break;
                }

                default:
                    rishka_panic("Invalid immediate instruction.", vm);
                    break;
            }

            if(rd != 0)
                (((rishka_u64_arrptr*) & vm->registers)->a).v[rd] = (uint64_t) val;
            break;
        }

        case RISHKA_OPINST_RT64: {
            uint32_t function_code_3 = ((inst >> 12) & 7), function_code_7 = ((inst >> 25) & 127);
            int64_t val1 = (int64_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[rs1];
            int64_t val2 = (int64_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[rs2];

            int64_t val;
            switch(((function_code_7 << 3) | function_code_3)) {
                case 0x0:
                    val = (val1 + val2);
                    break;

                case 0x100:
                    val = (val1 - val2);
                    break;

                case 0x1:
                    val = rishka_shl_riscvi64(val1, (val2 & 0x1f));
                    break;

                case 0x2:
                    if((val1 < val2))
                        val = 1;
                    else val = 0;
                    break;

                case 0x3:
                    if(((uint64_t) val1 < (uint64_t) val2))
                        val = 1;
                    else val = 0;
                    break;

                case 0x4:
                    val = (val1 ^ val2);
                    break;

                case 0x5:
                    val = rishka_shr_riscvi64(val1, (val2 & 0x1f));
                    break;

                case 0x105:
                    val = rishka_asr_riscvi64(val1, (val2 & 0x1f));
                    break;

                case 0x6:
                    val = (val1 | val2);
                    break;

                case 0x7:
                    val = (val1 & val2);
                    break;

                case 0x8:
                    val = (val1 * val2);
                    break;

                case 0x9:
                    val = (int64_t) rishka_shr_riscvi128(((int64_t) val1 * (int64_t) val2), 64);
                    break;

                case 0xa:
                    val = (int64_t)(uint64_t) rishka_shr_riscvi128(((int64_t) val1 * (int64_t)(uint64_t) val2), 64);
                    break;

                case 0xb:
                    val = (int64_t)(uint64_t) rishka_shr_riscvi128(((int64_t)(uint64_t) val1 * (int64_t)(uint64_t) val2), 64);
                    break;

                case 0xc: {
                    int64_t dividend = val1, divisor = val2;

                    if(dividend == (-9223372036854775807LL - 1) && divisor == -1)
                        val = (-9223372036854775807LL - 1);
                    else if(divisor == 0)
                        val = -1;
                    else val = (dividend / divisor);
                    break;
                }

                case 0xd: {
                    uint64_t dividend = (uint64_t) val1, divisor = (uint64_t) val2;

                    if(divisor == 0)
                        val = -1;
                    else val = (int64_t)(dividend / divisor);
                    break;
                }

                case 0xe: {
                    int64_t dividend = val1, divisor = val2;

                    if(dividend == (-9223372036854775807LL - 1) && divisor == -1)
                        val = 0;
                    else if(divisor == 0)
                        val = dividend;
                    else val = (dividend % divisor);
                    break;
                }

                case 0xf: {
                    uint64_t dividend = (uint64_t) val1, divisor = (uint64_t) val2;

                    if(divisor == 0)
                        val = (int64_t) dividend;
                    else val = (int64_t)(dividend % divisor);
                    break;
                }

                default:
                    rishka_panic("Invalid arithmetic instruction.", vm);
                    break;
            }

            if(rd != 0)
                (((rishka_u64_arrptr*) & vm->registers)->a).v[rd] = (uint64_t) val;
            break;
        }

        case RISHKA_OPINST_RT32: {
            uint32_t function_code_3 = ((inst >> 12) & 7);
            uint32_t function_code_7 = ((inst >> 25) & 127);

            int64_t val1 = (int64_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[rs1];
            int64_t val2 = (int64_t)(((rishka_u64_arrptr*) & vm->registers)->a).v[rs2];

            int64_t val;
            switch(((function_code_7 << 3) | function_code_3)) {
                case 0x0:
                    val = (int64_t)(val1 + val2);
                    break;

                case 0x100:
                    val = (int64_t)(val1 - val2);
                    break;

                case 0x1:
                    val = (int64_t) rishka_shl_riscvi64(val1, (val2 & 0x1f));
                    break;
        
                case 0x5:
                    val = (int64_t) rishka_shr_riscvi64(val1, (val2 & 0x1f));
                    break;

                case 0x105:
                    val = (int64_t) rishka_asr_riscvi64(val1, (val2 & 0x1f));
                    break;

                case 0x8:
                    val = (int64_t)((int32_t) val1 * (int32_t) val2);
                    break;

                case 0xc: {
                    int32_t dividend = (int32_t) val1, divisor = (int32_t) val2;

                    if(dividend == (-2147483647 - 1) && divisor == -1)
                        val = -2147483648LL;
                    else if(divisor == 0)
                        val = -1;
                    else val = (int64_t)(dividend / divisor);
                    break;
                }

                case 0xd: {
                    uint32_t dividend = (uint32_t) val1, divisor = (uint32_t) val2;

                    if(divisor == 0)
                        val = -1;
                    else val = (int64_t)(dividend / divisor);
                    break;
                }

                case 0xe: {
                    int32_t dividend = (int32_t) val1, divisor = (int32_t) val2;

                    if(dividend == (-2147483647 - 1) && divisor == -1)
                        val = 0;
                    else if(divisor == 0)
                        val = (int64_t) dividend;
                    else val = (int64_t)(dividend % divisor);
                    break;
                }

                case 0xf: {
                    uint32_t dividend = (uint32_t) val1, divisor = (uint32_t) val2;

                    if(divisor == 0)
                        val = (int64_t) dividend;
                    else val = (int64_t)(dividend % divisor);
                    break;
                }

                default:
                    rishka_panic("Invalid store doubleword instruction.", vm);
                    break;
            }

            if(rd != 0)
                (((rishka_u64_arrptr*) & vm->registers)->a).v[rd] = (uint64_t) val;
            break;
        }

        case RISHKA_OPINST_LUI: {
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)((inst << 0) & 4294963200LL) << 0)) >> 0);

            if(rd != 0)
                (((rishka_u64_arrptr*) & vm->registers)->a).v[rd] = (uint64_t) immediate;
            break;
        }

        case RISHKA_OPINST_AUIPC: {
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)((inst << 0) & 4294963200LL) << 0)) >> 0);

            if(rd != 0)
                (((rishka_u64_arrptr*) & vm->registers)->a).v[rd] = (uint64_t)(vm->pc + immediate);
            break;
        }

        case RISHKA_OPINST_JAL: {
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)(((((inst >> 11) & 1048576) | ((inst >> 20) & 2046)) | ((inst >> 9) & 2048)) | ((inst << 0) & 1044480)) << 11)) >> 11);

            if(rd != 0)
                (((rishka_u64_arrptr*) & vm->registers)->a).v[rd] = (uint64_t)(vm->pc + 4);
            vm->pc = (vm->pc + immediate);

            return;
        }

        case RISHKA_OPINST_JALR: {
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)((inst >> 20) & 4095) << 20)) >> 20);
            int64_t pc = (vm->pc + 4);

            vm->pc = ((int64_t)((((rishka_u64_arrptr*) & vm->registers)->a).v[rs1] + immediate) & - 2);

            if(rd != 0)
                (((rishka_u64_arrptr*) & vm->registers)->a).v[rd] = (uint64_t) pc;
            return;
        }

        case RISHKA_OPINST_BRANCH: {
            uint32_t function_code_3 = ((inst >> 12) & 7);
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)(((((inst >> 19) & 4096) | ((inst >> 20) & 2016)) | ((inst >> 7) & 30)) | ((inst << 4) & 2048)) << 19)) >> 19);

            uint64_t val1 = (((rishka_u64_arrptr*) & vm->registers)->a).v[rs1];
            uint64_t val2 = (((rishka_u64_arrptr*) & vm->registers)->a).v[rs2];

            bool condition;
            switch(function_code_3) {
                case RISHKA_FC3_BEQ:
                    condition = (val1 == val2);
                    break;

                case RISHKA_FC3_BNE:
                    condition = (val1 != val2);
                    break;

                case RISHKA_FC3_BLT:
                    condition = ((int64_t) val1 < (int64_t) val2);
                    break;

                case RISHKA_FC3_BGE:
                    condition = ((int64_t) val1 >= (int64_t) val2);
                    break;

                case RISHKA_FC3_BLTU:
                    condition = (val1 < val2);
                    break;

                case RISHKA_FC3_BGEU:
                    condition = (val1 >= val2);
                    break;

                default:
                    rishka_panic("Invalid branch instruction.", vm);
                    break;
            }

            if(condition) {
                vm->pc = (vm->pc + immediate);
                return;
            }

            break;
        }

        case RISHKA_OPINST_FENCE:
            break;

        case RISHKA_OPINST_CALL: {
            uint32_t function_code_11 = ((inst >> 20) & 4095);

            switch(function_code_11) {
                case 0x0: {
                    uint64_t code = (((rishka_u64_arrptr*) & vm->registers)->a).v[17];
                    (((rishka_u64_arrptr*) & vm->registers)->a).v[10] = rishka_vm_handle_syscall(vm, code);
                    break;
                }

                case 0x1:
                    vm->exitcode = -1;
                    vm->running = false;
                    break;

                default:
                    rishka_panic("Invalid system instruction.", vm);
                    break;
            }

            break;
        }

        default:
            rishka_panic("Invalid opcode instruction.", vm);
            break;
    }

    vm->pc = (vm->pc + 4);
}

bool rishka_vm_loadfile(rishka_virtual_machine* vm, const char* file_name) {
    File file = SD.open(file_name);
    if(!file) {
        file.close();
        return false;
    }

    if(file.read(&(((rishka_u8_arrptr*) &vm->memory)->a).v[4096], file.size())) {
        file.close();

        (((rishka_u64_arrptr*) &vm->registers)->a).v[2] = RISHKA_VM_STACK_SIZE;
        vm->pc = 4096;

        return true;
    }

    return false;
}

uint32_t rishka_vm_fetch(rishka_virtual_machine* vm) {
    return (*(rishka_u32ptr)(&(((rishka_u8_arrptr*) & vm->memory)->a).v[vm->pc]));
}

uint64_t rishka_vm_handle_syscall(rishka_virtual_machine* vm, uint64_t code) {
    switch(code) {
        case RISHKA_SC_IO_PRINTS:
            rishka_syscall_io_prints(vm);
            break;

        case RISHKA_SC_IO_PRINTN:
            rishka_syscall_io_printn(vm);
            break;

        case RISHKA_SC_IO_PRINTD:
            rishka_syscall_io_printd(vm);
            break;

        case RISHKA_SC_IO_READCH:
            return rishka_syscall_io_readch(vm);

        case RISHKA_SC_IO_READLINE:
            return rishka_syscall_io_readline(vm);

        case RISHKA_SC_IO_READ:
            return rishka_syscall_io_read(vm);

        case RISHKA_SC_IO_AVAILABLE:
            return rishka_syscall_io_available(vm);

        case RISHKA_SC_IO_PEEK:
            return rishka_syscall_io_peek(vm);

        case RISHKA_SC_IO_FIND:
            return rishka_syscall_io_find(vm);

        case RISHKA_SC_IO_FIND_UNTIL:
            return rishka_syscall_io_find_until(vm);

        case RISHKA_SC_IO_SET_TIMEOUT:
            rishka_syscall_io_set_timeout(vm);
            break;

        case RISHKA_SC_IO_GET_TIMEOUT:
            return rishka_syscall_io_get_timeout(vm);

        case RISHKA_SC_SYS_DELAY_MS:
            rishka_syscall_sys_delay(vm);
            break;

        case RISHKA_SC_SYS_MICROS:
            return rishka_syscall_sys_micros();

        case RISHKA_SC_SYS_MILLIS:
            return rishka_syscall_sys_millis();

        case RISHKA_SC_SYS_SHELLEXEC:
            return rishka_syscall_sys_shellexec(vm);

        case RISHKA_SC_SYS_EXIT:
            rishka_syscall_sys_exit(vm);
            break;

        case RISHKA_SC_SYS_INFOS:
            return rishka_syscall_sys_infos(vm);

        case RISHKA_SC_SYS_INFON:
            return rishka_syscall_sys_infon(vm);

        case RISHKA_SC_SYS_RANDOM:
            return rishka_syscall_sys_random();

        case RISHKA_SC_MEM_ALLOC:
            rishka_syscall_mem_alloc(vm);
            break;

        case RISHKA_SC_MEM_CALLOC:
            rishka_syscall_mem_calloc(vm);
            break;

        case RISHKA_SC_MEM_REALLOC:
            rishka_syscall_mem_realloc(vm);
            break;

        case RISHKA_SC_MEM_FREE:
            rishka_syscall_mem_free(vm);
            break;

        case RISHKA_SC_MEM_SET:
            return (uint64_t) rishka_syscall_mem_set(vm);

        case RISHKA_SC_GPIO_PIN_MODE:
            rishka_syscall_gpio_pinmode(vm);
            break;

        case RISHKA_SC_GPIO_DIGITAL_READ:
            return rishka_syscall_gpio_digitalread(vm);

        case RISHKA_SC_GPIO_DIGITAL_WRITE:
            rishka_syscall_gpio_digitalwrite(vm);
            break;

        case RISHKA_SC_GPIO_ANALOG_READ:
            return rishka_syscall_gpio_analogread(vm);

        case RISHKA_SC_GPIO_ANALOG_WRITE:
            rishka_syscall_gpio_analogwrite(vm);
            break;

        case RISHKA_SC_GPIO_PULSE_IN:
            return rishka_syscall_gpio_pulse_in(vm);

        case RISHKA_SC_GPIO_PULSE_IN_LONG:
            return rishka_syscall_gpio_pulse_in_long(vm);

        case RISHKA_SC_GPIO_SHIFT_IN:
            return rishka_syscall_gpio_shift_in(vm);

        case RISHKA_SC_GPIO_SHIFT_OUT:
            rishka_syscall_gpio_shift_out(vm);
            break;

        case RISHKA_SC_GPIO_TONE:
            rishka_syscall_gpio_tone(vm);
            break;

        case RISHKA_SC_GPIO_NO_TONE:
            rishka_syscall_gpio_no_tone(vm);
            break;

        case RISHKA_SC_INT_ENABLE:
            rishka_syscall_int_enable();
            break;

        case RISHKA_SC_INT_DISABLE:
            rishka_syscall_int_disable();
            break;

        case RISHKA_SC_INT_ATTACH:
            rishka_syscall_int_attach(vm);
            break;

        case RISHKA_SC_INT_DETACH:
            rishka_syscall_int_detach(vm);
            break;

        case RISHKA_SC_FS_MKDIR:
            return rishka_syscall_fs_mkdir(vm);

        case RISHKA_SC_FS_RMDIR:
            return rishka_syscall_fs_rmdir(vm);

        case RISHKA_SC_FS_DELETE:
            return rishka_syscall_fs_delete(vm);

        case RISHKA_SC_FS_EXISTS:
            return rishka_syscall_fs_exists(vm);

        case RISHKA_SC_FS_ISFILE:
            return rishka_syscall_fs_isfile(vm);

        case RISHKA_SC_FS_ISDIR:
            return rishka_syscall_fs_isdir(vm);

        case RISHKA_SC_FS_OPEN:
            return rishka_syscall_fs_open(vm);

        case RISHKA_SC_FS_CLOSE:
            rishka_syscall_fs_close(vm);
            break;

        case RISHKA_SC_FS_AVAILABLE:
            return rishka_syscall_fs_available(vm);

        case RISHKA_SC_FS_FLUSH:
            rishka_syscall_fs_flush(vm);
            break;

        case RISHKA_SC_FS_PEEK:
            return rishka_syscall_fs_peek(vm);

        case RISHKA_SC_FS_SEEK:
            return rishka_syscall_fs_seek(vm);

        case RISHKA_SC_FS_SIZE:
            return rishka_syscall_fs_size(vm);

        case RISHKA_SC_FS_READ:
            return rishka_syscall_fs_read(vm);

        case RISHKA_SC_FS_WRITEB:
            rishka_syscall_fs_writeb(vm);
            break;

        case RISHKA_SC_FS_WRITES:
            rishka_syscall_fs_writes(vm);
            break;

        case RISHKA_SC_FS_POS:
            return rishka_syscall_fs_position(vm);

        case RISHKA_SC_FS_PATH:
            return rishka_syscall_fs_path(vm);

        case RISHKA_SC_FS_NAME:
            return rishka_syscall_fs_name(vm);

        case RISHKA_SC_FS_NEXT:
            return rishka_syscall_fs_next(vm);

        case RISHKA_SC_FS_BUFSIZE:
            return rishka_syscall_fs_bufsize(vm);

        case RISHKA_SC_FS_LASTWRITE:
            return rishka_syscall_fs_lastwrite(vm);

        case RISHKA_SC_FS_SEEKDIR:
            return rishka_syscall_fs_seekdir(vm);

        case RISHKA_SC_FS_NEXT_NAME:
            return rishka_syscall_fs_next_name(vm);

        case RISHKA_SC_FS_REWIND:
            rishka_syscall_fs_rewind(vm);
            break;

        case RISHKA_SC_ARG_COUNT:
            return rishka_syscall_arg_count(vm);

        case RISHKA_SC_ARG_STR:
            return rishka_syscall_arg_value(vm);

        case RISHKA_SC_I2C_BEGIN:
            return rishka_syscall_i2c_begin(vm);

        case RISHKA_SC_I2C_END:
            return rishka_syscall_i2c_end(vm);

        case RISHKA_SC_I2C_BEGIN_TRANSMISSION:
            rishka_syscall_i2c_begin_transmission(vm);
            break;

        case RISHKA_SC_I2C_END_TRANSMISSION:
            return rishka_syscall_i2c_end_transmission(vm);

        case RISHKA_SC_I2C_WRITE:
            return rishka_syscall_i2c_write(vm);

        case RISHKA_SC_I2C_SLAVE_WRITE:
            return rishka_syscall_i2c_slave_write(vm);

        case RISHKA_SC_I2C_READ:
            return rishka_syscall_i2c_read(vm);

        case RISHKA_SC_I2C_PEEK:
            return rishka_syscall_i2c_peek(vm);

        case RISHKA_SC_I2C_REQUEST:
            return rishka_syscall_i2c_request(vm);

        case RISHKA_SC_I2C_AVAILABLE:
            return rishka_syscall_i2c_available(vm);

        case RISHKA_SC_I2C_FLUSH:
            rishka_syscall_i2c_flush();
            break;

        case RISHKA_SC_I2C_ON_RECEIVE:
            rishka_syscall_i2c_on_receive(vm);
            break;
        
        case RISHKA_SC_I2C_ON_REQUEST:
            rishka_syscall_i2c_on_request(vm);
            break;

        case RISHKA_SC_I2C_GET_TIMEOUT:
            return rishka_syscall_i2c_get_timeout();

        case RISHKA_SC_I2C_SET_TIMEOUT:
            rishka_syscall_i2c_set_timeout(vm);
            break;

        case RISHKA_SC_I2C_SET_CLOCK:
            return rishka_syscall_i2c_set_clock(vm);

        case RISHKA_SC_I2C_GET_CLOCK:
            return rishka_syscall_i2c_get_clock();

        case RISHKA_SC_I2C_PINS:
            return rishka_syscall_i2c_pins(vm);

        case RISHKA_SC_I2C_BUFSIZE:
            return rishka_syscall_i2c_bufsize(vm);

        case RISHKA_SC_SPI_BEGIN:
            rishka_syscall_spi_begin(vm);
            break;

        case RISHKA_SC_SPI_END:
            rishka_syscall_spi_end();
            break;

        case RISHKA_SC_SPI_BEGIN_TRANSACTION:
            rishka_syscall_spi_begin_transaction(vm);
            break;

        case RISHKA_SC_SPI_END_TRANSACTION:
            rishka_syscall_spi_end_transaction();
            break;

        case RISHKA_SC_SPI_TRANSFER8:
            return rishka_syscall_spi_transfer8(vm);

        case RISHKA_SC_SPI_TRANSFER16:
            return rishka_syscall_spi_transfer16(vm);

        case RISHKA_SC_SPI_TRANSFER32:
            return rishka_syscall_spi_transfer32(vm);

        case RISHKA_SC_SPI_TRANSFER_BYTES:
            rishka_syscall_spi_transfer_bytes(vm);
            break;

        case RISHKA_SC_SPI_TRANSFER_BITS:
            rishka_syscall_spi_transfer_bits(vm);
            break;

        case RISHKA_SC_SPI_SET_HWCS:
            rishka_syscall_spi_set_hwcs(vm);
            break;

        case RISHKA_SC_SPI_SET_BIT_ORDER:
            rishka_syscall_spi_set_bit_order(vm);
            break;

        case RISHKA_SC_SPI_SET_DATA_MODE:
            rishka_syscall_spi_set_data_mode(vm);
            break;

        case RISHKA_SC_SPI_SET_FREQ:
            rishka_syscall_spi_set_frequency(vm);
            break;

        case RISHKA_SC_SPI_SET_CLOCK_DIV:
            rishka_syscall_spi_set_clock_div(vm);
            break;

        case RISHKA_SC_SPI_GET_CLOCK_DIV:
            return rishka_syscall_spi_get_clock_div();

        case RISHKA_SC_SPI_WRITE8:
            rishka_syscall_spi_write8(vm);
            break;

        case RISHKA_SC_SPI_WRITE16:
            rishka_syscall_spi_write16(vm);
            break;

        case RISHKA_SC_SPI_WRITE32:
            rishka_syscall_spi_write32(vm);
            break;

        case RISHKA_SC_SPI_WRITE_BYTES:
            rishka_syscall_spi_write_bytes(vm);
            break;

        case RISHKA_SC_SPI_WRITE_PIXELS:
            rishka_syscall_spi_write_pixels(vm);
            break;

        case RISHKA_SC_SPI_WRITE_PATTERN:
            rishka_syscall_spi_write_pattern(vm);
            break;

        case RISHKA_SC_RT_STRPASS:
            return rishka_syscall_rt_strpass();

        default:
            rishka_panic("Invalid system call.", vm);
            break;
    }

    return 0;
}

void rishka_vm_reset(rishka_virtual_machine* vm) {
    vm->running = false;
    vm->argv = NULL;
    vm->argc = 0;
    vm->pc = 0;
    vm->exitcode = 0;

    for(uint8_t i = 0; i < 32; i++)
        vm->registers[i] = 0;

    for(int i = 0; i < RISHKA_VM_STACK_SIZE; i++)
        vm->memory[i] = 0;

    vm->file_handles.clear();
}

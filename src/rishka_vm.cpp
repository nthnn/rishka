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

#include <rishka_instructions.h>
#include <rishka_syscalls.h>
#include <rishka_types.h>
#include <rishka_util.h>
#include <rishka_vm.h>

void RishkaVM::initialize(Stream* stream) {
    this->running = false;
    this->argv = NULL;
    this->argc = 0;
    this->pc = 0;
    this->exitCode = 0;
    this->stream = stream;
}

void RishkaVM::stopVM() {
    this->running = false;
}

void RishkaVM::setExitCode(int64_t exitCode) {
    this->exitCode = exitCode;
}

int64_t RishkaVM::getExitCode() const {
    return this->exitCode;
}

Stream* RishkaVM::getStream() const {
    return this->stream;
}

bool RishkaVM::loadFile(const char* fileName) {
    File file = SD.open(fileName);
    if(!file) {
        file.close();
        return false;
    }

    if(file.read(&(((rishka_u8_arrptr*) &this->memory)->a).v[4096], file.size())) {
        file.close();

        (((rishka_u64_arrptr*) &this->registers)->a).v[2] = RISHKA_VM_STACK_SIZE;
        this->pc = 4096;

        return true;
    }

    return false;
}

void RishkaVM::run(int argc, char** argv) {
    this->running = true;
    this->argc = argc;
    this->argv = argv;

    while(this->running)
        this->execute(this->fetch());
}

void RishkaVM::panic(const char* message) {
    this->stopVM();
    this->setExitCode(-1);

    this->stream->print("\r\n");
    this->stream->print(message);
    this->stream->print("\r\n");
}

void RishkaVM::reset() {
    this->running = false;
    this->argv = NULL;
    this->argc = 0;
    this->pc = 0;
    this->exitCode = 0;

    this->fileHandles.clear();
}

void RishkaVM::execute(uint32_t inst) {
    uint32_t opcode = ((inst >> 0) &127);
    uint32_t rd = ((inst >> 7) &31),
        rs1 = ((inst >> 15) &31),
        rs2 = ((inst >> 20) &31);

    switch(opcode) {
        case RISHKA_OPINST_LOAD: {
            uint32_t function_code_3 = ((inst >> 12) &7);
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)((inst >> 20) &4095) << 20)) >> 20);
            uint64_t addr = ((((rishka_u64_arrptr*) &this->registers)->a).v[rs1] + (uint64_t) immediate);

            int64_t val;
            switch(function_code_3) {
                case RISHKA_FC3_LB:
                    val = (int64_t)(*(&(((rishka_u8_arrptr*) &this->memory)->a).v[addr]));
                    break;

                case RISHKA_FC3_LHW:
                    val = (int64_t)(*(uint16_t*)(&(((rishka_u8_arrptr*) &this->memory)->a).v[addr]));
                    break;

                case RISHKA_FC3_LW:
                    val = (int64_t) (*(uint32_t*)(&(((rishka_u8_arrptr*) &this->memory)->a).v[addr]));
                    break;

                case RISHKA_FC3_LDW:
                    val = (int64_t)(*(uint64_t*)(&(((rishka_u8_arrptr*) &this->memory)->a).v[addr]));
                    break;

                case RISHKA_FC3_LBU:
                    val = (int64_t)(*(&(((rishka_u8_arrptr*) &this->memory)->a).v[addr]));
                    break;

                case RISHKA_FC3_LHU:
                    val = (int64_t)(*(uint16_t*)(&(((rishka_u8_arrptr*) &this->memory)->a).v[addr]));
                    break;

                case RISHKA_FC3_LRES:
                    val = (int64_t)(*(uint32_t*)(&(((rishka_u8_arrptr*) &this->memory)->a).v[addr]));
                    break;

                default:
                    this->panic("Invalid load instruction.");
                    break;
            }

            if(rd != 0)
                (((rishka_u64_arrptr*) &this->registers)->a).v[rd] = (uint64_t) val;
            break;
        }
    
        case RISHKA_OPINST_STORE: {
            uint32_t function_code_3 = ((inst >> 12) &7);
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)(((inst >> 20) &4064) | ((inst >> 7) &31)) << 20)) >> 20);

            uint64_t addr = ((((rishka_u64_arrptr*) &this->registers)->a).v[rs1] + (uint64_t) immediate);
            uint64_t val = (((rishka_u64_arrptr*) &this->registers)->a).v[rs2];

            switch(function_code_3) {
                case RISHKA_FC3_SB:
                    (*(&(((rishka_u8_arrptr*) &this->memory)->a).v[addr])) = val;
                    break;

                case RISHKA_FC3_SHW:
                    (*(uint16_t*)(&(((rishka_u8_arrptr*) &this->memory)->a).v[addr])) = val;
                    break;

                case RISHKA_FC3_SW:
                    (*(uint32_t*)(&(((rishka_u8_arrptr*) &this->memory)->a).v[addr])) = val;
                    break;

                case RISHKA_FC3_SDW:
                    (*(uint64_t*)(&(((rishka_u8_arrptr*) &this->memory)->a).v[addr])) = val;
                    break;

                default:
                    this->panic("Invalid store instruction.");
                    break;
            }
            break;
        }

        case RISHKA_OPINST_IMM: {
            uint32_t function_code_3 = ((inst >> 12) &7);
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)((inst >> 20) &4095) << 20)) >> 20);
            uint32_t shift_amount = ((inst >> 20) &63);
            int64_t val = (int64_t)(((rishka_u64_arrptr*) &this->registers)->a).v[rs1];

            switch(function_code_3) {
                case RISHKA_FC3_ADDI:
                    val = (val + immediate);
                    break;

                case RISHKA_FC3_SLLI:
                    val = this->shiftLeftInt64(val, shift_amount);
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
                    uint32_t function_code_6 = ((inst >> 26) &63);
                    switch((function_code_6 >> 4)) {
                        case 0x0:
                            val = this->shiftRightInt64(val, shift_amount);
                            break;

                        case 0x1:
                            val = this->arithmeticShiftRightInt64(val, shift_amount);
                            break;

                        default:
                            this->panic("Invalid immediate shift instruction.");
                            break;
                    }
                    break;
                }

                case RISHKA_FC3_ORI:
                    val = (val | immediate);
                    break;

                case RISHKA_FC3_ANDI: {
                    val = (val &immediate);
                    break;
                }

                default:
                    this->panic("Invalid immediate instruction.");
                    break;
            }

            if(rd != 0)
                (((rishka_u64_arrptr*) &this->registers)->a).v[rd] = (uint64_t) val;
            break;
        }

        case RISHKA_OPINST_IALU: {
            uint32_t function_code_3 = ((inst >> 12) &7);
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)((inst >> 20) &4095) << 20)) >> 20);
            int64_t val = (int64_t)(((rishka_u64_arrptr*) &this->registers)->a).v[rs1];

            switch(function_code_3) {
                case RISHKA_FC3_SLLIW:
                    val = (int64_t)(val + immediate);
                    break;

                case RISHKA_FC3_SRLIW:
                    val = (int64_t) this->shiftLeftInt64(val, immediate);
                    break;

                case RISHKA_FC3_SRAIW: {
                    uint32_t shift_amount = rs2;
                    uint32_t function_code_7 = ((inst >> 25) &127);

                    switch((function_code_7 >> 5)) {
                        case 0x0:
                            val = (int64_t) this->shiftRightInt64(val, shift_amount);
                            break;

                        case 0x1:
                            val = (int64_t) this->arithmeticShiftRightInt64(val, shift_amount);
                            break;

                        default:
                            this->panic("Invalid immediate shift instruction.");
                            break;
                    }

                    break;
                }

                default:
                    this->panic("Invalid immediate instruction.");
                    break;
            }

            if(rd != 0)
                (((rishka_u64_arrptr*) &this->registers)->a).v[rd] = (uint64_t) val;
            break;
        }

        case RISHKA_OPINST_RT64: {
            uint32_t function_code_3 = ((inst >> 12) &7), function_code_7 = ((inst >> 25) &127);
            int64_t val1 = (int64_t)(((rishka_u64_arrptr*) &this->registers)->a).v[rs1];
            int64_t val2 = (int64_t)(((rishka_u64_arrptr*) &this->registers)->a).v[rs2];

            int64_t val;
            switch(((function_code_7 << 3) | function_code_3)) {
                case 0x0:
                    val = (val1 + val2);
                    break;

                case 0x100:
                    val = (val1 - val2);
                    break;

                case 0x1:
                    val = this->shiftLeftInt64(val1, (val2 & 0x1f));
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
                    val = this->shiftRightInt64(val1, (val2 & 0x1f));
                    break;

                case 0x105:
                    val = this->arithmeticShiftRightInt64(val1, (val2 & 0x1f));
                    break;

                case 0x6:
                    val = (val1 | val2);
                    break;

                case 0x7:
                    val = (val1 &val2);
                    break;

                case 0x8:
                    val = (val1 * val2);
                    break;

                case 0x9:
                    val = (int64_t) this->shiftRightInt128(((int64_t) val1 * (int64_t) val2), 64);
                    break;

                case 0xa:
                    val = (int64_t)(uint64_t) this->shiftRightInt128(((int64_t) val1 * (int64_t)(uint64_t) val2), 64);
                    break;

                case 0xb:
                    val = (int64_t)(uint64_t) this->shiftRightInt128(((int64_t)(uint64_t) val1 * (int64_t)(uint64_t) val2), 64);
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
                    this->panic("Invalid arithmetic instruction.");
                    break;
            }

            if(rd != 0)
                (((rishka_u64_arrptr*) &this->registers)->a).v[rd] = (uint64_t) val;
            break;
        }

        case RISHKA_OPINST_RT32: {
            uint32_t function_code_3 = ((inst >> 12) &7);
            uint32_t function_code_7 = ((inst >> 25) &127);

            int64_t val1 = (int64_t)(((rishka_u64_arrptr*) &this->registers)->a).v[rs1];
            int64_t val2 = (int64_t)(((rishka_u64_arrptr*) &this->registers)->a).v[rs2];

            int64_t val;
            switch(((function_code_7 << 3) | function_code_3)) {
                case 0x0:
                    val = (int64_t)(val1 + val2);
                    break;

                case 0x100:
                    val = (int64_t)(val1 - val2);
                    break;

                case 0x1:
                    val = (int64_t) this->shiftLeftInt64(val1, (val2 & 0x1f));
                    break;
        
                case 0x5:
                    val = (int64_t) this->shiftRightInt64(val1, (val2 & 0x1f));
                    break;

                case 0x105:
                    val = (int64_t) this->arithmeticShiftRightInt64(val1, (val2 & 0x1f));
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
                    this->panic("Invalid store doubleword instruction.");
                    break;
            }

            if(rd != 0)
                (((rishka_u64_arrptr*) &this->registers)->a).v[rd] = (uint64_t) val;
            break;
        }

        case RISHKA_OPINST_LUI: {
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)((inst << 0) &4294963200LL) << 0)) >> 0);

            if(rd != 0)
                (((rishka_u64_arrptr*) &this->registers)->a).v[rd] = (uint64_t) immediate;
            break;
        }

        case RISHKA_OPINST_AUIPC: {
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)((inst << 0) &4294963200LL) << 0)) >> 0);

            if(rd != 0)
                (((rishka_u64_arrptr*) &this->registers)->a).v[rd] = (uint64_t)(this->pc + immediate);
            break;
        }

        case RISHKA_OPINST_JAL: {
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)(((((inst >> 11) &1048576) | ((inst >> 20) &2046)) | ((inst >> 9) &2048)) | ((inst << 0) &1044480)) << 11)) >> 11);

            if(rd != 0)
                (((rishka_u64_arrptr*) &this->registers)->a).v[rd] = (uint64_t)(this->pc + 4);
            this->pc = (this->pc + immediate);

            return;
        }

        case RISHKA_OPINST_JALR: {
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)((inst >> 20) &4095) << 20)) >> 20);
            int64_t pc = (this->pc + 4);

            this->pc = ((int64_t)((((rishka_u64_arrptr*) &this->registers)->a).v[rs1] + immediate) &- 2);

            if(rd != 0)
                (((rishka_u64_arrptr*) &this->registers)->a).v[rd] = (uint64_t) pc;
            return;
        }

        case RISHKA_OPINST_BRANCH: {
            uint32_t function_code_3 = ((inst >> 12) &7);
            int64_t immediate = (int64_t)(((int32_t)((uint32_t)(((((inst >> 19) &4096) | ((inst >> 20) &2016)) | ((inst >> 7) &30)) | ((inst << 4) &2048)) << 19)) >> 19);

            uint64_t val1 = (((rishka_u64_arrptr*) &this->registers)->a).v[rs1];
            uint64_t val2 = (((rishka_u64_arrptr*) &this->registers)->a).v[rs2];

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
                    this->panic("Invalid branch instruction.");
                    break;
            }

            if(condition) {
                this->pc = (this->pc + immediate);
                return;
            }

            break;
        }

        case RISHKA_OPINST_FENCE:
            break;

        case RISHKA_OPINST_CALL: {
            uint32_t function_code_11 = ((inst >> 20) &4095);

            switch(function_code_11) {
                case 0x0: {
                    uint64_t code = (((rishka_u64_arrptr*) &this->registers)->a).v[17];
                    (((rishka_u64_arrptr*) &this->registers)->a).v[10] = this->handleSyscall(code);
                    break;
                }

                case 0x1:
                    this->exitCode = -1;
                    this->running = false;
                    break;

                default:
                    this->panic("Invalid system instruction.");
                    break;
            }

            break;
        }

        default:
            this->panic("Invalid opcode instruction.");
            break;
    }

    this->pc = (this->pc + 4);
}

inline uint32_t RishkaVM::fetch() {
    return (*(uint32_t*)(&(((rishka_u8_arrptr*) &this->memory)->a).v[this->pc]));
}

uint64_t RishkaVM::handleSyscall(uint64_t code) {
    switch(code) {
        case RISHKA_SC_IO_PRINTS:
            rishka_syscall_io_prints(this);
            break;

        case RISHKA_SC_IO_PRINTN:
            rishka_syscall_io_printn(this);
            break;

        case RISHKA_SC_IO_PRINTD:
            rishka_syscall_io_printd(this);
            break;

        case RISHKA_SC_IO_READCH:
            return rishka_syscall_io_readch(this);

        case RISHKA_SC_IO_READLINE:
            return rishka_syscall_io_readline(this);

        case RISHKA_SC_IO_READ:
            return rishka_syscall_io_read(this);

        case RISHKA_SC_IO_AVAILABLE:
            return rishka_syscall_io_available(this);

        case RISHKA_SC_IO_PEEK:
            return rishka_syscall_io_peek(this);

        case RISHKA_SC_IO_FIND:
            return rishka_syscall_io_find(this);

        case RISHKA_SC_IO_FIND_UNTIL:
            return rishka_syscall_io_find_until(this);

        case RISHKA_SC_IO_SET_TIMEOUT:
            rishka_syscall_io_set_timeout(this);
            break;

        case RISHKA_SC_IO_GET_TIMEOUT:
            return rishka_syscall_io_get_timeout(this);

        case RISHKA_SC_SYS_DELAY_MS:
            rishka_syscall_sys_delay(this);
            break;

        case RISHKA_SC_SYS_MICROS:
            return rishka_syscall_sys_micros();

        case RISHKA_SC_SYS_MILLIS:
            return rishka_syscall_sys_millis();

        case RISHKA_SC_SYS_SHELLEXEC:
            return rishka_syscall_sys_shellexec(this);

        case RISHKA_SC_SYS_EXIT:
            rishka_syscall_sys_exit(this);
            break;

        case RISHKA_SC_SYS_INFOS:
            return rishka_syscall_sys_infos(this);

        case RISHKA_SC_SYS_INFON:
            return rishka_syscall_sys_infon(this);

        case RISHKA_SC_SYS_RANDOM:
            return rishka_syscall_sys_random();

        case RISHKA_SC_MEM_ALLOC:
            rishka_syscall_mem_alloc(this);
            break;

        case RISHKA_SC_MEM_CALLOC:
            rishka_syscall_mem_calloc(this);
            break;

        case RISHKA_SC_MEM_REALLOC:
            rishka_syscall_mem_realloc(this);
            break;

        case RISHKA_SC_MEM_FREE:
            rishka_syscall_mem_free(this);
            break;

        case RISHKA_SC_MEM_SET:
            return (uint64_t) rishka_syscall_mem_set(this);

        case RISHKA_SC_GPIO_PIN_MODE:
            rishka_syscall_gpio_pinmode(this);
            break;

        case RISHKA_SC_GPIO_DIGITAL_READ:
            return rishka_syscall_gpio_digitalread(this);

        case RISHKA_SC_GPIO_DIGITAL_WRITE:
            rishka_syscall_gpio_digitalwrite(this);
            break;

        case RISHKA_SC_GPIO_ANALOG_READ:
            return rishka_syscall_gpio_analogread(this);

        case RISHKA_SC_GPIO_ANALOG_WRITE:
            rishka_syscall_gpio_analogwrite(this);
            break;

        case RISHKA_SC_GPIO_PULSE_IN:
            return rishka_syscall_gpio_pulse_in(this);

        case RISHKA_SC_GPIO_PULSE_IN_LONG:
            return rishka_syscall_gpio_pulse_in_long(this);

        case RISHKA_SC_GPIO_SHIFT_IN:
            return rishka_syscall_gpio_shift_in(this);

        case RISHKA_SC_GPIO_SHIFT_OUT:
            rishka_syscall_gpio_shift_out(this);
            break;

        case RISHKA_SC_GPIO_TONE:
            rishka_syscall_gpio_tone(this);
            break;

        case RISHKA_SC_GPIO_NO_TONE:
            rishka_syscall_gpio_no_tone(this);
            break;

        case RISHKA_SC_INT_ENABLE:
            rishka_syscall_int_enable();
            break;

        case RISHKA_SC_INT_DISABLE:
            rishka_syscall_int_disable();
            break;

        case RISHKA_SC_INT_ATTACH:
            rishka_syscall_int_attach(this);
            break;

        case RISHKA_SC_INT_DETACH:
            rishka_syscall_int_detach(this);
            break;

        case RISHKA_SC_FS_MKDIR:
            return rishka_syscall_fs_mkdir(this);

        case RISHKA_SC_FS_RMDIR:
            return rishka_syscall_fs_rmdir(this);

        case RISHKA_SC_FS_DELETE:
            return rishka_syscall_fs_delete(this);

        case RISHKA_SC_FS_EXISTS:
            return rishka_syscall_fs_exists(this);

        case RISHKA_SC_FS_ISFILE:
            return rishka_syscall_fs_isfile(this);

        case RISHKA_SC_FS_ISDIR:
            return rishka_syscall_fs_isdir(this);

        case RISHKA_SC_FS_OPEN:
            return rishka_syscall_fs_open(this);

        case RISHKA_SC_FS_CLOSE:
            rishka_syscall_fs_close(this);
            break;

        case RISHKA_SC_FS_AVAILABLE:
            return rishka_syscall_fs_available(this);

        case RISHKA_SC_FS_FLUSH:
            rishka_syscall_fs_flush(this);
            break;

        case RISHKA_SC_FS_PEEK:
            return rishka_syscall_fs_peek(this);

        case RISHKA_SC_FS_SEEK:
            return rishka_syscall_fs_seek(this);

        case RISHKA_SC_FS_SIZE:
            return rishka_syscall_fs_size(this);

        case RISHKA_SC_FS_READ:
            return rishka_syscall_fs_read(this);

        case RISHKA_SC_FS_WRITEB:
            rishka_syscall_fs_writeb(this);
            break;

        case RISHKA_SC_FS_WRITES:
            rishka_syscall_fs_writes(this);
            break;

        case RISHKA_SC_FS_POS:
            return rishka_syscall_fs_position(this);

        case RISHKA_SC_FS_PATH:
            return rishka_syscall_fs_path(this);

        case RISHKA_SC_FS_NAME:
            return rishka_syscall_fs_name(this);

        case RISHKA_SC_FS_NEXT:
            return rishka_syscall_fs_next(this);

        case RISHKA_SC_FS_BUFSIZE:
            return rishka_syscall_fs_bufsize(this);

        case RISHKA_SC_FS_LASTWRITE:
            return rishka_syscall_fs_lastwrite(this);

        case RISHKA_SC_FS_SEEKDIR:
            return rishka_syscall_fs_seekdir(this);

        case RISHKA_SC_FS_NEXT_NAME:
            return rishka_syscall_fs_next_name(this);

        case RISHKA_SC_FS_REWIND:
            rishka_syscall_fs_rewind(this);
            break;

        case RISHKA_SC_ARG_COUNT:
            return rishka_syscall_arg_count(this);

        case RISHKA_SC_ARG_STR:
            return rishka_syscall_arg_value(this);

        /*case RISHKA_SC_I2C_BEGIN:
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
            break;*/

        case RISHKA_SC_RT_STRPASS:
            return rishka_syscall_rt_strpass();

        default:
            this->panic("Invalid system call.");
            break;
    }

    return 0;
}

inline int64_t RishkaVM::shiftLeftInt64(int64_t a, int64_t b) {
    if(b >= 0 && b < 64)
        return ((uint64_t) a) << b;
    else if(b < 0 && b > -64)
        return (uint64_t) a >> -b;

    return 0;
}

inline int64_t RishkaVM::shiftRightInt64(int64_t a, int64_t b) {
    if(b >= 0 && b < 64)
        return (uint64_t) a >> b;
    else if(b < 0 && b > -64)
        return (uint64_t) a << -b;

    return 0;
}

inline int64_t RishkaVM::shiftRightInt128(int64_t a, int64_t b) {
    if(b >= 0 && b < 128)
        return (uint64_t) a >> b;
    else if(b < 0 && b > -128)
        return (uint64_t) a << -b;

    return 0;
}

inline int64_t RishkaVM::arithmeticShiftRightInt64(int64_t a, int64_t b) {
    if(b >= 0 && b < 64)
        return a >> b;
    else if(b >= 64)
        return a < 0 ? -1 : 0;
    else if(b < 0 && b > -64)
        return a << -b;

    return 0;
}
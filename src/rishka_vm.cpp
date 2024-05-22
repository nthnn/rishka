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

void RishkaVM::initialize(
    fabgl::Terminal* terminal,
    fabgl::BaseDisplayController* displayCtrl,
    ArduinoNvs* nvsStorage,
    String workingDirectory
) {
    this->running = false;
    this->argv = NULL;
    this->argc = 0;
    this->pc = 0;
    this->exitCode = 0;
    this->workingDirectory = workingDirectory;
    this->outputStream = "";

    this->terminal = terminal;
    this->display = displayCtrl;
    this->nvsStorage = nvsStorage;
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

fabgl::Terminal* RishkaVM::getTerminal() const {
    return this->terminal;
}

fabgl::BaseDisplayController* RishkaVM::getDisplay() const {
    return this->display;
}

ArduinoNvs* RishkaVM::getNvsStorage() const {
    return this->nvsStorage;
}

uint8_t RishkaVM::getArgCount() const {
    return this->argc;
}

char* RishkaVM::getArgValue(const uint8_t index) const {
    return this->argv[index];
}

bool RishkaVM::loadFile(const char* fileName, bool enableBoot) {
    String absoluteFilename = "/bin/" + String(fileName) + ".bin";
    if(!SD.exists(absoluteFilename))
        absoluteFilename = rishka_sanitize_path(
            this->getWorkingDirectory(),
            (char*) fileName);

    if(!SD.exists(absoluteFilename))
        absoluteFilename = rishka_sanitize_path(
            this->getWorkingDirectory(),
            (char*) (String(fileName) + ".bin").c_str());

    if(!SD.exists(absoluteFilename))
        return false;

    if(!enableBoot && absoluteFilename == "/bin/boot.bin")
        return false;

    File file = SD.open(absoluteFilename);
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

bool RishkaVM::isRunning() {
    return this->running;
}

void RishkaVM::panic(const char* message) {
    this->terminal->print("\r\n");
    this->terminal->print(message);
    this->terminal->print("\r\n");

    this->stopVM();
    this->reset();
    this->setExitCode(-1);
}

void RishkaVM::reset() {
    this->running = false;
    this->argv = NULL;
    this->argc = 0;
    this->pc = 0;
    this->exitCode = 0;
    this->outputStream = "";

    this->fileHandles.clear();
    this->initialize(
        this->terminal,
        this->display,
        this->nvsStorage,
        this->workingDirectory
    );
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
                    val = RishkaVM::shiftLeftInt64(val, shift_amount);
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
                            val = RishkaVM::shiftRightInt64(val, shift_amount);
                            break;

                        case 0x1:
                            val = RishkaVM::arithmeticShiftRightInt64(val, shift_amount);
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
                    val = RishkaVM::shiftLeftInt64(val, immediate);
                    break;

                case RISHKA_FC3_SRAIW: {
                    uint32_t shift_amount = rs2;
                    uint32_t function_code_7 = ((inst >> 25) &127);

                    switch((function_code_7 >> 5)) {
                        case 0x0:
                            val = RishkaVM::shiftRightInt64(val, shift_amount);
                            break;

                        case 0x1:
                            val = RishkaVM::arithmeticShiftRightInt64(val, shift_amount);
                            break;

                        default:
                            this->panic("Invalid immediate shift instruction.");
                            break;
                    }

                    break;
                }

                case RISHKA_FC3_SLLI64:
                    val = RishkaVM::shiftLeftInt64(val, immediate & 0x3F);
                    break;

                case RISHKA_FC3_SRLI64:
                    val = RishkaVM::shiftRightInt64(val, immediate & 0x3F);
                    break;

                case RISHKA_FC3_SRAI64:
                    val = RishkaVM::arithmeticShiftRightInt64(val, immediate & 0x3F);
                    break;

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
                    val = RishkaVM::shiftLeftInt64(val1, (val2 & 0x1f));
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
                    val = RishkaVM::shiftRightInt64(val1, (val2 & 0x1f));
                    break;

                case 0x105:
                    val = RishkaVM::arithmeticShiftRightInt64(val1, (val2 & 0x1f));
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
                    val = RishkaVM::shiftRightInt128(((int64_t) val1 * (int64_t) val2), 64);
                    break;

                case 0xa:
                    val = (int64_t)(uint64_t) RishkaVM::shiftRightInt128(((int64_t) val1 * (int64_t)(uint64_t) val2), 64);
                    break;

                case 0xb:
                    val = (int64_t)(uint64_t) RishkaVM::shiftRightInt128(((int64_t)(uint64_t) val1 * (int64_t)(uint64_t) val2), 64);
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
                    val = RishkaVM::shiftLeftInt64(val1, (val2 & 0x1f));
                    break;
        
                case 0x5:
                    val = RishkaVM::shiftRightInt64(val1, (val2 & 0x1f));
                    break;

                case 0x105:
                    val = (int64_t) RishkaVM::arithmeticShiftRightInt64(val1, (val2 & 0x1f));
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
            RishkaSyscall::IO::prints(this);
            break;

        case RISHKA_SC_IO_PRINTN:
            RishkaSyscall::IO::printn(this);
            break;

        case RISHKA_SC_IO_PRINTU:
            RishkaSyscall::IO::printu(this);
            break;

        case RISHKA_SC_IO_PRINTD:
            RishkaSyscall::IO::printd(this);
            break;

        case RISHKA_SC_IO_READCH:
            return RishkaSyscall::IO::readch(this);

        case RISHKA_SC_IO_READLINE:
            return RishkaSyscall::IO::readLine(this);

        case RISHKA_SC_IO_AVAILABLE:
            return RishkaSyscall::IO::available(this);

        case RISHKA_SC_IO_PEEK:
            return RishkaSyscall::IO::peek(this);

        case RISHKA_SC_IO_FIND:
            return RishkaSyscall::IO::find(this);

        case RISHKA_SC_IO_FIND_UNTIL:
            return RishkaSyscall::IO::findUntil(this);

        case RISHKA_SC_IO_SET_TIMEOUT:
            RishkaSyscall::IO::setTimeout(this);
            break;

        case RISHKA_SC_IO_GET_TIMEOUT:
            return RishkaSyscall::IO::getTimeout(this);

        case RISHKA_SC_SYS_DELAY_MS:
            RishkaSyscall::Sys::delayImpl(this);
            break;

        case RISHKA_SC_SYS_MICROS:
            return RishkaSyscall::Sys::microsImpl();

        case RISHKA_SC_SYS_MILLIS:
            return RishkaSyscall::Sys::millisImpl();

        case RISHKA_SC_SYS_SHELLEXEC:
            return RishkaSyscall::Sys::shellExec(this);

        case RISHKA_SC_SYS_EXIT:
            RishkaSyscall::Sys::exit(this);
            break;

        case RISHKA_SC_SYS_INFOS:
            return RishkaSyscall::Sys::infos(this);

        case RISHKA_SC_SYS_INFON:
            return RishkaSyscall::Sys::infon(this);

        case RISHKA_SC_SYS_RANDOM:
            return RishkaSyscall::Sys::randomImpl();

        case RISHKA_SC_SYS_CD:
            return RishkaSyscall::Sys::changeDir(this);

        case RISHKA_SC_SYS_WD:
            return RishkaSyscall::Sys::workingDirectory(this);

        case RISHKA_SC_GPIO_PIN_MODE:
            RishkaSyscall::Gpio::pinModeImpl(this);
            break;

        case RISHKA_SC_GPIO_DIGITAL_READ:
            return RishkaSyscall::Gpio::digitalReadImpl(this);

        case RISHKA_SC_GPIO_DIGITAL_WRITE:
            RishkaSyscall::Gpio::digitalWriteImpl(this);
            break;

        case RISHKA_SC_GPIO_ANALOG_READ:
            return RishkaSyscall::Gpio::analogReadImpl(this);

        case RISHKA_SC_GPIO_ANALOG_WRITE:
            RishkaSyscall::Gpio::analogWriteImpl(this);
            break;

        case RISHKA_SC_GPIO_PULSE_IN:
            return RishkaSyscall::Gpio::pulseInImpl(this);

        case RISHKA_SC_GPIO_PULSE_IN_LONG:
            return RishkaSyscall::Gpio::pulseInLongImpl(this);

        case RISHKA_SC_GPIO_SHIFT_IN:
            return RishkaSyscall::Gpio::shiftInImpl(this);

        case RISHKA_SC_GPIO_SHIFT_OUT:
            RishkaSyscall::Gpio::shiftOutImpl(this);
            break;

        case RISHKA_SC_GPIO_TONE:
            RishkaSyscall::Gpio::toneImpl(this);
            break;

        case RISHKA_SC_GPIO_NO_TONE:
            RishkaSyscall::Gpio::noToneImpl(this);
            break;

        case RISHKA_SC_INT_ENABLE:
            RishkaSyscall::Int::enable();
            break;

        case RISHKA_SC_INT_DISABLE:
            RishkaSyscall::Int::disable();
            break;

        case RISHKA_SC_INT_ATTACH:
            RishkaSyscall::Int::attach(this);
            break;

        case RISHKA_SC_INT_DETACH:
            RishkaSyscall::Int::detach(this);
            break;

        case RISHKA_SC_FS_MKDIR:
            return RishkaSyscall::FS::mkdir(this);

        case RISHKA_SC_FS_RMDIR:
            return RishkaSyscall::FS::rmdir(this);

        case RISHKA_SC_FS_DELETE:
            return RishkaSyscall::FS::remove(this);

        case RISHKA_SC_FS_EXISTS:
            return RishkaSyscall::FS::exists(this);

        case RISHKA_SC_FS_ISFILE:
            return RishkaSyscall::FS::isfile(this);

        case RISHKA_SC_FS_ISDIR:
            return RishkaSyscall::FS::isdir(this);

        case RISHKA_SC_FS_OPEN:
            return RishkaSyscall::FS::open(this);

        case RISHKA_SC_FS_CLOSE:
            RishkaSyscall::FS::close(this);
            break;

        case RISHKA_SC_FS_AVAILABLE:
            return RishkaSyscall::FS::available(this);

        case RISHKA_SC_FS_FLUSH:
            RishkaSyscall::FS::flush(this);
            break;

        case RISHKA_SC_FS_PEEK:
            return RishkaSyscall::FS::peek(this);

        case RISHKA_SC_FS_SEEK:
            return RishkaSyscall::FS::seek(this);

        case RISHKA_SC_FS_SIZE:
            return RishkaSyscall::FS::size(this);

        case RISHKA_SC_FS_READ:
            return RishkaSyscall::FS::read(this);

        case RISHKA_SC_FS_WRITEB:
            RishkaSyscall::FS::writeb(this);
            break;

        case RISHKA_SC_FS_WRITES:
            RishkaSyscall::FS::writes(this);
            break;

        case RISHKA_SC_FS_POS:
            return RishkaSyscall::FS::position(this);

        case RISHKA_SC_FS_PATH:
            return RishkaSyscall::FS::path(this);

        case RISHKA_SC_FS_NAME:
            return RishkaSyscall::FS::name(this);

        case RISHKA_SC_FS_IS_OK:
            return RishkaSyscall::FS::isOk(this);

        case RISHKA_SC_FS_NEXT:
            return RishkaSyscall::FS::next(this);

        case RISHKA_SC_FS_BUFSIZE:
            return RishkaSyscall::FS::bufsize(this);

        case RISHKA_SC_FS_LASTWRITE:
            return RishkaSyscall::FS::lastwrite(this);

        case RISHKA_SC_FS_SEEKDIR:
            return RishkaSyscall::FS::seekdir(this);

        case RISHKA_SC_FS_NEXT_NAME:
            return RishkaSyscall::FS::next_name(this);

        case RISHKA_SC_FS_REWIND:
            RishkaSyscall::FS::rewind(this);
            break;

        case RISHKA_SC_ARG_COUNT:
            return RishkaSyscall::Args::count(this);

        case RISHKA_SC_ARG_STR:
            return RishkaSyscall::Args::value(this);

        case RISHKA_SC_I2C_BEGIN:
            return RishkaSyscall::I2C::begin(this);

        case RISHKA_SC_I2C_END:
            return RishkaSyscall::I2C::end(this);

        case RISHKA_SC_I2C_BEGIN_TRANSMISSION:
            RishkaSyscall::I2C::begin_transmission(this);
            break;

        case RISHKA_SC_I2C_END_TRANSMISSION:
            return RishkaSyscall::I2C::end_transmission(this);

        case RISHKA_SC_I2C_WRITE:
            return RishkaSyscall::I2C::write(this);

        case RISHKA_SC_I2C_SLAVE_WRITE:
            return RishkaSyscall::I2C::slave_write(this);

        case RISHKA_SC_I2C_READ:
            return RishkaSyscall::I2C::read(this);

        case RISHKA_SC_I2C_PEEK:
            return RishkaSyscall::I2C::peek(this);

        case RISHKA_SC_I2C_REQUEST:
            return RishkaSyscall::I2C::request(this);

        case RISHKA_SC_I2C_AVAILABLE:
            return RishkaSyscall::I2C::available(this);

        case RISHKA_SC_I2C_FLUSH:
            RishkaSyscall::I2C::flush();
            break;

        case RISHKA_SC_I2C_ON_RECEIVE:
            RishkaSyscall::I2C::on_receive(this);
            break;
        
        case RISHKA_SC_I2C_ON_REQUEST:
            RishkaSyscall::I2C::on_request(this);
            break;

        case RISHKA_SC_I2C_GET_TIMEOUT:
            return RishkaSyscall::I2C::get_timeout();

        case RISHKA_SC_I2C_SET_TIMEOUT:
            RishkaSyscall::I2C::set_timeout(this);
            break;

        case RISHKA_SC_I2C_SET_CLOCK:
            return RishkaSyscall::I2C::set_clock(this);

        case RISHKA_SC_I2C_GET_CLOCK:
            return RishkaSyscall::I2C::get_clock();

        case RISHKA_SC_I2C_PINS:
            return RishkaSyscall::I2C::pins(this);

        case RISHKA_SC_I2C_BUFSIZE:
            return RishkaSyscall::I2C::bufsize(this);

        case RISHKA_SC_KB_LAYOUT_NAME:
            return RishkaSyscall::Keyboard::layout_name();

        case RISHKA_SC_KB_LAYOUT_DESC:
            return RishkaSyscall::Keyboard::layout_desc();

        case RISHKA_SC_KB_DEVICE_TYPE:
            return RishkaSyscall::Keyboard::device_type();

        case RISHKA_SC_KB_LED_GET_NUM:
            return RishkaSyscall::Keyboard::is_num_lock();

        case RISHKA_SC_KB_LED_GET_CAPS:
            return RishkaSyscall::Keyboard::is_caps_lock();

        case RISHKA_SC_KB_LED_GET_SCROLL:
            return RishkaSyscall::Keyboard::is_scroll_lock();

        case RISHKA_SC_KB_LED_SET_NUM:
            RishkaSyscall::Keyboard::num_lock(this);
            break;

        case RISHKA_SC_KB_LED_SET_CAPS:
            RishkaSyscall::Keyboard::caps_lock(this);
            break;

        case RISHKA_SC_KB_LED_SET_SCROLL:
            RishkaSyscall::Keyboard::scroll_lock(this);
            break;

        case RISHKA_SC_KB_NEXT_SCAN_CODE:
            return RishkaSyscall::Keyboard::next_scan_code(this);

        case RISHKA_SC_KB_LOCK:
            RishkaSyscall::Keyboard::lock(this);
            break;

        case RISHKA_SC_KB_UNLOCK:
            RishkaSyscall::Keyboard::unlock();
            break;

        case RISHKA_SC_KB_RESET:
            RishkaSyscall::Keyboard::reset();
            break;

        case RISHKA_SC_DISPLAY_SCREEN_HEIGHT:
            return RishkaSyscall::Display::screen_height(this);

        case RISHKA_SC_DISPLAY_SCREEN_WIDTH:
            return RishkaSyscall::Display::screen_width(this);

        case RISHKA_SC_DISPLAY_VIEWPORT_HEIGHT:
            return RishkaSyscall::Display::viewport_height(this);

        case RISHKA_SC_DISPLAY_VIEWPORT_WIDTH:
            return RishkaSyscall::Display::viewport_width(this);

        case RISHKA_SC_DISPLAY_SUPPORTED_COLORS:
            return RishkaSyscall::Display::supported_colors(this);

        case RISHKA_SC_NVS_COMMIT:
            return RishkaSyscall::NVS::commit(this);

        case RISHKA_SC_NVS_ERASE_ALL:
            return RishkaSyscall::NVS::erase_all(this);

        case RISHKA_SC_NVS_ERASE:
            return RishkaSyscall::NVS::erase(this);

        case RISHKA_SC_NVS_SET_I8:
            return RishkaSyscall::NVS::set_i8(this);

        case RISHKA_SC_NVS_SET_I16:
            return RishkaSyscall::NVS::set_i16(this);

        case RISHKA_SC_NVS_SET_I32:
            return RishkaSyscall::NVS::set_i32(this);

        case RISHKA_SC_NVS_SET_I64:
            return RishkaSyscall::NVS::set_i64(this);

        case RISHKA_SC_NVS_SET_U8:
            return RishkaSyscall::NVS::set_u8(this);

        case RISHKA_SC_NVS_SET_U16:
            return RishkaSyscall::NVS::set_u16(this);

        case RISHKA_SC_NVS_SET_U32:
            return RishkaSyscall::NVS::set_u32(this);

        case RISHKA_SC_NVS_SET_U64:
            return RishkaSyscall::NVS::set_u64(this);

        case RISHKA_SC_NVS_GET_I8:
            return RishkaSyscall::NVS::get_i8(this);

        case RISHKA_SC_NVS_GET_I16:
            return RishkaSyscall::NVS::get_i16(this);

        case RISHKA_SC_NVS_GET_I32:
            return RishkaSyscall::NVS::get_i32(this);

        case RISHKA_SC_NVS_GET_I64:
            return RishkaSyscall::NVS::get_i64(this);

        case RISHKA_SC_NVS_GET_U8:
            return RishkaSyscall::NVS::get_u8(this);

        case RISHKA_SC_NVS_GET_U16:
            return RishkaSyscall::NVS::get_u16(this);

        case RISHKA_SC_NVS_GET_U32:
            return RishkaSyscall::NVS::get_u32(this);

        case RISHKA_SC_NVS_GET_U64:
            return RishkaSyscall::NVS::get_u64(this);

        case RISHKA_SC_NVS_SET_STRING:
            return RishkaSyscall::NVS::set_string(this);

        case RISHKA_SC_NVS_GET_STRING:
            return RishkaSyscall::NVS::get_string(this);

        case RISHKA_SC_NVS_HAS_WIFI_CONFIG:
            return RishkaSyscall::NVS::has_wifi_config(this);

        case RISHKA_SC_NVS_SET_WIFI_SSID:
            return RishkaSyscall::NVS::set_wifi_ssid(this);

        case RISHKA_SC_NVS_SET_WIFI_PWORD:
            return RishkaSyscall::NVS::set_wifi_passkey(this);

        case RISHKA_SC_WIFI_CONNECT:
            return RishkaSyscall::WiFiDev::connect(this);

        case RISHKA_SC_WIFI_RECONNECT:
            return RishkaSyscall::WiFiDev::reconnect();

        case RISHKA_SC_WIFI_DISCONNECT:
            return RishkaSyscall::WiFiDev::disconnect(this);

        case RISHKA_SC_WIFI_ERASE_AP:
            return RishkaSyscall::WiFiDev::erase_ap();

        case RISHKA_SC_WIFI_IS_CONNECTED:
            return RishkaSyscall::WiFiDev::is_connected();

        case RISHKA_SC_WIFI_SET_AUTO_RECONNECT:
            return RishkaSyscall::WiFiDev::set_autoreconnect(this);

        case RISHKA_SC_WIFI_GET_AUTO_RECONNECT:
            return RishkaSyscall::WiFiDev::is_autoreconnect();

        case RISHKA_SC_WIFI_WAIT_FOR_RESULT:
            return RishkaSyscall::WiFiDev::wait_for_result(this);

        case RISHKA_SC_WIFI_SET_MINSEC:
            RishkaSyscall::WiFiDev::set_min_security(this);
            break;

        case RISHKA_SC_WIFI_SET_SCAN_METHOD:
            RishkaSyscall::WiFiDev::set_scan_method(this);
            break;

        case RISHKA_SC_WIFI_SET_SORT_METHOD:
            RishkaSyscall::WiFiDev::set_sort_method(this);
            break;

        case RISHKA_SC_WIFI_STATUS:
            return RishkaSyscall::WiFiDev::status();

        case RISHKA_SC_WIFI_SSID:
            return RishkaSyscall::WiFiDev::ssid();

        case RISHKA_SC_WIFI_PSK:
            return RishkaSyscall::WiFiDev::psk();
        
        case RISHKA_SC_WIFI_BSSID:
            return RishkaSyscall::WiFiDev::bssid();

        case RISHKA_SC_WIFI_RSSI:
            return RishkaSyscall::WiFiDev::rssi();

        case RISHKA_SC_RT_STRPASS:
            return RishkaSyscall::Runtime::strpass();

        case RISHKA_SC_RT_YIELD:
            RishkaSyscall::Runtime::yield();
            break;

        case RISHKA_SC_RT_FORK_STREAM:
            return RishkaSyscall::Runtime::getForkString(this);

        default:
            this->panic("Invalid system call.");
            break;
    }

    return 0;
}

void RishkaVM::setWorkingDirectory(String directory) {
    this->workingDirectory = String(directory.c_str());
}

String RishkaVM::getWorkingDirectory() const {
    return String(this->workingDirectory.c_str());
}

String RishkaVM::getOutputStream() const {
    return String(this->outputStream.c_str());
}

void RishkaVM::appendToOutputStream(String text) {
    this->outputStream += text;
}

void RishkaVM::appendToOutputStream(char ch) {
    this->outputStream += ch;
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
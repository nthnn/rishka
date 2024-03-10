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

#include <List.hpp>
#include <rishka_types.h>
#include <SD.h>

class RishkaVM final {
private:
    uint64_t registers[32];
    uint8_t memory[RISHKA_VM_STACK_SIZE];

    int64_t pc;
    Stream* stream;

    bool running;
    int64_t exitCode;

    char** argv;
    uint8_t argc;

    uint32_t fetch();
    uint64_t handleSyscall(uint64_t code);
    void execute(uint32_t inst);

    int64_t shiftLeftInt64(int64_t a, int64_t b);
    int64_t shiftRightInt128(int64_t a, int64_t b);
    int64_t shiftRightInt64(int64_t a, int64_t b);
    int64_t arithmeticShiftRightInt64(int64_t a, int64_t b);

public:
    List<File> fileHandles;

    void stopVM();
    void setExitCode(int64_t exitCode);

    int64_t getExitCode() const;
    Stream* getStream() const;

    uint8_t getArgCount() const;
    char* getArgValue(const uint8_t index) const;

    void initialize(Stream* stream);
    void reset();

    bool loadFile(const char* fileName);
    void run(int argc, char** argv);
    void panic(const char* message);

    template<typename T>
    inline constexpr T getParam(const uint8_t pos) const {
        return (T)(((rishka_u64_arrptr*) &this->registers)->a).v[10 + pos];
    }

    template<typename T>
    inline constexpr T getPointerParam(const uint8_t pos) const {
        return (T)(&(((rishka_u8_arrptr*) this->memory)->a).v[
            (((rishka_u64_arrptr*) &this->registers)->a).v[10 + pos]
        ]);
    }
};

#endif
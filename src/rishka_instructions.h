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

#ifndef RISHKA_INSTRUCTIONS_H
#define RISHKA_INSTRUCTIONS_H

enum rishka_instruction {
    RISHKA_OPINST_LOAD   = 0x03,
    RISHKA_OPINST_STORE  = 0x23,
    RISHKA_OPINST_IMM    = 0x13,
    RISHKA_OPINST_IALU   = 0x1b,
    RISHKA_OPINST_RT32   = 0x3b,
    RISHKA_OPINST_RT64   = 0x33,
    RISHKA_OPINST_LUI    = 0x37,
    RISHKA_OPINST_AUIPC  = 0x17,
    RISHKA_OPINST_JAL    = 0x6f,
    RISHKA_OPINST_JALR   = 0x67,
    RISHKA_OPINST_BRANCH = 0x63,
    RISHKA_OPINST_FENCE  = 0x0f,
    RISHKA_OPINST_CALL   = 0x73,
};

enum rishka_load_fc3 {
    RISHKA_FC3_LB,
    RISHKA_FC3_LHW,
    RISHKA_FC3_LW,
    RISHKA_FC3_LDW,
    RISHKA_FC3_LBU,
    RISHKA_FC3_LHU,
    RISHKA_FC3_LRES,
};

enum rishka_store_fc3 {
    RISHKA_FC3_SB,
    RISHKA_FC3_SHW,
    RISHKA_FC3_SW,
    RISHKA_FC3_SDW
};

enum rishka_immediate_fc3 {
    RISHKA_FC3_ADDI,
    RISHKA_FC3_SLLI,
    RISHKA_FC3_SLTI,
    RISHKA_FC3_SLTIU,
    RISHKA_FC3_XORI,
    RISHKA_FC3_SRLI,
    RISHKA_FC3_ORI,
    RISHKA_FC3_ANDI
};

enum rishka_ialu_fc3 {
    RISHKA_FC3_SLLIW,
    RISHKA_FC3_SRLIW,
    RISHKA_FC3_SRAIW
};

enum rishka_branch_fc3 {
    RISHKA_FC3_BEQ,
    RISHKA_FC3_BNE,
    RISHKA_FC3_BLT,
    RISHKA_FC3_BGE,
    RISHKA_FC3_BLTU,
    RISHKA_FC3_BGEU
};

#endif
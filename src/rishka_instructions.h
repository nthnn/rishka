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

/**
 * @file rishka_instructions.h
 * @author [Nathanne Isip](https://github.com/nthnn)
 * @brief Header file defining enumerations for Rishka instructions and their formats.
 *
 * This file defines several enumerations related to Rishka instructions and their formats.
 * The Rishka architecture utilizes these enumerations to represent various types of instructions,
 * such as load, store, immediate arithmetic/logical, integer arithmetic/logical, branch,
 * and other types of instructions. Each enumeration provides symbolic names for specific opcode
 * or funct3 field values, making the code more readable and maintainable.
 */

#ifndef RISHKA_INSTRUCTIONS_H
#define RISHKA_INSTRUCTIONS_H

/**
 * @enum rishka_instruction
 * @brief Enumeration of Rishka instruction opcodes.
 *
 * This enumeration defines symbolic names for each instruction opcode supported by the Rishka architecture.
 * Each opcode represents a specific instruction that can be executed by the Rishka processor.
 */
enum rishka_instruction {
    RISHKA_OPINST_LOAD   = 0x03,    /**< Load instruction. */
    RISHKA_OPINST_STORE  = 0x23,    /**< Store instruction. */
    RISHKA_OPINST_IMM    = 0x13,    /**< Immediate arithmetic/logical instruction. */
    RISHKA_OPINST_IALU   = 0x1b,    /**< Integer arithmetic/logical instruction. */
    RISHKA_OPINST_RT32   = 0x3b,    /**< 32-bit register transfer instruction. */
    RISHKA_OPINST_RT64   = 0x33,    /**< 64-bit register transfer instruction. */
    RISHKA_OPINST_LUI    = 0x37,    /**< Load upper immediate instruction. */
    RISHKA_OPINST_AUIPC  = 0x17,    /**< Add upper immediate to PC instruction. */
    RISHKA_OPINST_JAL    = 0x6f,    /**< Jump and link instruction. */
    RISHKA_OPINST_JALR   = 0x67,    /**< Jump and link register instruction. */
    RISHKA_OPINST_BRANCH = 0x63,    /**< Branch instruction. */
    RISHKA_OPINST_FENCE  = 0x0f,    /**< Fence instruction. */
    RISHKA_OPINST_CALL   = 0x73,    /**< Call instruction. */
};

/**
 * @enum rishka_load_fc3
 * @brief Enumeration of Rishka load instruction funct3 values.
 *
 * This enumeration defines symbolic names for the funct3 field
 * values used in load instructions in the Rishka architecture.
 */
enum rishka_load_fc3 {
    RISHKA_FC3_LB,     /**< Load byte. */
    RISHKA_FC3_LHW,    /**< Load half-word. */
    RISHKA_FC3_LW,     /**< Load word. */
    RISHKA_FC3_LDW,    /**< Load double-word. */
    RISHKA_FC3_LBU,    /**< Load byte unsigned. */
    RISHKA_FC3_LHU,    /**< Load half-word unsigned. */
    RISHKA_FC3_LRES    /**< Reserved load format. */
};

/**
 * @enum rishka_store_fc3
 * @brief Enumeration of Rishka store instruction funct3 values.
 *
 * This enumeration defines symbolic names for the funct3
 * field values used in store instructions in the Rishka
 * architecture.
 */
enum rishka_store_fc3 {
    RISHKA_FC3_SB,     /**< Store byte. */
    RISHKA_FC3_SHW,    /**< Store half-word. */
    RISHKA_FC3_SW,     /**< Store word. */
    RISHKA_FC3_SDW     /**< Store double-word. */
};

/**
 * @enum rishka_immediate_fc3
 * @brief Enumeration of Rishka immediate arithmetic/logical instruction funct3 values.
 *
 * This enumeration defines symbolic names for the funct3 field
 * values used in immediate arithmetic/logical instructions in
 * the Rishka architecture.
 */
enum rishka_immediate_fc3 {
    RISHKA_FC3_ADDI,    /**< Add immediate. */
    RISHKA_FC3_SLLI,    /**< Shift left logical immediate. */
    RISHKA_FC3_SLTI,    /**< Set less than immediate. */
    RISHKA_FC3_SLTIU,   /**< Set less than immediate unsigned. */
    RISHKA_FC3_XORI,    /**< XOR immediate. */
    RISHKA_FC3_SRLI,    /**< Shift right logical immediate. */
    RISHKA_FC3_ORI,     /**< OR immediate. */
    RISHKA_FC3_ANDI     /**< AND immediate. */
};

/**
 * @enum rishka_ialu_fc3
 * @brief Enumeration of Rishka integer arithmetic/logical instruction funct3 values.
 *
 * This enumeration defines symbolic names for the funct3
 * field values used in integer arithmetic/logical instructions
 * in the Rishka architecture.
 */
enum rishka_ialu_fc3 {
    RISHKA_FC3_SLLIW,  /**< Shift left logical immediate word. */
    RISHKA_FC3_SRLIW,  /**< Shift right logical immediate word. */
    RISHKA_FC3_SRAIW,  /**< Shift right arithmetic immediate word. */
    RISHKA_FC3_SLLI64, /**< Shift left logical immediate for 64-bit integers. */
    RISHKA_FC3_SRLI64, /**< Shift right logical immediate for 64-bit integers. */
    RISHKA_FC3_SRAI64  /**< Shift right arithmetic immediate for 64-bit integers. */
};

/**
 * @enum rishka_branch_fc3
 * @brief Enumeration of Rishka branch instruction funct3 values.
 *
 * This enumeration defines symbolic names for the funct3
 * field values used in branch instructions in the Rishka
 * architecture.
 */
enum rishka_branch_fc3 {
    RISHKA_FC3_BEQ  = 0x00,  /**< Branch if equal. */
    RISHKA_FC3_BNE  = 0x01,  /**< Branch if not equal. */
    RISHKA_FC3_BLT  = 0x04,  /**< Branch if less than. */
    RISHKA_FC3_BGE  = 0x05,  /**< Branch if greater than or equal. */
    RISHKA_FC3_BLTU = 0x06,  /**< Branch if less than unsigned. */
    RISHKA_FC3_BGEU = 0x07   /**< Branch if greater than or equal unsigned. */
};

#endif /* RISHKA_INSTRUCTIONS_H */
/* 
 * This file is part of the Rishka distribution (https://github.com/rishka-esp32/rishka-sdk).
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

.section .init, "ax"
.global _start

_start:
    # Load global pointer
    la      gp, __global_pointer$

    # Initialize arguments for main function
    li      a1,0    # Argument 1: Not used (argc)
    li      a0,0    # Argument 0: Not used (argv)

    # Call the main function
    jal     ra, main

    # Call the exit system call
    jal     ra, _exit

_exit:
    # Allocate space for the stack frame
    addi    sp,sp,-32
    sd      s0,24(sp)
    addi    s0,sp,32

    # Move the return value of main to a5
    mv      a5,a0

    # Store the return value on the stack
    sw      a5,-20(s0)

    # Load the return value from the stack
    lw      a5,-20(s0)

    # Set up the arguments for the exit system call
    mv      a0,a5   # Argument 0: Return value from main
    li      a7,15   # Rishka system call number for exit

    # Perform the exit system call
    scall

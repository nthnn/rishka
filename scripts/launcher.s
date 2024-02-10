.section .init, "ax"
.global _start

_start:
    la      gp, __global_pointer$
    li      a1,0
    li      a0,0
    jal     ra, main
    jal     ra, _exit

_exit:
    addi    sp,sp,-32
    sd      s0,24(sp)
    addi    s0,sp,32
    mv      a5,a0
    sw      a5,-20(s0)
    lw      a5,-20(s0)
    mv      a0,a5
    li      a7,8
    scall

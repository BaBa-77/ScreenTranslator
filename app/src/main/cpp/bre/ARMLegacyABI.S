.section .text, "x"

.global __ARM_ll_addlu
.global __ARM_ll_cmpu
.global __ARM_scalbn
.global __ARM_ll_addls
.global __ARM_ll_muluu
.global __ARM_ll_sublu

__ARM_scalbn:
    B scalbn

__ARM_ll_addlu:
    ADDS r0, r0, r2
    ADC r1, r1, #0
    BX lr

__ARM_ll_cmpu:
    CMP r1, r3
    CMPEQ r0, r2
    BX lr

__ARM_ll_addls:
    ADDS r0, r0, r2
    ADC r1, r1, r2,ASR#31
    BX lr

__ARM_ll_muluu:
    MOV R2, R1
    UMULL R0, R1, R2, R0
    BX LR

__ARM_ll_sublu:
    SUBS R0, R0, R2
    SBC R1, R1, #0
    BX LR

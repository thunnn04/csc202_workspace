    .thumb
    .thumb_func
    .global my_asm_bitset
    .global my_asm_bitclr
    .global my_asm_bitcheck

my_asm_bitset:
    ORRS R0, R0, R1
    BX LR

my_asm_bitclr:
    BICS R0, R0, R1
    BX LR

my_asm_bitcheck:
    ANDS    R0,
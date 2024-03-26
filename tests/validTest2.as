.entry START
    .extern Z
    .define size = 5
    .define maximum = 20
MAIN: mov r2,START
LOOP1: jmp L2
prn #-4
bne Z
sub r1,r3
    bne L4
L2: inc J
    .entry LOOP1
jmp Z
END1: hlt
STR1: .string "This is a valid string"
START: .data 8,-12,maximum
J: .data 33
    .extern L4


LOOP2: cmp #size,r6
    dec r5
    not r2
    clr r6
    add #maximum,r5
    red r1
    jsr LOOP2
rts
END2: hlt
STR2: .string "check this out!"
DATA2: .data 10,-15,25
L: .data 44
    .extern L5
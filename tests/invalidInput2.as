.entry START
    .extern EXTERN
MAIN: mov r1,r2
    LOOP: cmp -5,r3
bne ENDLOOP1
add r3,R0
jsr SUBROUTINE
prn STR
lea ARR,r5
SUBROUTINE: bne EXTERNVAR
halt
ENDLOOP: dec K
jmp LOOP
STA$RT: sub r2,r8
    clr STR
red r7
    halttttttt
not r2
    inc R0
bne MAIN
EXTERNVAR: .data 100000000000000000000000000
STR: .string "Hello,World!"""""
ARR: .data 1,2,3,4.5
K: .data 10,gg
R0: .data 15,#17
ThisLabelIsTooLongToBe_A_ValidLabelNameAndWillCauseAnError: mov r5, r7

;This will cause an error because of the '.' between 4 and 5
L1: .data 1,2,3,4,5.6,7,8,9,10
;This will cause an error because of a missing " after the string
S1: .string "Hello World"


.data "Hello World"
    .data 1,#2,3,4,5,6

    ;This is a general macro
    mcr mymcr
    add r1, r2
    sub r2, r2
    lea r4, r5
    endmcr

        ;This is a valid constant definition
.define len = 5

;All three will raise errors
LIST5: .data 6, -9 len
LIST6: .data 2048, -9, len
LIST7: .data -2049, -9, len

;All three will raise errors
    .define sz = 4+4
    .define sz = 4+4+4
    .define sz = 4*4

mymcr

;Invalid operand
hlt r5

;Invalid operands
cmp r3,r2, ARR[2]
cmp r3, ARR[2]r2
cmp r3, ARR[2] r2
cmp r3,
    cmp r3
cmp
cmp ,

;Invalid operands
    not , r3
lea r3, STR
lea r3, STR[2], r2
inc STR[sz],  r7
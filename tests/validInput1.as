;This is a test file that demonstrates many different operations and features of the assembler.
.entry Main
    .extern ExternalFunc
    .define addValue = 5
    .define subValue = 3
    .define maxNumber = 2047

Main:   mov #100, r1
clr r2

; Macro for arithmetic operations on r1
mcr ArithmeticOps
add #addValue, r1
sub #subValue, r1
endmcr

; Macro for checking and branching
mcr CheckAndBranch
cmp #150, r1
bne NotEqual
jmp Equal
endmcr

; Main program logic using macros
ArithmeticOps
CheckAndBranch

Equal:   prn r1
jmp EndProgram

NotEqual: prn #999
sub #maxNumber, r1
jmp L3

EndProgram: hlt

; Data section for demonstration
    Numbers:  .data 10, 20, 30
Message:  .string "Testing Macros"

; External and entry declarations for demonstration
                                          .extern L3
    .entry Numbers
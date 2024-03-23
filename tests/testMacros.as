; Example file loaded with different macro definitions and usage
; There are 2 extern declarations here, but no extern file will be created because they are not used in the program
; Demonstrates the use of macros for arithmetic operations and simple control flow
    .entry Start
    .extern W
    .extern STR
    .extern LOOP
    .define incrementValue = 3
    .define decrementValue = 1
    .define offset = 2

Start:  mov #100, r3
    sub #20, r3
clr r4

mcr ArithmeticOps
add #incrementValue, r3
sub #decrementValue, r3
inc r4
dec r4
endmcr

mcr LogicalOps
not r3
clr r6
mov r3, r6
endmcr

mcr ControlFlow
cmp r3, r4
bne Start
endmcr

mcr Different
prn r3
jmp Start
endmcr

mcr DataManipulation
lea END, r1
mov LIST[offset], r2
endmcr


; Main program logic
ArithmeticOps
LogicalOps
DataManipulation
ControlFlow
Different

END: hlt

; Data section for potential use in operations
    .data 5, -3, 10
    .string "Macro Test"
    .extern L3
LIST: .data 6, 0, -9

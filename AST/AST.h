#ifndef AST_H
#define AST_H

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#define MAXERRORDESCRIPTION 256 /*Maximum number of characters in an error description*/
#define MAXNAME 31 /*Maximum number of label name characterse*/
#define MAXNUMBERDATA 256 /*Maximum number of data in a data instruction*/

/**
 * This struct will define the Abstract Syntax Tree (AST) for the assembler.
 */
typedef struct AST{
    /*int createSymbol;*/ /*TODO is this needed? meaning do i need an int to signify if i created a new symbol*/
    char errorDescription[MAXERRORDESCRIPTION];
    char name[MAXNAME];
    enum{
        command,
        instruction,
        definition
    }wordType;
    union {
        struct { /*Instruction*/
            enum {
                string_instruction,
                label_instruction,
                data_instruction,
                entry_instruction,
                extern_instruction,
                define_instruction
            } instructionType;
                union { /*Choose either string OR data OR define OR entry/extern TODO does it actually need to be union?*/
                    struct {
                        char *label_name;
                        char *string;
                    } string;
                    struct {
                        char *data_name;
                        int data[MAXNUMBERDATA];
                        int dataCounter;
                    } data;
                    struct {
                        char *define_name;
                        int value;
                    } define;
                    union {
                        char *entryLabelName;
                        char *externLabelName;
                    } entryOrExtern;
                }instructionDetails;
        } instruction;
        struct { /*command*/
            enum {
                mov, cmp, add, sub, lea, not, clr, inc, dec, jmp, bne, red, prn, jsr, rts, hlt
            } opcode;
            struct {
                enum {
                    noOperand,
                    oneOperand,
                    twoOperands
                } numOfOperands;
                union { /*TODO update this part to create a correct command struct, something's wrong ( i need label AND registers and stuff etc.*/
                    char *label;
                    int num;
                    int reg;
                } operand;
            } operands[2];
        } command;
    }wordSummary;
}AST;

#endif

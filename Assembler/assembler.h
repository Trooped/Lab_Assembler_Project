
#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "errno.h"

#define OPERATIONS 16 /*Number of operations*/
#define WORDSIZE 14 /*Word size*/
#define MAXDATA 1024 /*Maximum number of data*/
#define MAXINSTRUCTIONS 1024 /*Maximum number of instructions*/
#define MAXCHARSPERWORD 30 /*Maximum number of characters in a word*/
#define MAXCHARSPERLINE 80 /*Maximum number of characters in a line, not including null terminator \0 */
#define MAXLABELNAME 31 /*Maximum number of characters in a label name*/
#define MAXOPERANDS 4 /*Maximum number of operands in an instruction + 2 spaces for error handling*/
#define MAXOPERANDLENGTH 32 /*Maximum number of characters in an operand*/


typedef struct word{
    int wordBits : WORDSIZE;
}word;

typedef struct operations{
    char* name;
    int opcode;
    int numOfOperands;
}operation;


#endif /*_ASSEMBLER_H*/

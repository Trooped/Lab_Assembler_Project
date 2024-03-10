
#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "errno.h"
#include "ctype.h"
#include "firstPass.h"

#define OPERATIONS 16 /*Number of operations*/
#define WORDSIZE 14 /*Word size*/
#define MAXDATA 1024 /*Maximum number of data*/
#define MAXINSTRUCTIONS 1024 /*Maximum number of instructions*/
#define MAXDATAVALUESINARRAY 36 /*Maximum number of data values in an array*/
#define MAXCHARSPERLINE 80 /*Maximum number of characters in a line, not including null terminator \0 */
#define MAXLABELNAME 31 /*Maximum number of characters in a label name*/
#define MAXOPERANDS 4 /*Maximum number of operands in an instruction + 2 spaces for error handling*/
#define MAXOPERANDLENGTH 32 /*Maximum number of characters in an operand*/
#define MININTEGER -2048
#define MAXINTEGER 2048


typedef struct word{
    int wordBits : WORDSIZE;
}word;

typedef struct operations{
    char* name;
    int opcode;
    int numOfOperands;
}operation;


#endif /*_ASSEMBLER_H*/


#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "errno.h"
#include "ctype.h"
#include "firstPass.h"

#define OPERATIONS 16 /*Number of operations*/
#define MAXFILENAME 64 /*Maximum file name length*/ /*TODO change the definitions that are more global to one file, maybe inside main.h??*/
#define MAXNAME 32 /*Maximum name length*/ /*TODO order in those fucking defines omg*/
#define WORDSIZE 14 /*Word size*/
#define MAXDATA 2048 /*Maximum number of data in the array*/
#define MAXINSTRUCTIONS 2048 /*Maximum number of instructions in the array*/
#define MAXDATAVALUESINARRAY 36 /*Maximum number of data values in an array*/
#define MAXCHARSPERLINE 80 /*Maximum number of characters in a line, not including null terminator \0 */
#define MAXLABELNAME 31 /*Maximum number of characters in a label name*/
#define MAXOPERANDS 4 /*Maximum number of operands in an instruction + 2 spaces for error handling*/
#define MAXOPERANDLENGTH 32 /*Maximum number of characters in an operand*/
#define MININTEGER -2048
#define MAXINTEGER 2048
#define MAXERRORDESCRIPTION 64 /*Maximum number of characters in an error description*/

typedef struct symbolList {
    char name[MAXNAME]; /* Label name*/
    char type[MAXNAME]; /*define, data, extern, entry or code*/
    int value; /* Label value*/
    struct symbolList* next; /* Pointer to the next node*/
} symbolList;


typedef struct word{
    int wordBits : WORDSIZE;
}word;

typedef struct operations{
    char* name;
    int opcode;
    int numOfOperands;
}operation;

typedef struct errorInfo{
    int errorFlag;
    char *fileName;
    char errorDescription[MAXERRORDESCRIPTION]; /*TODO maybe turn this into an array of chars with fixed size?*/
}error;

void initializeDataArray(word* dataArray, int initialValue);

void initializeInstructionArray(word* instructionArray, int initialValue);


#endif /*_ASSEMBLER_H*/


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


typedef struct word{
    int wordBits : WORDSIZE;
}word;

typedef struct operations{
    char* name;
    int opcode;
    int numOfOperands;
}operation;


#endif /*_ASSEMBLER_H*/

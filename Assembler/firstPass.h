#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include "../PreAsmblr/PreAsmblr.h"

#define MAXICDC 2056 /*Maximum number of instructions and data counter*/


#ifndef firstPass_H
#define firstPass_H

typedef struct symbolList {
    char name[MAXNAME]; /* Label name TODO define MAXNAME already omg*/
    char type[MAXNAME]; /*define, data, or code*/
    int value; /* Label value*/
    struct symbolList* next; /* Pointer to the next node*/
} symbolList;

typedef struct validData{
    char* type;
    int valid;
    int dataCounter;
}validData;














#endif



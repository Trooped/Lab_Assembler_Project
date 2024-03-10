#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include "../PreAsmblr/PreAsmblr.h"
#include "assembler.h"
#include "syntaxAnalyzer.h"
#include "utilities.h"


#ifndef firstPass_H
#define firstPass_H

typedef struct symbolList {
    char name[MAXNAME]; /* Label name TODO define MAXNAME already omg*/
    char type[MAXNAME]; /*define, data, or code*/
    int value; /* Label value*/
    struct symbolList* next; /* Pointer to the next node*/
} symbolList;

void firstPass(FILE *sourceFile, word *dataArray, word *instructionArray, operation *operationsArray, symbolList *symbolTable, symbolList *externalSymbols, symbolList *entrySymbols, int *IC, int *DC);

int handleOperation(symbolList** head, word** instructionArray, int operation, char* line, int *IC, operation* operationsArray);

void handleData(char* type, char* line, symbolList ** head, int *DC, word** dataArray);

void handleExtern(symbolList** head, char* line);

void handleDefine(symbolList** head, operation* operationsArray, char* line);













#endif



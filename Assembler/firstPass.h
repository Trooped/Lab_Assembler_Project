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

void firstPass(FILE *sourceFile, word *dataArray, word *instructionArray, operation *operationsArray, symbolList** symbolTable, int *IC, int *DC, error** errorInfo);

int handleOperation(symbolList** head, word* instructionArray, int operation, char* line, int *IC, operation* operationsArray, error** errorInfo);

void handleData(char* type, char* line, symbolList ** head, int *DC, word* dataArray, error** errorInfo);

void handleExtern(symbolList** head, char* line);

void handleDefine(symbolList** head, operation* operationsArray, char* line, error** errorInfo);













#endif



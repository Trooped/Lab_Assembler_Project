
#ifndef firstPass_H
#define firstPass_H

#include "dataStructuresAndConstants.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include "syntaxAnalyzer.h"
#include "utilities.h"


void firstPass(FILE *sourceFile, binaryWord *dataArray, binaryWord *instructionArray, operationInfo *operationsArray, symbolList** symbolTable, int *IC, int *DC, error** errorInfo);

int handleOperation(symbolList** head, binaryWord* instructionArray, int operation, char* line, int *IC, operationInfo* operationsArray, error** errorInfo);

void handleData(char* type, char* line, symbolList ** head, int *DC, binaryWord* dataArray, error** errorInfo);

void handleExtern(symbolList** head, char* line, error** errorInfo, operationInfo* operationsArray);

int handleDefine(symbolList** head, operationInfo* operationsArray, char* line, error** errorInfo);





#endif



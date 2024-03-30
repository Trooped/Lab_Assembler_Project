
#ifndef firstPass_H
#define firstPass_H

#include "dataStructuresAndConstants.h"
#include "error.h"
#include "listFunctions.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include "syntaxAnalyzer.h"
#include "utilities.h"


void firstPass(FILE *sourceFile, memoryWord *dataArray, memoryWord *instructionArray, operationInfo *operationsArray, symbolList** symbolTable, int *IC, int *DC, error** errorInfo);




#endif



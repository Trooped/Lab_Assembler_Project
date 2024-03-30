#ifndef _SECONDPASS_H
#define _SECONDPASS_H

#include "dataStructuresAndConstants.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include "syntaxAnalyzer.h"
#include "utilities.h"
#include "firstPass.h"

void secondPass(FILE *sourceFile, memoryWord *dataArray, memoryWord *instructionArray, operationInfo *operationsArray, symbolList** symbolTable, int *IC, int *DC, error** errorInfo);


#endif

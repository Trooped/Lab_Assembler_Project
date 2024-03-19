#ifndef _ERROR_H
#define _ERROR_H

#include "dataStructuresAndConstants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listFunctions.h"

void printError(error** errorInfo, char* errorDescription);

void initializeErrorInfo(error** errorInfo,symbolList** symbolTable, char* fileName, FILE* file);

void closeFileAndExit(error** errorInfo, symbolList** symbolTable);


#endif


#ifndef _FILEFUNCTIONS_H
#define _FILEFUNCTIONS_H

#include "dataStructuresAndConstants.h"
#include "utilities.h"
#include <stdio.h>
#include "dataConversion.h"
#include "error.h"
#include "preAsmblr.h"

/**
 * This function will create a new file to copy macros from the old file into the new file.
 * @param source the old file
 * @param oldFileName the old file name
 * @return the new file
 */
FILE* createFileWithMacros(FILE* source, const char* oldFileName, error** errorInfo);



void createEntFile(symbolList** head, char* fileName, error** errorInfo);

void createExtFile(symbolList** head, char* fileName, error** errorInfo);

void createObjectFile(binaryWord* dataArray, binaryWord* instructionArray, int IC, int DC, char* fileName, error** errorInfo, symbolList** symbolTable);



#endif

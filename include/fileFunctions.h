#ifndef FILEFUNCTIONS_H
#define FILEFUNCTIONS_H

#include "dataStructuresAndConstants.h"
#include "utilities.h"
#include <stdio.h>
#include "dataConversion.h"
#include "error.h"
#include "preAsmblr.h"

/**
 * This function will create a new file to copy macros from the old file into the new file.
 * @param sourceFile the old file
 * @param oldFileName the old file name
 * @return the new file
 */
FILE* createFileWithMacros(FILE* sourceFile, const char* oldFileName);

void createEntFile(symbolList** head, char* fileName, error** errorInfo);

void createExtFile(symbolList** head, char* fileName, error** errorInfo);

void createObjectFile(binaryWord* dataArray, binaryWord* instructionArray, int IC, int DC, char* fileName, error** errorInfo, symbolList** symbolTable);



#endif

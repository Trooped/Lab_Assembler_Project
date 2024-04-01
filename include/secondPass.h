/**
 * @file secondPass.h
 * This file contains the function that handles the second pass of the assembler.
 */
#ifndef _SECONDPASS_H
#define _SECONDPASS_H

#include "dataStructuresAndConstants.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include "syntaxAnalyzer.h"
#include "firstPass.h"

/**
 * This function is the second pass of the assembler. It goes through the source file and creates the binary words for the instructions.
 * In the end of the second pass function, the program checks if the program is too large for the memory we have, and if it is- ends the program prematurely.
 * @param sourceFile The source file to be read.
 * @param dataArray The array to store the data.
 * @param instructionArray The array to store the instructions.
 * @param operationsArray The array to store the operations.
 * @param symbolTable The symbol table.
 * @param IC The instruction errorCounter.
 * @param DC The data errorCounter.
 * @param errorInfo The error struct.
 */
void secondPass(FILE *sourceFile, memoryWord *dataArray, memoryWord *instructionArray, operationInfo *operationsArray, symbolList** symbolTable, int *IC, int *DC, error** errorInfo);


#endif

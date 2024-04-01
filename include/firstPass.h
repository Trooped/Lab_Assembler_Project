/**
 * @file firstPass.h
 * This file contains the function that handles the first pass of the assembler.
 */

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

/**
 * This function is the first pass of the assembler. It goes through the source file and creates the symbol table, and the data and initial instruction arrays.
 * @param sourceFile The source file to be read.
 * @param dataArray The array to store the data.
 * @param instructionArray The array to store the instructions.
 * @param operationsArray The array to store the operations.
 * @param symbolTable The symbol table.
 * @param IC The instruction errorCounter.
 * @param DC The data errorCounter.
 * @param errorInfo The error struct.
 */
void firstPass(FILE *sourceFile, memoryWord *dataArray, memoryWord *instructionArray, operationInfo *operationsArray, symbolList** symbolTable, int *IC, int *DC, error** errorInfo);




#endif



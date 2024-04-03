/**
 * @file fileFunctions.h
 * This file contains the functions that handle file operations in the assembler.
 *
 * The functions in this file are:
 * 1. createFileWithMacros - This function creates a new file to copy macros from the old file into the new file.
 * 2. createObjectFile - This function creates the object file.
 * 3. createEntFile - This function creates the entry file.
 * 4. createExtFile - This function creates the external file.
 */

#ifndef FILEFUNCTIONS_H
#define FILEFUNCTIONS_H

#include "preAsmblr.h"
#include "dataStructuresAndConstants.h"
#include <stdio.h>
#include "dataConversion.h"
#include "error.h"

/**
 * This function will create a new file to copy macros from the old file into the new file.
 * @param sourceFile the old file
 * @param oldFileName the old file name
 * @return the new file
 */
FILE* createFileWithMacros(FILE* sourceFile, const char* oldFileName);

/**
 * This function will create the object file.
 * @param dataArray the data array
 * @param instructionArray the instruction array
 * @param IC the instruction counter
 * @param DC the data counter
 * @param fileName the file name
 */
void createObjectFile(memoryWord* dataArray, memoryWord* instructionArray, int IC, int DC, char* fileName, error** errorInfo, symbolList** symbolTable);

/**
 * This function will create the entry file.
 * @param head the symbol table
 * @param fileName the file name
 */
void createEntFile(symbolList** head, char* fileName, error** errorInfo);

/**
 * This function will create the external file.
 * It'll only be called if there are external symbols in the symbol table, which are used in the source code.
 * @param head the symbol table
 * @param fileName the file name
 */
void createExtFile(symbolList** head, char* fileName, error** errorInfo);




#endif

/**
 * @file error.h
 * This file contains the functions that handle errors in the program.
 *
 * The functions in this file are:
 * 1. printError - This function prints an error message and updates the errorInfo struct accordingly.
 * 2. initializeErrorInfo - This function initializes the errorInfo struct to NULL, with the file name and file pointer.
 * 3. closeFileAndExit - This function closes the file and frees the memory before exiting the program.
 */

#ifndef _ERROR_H
#define _ERROR_H

#include "dataStructuresAndConstants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listFunctions.h"

/**
 * This function prints an error message and updates the errorInfo struct accordingly.
 *
 * @param errorInfo A pointer to the errorInfo struct.
 * @param errorDescription The description of the error.
 */
void printError(error** errorInfo, char* errorDescription);

/**
 * This function initializes the errorInfo struct to NULL, with the file name and file pointer.
 *
 * @param errorInfo A pointer to the errorInfo struct.
 * @param symbolTable A pointer to the symbol table.
 * @param fileName The name of the file.
 * @param file The file pointer.
 */
void initializeErrorInfo(error** errorInfo,symbolList** symbolTable, char* fileName, FILE* file);

/**
 * This function closes the file and frees the memory before exiting the program.
 *
 * @param errorInfo A pointer to the errorInfo struct.
 * @param symbolTable A pointer to the symbol table.
 */
void closeFileAndExit(error** errorInfo, symbolList** symbolTable);


#endif


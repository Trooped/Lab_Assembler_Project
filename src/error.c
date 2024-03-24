/**
 * @file error.c
 * This file contains the implementation of the error handling functions.
 *
 * The functions in this file are:
 * 1. printError - This function prints an error message and updates the errorInfo struct accordingly.
 * 2. initializeErrorInfo - This function initializes the errorInfo struct to NULL, with the file name and file pointer.
 * 3. closeFileAndExit - This function closes the file and frees the memory before exiting the program.
 */

#include "include/error.h"


/**
 * This function prints an error message and updates the errorInfo struct accordingly.
 *
 * @param errorInfo A pointer to the errorInfo struct.
 * @param errorDescription The description of the error.
 */
void printError(error** errorInfo, char* errorDescription){
    (*errorInfo)->errorCounter++; /* Increment the error counter*/
    (*errorInfo)->errorFlag = 1; /* Set the error flag*/
    if ((*errorInfo)->lineText[strlen((*errorInfo)->lineText) - 1] == '\n') { /* Check if the line ends with a newline character*/
        (*errorInfo)->lineText[strlen((*errorInfo)->lineText) - 1] = '\0'; /* Remove the newline character*/
    }
    /* Print the detailed error message*/
    fprintf(stderr,"Error #%d found in file '%s', line %d:\n'%s'\nerror description: %s\n\n", (*errorInfo)->errorCounter, (*errorInfo)->fileName,(*errorInfo)->lineCounter, (*errorInfo)->lineText, errorDescription);
}


/**
 * This function initializes the errorInfo struct to NULL, with the file name and file pointer.
 *
 * @param errorInfo A pointer to the errorInfo struct.
 * @param symbolTable A pointer to the symbol table.
 * @param fileName The name of the file.
 * @param file The file pointer.
 */
void initializeErrorInfo(error** errorInfo,symbolList** symbolTable, char* fileName, FILE* file) {
    *errorInfo = malloc(sizeof(error)); /* Allocate memory for the errorInfo struct*/
    if (*errorInfo == NULL) { /* Check if the memory allocation was successful*/
        fprintf(stderr, "Failed to allocate memory for errorInfo\n");
        closeFileAndExit(errorInfo, symbolTable); /* Close the file and free the memory*/
    }
    /* Initialize errorInfo*/
    (*errorInfo)->errorFlag = 0; /* Set the error flag to 0*/
    (*errorInfo)->errorCounter = 0; /* Set the error counter to 0*/
    strcpy((*errorInfo)->fileName, fileName);
    (*errorInfo)->file = NULL; /*set the file pointer to NULL*/
    (*errorInfo)->lineText[0] = '\0'; /* Set the line text to an empty string*/
    (*errorInfo)->lineCounter = 0; /* Set the line counter to 0*/
}

/**
 * This function closes the file and frees the memory before exiting the program.
 *
 * @param errorInfo A pointer to the errorInfo struct.
 * @param symbolTable A pointer to the symbol table.
 */
void closeFileAndExit(error** errorInfo, symbolList** symbolTable) {
    if ((*errorInfo)->file != NULL) { /* Check if the file pointer is not NULL*/
        fclose((*errorInfo)->file); /* Close the file*/
    }
    if ((*errorInfo) != NULL) {
        free(*errorInfo); /* Free the errorInfo struct*/
    }
    if ((*symbolTable) != NULL) {
        deleteSymbolList(symbolTable); /* Free the symbol table*/
    }
    exit(1);
}


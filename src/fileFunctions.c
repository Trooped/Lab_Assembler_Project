/**
 * @file fileFunctions.c
 *
 * This file contains the functions that are used to create the output files, and to create a new file with the macros from the old file.
 * These functions are used to create the object file, the entry file, and the external file, and to create a new file with the macros from the old file.
 * The object file contains the binary representation of the instructions and the data.
 * The entry file contains the names of the entry symbols and their addresses.
 * The external file contains the names of the external symbols and their addresses
 *
 * The functions in this file are:
 * 1. createFileWithMacros - This function creates a new file to copy macros from the old file into the new file.
 * 2. createObjectFile - This function creates the object file.
 * 3. createEntFile - This function creates the entry file.
 * 4. createExtFile - This function creates the external file.
 */

#include "include/fileFunctions.h"


/**
 * This function will create a new file to copy macros from the old file into the new file.
 * @param source the old file
 * @param oldFileName the old file name
 * @return the new file
 */
FILE* createFileWithMacros(FILE* source, const char* oldFileName, error** errorInfo) {
    char newFileName[MAXFILENAME] = {0};
    FILE * resultFile;

    strcpy(newFileName, oldFileName);/*copy the old file name to a new string*/

    sprintf(newFileName, "%s.am", oldFileName); /*add the suffix to the new file name*/

    resultFile = fopen(newFileName, "w+");
    if (resultFile == NULL) {
        fprintf(stderr, "Error opening file '%s': %s\n", newFileName, strerror(errno));
        (*errorInfo)->errorFlag = 1;
        return NULL;
    }

    processFileLines(source, resultFile);

    return resultFile;
}


void createObjectFile(binaryWord* dataArray, binaryWord* instructionArray, int IC, int DC, char* fileName, error** errorInfo, symbolList** symbolTable){
    FILE* objectFile;
    char tempFileName[MAXFILENAME];
    int i;
    sprintf(tempFileName, "%s.ob", fileName);
    objectFile = fopen(tempFileName, "w");
    if (objectFile == NULL) {
        printf("Failed to open file %s\n", tempFileName);
        closeFileAndExit(errorInfo, symbolTable);
    }

    fprintf(objectFile, "%d %d\n", IC, DC);
    for (i = 0; i < IC; i++) {
        char* base4String = convertBinaryToBase4Symbols(instructionArray[i].wordBits);
        if (base4String != NULL) {
            fprintf(objectFile, "0%d %s\n", i+INITIAL_IC_VALUE, base4String);
            free(base4String);
        } else {
            fprintf(stderr, "Memory allocation failed for value at index %d.\n", i);
            break;
        }
    }
    for (i = 0; i < DC; i++) {
        char* base4String = convertBinaryToBase4Symbols(dataArray[i].wordBits);
        if (base4String != NULL) {
            fprintf(objectFile, "0%d %s\n", i+IC+INITIAL_IC_VALUE, base4String);
            free(base4String);
        } else {
            fprintf(stderr, "Memory allocation failed for value at index %d.\n", i);
            break;
        }
    }
    fclose(objectFile);
}


void createEntFile(symbolList** head, char* fileName, error** errorInfo) {
    char tempFileName[MAXFILENAME];
    symbolList* current;
    FILE* entFile;
    sprintf(tempFileName, "%s.ent", fileName);
    entFile = fopen(tempFileName, "w");
    if (entFile == NULL) {
        printf("Failed to open file %s\n", tempFileName);
        closeFileAndExit(errorInfo, head);
    }

    current = *head;
    while (current != NULL) {
        if (current->isEntry == 1) {
            fprintf(entFile, "%s 0%d\n", current->name, current->value);
        }
        current = current->next;
    }
    fclose(entFile);
}

void createExtFile(symbolList** head, char* fileName, error** errorInfo) {
    char tempFileName[MAXFILENAME];
    int i;
    symbolList* current;
    FILE* extFile;
    sprintf(tempFileName, "%s.ext", fileName);
    extFile = fopen(tempFileName, "w");
    if (extFile == NULL) {
        printf("Failed to open file %s\n", tempFileName);
        closeFileAndExit(errorInfo, head);
    }

    current = *head;
    while (current != NULL) {
        if (strcmp(current->type, "external") == 0) {
            i = 0;
            while (current->externalAddresses[i] != -1) {
                fprintf(extFile, "%s\t0%d\n", current->name, current->externalAddresses[i]);
                i++;
            }
        }
        current = current->next;
    }
    fclose(extFile);
}

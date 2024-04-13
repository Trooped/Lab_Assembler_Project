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

#include "fileFunctions.h"


/**
 * This function will create a new file to copy macros from the old file into the new file.
 * @param sourceFile the old file
 * @param oldFileName the old file name
 * @return the new file
 */
FILE* createFileWithMacros(FILE* sourceFile, const char* oldFileName) {
    char newFileName[MAX_FILE_NAME] = {0}; /*the new file name*/
    FILE * resultFile; /*the new file*/
    error* error; /*Empty error struct for the preAsmblr*/

    sprintf(newFileName, "%s.am", oldFileName); /*add the suffix to the new file name*/
    initializeErrorInfo(&error, NULL, newFileName, NULL); /*initialize empty errorInfo struct for the preAsmblr*/

    resultFile = fopen(newFileName, "w+"); /*open the new file*/
    if (resultFile == NULL) { /*if the file couldn't be opened, print an error message*/
        fprintf(stderr, "Error opening file '%s': %s\n", newFileName, strerror(errno));
        free(error); /*free the error struct*/
        return NULL;
    }
    /*process the lines of the file*/
    expandMacrosAndWriteToNewFile(sourceFile, resultFile, error);

    /*If there are errors in pre-assembly, stop the run*/
    if (error->errorFlag == TRUE) {
        fprintf(stderr, "Errors were found in the pre-assembly process, exiting the process\n\n");
        free(error); /*free the error struct*/
        fclose(resultFile); /*close the file*/
        return NULL;
    }

    free(error); /*free the error struct*/
    return resultFile; /*return the new result file*/
}


/**
 * This function will create the object file.
 * @param dataArray the data array
 * @param instructionArray the instruction array
 * @param IC the instruction counter
 * @param DC the data counter
 * @param fileName the file name
 */
void createObjectFile(memoryWord* dataArray, memoryWord* instructionArray, int IC, int DC, char* fileName, error** errorInfo, symbolList** symbolTable){
    FILE* objectFile; /*the object file*/
    char tempFileName[MAX_FILE_NAME]; /*the temporary file name*/
    int i; /*index*/
    sprintf(tempFileName, "%s.ob", fileName); /*add the suffix to the file name*/
    objectFile = fopen(tempFileName, "w"); /*open the object file*/
    if (objectFile == NULL) { /*if the file couldn't be opened, print an error message and exit*/
        printf("Failed to open file %s\n", tempFileName);
        closeFileAndExit(errorInfo, symbolTable);
    }

    fprintf(objectFile, "%d %d\n", IC, DC); /*print the IC and DC to the top of the file*/
    for (i = 0; i < IC; i++) { /*print the instructions memory image to the file*/
        char* base4String = convertBinaryToBase4Symbols(instructionArray[i].wordBits); /*convert the binary to base 4*/
        if (base4String != NULL) {
            fprintf(objectFile, "0%d\t%s\n", i+INITIAL_IC_VALUE, base4String); /*print the instruction to the file*/
            free(base4String); /*free the base 4 string*/
        } else { /*if the memory allocation failed, print an error message and exit*/
            printf("Memory allocation failed for value at index %d.\n", i);
            closeFileAndExit(errorInfo, symbolTable);
        }
    }
    for (i = 0; i < DC; i++) { /*print the data memory image to the file*/
        char* base4String = convertBinaryToBase4Symbols(dataArray[i].wordBits); /*convert the binary to base 4*/
        if (base4String != NULL) {
            fprintf(objectFile, "0%d\t%s\n", i+IC+INITIAL_IC_VALUE, base4String); /*print the data to the file*/
            free(base4String); /*free the base 4 string*/
        } else { /*if the memory allocation failed, print an error message and exit*/
            printf("Memory allocation failed for value at index %d.\n", i);
            closeFileAndExit(errorInfo, symbolTable);
        }
    }
    fclose(objectFile); /*close the object file*/
}

/**
 * This function will create the entry file.
 * @param head the symbol table
 * @param fileName the file name
 */
void createEntFile(symbolList** head, char* fileName, error** errorInfo) {
    char tempFileName[MAX_FILE_NAME]; /*the temporary file name*/
    symbolList* current; /*the current symbol*/
    FILE* entFile; /*the entry file*/
    sprintf(tempFileName, "%s.ent", fileName); /*add the suffix to the file name*/
    entFile = fopen(tempFileName, "w"); /*open the entry file*/
    if (entFile == NULL) { /*if the file couldn't be opened, print an error message and exit*/
        printf("Failed to open file %s\n", tempFileName);
        closeFileAndExit(errorInfo, head);
    }

    current = *head; /*set the current symbol to the head of the symbol table*/
    while (current != NULL) { /*loop through the symbol table*/
        if (current->isEntry == TRUE) { /*if the symbol is an entry symbol, print it to the file*/
            fprintf(entFile, "%s\t0%d\n", current->name, current->value);
        }
        current = current->next; /*move to the next symbol*/
    }
    fclose(entFile); /*close the entry file*/
}

/**
 * This function will create the external file.
 * It'll only be called if there are external symbols in the symbol table, which are used in the source code.
 * @param head the symbol table
 * @param fileName the file name
 */
void createExtFile(symbolList** head, char* fileName, error** errorInfo) {
    char tempFileName[MAX_FILE_NAME]; /*the temporary file name*/
    int i; /*index*/
    symbolList* current; /*the current symbol*/
    FILE* extFile; /*the external file*/
    sprintf(tempFileName, "%s.ext", fileName); /*add the suffix to the file name*/
    extFile = fopen(tempFileName, "w"); /*open the external file*/
    if (extFile == NULL) { /*if the file couldn't be opened, print an error message and exit*/
        printf("Failed to open file %s\n", tempFileName);
        closeFileAndExit(errorInfo, head);
    }

    current = *head; /*set the current symbol to the head of the symbol table*/
    while (current != NULL) {
        if (strcmp(current->type, "external") == 0) { /*if the symbol is an external symbol, print it to the file*/
            i = 0;
            while (current->externalAddresses[i] != -1) { /*loop through the external addresses, and print them after the Label*/
                fprintf(extFile, "%s\t0%d\n", current->name, current->externalAddresses[i]);
                i++;
            }
        }
        current = current->next; /*move to the next symbol*/
    }
    fclose(extFile); /*close the external file*/
}

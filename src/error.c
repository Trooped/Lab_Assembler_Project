#include "include/error.h"


/**
 * This function prints an error message and updates the errorInfo struct accordingly.
 *
 * @param errorInfo A pointer to the errorInfo struct.
 * @param errorDescription The description of the error.
 */
void printError(error** errorInfo, char* errorDescription){
    (*errorInfo)->errorCounter++;
    (*errorInfo)->errorFlag = 1;
    if ((*errorInfo)->lineText[strlen((*errorInfo)->lineText) - 1] == '\n') {
        (*errorInfo)->lineText[strlen((*errorInfo)->lineText) - 1] = '\0'; /* Remove the newline character*/
    }
    printf("Error #%d found in file '%s', line %d:\n'%s'\nerror description: %s\n\n", (*errorInfo)->errorCounter, (*errorInfo)->fileName,(*errorInfo)->lineCounter, (*errorInfo)->lineText, errorDescription);
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
    *errorInfo = malloc(sizeof(error));
    if (*errorInfo == NULL) {
        fprintf(stderr, "Failed to allocate memory for errorInfo\n");
        closeFileAndExit(errorInfo, symbolTable);
    }
    /* Initialize errorInfo*/
    (*errorInfo)->errorFlag = 0;
    (*errorInfo)->errorCounter = 0;
    strcpy((*errorInfo)->fileName, fileName);
    (*errorInfo)->file = NULL;
    (*errorInfo)->lineText[0] = '\0';
    (*errorInfo)->lineCounter = 0;
}

/**
 * This function closes the file and frees the memory before exiting the program.
 *
 * @param errorInfo A pointer to the errorInfo struct.
 * @param symbolTable A pointer to the symbol table.
 */
void closeFileAndExit(error** errorInfo, symbolList** symbolTable) {
    if ((*errorInfo)->file != NULL) {
        fclose((*errorInfo)->file);
    }
    if ((*errorInfo) != NULL) {
        free(*errorInfo);
    }
    if ((*symbolTable) != NULL) {
        deleteSymbolList(symbolTable);
    }
    exit(1);
}


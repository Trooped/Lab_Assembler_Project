
#include "include/fileFunctions.h"


/**
 * This function will create a new file to copy macros from the old file into the new file.
 * @param source the old file
 * @param oldFileName the old file name
 * @return the new file
 */
FILE* createFileWithMacros(FILE* source, const char* oldFileName, error** errorInfo) {
    char oldFileNameCopy[MAXFILENAME];
    char newFileName[MAXFILENAME];
    FILE * resultFile;

    /*TODO add more comments here.*/
    strcpy(oldFileNameCopy, oldFileName);

    removeSubstring(oldFileNameCopy, ".as");/*TODO IS IT EVEN NEEDED? REMOVE.AS*/
    sprintf(newFileName, "%s.am", oldFileNameCopy);

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

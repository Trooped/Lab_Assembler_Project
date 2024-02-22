
#include "PreAsmblr.h"

/**
 * This function will copy the macros from the old file into the new file.
 * @param source the old file
 * @param oldFileName the old file name
 * @return the new file
 */
FILE* createNewFileAndWriteMacros(FILE* source, const char* oldFileName) {
    char newFileName[MAX_FILE_NAME];
    FILE * resultFile;
    int macroArrSize = MAXMACROS; /* Maximum number of macros*/
    int tmpSize = 0;
    int macroCount = 0;
    macro **macros = allocateMemoryToMacros(macroArrSize);

    removeSubstring(oldFileName, oldFileFormat);/*TODO IS IT EVEN NEEDED? REMOVE.AS*/
    sprintf(newFileName, "%s%s", oldFileName, newFileFormat);

    resultFile = fopen(newFileName, "w+");
    if (resultFile == NULL) {
        fprintf(stderr, "Error opening file '%s': %s\n", newFileName, strerror(errno));
        return NULL;
    }

    processFileLines(source, resultFile, macros, &macroCount, &macroArrSize, &tmpSize);

    freeMacrosMemory(macros, macroCount);

    fclose(resultFile);
    return resultFile;
}

/**
 * This function will allocate memory for the macros array.
 * @param size the size of the array
 * @return the macros array
 */
macro** allocateMemoryToMacros(int macroArrSize) {
    macro **macros = (macro**)malloc(macroArrSize * sizeof(macro*));
    if (macros == NULL) {
        fprintf(stderr, "Failed to allocate memory for macros.\n");
        return NULL; /* Return NULL to indicate failure */
    }
    return macros;
}

/*TODO ADD COMMENT!*/
void processFileLines(FILE* source, FILE* resultFile, macro **macros, int *macroCount, int * macroArrSize, int * tmpSize) {
    char lineBuffer[MAXCHARSPERLINE];
    char currentLine[MAXCHARSPERLINE];
    char *word;
    while (fgets(lineBuffer, sizeof(lineBuffer), source) != NULL) {
        strcpy(currentLine, lineBuffer);
        word = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
        if (strlen(currentLine) > 1) {
            if (strcmp(word, "mcr") == 0) {
                addNewMacroToMacrosArray(source, resultFile, lineBuffer, word, &macros, macroCount, currentLine, macroArrSize,tmpSize);
            } else if (checkIfMacroExists(word, *macroCount, macros)) {
                writeCurrentMacroIntoFile(resultFile, word, macros, *macroCount);
            } else {
                fputs(currentLine, resultFile);
            }
        }
    }
}

/**
 * This function will add a new macro to the macros array.
 * @param source the old file
 * @param resultFile the new file
 * @param lineBuffer the current line
 * @param word the current word
 * @param macros the macros array
 * @param macroCount the number of macros
 * @param currentLine the current line
 * @param macroArrSize the size of the macros array
 * @param tmpSize the temporary size of the macros array
 */
void addNewMacroToMacrosArray(FILE* source, FILE* resultFile, char *lineBuffer, char *word, macro ***macros, int *macroCount, char *currentLine, int *macroArrSize, int *tmpSize) {
    macro **newMacros = NULL;
    word = strtok(NULL, " \n\r\t"); /* Get the next word, which is the macro's name.*/
    if (word != NULL) {
        if (checkIfMacroExists(word, *macroCount, *macros)) {/*Checks if the macro already exists, if it does: return.*/
            return;
        }
        else {/*Macro doesn't exist in the macros array*/
            /*Check if we reached the macroArrSize (current maximum size), if we did, then reallocate 2x the memory of the current.*/
            if (*macroCount == *macroArrSize) {
                *tmpSize = *macroArrSize * 2; /* Double the size of the array*/
                newMacros = (macro**)realloc((*macros), *tmpSize * sizeof(macro*));
                if (newMacros == NULL) {
                    fclose(resultFile);
                    return;
                }
                (*macros) = newMacros; /*assigning the newMacros array we created to our original array.*/
                *macroArrSize = *tmpSize; /*updating the macroArrSize to the new size.*/
            }

            /*allocate memory for the current macro element in the macro array.*/
            (*macros)[*macroCount] = (macro *) malloc(sizeof(macro));
            (*macros)[*macroCount]->macroName = (char*)malloc(strlen(word) + 1);
            if (*macros[*macroCount] == NULL || (*macros)[*macroCount]->macroName == NULL) {
                fclose(resultFile);
                return;
            }
            /*Copy the new macro's name into the relevant macro element in the macros array, initalize the lines counter and add 1 to the macroCount*/
            strcpy((*macros)[*macroCount]->macroName, word);
            (*macros)[*macroCount]->linesCounter = 0;
            (*macroCount)++;

            /*Checking if there's another word after the macro's name, which leads to an error.*/
            word = strtok(NULL, " \n\r\t"); /* Get the word following the macro's name */
            if (word != NULL) {
                /*another word after macro name, exit the program!*/
                printf("\nError: macro name cannot be followed by another word\n");
                fclose(resultFile);
                return;
            }

            /*Write the macro's lines into the macros array's relevant macro element. (so we can copy it later into the new file)*/
            while (fgets(lineBuffer, MAXCHARSPERLINE, source) != NULL) {
                strcpy(currentLine, lineBuffer);/*TODO maybe not use strstr, and just check the first word?*/
                if (strstr(currentLine, "endmcr") != NULL) {/*If we find endmcr in our line, then we terminate the process of copying.*/
                    break;
                }
                strcpy((*macros)[(*macroCount) - 1]->lines[(*macros)[(*macroCount) - 1]->linesCounter++], lineBuffer);
            }
        }
    }
}

/**
 * This function will free the memory allocated for the macros array.
 * @param macros the macros array
 * @param macroCount the number of macros
 */
void freeMacrosMemory(macro **macros, int macroCount) {
    int i;
    for (i = 0; i < macroCount; i++) {
        free(macros[i]->macroName);
        free(macros[i]);
    }
    free(macros);
}


/**
 * This function will check if the given word is an existing macro.
 * @param word the word to check
 * @param macroCount the number of macros
 * @param macros the macros array
 * @return 1 if the word is a macro, else 0
 */
int checkIfMacroExists(char* word, int macroCount, macro *macros[]){
    int i;
    for (i = 0; i < macroCount; i++) {
        if (strcmp(macros[i]->macroName, word) == 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * This function will write the macro with the given name to the new file.
 * @param newFile the new file
 * @param macroName the macro name
 * @param macros the macros array
 * @param macroCount the number of macros
 */
void writeCurrentMacroIntoFile(FILE* newFile, char* macroName, macro *macros[], int macroCount){
    int i, j;
    /* Find the macro with the given name*/
    for (i = 0; i < macroCount; i++) {
        if (strcmp(macros[i]->macroName, macroName) == 0) {
            /* Write each line of the macro to the file*/
            for (j = 0; j < macros[i]->linesCounter; j++) {
                fputs(macros[i]->lines[j], newFile);
            }
            break;
        }
    }
}


void removeSubstring(char* string, const char* sub) {
    char* match = strstr(string, sub);
    if(match) {
        size_t len = strlen(sub);
        memmove(match, match + len, 1 + strlen(match + len));
    }
}


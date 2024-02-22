
#include "PreAsmblr.h"

/**
 * This function will copy the macros from the old file into the new file.
 * @param source the old file
 * @param oldFileName the old file name
 * @return the new file
 */
FILE* findMacrosAndWriteIntoFile(FILE* source, const char* oldFileName) {
    char newFileName[MAX_FILE_NAME];
    FILE * resultFile;
    int macroArrSize = MAXMACROS; /* Maximum number of macros*/
    int tmpSize = 0;
    int macroCount = 0;
    macro **macros = allocateMacros(macroArrSize);

    removeSubstring(oldFileName, oldFileFormat);/*TODO IS IT EVEN NEEDED? REMOVE.AS*/
    sprintf(newFileName, "%s%s", oldFileName, newFileFormat);

    resultFile = fopen(newFileName, "w+");
    if (resultFile == NULL) {
        fprintf(stderr, "Error opening file '%s': %s\n", newFileName, strerror(errno));
        return NULL;
    }

    processFileLines(source, resultFile, macros, &macroCount, &macroArrSize, &tmpSize);

#if 0
    while (fgets(lineBuffer, sizeof(lineBuffer), source) != NULL) {
        strcpy(currentLine, lineBuffer);
        char *word = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
        if (strlen(currentLine) > 1) {
            if (strcmp(word, "mcr") == 0) {
                word = strtok(NULL, " \n\r\t"); /* Get the next word, which is the macro's name.*/
                strcpy(curMacroName, word);
                if (word != NULL) {
                    if (checkIfMacroExists(curMacroName, macroCount, macros)) {
                        break;
                    }

                    else {
                        if (macroCount == macroArrSize) {
                            tmpSize = macroArrSize * 2; /* Double the size of the array*/
                            macro **newMacros = (macro**)realloc(macros, tmpSize * sizeof(macro*));
                            if (newMacros == NULL) {
                                /* Handle memory allocation failure*/
                                free(macros);
                                fclose(resultFile);
                                return NULL;
                            }
                            macros = newMacros;
                            macroArrSize = tmpSize;
                        }


                        macros[macroCount] = (macro*)malloc(sizeof(macro));
                        if (macros[macroCount] == NULL) {
                            fclose(resultFile);
                            return NULL;
                        }
                        macros[macroCount]->macroName = (char*)malloc(strlen(curMacroName) + 1);
                        if (macros[macroCount]->macroName == NULL) {
                            fclose(resultFile);
                            return NULL;
                        }


                        strcpy(macros[macroCount]->macroName, curMacroName);
                        macros[macroCount]->linesCounter = 0;
                        macroCount++;


                        word = strtok(NULL, " \n\r\t"); /* Get the next word */
                        if (word != NULL) {
                            /*another word after macro name, exit the program!*/
                            printf("\nError: macro name cannot be followed by another word\n");
                            fclose(resultFile);
                            return NULL;
                        }

                        while (fgets(lineBuffer, sizeof(lineBuffer), source) != NULL) {
                            strcpy(currentLine, lineBuffer);
                            if (strstr(currentLine, "endmcr") != NULL) {
                                break;
                            }
                            strcpy(macros[macroCount - 1]->lines[macros[macroCount - 1]->linesCounter++], lineBuffer);
                        }
                    }
                }
            } else if (checkIfMacroExists(word, macroCount, macros)) {
                writeCurrentMacroIntoFile(resultFile, word, macros, macroCount);
            } else {
                fputs(currentLine, resultFile);
            }
        }
    }
#endif

    freeMacros(macros, macroCount);

    fclose(resultFile);
    return resultFile;
}



/**
 * This function will allocate memory for the macros array.
 * @param size the size of the array
 * @return the macros array
 */
macro** allocateMacros(int macroArrSize) {
    macro **macros = (macro**)malloc(macroArrSize * sizeof(macro*));
    if (macros == NULL) {
        fprintf(stderr, "Failed to allocate memory for macros.\n");
        return NULL; /* Return NULL to indicate failure */
    }
    return macros;
}


void processFileLines(FILE* source, FILE* resultFile, macro **macros, int *macroCount, int * macroArrSize, int * tmpSize) {
    char lineBuffer[MAXCHARSPERLINE];
    char currentLine[MAXCHARSPERLINE];
    char *word;
    while (fgets(lineBuffer, sizeof(lineBuffer), source) != NULL) {
        strcpy(currentLine, lineBuffer);
        word = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
        if (strlen(currentLine) > 1) {
            if (strcmp(word, "mcr") == 0) {
                addNewMacroToArray(source, resultFile, lineBuffer, word, &macros, macroCount, currentLine, macroArrSize,
                                   tmpSize);
            } else if (checkIfMacroExists(word, *macroCount, macros)) {
                writeCurrentMacroIntoFile(resultFile, word, macros, *macroCount);
            } else {
                fputs(currentLine, resultFile);
            }
        }
    }
}

void addNewMacroToArray(FILE* source, FILE* resultFile, char *lineBuffer, char *word, macro ***macros, int *macroCount, char *currentLine, int *macroArrSize, int *tmpSize) {
    macro **newMacros = NULL;
    word = strtok(NULL, " \n\r\t"); /* Get the next word, which is the macro's name.*/
    if (word != NULL) {
        if (checkIfMacroExists(word, *macroCount, *macros)) {
            return; /*TODO maybe return 0 or 1 if fail ro success?*/
        }

        else {
            if (*macroCount == *macroArrSize) {
                *tmpSize = *macroArrSize * 2; /* Double the size of the array*/
                newMacros = (macro**)realloc((*macros), *tmpSize * sizeof(macro*));
                if (newMacros == NULL) {
                    fclose(resultFile);
                    return;
                }
                (*macros) = newMacros;
                *macroArrSize = *tmpSize;
            }


            (*macros)[*macroCount] = (macro *) malloc(sizeof(macro));
            if (*macros[*macroCount] == NULL) {
                fclose(resultFile);
                return;
            }
            (*macros)[*macroCount]->macroName = (char*)malloc(strlen(word) + 1);
            if ((*macros)[*macroCount]->macroName == NULL) {
                fclose(resultFile);
                return;
            }

            strcpy((*macros)[*macroCount]->macroName, word);
            (*macros)[*macroCount]->linesCounter = 0;
            (*macroCount)++;


            word = strtok(NULL, " \n\r\t"); /* Get the next word */
            if (word != NULL) {
                /*another word after macro name, exit the program!*/
                printf("\nError: macro name cannot be followed by another word\n");
                fclose(resultFile);
                return;
            }

            while (fgets(lineBuffer, MAXCHARSPERLINE, source) != NULL) {
                strcpy(currentLine, lineBuffer);
                if (strstr(currentLine, "endmcr") != NULL) {
                    break;
                }
                strcpy((*macros)[(*macroCount) - 1]->lines[(*macros)[(*macroCount) - 1]->linesCounter++], lineBuffer);
            }
        }
    }
}


void freeMacros(macro **macros, int macroCount) {
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


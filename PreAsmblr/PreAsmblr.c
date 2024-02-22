
#include "PreAsmblr.h"

/**
 * This function will copy the macros from the old file into the new file.
 * @param source the old file
 * @param oldFileName the old file name
 * @return the new file
 */
FILE* findMacrosAndWriteIntoFile(FILE* source, const char* oldFileName) {
    int i;
    char newFileName[MAX_FILE_NAME];
    char curMacroName[MAXCHARSPERLINE];
    char currentLine[MAXCHARSPERLINE];
    int macroCount = 0;
    /*
    macro * macros[MAXMACROS];
     */

    macro **macros = NULL;
    int macroArrSize = MAXMACROS; /* Maximum number of macros*/
    int tmpSize = 0;

    macros = (macro**)malloc(macroArrSize * sizeof(macro*));
    if (macros == NULL) {
        /* Handle memory allocation failure*/
        exit(EXIT_FAILURE);
    }

    FILE * resultFile;
    char lineBuffer[MAXCHARSPERLINE];
    sprintf(newFileName, "%s%s", oldFileName, newFileFormat);

    removeSubstring(oldFileName, oldFileFormat);

    resultFile = fopen(newFileName, "w+");
    /*TODO do i even need to check if the file is null? i just created it and it's new*/
    if (resultFile == NULL) {
        fprintf(stderr, "Error opening file '%s': %s\n", newFileName, strerror(errno));
        return NULL;
    }

    /*Allocating memory to the macros, for now TODO CHANGE THIS
    for (i = 0; i < MAXMACROS; i++) {
        macros[i] = malloc(sizeof(macro));
        if (macros[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        macros[i]->curMacroName = malloc(MAXCHARSPERLINE * sizeof(char));
        if (macros[i]->curMacroName == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
    }
     */




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
                                exit(EXIT_FAILURE);
                            }
                            macros = newMacros;
                            macroArrSize = tmpSize;
                        }



                        macros[macroCount] = (macro*)malloc(sizeof(macro));
                        if (macros[macroCount] == NULL) {
                            exit(EXIT_FAILURE);
                        }

                        macros[macroCount]->macroName = (char*)malloc(strlen(curMacroName) + 1);
                        if (macros[macroCount]->macroName == NULL) {
                            exit(EXIT_FAILURE);
                        }
                        strcpy(macros[macroCount]->macroName, curMacroName);
                        macros[macroCount]->linesCounter = 0;
                        macroCount++;




                        word = strtok(NULL, " \n\r\t"); /* Get the next word */
                        if (word != NULL) {
                            /*another word after macro name, exit the program!*/
                            printf(";Error: macro name cannot be followed by another word\n");
                            /*TODO add this functionality*/
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


    /*
    for (i = 0; i < MAXMACROS; i++) {
        free(macros[i]->curMacroName);
        free(macros[i]);
    }
     */

    for (i = 0; i < macroCount; i++) {
        free(macros[i]->macroName);
        free(macros[i]);
    }
    free(macros);


    return resultFile;
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


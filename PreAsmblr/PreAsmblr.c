
#include "PreAsmblr.h"

/**
 * This function will copy the macros from the old file into the new file.
 * @param source the old file
 * @param oldFileName the old file name
 * @return the new file
 */
FILE* copyMacrosIntoNewFile(FILE* source, const char* oldFileName) {
    int i;
    char newFileName[MAX_FILE_NAME];
    char macroName[CHARACTERS];
    char currentLine[CHARACTERS];
    int macroCount = 0;
    macro * macros[MAXMACROS]; /*TODO MAKE IT DINAMYC MEMORY*/ /*TODO FIRST!! CHANGE IT TO NON POINTER ARRAY?? IDK*/
    FILE * result;
    char lineBuffer[CHARACTERS];
    sprintf(newFileName, "%snewFileFormat", oldFileName);

    result = fopen(newFileName, "w+");
    /*TODO do i even need to check if the file is null? i just created it and it's new*/
    if (result == NULL) {
        fprintf(stderr, "Error opening file '%s': %s\n", newFileName, strerror(errno));
        return NULL;
    }

    while (fgets(lineBuffer, sizeof(lineBuffer), source) != NULL) {
        char *word = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
        while (word != NULL) {
            if (strcmp(word, "mcr")==0) {
                word = strtok(NULL, " \n\r\t"); /* Get the next word, which is the macro's name.*/
                strcpy(macroName, word);
                if (word != NULL) {
                    for (i = 0; i < MAXMACROS; i++) {
                        if (strcmp(macros[i]->macroName, word) == 0) {
                            break; /*TODO USE CHECKIFMACRO TO DO THIS INSTEAD???*/
                        }
                    }
                    if (i == macroCount) { /* macro not found, create a new one*/
                        strcpy(macros[macroCount]->macroName, word);
                        macros[macroCount]->linesCounter = 0;
                        macroCount++;
                    }
                    while (fgets(lineBuffer, sizeof(lineBuffer), source) != NULL) {
                        strcpy(currentLine, lineBuffer);
                        if (strstr(currentLine, "endmcr") != NULL) {
                            break;
                        }
                        strcpy(macros[i]->lines[macros[i]->linesCounter++], lineBuffer);
                    }
                }
            }
            else if (checkIfMacro(word, macroCount, macros)){
                writeMacroToFile(result, word, *macros, macroCount);
            }
            else {
                fputs(lineBuffer, result);
            }
            word = strtok(NULL, " \n\r\t"); /* Get the next word.*/
        }
    }
    return result;
}

/**
 * This function will check if the given word is a macro.
 * @param word the word to check
 * @param macroCount the number of macros
 * @param macros the macros array
 * @return 1 if the word is a macro, else 0
 */
int checkIfMacro(char* word, int macroCount, macro *macros){
    int i;
    for (i = 0; i < macroCount; i++) {
        if (strcmp(macros[i].macroName, word) == 0) {
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
void writeMacroToFile(FILE* newFile, char* macroName, macro *macros, int macroCount){
    int i, j;
    /* Find the macro with the given name*/
    for (i = 0; i < macroCount; i++) {
        if (strcmp(macros[i].macroName, macroName) == 0) {
            /* Write each line of the macro to the file*/
            for (j = 0; j < macros[i].linesCounter; j++) {
                fputs(macros[i].lines[j], newFile);
            }
            break;
        }
    }
}


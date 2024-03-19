/**
 * This file contains the implementation of the PreAsmblr module.
 * This module is responsible for copying the macros from the old file into the new file.
 * The module works with a macros array, which is an array of macro pointers- which is dynamically allocated using a pointer to a pointer.
 * The macros array is a struct, which contains the macro's name, the lines of the macro and a lines errorCounter for each macro.
 * The module contains functions to allocate memory for the macros array, process the lines of the old file and write the macros into the new file.
 * The module also contains functions to add a new macro to the macros array (if the macro's name is valid), free the memory allocated for the macros array and close the file.
 */

#include "include/PreAsmblr.h"


/**
 * This function will process the lines of the old file and write the macros into the new file.
 * only 81 characters per line will be read from the file, the rest will be ignored (if the line is longer than 81 characters).
 * @param source the old file
 * @param resultFile the new file
 */
void processFileLines(FILE* source, FILE* resultFile) {
    int macroArrSize = MAXMACROS; /* Maximum number of macros*/
    int tmpSize = 0;
    int macroCount = 0;
    char lineBuffer[MAXCHARSPERLINE];
    char currentLine[MAXCHARSPERLINE];
    char *word;
    macro **macros = allocateMemoryToMacros(macroArrSize); /*initialize a pointer to a pointer macros array, and allocate memory for it.*/

    while (fgets(lineBuffer, sizeof(lineBuffer), source) != NULL) {
        strcpy(currentLine, lineBuffer);
        word = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
        if (strlen(currentLine) > 1 && currentLine[0] != ';') {
            if (strcmp(word, "mcr") == 0) {
                addNewMacroToMacrosArray(source, resultFile, lineBuffer, word, &macros, &macroCount, currentLine, &macroArrSize,&tmpSize);
            } else if (checkIfMacroExists(word, macroCount, macros)) {
                writeCurrentMacroIntoFile(resultFile, word, macros, macroCount);
            } else {
                fputs(currentLine, resultFile);
            }
        }
    }
    fputs("\n", resultFile); /* Add a newline at the end of the file*/
    freeMacrosMemory(macros, macroCount);
}

/**
 * This function will allocate memory for the macros array.
 * @param size the size of the array
 * @param macros the macros array
 * @return the macros array
 */
macro** allocateMemoryToMacros(int macroArrSize){
    macro **macros = (macro**)malloc(macroArrSize * sizeof(macro*));
    if (macros == NULL) {
        fprintf(stderr, "Error allocating memory for macros array: %s\n", strerror(errno));
        exit(1); /*exiting without the freeMemoryAndCloseFile, because it's initial allocation*/
    }
    return macros;
}


/**
 * This function will add a new macro to the macros array.
 * @param source the old file
 * @param resultFile the new file
 * @param lineBuffer the current line
 * @param word the current binaryWord
 * @param macros the macros array
 * @param macroCount the number of macros
 * @param currentLine the current line
 * @param macroArrSize the size of the macros array
 * @param tmpSize the temporary size of the macros array
 */
void addNewMacroToMacrosArray(FILE* source, FILE* resultFile, char *lineBuffer, char *word, macro ***macros, int *macroCount, char *currentLine, int *macroArrSize, int *tmpSize) {
    int i;
    int tempLineCounter=LINES;
    macro **newMacros = NULL;
    word = strtok(NULL, " \n\r\t"); /* Get the next binaryWord, which is the macro's name.*/
    if (word != NULL) {
        /*Checking if the macro already exists in the macro array, or if it's valid. if any apply: return.*/
        if (checkIfMacroExists(word, *macroCount, *macros)) {
            printf("\nError: macro already exists\n");
            return;
        }
        else if (checkIfMacroNameIsValid(word)==0){
            printf("\nError: invalid macro name\n");/*TODO maybe add a search for 'endmcr', so that the lines wouldn't be copied after this part, if it fails??*/
            return;
        }
        else {/*Macro doesn't exist in the macros array, and has a valid name. create a new instance of it.*/
            /*Check if we reached the macroArrSize (current maximum size), if we did, then reallocate 2x the memory of the current.*/
            if (*macroCount == *macroArrSize) {
                *tmpSize = *macroArrSize * 2; /* Double the size of the array*/
                newMacros = (macro**)realloc((*macros), *tmpSize * sizeof(macro*));
                if (newMacros == NULL) {
                    freeMemoryAndCloseFile(resultFile, *macros, *macroCount);
                }
                (*macros) = newMacros; /*assigning the newMacros array we created to our original array.*/
                *macroArrSize = *tmpSize; /*updating the macroArrSize to the new size.*/
            }

            /*allocate memory for the current macro element in the macro array, and it's sub-elements.*/
            (*macros)[*macroCount] = (macro *) malloc(sizeof(macro));
            (*macros)[*macroCount]->macroName = (char*)malloc(strlen(word) + 1); /*TODO maybe change it to some defined size? does it work correctly now?*/
            (*macros)[*macroCount]->lines = (char**)malloc(LINES * sizeof(char*)); /* Allocate memory for lines*/
            for (i = 0; i < LINES; i++) {
                (*macros)[*macroCount]->lines[i] = (char*)malloc(MAXCHARSPERLINE * sizeof(char)); /* Allocate memory for each line*/
            }
            /*Check if any of the allocations failed.*/
            if (*macros[*macroCount] == NULL || (*macros)[*macroCount]->macroName == NULL || (*macros)[*macroCount]->lines == NULL){
                freeMemoryAndCloseFile(resultFile, *macros, *macroCount);
            }

            /*Copy the new macro's name into the relevant macro element in the macros array, initalize the lines errorCounter and add 1 to the macroCount*/
            strcpy((*macros)[*macroCount]->macroName, word);
            (*macros)[*macroCount]->linesCounter = 0;

            /*Checking if there's another binaryWord after the macro's name, which leads to an error.*/
            word = strtok(NULL, " \n\r\t"); /* Get the binaryWord following the macro's name */
            if (word != NULL) {
                /*another binaryWord after macro name, exit the program!*/
                printf("\nError: macro name cannot be followed by another word\n");
                freeMemoryAndCloseFile(resultFile, *macros, *macroCount);
                return;
            }

            /*Write the macro's lines into the macros array's relevant macro element. (so we can copy it later into the new file)*/
            while (fgets(lineBuffer, MAXCHARSPERLINE, source) != NULL) {
                strcpy(currentLine, lineBuffer);
                if (strstr(currentLine, "endmcr") != NULL) {/*If we find endmcr in our line, then we terminate the process of copying.*/
                    break;
                }
                /*Checking if we need to allocate more memory to the lines array, inside the specific macro's instance*/
                if ((*macros)[*macroCount]->linesCounter >= tempLineCounter) {
                    tempLineCounter = tempLineCounter * 2; /* Double the size of the lines array's current size*/
                    (*macros)[*macroCount]->lines = (char**)realloc((*macros)[*macroCount]->lines, tempLineCounter * sizeof(char*)); /* Reallocate memory for the lines array*/
                    if ((*macros)[*macroCount]->lines == NULL) {
                        freeMemoryAndCloseFile(resultFile, *macros, *macroCount);
                    }
                    for (i = (*macros)[*macroCount]->linesCounter; i < tempLineCounter; i++) {/* Allocate memory for each new line*/
                        (*macros)[*macroCount]->lines[i] = (char*)malloc(MAXCHARSPERLINE * sizeof(char));
                    }
                }
                /*The most important line- if there's no problem, copy the current line in the file, to it's respective counterpart in the macro's instance*/
                strcpy((*macros)[(*macroCount)]->lines[(*macros)[(*macroCount)]->linesCounter++], lineBuffer);
            }
            (*macroCount)++; /*New macro created, add 1 to the macroCount*/
        }
    }
}

/**
 * This function will free the memory allocated for the macros array.
 * @param macros the macros array
 * @param macroCount the number of macros
 */
void freeMacrosMemory(macro **macros, int macroCount) {
    int i,j;
    for (i = 0; i < macroCount; i++) {
        for (j = 0; j < macros[i]->linesCounter; j++) {
            free(macros[i]->lines[j]); /* Free each line*/
        }
        free(macros[i]->lines); /* Free the lines array*/
        free(macros[i]->macroName);
        free(macros[i]);
    }
    free(macros);
}


/**
 * This function will check if the given binaryWord is an existing macro.
 * @param word the binaryWord to check
 * @param macroCount the number of macros
 * @param macros the macros array
 * @return 1 if the binaryWord is a macro, else 0
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

/**
 * This function will remove the given substring from the given string.
 * @param string the string
 * @param sub the substring
 */
void removeSubstring(char* string, const char* sub) {
    char* match = strstr(string, sub);
    if(match) {
        size_t len = strlen(sub);
        memmove(match, match + len, 1 + strlen(match + len));
    }
}

/**
 * This function will check if the given binaryWord is a valid macro name.
 * TODO do i need to pause the program if the macro name is invalid?
 * @param word the binaryWord to check
 * @return 1 if the binaryWord is a valid macro name, else 0
 */
int checkIfMacroNameIsValid(char* word){
    int numSavedWords;
    int i;
    char* endptr;
    char* savedWords[] = {".data", ".string", ".entry", ".extern", "mov", "cmp", "add", "sub", "lea", "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "hlt", "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    if (word == NULL || strlen(word)>=MAXMACRONAMELENGTH || (word[0]>= '0' && word[0]<='9')) {
        return 0;
    }

    strtol(word, &endptr, 10); /* Try to convert the binaryWord to a long integer*/
    if (*endptr == '\0') { /* If the conversion is successful, endptr should point to the null terminator*/
        return 0; /* Return 0 (false) if the binaryWord is a number*/
    }

    numSavedWords = sizeof(savedWords) / sizeof(savedWords[0]);
    for (i = 0; i < numSavedWords; i++) {
        if (strcmp(word, savedWords[i]) == 0) {
            return 0; /* Return 0 (false) if the binaryWord is a saved binaryWord*/
        }
    }
    return 1;
}


/**
 * This function will free the memory allocated for the macros array and close the file.
 * @param file the file to close
 * @param macros the macros array
 * @param macroCount the number of macros
 */
void freeMemoryAndCloseFile(FILE* file, macro **macros, int macroCount) {
    freeMacrosMemory(macros, macroCount);
    fclose(file);
    exit(1);
}



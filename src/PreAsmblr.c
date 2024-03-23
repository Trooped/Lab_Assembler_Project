/**
 * @file PreAsmblr.c
 *
 * This file contains the implementation of the PreAsmblr module.
 * This module is responsible for copying the macros from the old file into the new file.
 * The module works with a macros array, which is an array of macro pointers- which is dynamically allocated using a pointer to a pointer.
 *
 * The functions in this file are:
 * 1. processFileLines - This function will process the lines of the old file and write the macros into the new file.
 * 2. allocateMemoryToMacros - This function will allocate memory for the macros array.
 * 3. addNewMacroToMacrosArray - This function will add a new macro to the macros array.
 * 4. freeMacrosMemory - This function will free the memory allocated for the macros array.
 * 5. checkIfMacroExists - This function will check if the given binaryWord is an existing macro.
 * 6. writeCurrentMacroIntoFile - This function will write the macro with the given name to the new file.
 * 7. removeSubstring - This function will remove the given substring from the given string.
 * 8. checkIfMacroNameIsValid - This function will check if the given binaryWord is a valid macro name.
 * 9. freeMemoryAndCloseFile - This function will free the memory allocated for the macros array and close the file.
 */

#include "include/PreAsmblr.h"


/**
 * This function will process the lines of the old file and write the macros into the new file.
 * only 81 characters per line will be read from the file, the rest will be ignored (if the line is longer than 81 characters).
 * @param source the old file
 * @param resultFile the new file
 */
void processFileLines(FILE* source, FILE* resultFile) {
    int macroArrSize = MAXMACROS; /* Maximum number of macros (initial size of the macros array, which will grow with need)*/
    int tmpSize = 0; /* Temporary size for reallocating the macros array*/
    int macroCount = 0; /* Number of macros in total*/
    char lineBuffer[MAXCHARSPERLINE]; /* Buffer for the current line parsing*/
    char currentLine[MAXCHARSPERLINE]; /* Buffer for the current full line*/
    char *word; /* The current word in the line*/
    macro **macros = allocateMemoryToMacros(macroArrSize); /*initialize a pointer to a pointer macros array, and allocate memory for it.*/

    while (fgets(lineBuffer, sizeof(lineBuffer), source) != NULL) { /* Read each line from the file*/
        /*Checking if line is longer than 81 characters, if it is- skip the excess characters. (we'll check it again in the first pass and throw an error.)*/
        if (strchr(lineBuffer, '\n') == NULL && !feof(source)) {
            /* Did not reach the end of the line*/
            int ch;
            while ((ch = fgetc(source)) != '\n' && ch != EOF); /* Discard until end of line or file*/
        }

        strcpy(currentLine, lineBuffer); /* Copy the line to the current line buffer*/
        word = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
        if (strlen(currentLine) > 1 && currentLine[0] != ';') { /* Check if the line is not empty and not a comment*/
            if (strcmp(word, "mcr") == 0) { /* Check if the line starts with a macro definition*/
                addNewMacroToMacrosArray(source, resultFile, lineBuffer, word, &macros, &macroCount, currentLine, &macroArrSize,&tmpSize); /* Add the macro to the macros array*/
            }
            else if (checkIfMacroExists(word, macroCount, macros)) { /* Check if the current word is an existing macro*/
                writeCurrentMacroIntoFile(resultFile, word, macros, macroCount);
            }
            else { /* If the line is not a macro definition or an existing macro, write it to the file as it is*/
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
    macro **macros = (macro**)malloc(macroArrSize * sizeof(macro*)); /* Allocate memory for the macros array*/
    if (macros == NULL) { /* Check if the memory allocation was successful*/
        fprintf(stderr, "Error allocating memory for macros array: %s\n", strerror(errno));
        exit(1); /*exiting without the freeMemoryAndCloseFile, because it's initial allocation*/
    }
    return macros; /* Return the macros array*/
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
    int i; /* Loop index */
    macro *newMacro = NULL; /* Temporary pointer for the new macro instance */
    macro **newMacros = NULL; /* Temporary pointer for the macros array */

    /* Move to the next word, which should be the macro's name. */
    word = strtok(NULL, " \n\r\t");
    if (word == NULL) {
        printf("\nError: Macro name missing.\n");
        return;
    }

    /* Check for macro name validity and existence. */
    if (!checkIfMacroNameIsValid(word)) {
        printf("\nError: Invalid macro name.\n");
        return;
    }
    if (checkIfMacroExists(word, *macroCount, *macros)) {
        printf("\nError: Macro already exists.\n");
        return;
    }

    /* Ensure there's enough space in the macros array, reallocating if necessary. */
    if (*macroCount == *macroArrSize) {
        *tmpSize = *macroArrSize * 2; /* Calculate new size */
        newMacros = (macro **) realloc(*macros, *tmpSize * sizeof(macro *));
        if (newMacros == NULL) {
            printf("\nError: Memory reallocation failed.\n");
            freeMemoryAndCloseFile(resultFile, *macros, *macroCount);
            return;
        }
        *macros = newMacros;
        *macroArrSize = *tmpSize;
    }

    /* Allocate memory for the new macro structure. */
    newMacro = (macro *) malloc(sizeof(macro));
    if (newMacro == NULL) {
        printf("\nError: Memory allocation for new macro failed.\n");
        freeMemoryAndCloseFile(resultFile, *macros, *macroCount);
        return;
    }

    /* Allocate and set the macro's name. */
    newMacro->macroName = (char *) malloc(strlen(word) + 1);
    if (newMacro->macroName == NULL) {
        printf("\nError: Memory allocation for macro name failed.\n");
        free(newMacro); /* Free the macro structure on failure */
        freeMemoryAndCloseFile(resultFile, *macros, *macroCount);
        return;
    }
    strcpy(newMacro->macroName, word); /* Copy the name into the allocated space */

    /* Allocate initial memory for the macro's lines. */
    newMacro->lines = (char **) malloc(LINES * sizeof(char *));
    newMacro->linesCounter = 0; /* Initialize lines counter. */
    if (newMacro->lines == NULL) {
        printf("\nError: Memory allocation for macro lines failed.\n");
        free(newMacro->macroName); /* Free the name on failure */
        free(newMacro); /* Free the macro structure */
        freeMemoryAndCloseFile(resultFile, *macros, *macroCount);
        return;
    }

    /* Allocate memory for each line */
    for (i = 0; i < LINES; i++) {
        newMacro->lines[i] = (char *) malloc(MAXCHARSPERLINE * sizeof(char));
        if (newMacro->lines[i] == NULL) {
            /* Handle line allocation failure by freeing all previously allocated lines */
            while (i--) free(newMacro->lines[i]);
            free(newMacro->lines);
            free(newMacro->macroName);
            free(newMacro);
            freeMemoryAndCloseFile(resultFile, *macros, *macroCount);
            return;
        }
    }

    /* Prepare to read the macro's content. */
    while (fgets(lineBuffer, MAXCHARSPERLINE, source) != NULL) {
        if (strstr(lineBuffer, "endmcr") != NULL) {
            break; /* Found the end of the macro definition. */
        }
        /* Additional logic for expanding the lines array as needed was omitted for brevity */
        /* This is where you would reallocate `newMacro->lines` if `newMacro->linesCounter >= tempLineCounter` */

        /* Copy the current line into the macro's lines */
        /* Make sure to allocate memory for the new line and copy it */
        newMacro->lines[newMacro->linesCounter] = (char *) malloc(strlen(lineBuffer) + 1);
        if (newMacro->lines[newMacro->linesCounter] == NULL) {
            /* Allocation failure for the new line */
            printf("\nError: Memory allocation for a macro line failed.\n");
            break; /* Handle cleanup */
        }
        strcpy(newMacro->lines[newMacro->linesCounter], lineBuffer);
        newMacro->linesCounter++;
    }
    /* Add the fully defined macro to the array and increment the macro count. */
    (*macros)[*macroCount] = newMacro;
    (*macroCount)++;
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



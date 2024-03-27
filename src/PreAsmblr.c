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
 * 4. freeMacrosArrayMemory - This function will free the memory allocated for the macros array.
 * 5. checkIfMacroExists - This function will check if the given word is an existing macro.
 * 6. writeCurrentMacroIntoFile - This function will write the macro with the given name to the new file.
 * 7. checkIfMacroNameIsValid - This function will check if the given word is a valid macro name.
 * 8. freeMemoryAndCloseFile - This function will free the memory allocated for the macros array and close the file.
 * 9. freeMacroMemory - This function will free the memory allocated for an individual macro.
 */

#include "include/PreAsmblr.h"


/**
 * This function will process the lines of the old file and write the macros into the new file.
 * only 81 characters per line will be read from the file, the rest will be ignored (if the line is longer than 81 characters).
 * @param source the old file
 * @param resultFile the new file
 */
void processFileLines(FILE* source, FILE* resultFile) {
    int macroArrSize = MAX_MACROS; /* Maximum number of macros (initial size of the macros array, which will grow with need)*/
    int tmpSize = 0; /* Temporary size for reallocating the macros array*/
    int macroCount = 0; /* Number of macros in total*/
    char lineBuffer[MAX_CHARS_PER_LINE]; /* Buffer for the current line parsing*/
    char currentLine[MAX_CHARS_PER_LINE]; /* Buffer for the current full line*/
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
                /* Add the macro to the macros array*/
                if (!addNewMacroToMacrosArray(source, resultFile, lineBuffer, word, &macros, &macroCount, currentLine, &macroArrSize,&tmpSize)){
                    freeMemoryAndCloseFile(resultFile, macros, macroCount);
                }
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
    freeMacrosArrayMemory(macros, macroCount);
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
 * @param word the current word
 * @param macros the macros array
 * @param macroCount the number of macros
 * @param currentLine the current line
 * @param macroArrSize the size of the macros array
 * @param tmpSize the temporary size of the macros array
 */
int addNewMacroToMacrosArray(FILE* source, FILE* resultFile, char *lineBuffer, char *word, macro ***macros, int *macroCount, char *currentLine, int *macroArrSize, int *tmpSize) {
    int i; /* Loop index */
    int tempLineCounter = MAX_LINES_PER_MACRO; /* Initial number of lines to allocate memory for */
    macro *newMacro = NULL; /* Temporary pointer for the new macro instance */
    macro **newMacros = NULL; /* Temporary pointer for the macros array */

    /* Move to the next word, which should be the macro's name. */
    word = strtok(NULL, " \n\r\t");
    if (word == NULL) {
        fprintf(stderr,"\nError: Macro name missing.\n");
        return FALSE;
    }

    /* Check for macro name validity and existence. */
    if (!checkIfMacroNameIsValid(word)) {
        fprintf(stderr,"\nError: Invalid macro name.\n");
        return FALSE;
    }
    if (checkIfMacroExists(word, *macroCount, *macros)) {
        fprintf(stderr,"\nError: Macro already exists.\n");
        return FALSE;
    }

    /* Ensure there's enough space in the macros array, reallocating if necessary. */
    if (*macroCount == *macroArrSize) {
        *tmpSize = *macroArrSize * 2; /* Calculate new size */
        newMacros = (macro **) realloc(*macros, *tmpSize * sizeof(macro *));
        if (newMacros == NULL) {
            fprintf(stderr,"\nError: Memory reallocation failed for the macros array.\n");
            return FALSE; /* Return 0 on failure */
        }
        *macros = newMacros;
        *macroArrSize = *tmpSize;
    }

    /* Allocate memory for the new macro struct. */
    newMacro = (macro *) malloc(sizeof(macro));
    if (newMacro == NULL) { /* Check if the allocation was successful */
        fprintf(stderr,"\nError: Memory allocation for new macro failed.\n");
        return FALSE; /* Return 0 on failure */
    }
    strcpy(newMacro->macroName, word); /* Copy the name into the macro struct */

    /* Allocate initial memory for the macro's lines. */
    newMacro->lines = (char **) malloc(MAX_LINES_PER_MACRO * sizeof(char *));
    newMacro->linesCounter = 0; /* Initialize lines counter. */
    if (newMacro->lines == NULL) { /* Check if the allocation was successful */
        fprintf(stderr,"\nError: Memory allocation for macro lines failed.\n");
        freeMacroMemory(newMacro);
        return FALSE; /* Return 0 on failure */
    }

    /* Allocate memory for each line */
    for (i = 0; i < MAX_LINES_PER_MACRO; i++) {
        newMacro->lines[i] = (char *) malloc(MAX_CHARS_PER_LINE * sizeof(char));
        if (newMacro->lines[i] == NULL) { /* Check if the allocation was successful */
            fprintf(stderr,"\nError: Memory allocation for macro line failed.\n");
            freeMacroMemory(newMacro);
            return FALSE; /* Return 0 on failure */
        }
    }

    /* Prepare to read the macro's content. */
    while (fgets(lineBuffer, MAX_CHARS_PER_LINE, source) != NULL) {
        char** expandedLines = NULL; /* Temporary pointer for reallocating the lines array */
        if (strstr(lineBuffer, "endmcr") != NULL) {
            break; /* Found the end of the macro definition. */
        }

        /* Check if we need to expand the lines array for the current macro */
        if (newMacro->linesCounter >= tempLineCounter) {
            /* Double the capacity of the current macro line counter */
            tempLineCounter = tempLineCounter * 2;
            expandedLines = (char**) realloc(newMacro->lines, tempLineCounter * sizeof(char*));

            if (!expandedLines) {
                fprintf(stderr,"\nError: Memory reallocation for macro lines failed.\n");
                /* Cleanup logic here: free allocated memory before returning */
                return FALSE;
            }

            newMacro->lines = expandedLines;

            /* Initialize new pointers to NULL */
            for (i = newMacro->linesCounter; i < tempLineCounter; i++) {
                newMacro->lines[i] = NULL;
            }
        }

        /* Copy the current line into the macro's lines */
        /* Make sure to allocate memory for the new line and copy it */
        newMacro->lines[newMacro->linesCounter] = (char *) malloc(strlen(lineBuffer) + 1);
        if (newMacro->lines[newMacro->linesCounter] == NULL) {
            /* Allocation failure for the new line */
            fprintf(stderr,"\nError: Memory allocation for a macro line failed.\n");
            freeMacroMemory(newMacro);
            return FALSE; /* Return 0 on failure */
        }
        strcpy(newMacro->lines[newMacro->linesCounter], lineBuffer);
        newMacro->linesCounter++;
    }

    /* Add the fully defined macro to the array and increment the macro count. */
    (*macros)[*macroCount] = newMacro;
    (*macroCount)++;
    return TRUE; /* return true on success */
}

/**
 * This function will free the memory allocated for the macros array, using a loop that calls freeMacroMemory for each macro.
 * @param macros the macros array
 * @param macroCount the number of macros
 */
void freeMacrosArrayMemory(macro **macros, int macroCount) {
    int i; /* Loop index */
    for (i = 0; i < macroCount; i++) {
        freeMacroMemory(macros[i]); /* Free each macro */
    }
    free(macros); /* Free the macros array */
}


/**
 * This function will check if the given word is an existing macro.
 * @param word the name to check
 * @param macroCount the number of macros
 * @param macros the macros array
 * @return true if the the name is a macro, else 0
 */
int checkIfMacroExists(char* word, int macroCount, macro *macros[]){
    int i; /* Loop index */
    for (i = 0; i < macroCount; i++) { /* Loop through the macros array */
        if (strcmp(macros[i]->macroName, word) == 0) { /* Check if the word is a macro name */
            return TRUE; /* return true if the word is a macro */
        }
    }
    return FALSE; /* Return 0 if the word is not a macro */
}

/**
 * This function will write the macro's reserved lines with the given name to the new file.
 * @param newFile the new file
 * @param macroName the macro name
 * @param macros the macros array
 * @param macroCount the number of macros
 */
void writeCurrentMacroIntoFile(FILE* newFile, char* macroName, macro *macros[], int macroCount){
    int i, j;
    /* Find the macro with the given name*/
    for (i = 0; i < macroCount; i++) {
        if (strcmp(macros[i]->macroName, macroName) == 0) { /* If the macro is found*/
            /* Write each line of the macro to the file*/
            for (j = 0; j < macros[i]->linesCounter; j++) {
                fputs(macros[i]->lines[j], newFile); /* Write the line to the file*/
            }
            break; /* Break the loop*/
        }
    }
}

/**
 * This function will check if the given string is a valid macro name.
 * A valid macro name must:
 * - Be less than 32 characters long
 * - Not start with a number
 * - Not contain a white space
 * - Not be a saved word
 * - Not be a number
 * @param word the string to check
 * @return true if the string is a valid macro name, else 0
 */
int checkIfMacroNameIsValid(char* word){
    char tempName[MAX_MACRO_NAME_LENGTH]; /* Temporary buffer for the macro name*/
    int numSavedWords; /* Number of saved words*/
    int i; /* Loop index*/
    char* endptr; /* Pointer to the end of the string*/
    /* Array of saved words*/
    char* savedWords[] = {".data", ".string", ".entry", ".extern","mcr","endmcr", "mov", "cmp", "add", "sub", "lea", "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "hlt", "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    if (word == NULL || strlen(word) >= MAX_MACRO_NAME_LENGTH || (word[0] >= '0' && word[0] <= '9')) {
        return FALSE; /* Return 0 (false) if the word is NULL, too long (more than 31 characters), or starts with a number*/
    }

    /* Checking if the word contains a white space*/
    strncpy(tempName, word, MAX_MACRO_NAME_LENGTH); /* Copy the word to the temporary buffer*/
    for (i = 0; i < strlen(tempName); i++) { /* Loop through the word*/
        if(isspace(tempName[i])){ /* If the character is a space*/
            return FALSE; /* Return 0 (false) if the word contains a space*/
        }
    }

    strtol(word, &endptr, 10); /* Try to convert the word to a long integer*/
    if (*endptr == '\0') { /* If the conversion is successful, endptr should point to the null terminator*/
        return FALSE; /* Return 0 (false) if the word is a number*/
    }

    numSavedWords = sizeof(savedWords) / sizeof(savedWords[0]); /* Calculate the number of saved words*/
    for (i = 0; i < numSavedWords; i++) { /* Loop through the saved words*/
        if (strcmp(word, savedWords[i]) == 0) { /* If the word is a saved word*/
            return FALSE; /* Return 0 (false) if the word is a saved word*/
        }
    }
    return TRUE; /* return true, we have valid macro name*/
}


/**
 * This function will free the memory allocated for the macros array and close the file.
 * @param file the file to close
 * @param macros the macros array
 * @param macroCount the number of macros
 */
void freeMemoryAndCloseFile(FILE* file, macro **macros, int macroCount) {
    freeMacrosArrayMemory(macros, macroCount); /* Free the memory allocated for the macros array*/
    fclose(file); /* Close the file*/
    exit(1); /* Exit the program*/
}

/**
 * This function will free the memory allocated for an individual macro.
 * @param macro the macro
 */
void freeMacroMemory(macro *macro){
    int i;
    for (i = 0; i < macro->linesCounter; i++) {
        free(macro->lines[i]); /* Free each line*/
    }
    free(macro->lines); /* Free the lines array*/
    free(macro); /* Free the macro*/
}



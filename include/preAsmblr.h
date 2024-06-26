/**
 * This header file contains the functions and structs for the pre-assembly process.
 * This module is responsible for copying the macros from the old file into the new file.
 * The module works with a macros array, which is an array of macro pointers- which is dynamically allocated using a pointer to a pointer.
 *
 * The functions in this file are:
 * 1. expandMacrosAndWriteToNewFile - This function will process the lines of the old file and write the macros into the new file.
 * 2. allocateMemoryToMacros - This function will allocate memory for the macros array.
 * 3. addNewMacroToMacrosArray - This function will add a new macro to the macros array.
 * 4. freeMacrosArrayMemory - This function will free the memory allocated for the macros array.
 * 5. checkIfMacroExists - This function will check if the given word is an existing macro.
 * 6. writeCurrentMacroIntoFile - This function will write the macro with the given name to the new file.
 * 7. checkIfMacroNameIsValid - This function will check if the given word is a valid macro name.
 * 8. freeMemoryAndCloseFile - This function will free the memory allocated for the macros array and close the file.
 * 9. freeMacroMemory - This function will free the memory allocated for an individual macro.
 */


#ifndef PREASM_H
#define PREASM_H

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "dataStructuresAndConstants.h"
#include "error.h"
#include <ctype.h>

/*Constants that are used solely in the pre-assembly process*/
#define MAX_MACRO_NAME_LENGTH 31 /*Maximum number of characters in a macro name*/
#define MAX_LINES_PER_MACRO 32 /*Initial maximum number of lines in a macro, will be adjusted dynamically*/
#define DEFAULT_MACRO_SIZE 32 /*Starter number for number of macros, dynamically allocated further on*/

/**
 * This struct will define each macro, it's lines (the actual lines of a text, a 2d pointer array) and a lines errorCounter.
 */
typedef struct macros{
    char macroName[MAX_MACRO_NAME_LENGTH]; /*The name of the macro*/
    char **lines; /*2d array of lines in the macro*/
    int linesCounter; /*Number of lines in the macro*/
}macro;

/**
 * This function will allocate memory for the macros array.
 * @param size the size of the array
 * @param macros the macros array
 * @param resultFile the new file
 * @return the macros array
 */
macro** allocateMemoryToMacros(int macroArrSize);

/**
 * This function will process the lines of the old file and write the macros into the new file.
 * @param source the old file
 * @param resultFile the new file
 * @param macros the macros array
 * @param macroCount the number of macros
 * @param macroArrSizePtr the size of the macros array
 * @param tmpSizePtr the temporary size of the macros array
 */
void expandMacrosAndWriteToNewFile(FILE* source, FILE* resultFile, error* error);

/**
 * This function will add a new macro to the macros array.
 * @param source the old file
 * @param resultFile the new file
 * @param lineBuffer the buffer for the current line
 * @param word the current word
 * @param macros the macros array
 * @param macroCount the number of macros
 * @param currentLine the current line
 * @param macroArrSize the size of the macros array
 * @param tmpSize the temporary size of the macros array
 */
int addNewMacroToMacrosArray(FILE* source, FILE* resultFile, char* lineBuffer, macro ***macros, int *macroCount, char *currentLine, int *macroArrSize, int *tmpSize, error* error);

/**
 * This function will free the memory allocated for the macros array.
 * @param macros the macros array
 * @param macroCount the number of macros
 */
void freeMacrosArrayMemory(macro **macros, int macroCount);

/**
 * This function will check if a macro exists in the macros array.
 * @param word the current word
 * @param macroCount the number of macros
 * @param macros the macros array
 * @return 1 if the macro exists, 0 otherwise
 */
int checkIfMacroExists(char* word, int macroCount, macro *macros[]);

/**
 * This function will write the current macro into the new file.
 * @param newFile the new file
 * @param macroName the name of the macro
 * @param macros the macros array
 * @param macroCount the number of macros
 */
void writeCurrentMacroIntoFile(FILE* newFile, char* macroName, macro *macros[], int macroCount);

/**
* This function will check if the given string is a valid macro name.
* @param word the string to check
* @return 1 if the string is a valid macro name, else 0
*/
int checkIfMacroNameIsValid(char* word);

/**
 * This function will free the memory allocated for the macros array and close the file.
 * @param file the file to close
 * @param macros the macros array
 * @param macroCount the number of macros
 */
void freeMemoryAndCloseFile(FILE* file, macro **macros, int macroCount);


/**
 * This function will free the memory allocated for an individual macro.
 * @param macro the macro
 */
void freeMacroMemory(macro *macro);

#endif

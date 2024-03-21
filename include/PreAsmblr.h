/**
 * This header file contains the functions and structs for the pre-assembly process.
 * The pre-assembly process will copy the macros from the old file into the new file.
 */


#ifndef PREASM_H
#define PREASM_H

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "dataStructuresAndConstants.h"
#include "error.h"

/*TODO move the definitions to a different file?*/
#define MAXMACRONAMELENGTH 31 /*Maximum number of characters in a macro name*/
#define LINES 32 /*Initial maximum number of lines in a macro, will be adjusted dynamically*/
#define MAXMACROS 32 /*Starter number for number of macros, dynamically allocated further on*/
#define MAXFILENAME 64 /*Maximum file name length*/

/**
 * This struct will define each macro, it's lines (the actual lines of a text, a 2d pointer array) and a lines errorCounter.
 */
typedef struct macros{
    char * macroName;
    char **lines;
    int linesCounter;
}macro;


/**
 * This function will copy the macros from the old file into the new file.
 * @param source the old file
 * @param oldFileName the old file name
 * @return the new file
 */
FILE* createFileWithMacros(FILE* source, const char* oldFileName, error** errorInfo);

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
void processFileLines(FILE* source, FILE* resultFile);

/**
 * This function will add a new macro to the macros array.
 * @param source the old file
 * @param resultFile the new file
 * @param lineBuffer the buffer for the current line
 * @param word the current binaryWord
 * @param macros the macros array
 * @param macroCount the number of macros
 * @param currentLine the current line
 * @param macroArrSize the size of the macros array
 * @param tmpSize the temporary size of the macros array
 */
void addNewMacroToMacrosArray(FILE* source, FILE* resultFile, char *lineBuffer, char *word, macro ***macros, int *macroCount, char *currentLine, int *macroArrSize, int *tmpSize);

/**
 * This function will free the memory allocated for the macros array.
 * @param macros the macros array
 * @param macroCount the number of macros
 */
void freeMacrosMemory(macro **macros, int macroCount);

/**
 * This function will check if a macro exists in the macros array.
 * @param word the current binaryWord
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
 * This function will remove the given substring from the given string.
 * @param string the string
 * @param sub the substring
 */
void removeSubstring(char* string, const char* sub);

/**
* This function will check if the given binaryWord is a valid macro name.
* @param word the binaryWord to check
* @return 1 if the binaryWord is a valid macro name, else 0
*/
int checkIfMacroNameIsValid(char* word);

/**
 * This function will free the memory allocated for the macros array and close the file.
 * @param file the file to close
 * @param macros the macros array
 * @param macroCount the number of macros
 */
void freeMemoryAndCloseFile(FILE* file, macro **macros, int macroCount);


#endif

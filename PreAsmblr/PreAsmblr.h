#ifndef PREASM_H
#define PREASM_H

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

/*TODO move the definitions to a different file?*/
#define LINES 32 /*Maximum number of lines in a macro, will be adjusted dynamically*/
#define MAXCHARSPERLINE (80+1) /*Maximum number of characters per line*/
#define oldFileFormat ".as" /*Constant with the .as file format*/
#define newFileFormat ".am" /*Constant with the .am file format*/
#define MAXMACROS 32 /*Starter number for number of macros, dynamically allocated further on*/
#define MAXFILENAME 256 /*Maximum file name length*/

/**
 * This struct will contain each indexed word, it's line appearances and the number of appearances, it'll be used extensively in the program.
 */
typedef struct macros{
    char * macroName;
    int linesCounter;
    char **lines;
}macro;



/*TODO ADD COMMENTS*/

/**
 * This function will copy the macros from the old file into the new file.
 * @param source the old file
 * @param oldFileName the old file name
 * @return the new file
 */
FILE* createNewFileAndWriteMacros(FILE* source, const char* oldFileName);

/**
 * This function will allocate memory for the macros array.
 * @param size the size of the array
 * @return the macros array
 */
macro** allocateMemoryToMacros(int size);

/**
 * This function will process the lines of the old file and write the macros into the new file.
 * @param source the old file
 * @param resultFile the new file
 * @param macros the macros array
 * @param macroCount the number of macros
 * @param macroArrSizePtr the size of the macros array
 * @param tmpSizePtr the temporary size of the macros array
 */
void processFileLines(FILE* source, FILE* resultFile, macro **macros, int *macroCount, int * macroArrSizePtr, int * tmpSizePtr);

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
void addNewMacroToMacrosArray(FILE* source, FILE* resultFile, char *lineBuffer, char *word, macro ***macros, int *macroCount, char *currentLine, int *macroArrSize, int *tmpSize);

/**
 * This function will free the memory allocated for the macros array.
 * @param macros the macros array
 * @param macroCount the number of macros
 */
void freeMacrosMemory(macro **macros, int macroCount);

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
 * This function will remove the given substring from the given string.
 * @param string the string
 * @param sub the substring
 */
void removeSubstring(char* string, const char* sub);



#endif

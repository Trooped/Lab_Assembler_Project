#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

/*TODO change the definitions to a different file?*/
#define LINES 50
#define CHARACTERS 81
#define oldFileFormat ".as"
#define newFileFormat ".am"
#define MAXMACROS 500 /*TODO MAKE IT DINAMYC MEMORY*/
#define MAX_FILE_NAME 256 /*TODO HAVE ONLY ONE DEFINITION OF MAXFIELNAME*/

/**
 * This struct will contain each indexed word, it's line appearances and the number of appearances, it'll be used extensively in the program.
 * TODO when creating the array, make it dynamic memory.
 */
typedef struct macros{
    char * macroName;
    int linesCounter; /*TODO make this number of lines dynamic.*/
    char lines[LINES][CHARACTERS];
}macro;

/*TODO ADD COMMENTS*/

FILE* copyMacrosIntoNewFile(FILE* source, const char* oldFileName);

int checkIfMacro(char* word, int macroCount, macro *macros[]);

void writeMacroToFile(FILE* newFile, char* macroName, macro *macros[], int macroCount);
void removeSubstring(char* string, const char* sub);

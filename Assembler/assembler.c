#include "assembler.h"
/*TODO add more stuff to include*/


void assembler(FILE* source, const char* fileName){
    char newFileName[MAX_FILE_NAME];
    FILE* objectFile, *entFile, *extFile;
    word* dataArray = NULL; /* array for data to be put in the memory image */
    word* instructionArray= NULL; /* array for the instructions to be put in the memory image.*/
    List symbolTable = { NULL, NULL }; /* linked list for labels */


    List externalSymbols = { NULL, NULL }; /* a separate list for the external labels defined,
                                             * to later create .ext file*/
    List entrySymbols = { NULL, NULL }; /* same thing for entries. */
    operation operationsArray[OPERATIONS] = {{"mov", 0, 2}, {"cmp", 1, 2}, {"add", 2, 2}, {"sub", 3, 2}, {"lea", 4, 2}, {"not", 5, 1}, {"clr", 6, 1}, {"inc", 7, 1}, {"dec", 8, 1}, {"jmp", 9, 1}, {"bne", 10, 1}, {"red", 11, 1}, {"prn", 12, 1}, {"jsr", 13, 1}, {"rts", 14, 0}, {"hlt", 15, 0}};
    int IC = 0, DC = 0; /* instruction counter and data counter */


    /*initializing both of the arrays*/
    initializeDataArray(&dataArray, 0);
    initializeInstructionArray(&instructionArray, 0);


    /*TODO take care of the above stuff^
     * and also, I need to call the firstPass function from here*/



}


void initializeDataArray(word** dataArray, int initialValue) {
    int i;
    // Allocate memory for initialSize elements
    *dataArray = malloc(MAXDATA * sizeof(word));
    if (*dataArray == NULL) {
        // Handle memory allocation failure
        printf("Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize each element
    for (i = 0; i < MAXDATA; i++) {
        (*dataArray)[i].wordBits = initialValue;
    }
}

void initializeInstructionArray(word** instructionArray, int initialValue) {
    int i;
    // Allocate memory for initialSize elements
    *instructionArray = malloc(MAXINSTRUCTIONS * sizeof(word));
    if (*instructionArray == NULL) {
        // Handle memory allocation failure
        printf("Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize each element
    for (i = 0; i < MAXINSTRUCTIONS; i++) {
        (*instructionArray)[i].wordBits = initialValue;
    }
}
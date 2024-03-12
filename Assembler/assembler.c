#include "assembler.h"
/*TODO add more stuff to include*/


void assembler(FILE* source, const char* fileName){
    char newFileName[MAXFILENAME];
    FILE* objectFile; /* *entFile, *extFile;*/ /*TODO maybe i need to initialize?*/
    word dataArray[MAXDATA]; /* array for data to be put in the memory image */
    word instructionArray[MAXINSTRUCTIONS]; /* array for the instructions to be put in the memory image.*/
    symbolList* symbolTable = NULL; /* linked list for labels */ /*TODO maybe i need to initialize?*/

    operation operationsArray[OPERATIONS] = {{"mov", 0, 2}, {"cmp", 1, 2}, {"add", 2, 2}, {"sub", 3, 2}, {"lea", 4, 2}, {"not", 5, 1}, {"clr", 6, 1}, {"inc", 7, 1}, {"dec", 8, 1}, {"jmp", 9, 1}, {"bne", 10, 1}, {"red", 11, 1}, {"prn", 12, 1}, {"jsr", 13, 1}, {"rts", 14, 0}, {"hlt", 15, 0}};
    int IC = 0, DC = 0; /* instruction counter and data counter */

    error errorInfo = {0, fileName, " "}; /* error flag, file name, error description */


    /*initializing both of the arrays*/
    initializeDataArray(dataArray, 0);
    initializeInstructionArray(instructionArray, 0);

    /*call firstPass function*/

    firstPass(source, dataArray, instructionArray, operationsArray, &symbolTable, &IC, &DC, &errorInfo);

    if (errorInfo.errorFlag == 1) {
        /*TODO print error and exit*/
        return;
    }

    incrementDataSymbolValues(symbolTable, 100);

    /*TODO call secondPass function*/

}


void initializeDataArray(word* dataArray, int initialValue) {
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

void initializeInstructionArray(word* instructionArray, int initialValue) {
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
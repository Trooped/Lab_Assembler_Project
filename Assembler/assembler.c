#include "assembler.h"
/*TODO add more stuff to include*/


void assembler(FILE* source, const char* fileName){
    int i; /*TODO FOR TESTING, DELETE LATER!!!*/

    char newFileName[MAXFILENAME];
    FILE* objectFile; /* *entFile, *extFile;*/ /*TODO maybe i need to initialize?*/
    binaryWord dataArray[MAXDATA]; /* array for data to be put in the memory image */
    binaryWord instructionArray[MAXINSTRUCTIONS]; /* array for the instructions to be put in the memory image.*/
    symbolList* symbolTable = NULL; /* linked list for labels */ /*TODO maybe i need to initialize?*/

    /*TODO maybe declare this in a separate function / just in the constants file?*/
    operationInfo operationsArray[NUMOFOPERATIONS] = {{"mov", 0, 2}, {"cmp", 1, 2}, {"add", 2, 2}, {"sub", 3, 2}, {"lea", 4, 2}, {"not", 5, 1}, {"clr", 6, 1}, {"inc", 7, 1}, {"dec", 8, 1}, {"jmp", 9, 1}, {"bne", 10, 1}, {"red", 11, 1}, {"prn", 12, 1}, {"jsr", 13, 1}, {"rts", 14, 0}, {"hlt", 15, 0}};
    int IC = 0, DC = 0; /* instruction counter and data counter */


    /*TODO initialize it correctly using a function!!!!!!!!*/
    error* errorInfo = malloc(sizeof(error));
    if (errorInfo == NULL) {
        /* Handle memory allocation failure*/
        fprintf(stderr, "Failed to allocate memory for errorInfo\n");
        return;
    }
    /* Initialize errorInfo*/
    errorInfo->errorFlag = 0;
    strcpy(errorInfo->fileName, fileName);
    errorInfo->errorDescription[0] = '\0';



    /*initializing both of the arrays*/
    initializeDataArray(dataArray, 0);
    initializeInstructionArray(instructionArray, 0);

    /*call firstPass function*/

    firstPass(source, dataArray, instructionArray, operationsArray, &symbolTable, &IC, &DC, &errorInfo);

    /*TODO TESTINGGGGGGGGG*/
    printSymbolList(symbolTable);
    printf("\nIC: %d\n", IC);
    printf("DC: %d\n", DC);
    for (i = 0; i < DC; i++) {
        printf("dataArray[%d]: ", i);
        printBits(dataArray[i].wordBits);
        printf("\n");
    }
    for (i = 0; i < IC; i++) {
        printf("instructionArray[%d]: ", i);
        printBits(instructionArray[i].wordBits);
        printf("\n");
    }
    /*TODO testinggggggggggggggggggggg*/


    if (errorInfo->errorFlag == 1) {
        /*TODO print error and exit*/
        deleteSymbolList(symbolTable);
        return;
    }

    incrementDataSymbolValues(symbolTable, 100);




    /*TODO call secondPass function*/


    deleteSymbolList(symbolTable);
}


void initializeDataArray(binaryWord* dataArray, int initialValue) {
    int i;

    /* Initialize each element*/
    for (i = 0; i < MAXDATA; i++) {
        dataArray[i].wordBits = initialValue;
    }
}

void initializeInstructionArray(binaryWord* instructionArray, int initialValue) {
    int i;

    /* Initialize each element*/
    for (i = 0; i < MAXINSTRUCTIONS; i++) {
        instructionArray[i].wordBits = initialValue;
    }
}

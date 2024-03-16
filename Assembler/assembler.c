#include "assembler.h"

void assembler(FILE* source, const char* fileName){
    binaryWord dataArray[MAXDATA]; /* array for data to be put in the memory image */
    binaryWord instructionArray[MAXINSTRUCTIONS]; /* array for the instructions to be put in the memory image.*/
    symbolList* symbolTable = NULL; /* linked list for labels */
    error* errorInfo; /* Empty errorInfo struct for errors!*/
    operationInfo operationsArray[NUMOFOPERATIONS];
    int IC = 0, DC = 0; /* instruction counter and data counter */

    int i; /*TODO FOR TESTING, DELETE LATER!!!*/

    initializeOperationsArray(operationsArray); /*initialize operations array according to the known operations*/



    /*TODO I want it here??*/
    char newFileName[MAXFILENAME];
    FILE* objectFile; /* *entFile, *extFile;*/





    /*TODO real start of assembler function!*/

    initializeErrorInfo(&errorInfo,&symbolTable, fileName, source); /*initialize errorInfo struct*/
    /*initializing both of the arrays*/
    initializeDataArray(dataArray, 0);
    initializeInstructionArray(instructionArray, 0);

    /*call firstPass function*/

    firstPass(source, dataArray, instructionArray, operationsArray, &symbolTable, &IC, &DC, &errorInfo);

    if (errorInfo->errorFlag == 1) {
        printf("%d Errors were found in your program, exiting the process\n", errorInfo->counter);
        closeFileAndExit(&errorInfo, &symbolTable);
    }

    incrementDataSymbolValues(&symbolTable, (IC)+100);

    rewind(source); /*reset the file pointer to the beginning of the file*/
    printf("SECOND PASS\n");
    secondPass(source, dataArray, instructionArray, operationsArray, &symbolTable, &IC, &DC, &errorInfo);


    /*TODO TESTINGGGGGGGGG*/
    printSymbolList(symbolTable);
    printf("\nIC: %d\n", IC);
    printf("DC: %d\n", DC);
    for (i = 0; i < DC; i++) {
        printf("dataArray[%d]: ", i);
        printBits(dataArray[i].wordBits);
        printf("\t decimal: %d", instructionArray[i].wordBits);
        printf("\n");
    }
    for (i = 0; i < IC; i++) {
        printf("instructionArray[%d]: ", i);
        printBits(instructionArray[i].wordBits);
        printf("\t decimal: %d", instructionArray[i].wordBits);
        printf("\n");
    }

    /*TODO testinggggggggggggggggggggg*/

    deleteSymbolList(&symbolTable);
    free(errorInfo);
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


#include "include/dataStrcuturesInitialization.h"


/**
 * This function initializes the operands array with null characters.
 *
 * @param operands The operands array to initialize.
 */
void initializeOperandsArray(char operands[MAXOPERANDS][MAXOPERANDLENGTH]) {
    int i, j;
    for (i = 0; i < MAXOPERANDS; i++) {
        for (j = 0; j < MAXOPERANDLENGTH; j++) {
            operands[i][j] = '\0';
        }
    }
}


/**
 * This function initializes the operations array with the names, codes, and number of operands for each of the 16 operations.
 *
 * @param operationsArray The operations array to initialize.
 */
void initializeOperationsArray(operationInfo* operationsArray) {
    /* Initialize operation 0 */
    operationsArray[0].name = "mov";
    operationsArray[0].code = 0;
    operationsArray[0].numOfOperands = 2;

    /* Initialize operation 1 */
    operationsArray[1].name = "cmp";
    operationsArray[1].code = 1;
    operationsArray[1].numOfOperands = 2;

    /* Initialize operation 2 */
    operationsArray[2].name = "add";
    operationsArray[2].code = 2;
    operationsArray[2].numOfOperands = 2;

    /* Initialize operation 3 */
    operationsArray[3].name = "sub";
    operationsArray[3].code = 3;
    operationsArray[3].numOfOperands = 2;

    /* Initialize operation 4 */
    operationsArray[4].name = "lea";
    operationsArray[4].code = 4;
    operationsArray[4].numOfOperands = 2;

    /* Initialize operation 5 */
    operationsArray[5].name = "not";
    operationsArray[5].code = 5;
    operationsArray[5].numOfOperands = 1;

    /* Initialize operation 6 */
    operationsArray[6].name = "clr";
    operationsArray[6].code = 6;
    operationsArray[6].numOfOperands = 1;

    /* Initialize operation 7 */
    operationsArray[7].name = "inc";
    operationsArray[7].code = 7;
    operationsArray[7].numOfOperands = 1;

    /* Initialize operation 8 */
    operationsArray[8].name = "dec";
    operationsArray[8].code = 8;
    operationsArray[8].numOfOperands = 1;

    /* Initialize operation 9 */
    operationsArray[9].name = "jmp";
    operationsArray[9].code = 9;
    operationsArray[9].numOfOperands = 1;

    /* Initialize operation 10 */
    operationsArray[10].name = "bne";
    operationsArray[10].code = 10;
    operationsArray[10].numOfOperands = 1;

    /* Initialize operation 11 */
    operationsArray[11].name = "red";
    operationsArray[11].code = 11;
    operationsArray[11].numOfOperands = 1;

    /* Initialize operation 12 */
    operationsArray[12].name = "prn";
    operationsArray[12].code = 12;
    operationsArray[12].numOfOperands = 1;

    /* Initialize operation 13 */
    operationsArray[13].name = "jsr";
    operationsArray[13].code = 13;
    operationsArray[13].numOfOperands = 1;

    /* Initialize operation 14 */
    operationsArray[14].name = "rts";
    operationsArray[14].code = 14;
    operationsArray[14].numOfOperands = 0;

    /* Initialize operation 15 */
    operationsArray[15].name = "hlt";
    operationsArray[15].code = 15;
    operationsArray[15].numOfOperands = 0;
}


/**
 * This function initializes the data array with a given value.
 * @param dataArray The array to be initialized.
 * @param initialValue The value to initialize the array with.
 */
void initializeDataArray(binaryWord* dataArray, int initialValue) {
    int i;

    /* Initialize each element*/
    for (i = 0; i < MAXDATA; i++) {
        dataArray[i].wordBits = initialValue;
    }
}

/**
 * This function initializes the instruction array with a given value.
 * @param instructionArray The array to be initialized.
 * @param initialValue The value to initialize the array with.
 */
void initializeInstructionArray(binaryWord* instructionArray, int initialValue) {
    int i;

    /* Initialize each element*/
    for (i = 0; i < MAXINSTRUCTIONS; i++) {
        instructionArray[i].wordBits = initialValue;
    }
}

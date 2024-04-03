/**
 * @file dataStrcuturesInitialization.c
 *
 * This file contains the implementation of the functions that initialize the data structures used in the assembler.
 *
 * The functions in this file are:
 * 1. initializeOperandsArray - This function initializes the operands array with null characters.
 * 2. initializeOperationsArray - This function initializes the operations array with the names, codes, and number of operands for each of the 16 operations.
 * 3. initializeDataArray - This function initializes the data array with a given value.
 * 4. initializeInstructionArray - This function initializes the instruction array with a given value.
 */

#include "initializationFunctions.h"
#include "syntaxAnalyzer.h"


/**
 * This function initializes the operands array with null characters.
 *
 * @param operands The operands array to initialize.
 */
void initializeOperandsArray(char operands[MAX_OPERANDS][MAX_OPERAND_LENGTH]) {
    int i, j; /* Loop counters */
    for (i = 0; i < MAX_OPERANDS; i++) { /* Loop through the array */
        for (j = 0; j < MAX_OPERAND_LENGTH; j++) { /* Loop through the string */
            operands[i][j] = '\0'; /* Set each character to null */
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
    operationsArray[0].code = MOV_OPCODE;
    operationsArray[0].numOfOperands = TWO_OPERANDS;

    /* Initialize operation 1 */
    operationsArray[1].name = "cmp";
    operationsArray[1].code = CMP_OPCODE;
    operationsArray[1].numOfOperands = TWO_OPERANDS;

    /* Initialize operation 2 */
    operationsArray[2].name = "add";
    operationsArray[2].code = ADD_OPCODE;
    operationsArray[2].numOfOperands = TWO_OPERANDS;

    /* Initialize operation 3 */
    operationsArray[3].name = "sub";
    operationsArray[3].code = SUB_OPCODE;
    operationsArray[3].numOfOperands = TWO_OPERANDS;

    /* Initialize operation 4 */
    operationsArray[4].name = "not";
    operationsArray[4].code = NOT_OPCODE;
    operationsArray[4].numOfOperands = ONE_OPERAND;

    /* Initialize operation 5 */
    operationsArray[5].name = "clr";
    operationsArray[5].code = CLR_OPCODE;
    operationsArray[5].numOfOperands = ONE_OPERAND;

    /* Initialize operation 6 */
    operationsArray[6].name = "lea";
    operationsArray[6].code = LEA_OPCODE;
    operationsArray[6].numOfOperands = TWO_OPERANDS;

    /* Initialize operation 7 */
    operationsArray[7].name = "inc";
    operationsArray[7].code = INC_OPCODE;
    operationsArray[7].numOfOperands = ONE_OPERAND;

    /* Initialize operation 8 */
    operationsArray[8].name = "dec";
    operationsArray[8].code = DEC_OPCODE;
    operationsArray[8].numOfOperands = ONE_OPERAND;

    /* Initialize operation 9 */
    operationsArray[9].name = "jmp";
    operationsArray[9].code = JMP_OPCODE;
    operationsArray[9].numOfOperands = ONE_OPERAND;

    /* Initialize operation 10 */
    operationsArray[10].name = "bne";
    operationsArray[10].code = BNE_OPCODE;
    operationsArray[10].numOfOperands = ONE_OPERAND;

    /* Initialize operation 11 */
    operationsArray[11].name = "red";
    operationsArray[11].code = RED_OPCODE;
    operationsArray[11].numOfOperands = ONE_OPERAND;

    /* Initialize operation 12 */
    operationsArray[12].name = "prn";
    operationsArray[12].code = PRN_OPCODE;
    operationsArray[12].numOfOperands = ONE_OPERAND;

    /* Initialize operation 13 */
    operationsArray[13].name = "jsr";
    operationsArray[13].code = JSR_OPCODE;
    operationsArray[13].numOfOperands = ONE_OPERAND;

    /* Initialize operation 14 */
    operationsArray[14].name = "rts";
    operationsArray[14].code = RTS_OPCODE;
    operationsArray[14].numOfOperands = ZERO_OPERANDS;

    /* Initialize operation 15 */
    operationsArray[15].name = "hlt";
    operationsArray[15].code = HLT_OPCODE;
    operationsArray[15].numOfOperands = ZERO_OPERANDS;
}


/**
 * This function initializes the data array with a given value.
 * @param dataArray The array to be initialized.
 * @param initialValue The value to initialize the array with.
 */
void initializeDataArray(memoryWord* dataArray, int initialValue) {
    int i; /* Loop counter */

    /* Initialize each element*/
    for (i = 0; i < DATA_ARRAY_SIZE; i++) { /* Loop through the array */
        dataArray[i].wordBits = initialValue; /* Set the value of the word to the initial value */
    }
}

/**
 * This function initializes the instruction array with a given value.
 * @param instructionArray The array to be initialized.
 * @param initialValue The value to initialize the array with.
 */
void initializeInstructionArray(memoryWord* instructionArray, int initialValue) {
    int i;  /* Loop counter */

    /* Initialize each element*/
    for (i = 0; i < INSTRUCTION_ARRAY_SIZE; i++) { /* Loop through the array */
        instructionArray[i].wordBits = initialValue; /* Set the value of the word to the initial value */
    }
}

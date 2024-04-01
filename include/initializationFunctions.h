/**
 * @file initializationFunctions.h
 * This file contains the functions that initialize the data structures used in the assembler.
 *
 * The functions in this file are:
 * 1. initializeOperandsArray - This function initializes the operands array with null characters.
 * 2. initializeOperationsArray - This function initializes the operations array with the names, codes, and number of operands for each of the 16 operations.
 * 3. initializeDataArray - This function initializes the data array with a given value.
 * 4. initializeInstructionArray - This function initializes the instruction array with a given value.
 */

#ifndef INITIALIZATIONFUNCTIONS_H
#define INITIALIZATIONFUNCTIONS_H

#include "dataStructuresAndConstants.h"

/**
 * This function initializes the operands array with null characters.
 *
 * @param operands The operands array to initialize.
 */
void initializeOperandsArray(char operands[MAX_OPERANDS][MAX_OPERAND_LENGTH]);

/**
 * This function initializes the operations array with the names, codes, and number of operands for each of the 16 operations.
 *
 * @param operationsArray The operations array to initialize.
 */
void initializeOperationsArray(operationInfo* operationsArray);

/**
 * This function initializes the data array with a given value.
 * @param dataArray The array to be initialized.
 * @param initialValue The value to initialize the array with.
 */
void initializeDataArray(memoryWord* dataArray, int initialValue);

/**
 * This function initializes the instruction array with a given value.
 * @param instructionArray The array to be initialized.
 * @param initialValue The value to initialize the array with.
 */
void initializeInstructionArray(memoryWord* instructionArray, int initialValue);

#endif

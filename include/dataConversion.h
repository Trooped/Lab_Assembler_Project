/**
 * @file dataConversion.h
 * This file contains the functions that convert the data to binary and insert it into the memory image.
 *
 * The functions in this file are:
 * 1. insertFirstInstructionIntoArray - This function inserts the first instruction into the instruction array.
 * 2. analyzeOperandsAndInsertIntoArraySecondPass - This function inserts the operands into the instruction array.
 * 3. convertOperandToBinaryAndInsertIntoArray - This function converts an operand to binary and inserts it into the instruction array.
 * 4. convertValueToBinaryAndInsertToDataArray - This function adds a value in binary to the data array.
 * 5. convertBinaryToBase4Symbols - This function converts a binary number to base 4 symbols.
 */

#ifndef _DATACONVERSION_H
#define _DATACONVERSION_H

#include "dataStructuresAndConstants.h"
#include "syntaxAnalyzer.h"
#include "listFunctions.h"
#include "error.h"

/* Constants for the number of bits to shift by*/
#define SHIFT_6 6 /* The number of bits to shift by 6*/
#define SHIFT_4 4 /* The number of bits to shift by 4*/
#define SHIFT_2 2 /* The number of bits to shift by 2*/
#define SHIFT_5 5 /* The number of bits to shift by 5*/

/* A,R,E for addressing methods values */
#define ABSOLUTE_ADDRESSING 0x0000 /*A*/
#define RELOCATABLE_ADDRESSING 0x0002 /*R*/
#define EXTERNAL_ADDRESSING 0x0001 /*E*/

#define NUM_OF_SYMBOLS 4 /*The number of symbols in base 4*/
#define NUM_OF_BITS_TO_EXTRACT (0x3) /*The number of bits to extract from the binary number*/

/**
 * This function inserts the first instruction into the instruction array.
 * meaning - the first word of each line in the instruction array.
 *
 * @param instructionArray The instruction array to insert the instruction into.
 * @param IC The instruction errorCounter.
 * @param opcode The opcode of the instruction.
 * @param firstOperand The first operand of the instruction.
 * @param secondOperand The second operand of the instruction.
 */
void insertFirstInstructionIntoArray(memoryWord* instructionArray, int IC, int opcode, int firstOperand, int secondOperand);

/**
 * This function inserts the operands into the instruction array, calling the convertOperandToBinaryAndInsertIntoArray function for each operand.
 *
 * @param instructionArray The instruction array to insert the operands into.
 * @param numOfLines The number of lines in the file.
 * @param IC The instruction errorCounter.
 * @param operands The operands to insert into the instruction array.
 * @param head The head of the symbol table.
 * @param errorInfo A pointer to the errorInfo struct.
 */
void analyzeOperandsAndInsertIntoArraySecondPass(memoryWord* instructionArray, int numOfLines, int *IC, char operands[MAX_OPERANDS][MAX_OPERAND_LENGTH], symbolList** head, error** errorInfo);

/**
 * This function converts an operand to binary and inserts it into the instruction array.
 * it first checks it's type, and then converts it to binary accordingly.
 *
 * @param instructionArray The instruction array to insert the operand into.
 * @param IC The instruction errorCounter.
 * @param operand The operand to convert and insert.
 * @param head The head of the symbol table.
 * @param errorInfo A pointer to the errorInfo struct.
 * @param source A flag to indicate if the operand is a source operand (1) or a destination operand (0).
 * @param offset A flag to indicate if the operand is an offset operand.
 * @param prevLabelPtr A pointer to the PREVIOUS label in the symbol table (to check if we're out of array bounds in offset declarations).
 */
void convertOperandToBinaryAndInsertIntoArray(memoryWord* instructionArray, int IC, char* operand, symbolList** head, error** errorInfo, int source, int offset, symbolList* prevLabelPtr);

/**
 * This function adds a value in binary to the data array.
 * used with .data and .string values.
 *
 * @param dataArray The data array to add the value to.
 * @param DC The data errorCounter.
 * @param value The value to add to the data array.
 */
void convertValueToBinaryAndInsertToDataArray(memoryWord* dataArray, int DC, int value);

/**
 * This function converts a binary number to base 4 symbols, which are encrypted symbols.
 * The function converts the binary number two bits at a time, and assigns a symbol to each pair of bits.
 * The symbols are '*' for 00, '#' for 01, '%' for 10, and '!' for 11.
 *
 * @param binary The binary number to convert.
 * @return The base 4 symbols representing the binary number.
 */
char* convertBinaryToBase4Symbols(int binary);


#endif


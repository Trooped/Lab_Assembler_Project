#ifndef _DATACONVERSION_H
#define _DATACONVERSION_H

#include "dataStructuresAndConstants.h"
#include "utilities.h"
#include "syntaxAnalyzer.h"
#include "listFunctions.h"
#include "error.h"

/* Constants for the number of bits to shift by*/
#define SHIFT_6 6 /* The number of bits to shift by 6*/
#define SHIFT_4 4 /* The number of bits to shift by 4*/
#define SHIFT_2 2 /* The number of bits to shift by 2*/
#define SHIFT_5 5 /* The number of bits to shift by 5*/

/* A,R,E for addressing methods values */
#define ABSOLUTE_ADDRESSING 0x0000
#define RELOCATABLE_ADDRESSING 0x0002
#define EXTERNAL_ADDRESSING 0x0001

void insertFirstInstructionIntoArray(binaryWord* instructionArray, int IC, int opcode, int firstOperand, int secondOperand);

void analyzeOperandsAndInsertIntoArraySecondPass(binaryWord* instructionArray, int numOfLines, int *IC, char operands[MAX_OPERANDS][MAX_OPERAND_LENGTH], symbolList** head, error** errorInfo);

void convertOperandToBinaryAndInsertIntoArray(binaryWord* instructionArray, int IC, char* operand, symbolList** head, error** errorInfo, int source, int offset);

void convertValueToBinaryAndInsertToDataArray(binaryWord* dataArray, int DC, int value);

char* convertBinaryToBase4Symbols(int binary);


#endif


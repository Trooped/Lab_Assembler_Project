#ifndef _DATACONVERSION_H
#define _DATACONVERSION_H

#include "dataStructuresAndConstants.h"
#include "utilities.h"
#include "syntaxAnalyzer.h"
#include "listFunctions.h"
#include "error.h"

void insertFirstInstructionIntoArray(binaryWord* instructionArray, int IC, int opcode, int firstOperand, int secondOperand);

void convertOperandToBinaryAndInsertIntoArray(binaryWord* instructionArray, int IC, char* operand, symbolList** head, error** errorInfo, int source, int offset);

void insertOperandsIntoInstructionArray(binaryWord* instructionArray, int numOfLines, int *IC, char operands[MAXOPERANDS][MAXOPERANDLENGTH], symbolList** head, error** errorInfo);

void addValueToDataArray(binaryWord* dataArray, int DC, int value);

char* convertBinaryToBase4Symbols(int binary);


#endif


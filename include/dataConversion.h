#ifndef _DATACONVERSION_H
#define _DATACONVERSION_H

#include "dataStructuresAndConstants.h"
#include "utilities.h"
#include "syntaxAnalyzer.h"
#include "listFunctions.h"
#include "error.h"

void insertFirstInstructionIntoArray(binaryWord* instructionArray, int IC, int opcode, int firstOperand, int secondOperand);

void convertOperandToBinaryAndInsertIntoArray(binaryWord* instructionArray, int IC, char* operand, symbolList** head, error** errorInfo, int source, int offset);

void convertValueToBinaryAndInsertToDataArray(binaryWord* dataArray, int DC, int value);

char* convertBinaryToBase4Symbols(int binary);


#endif


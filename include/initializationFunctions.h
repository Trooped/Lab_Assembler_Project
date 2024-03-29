
#ifndef INITIALIZATIONFUNCTIONS_H
#define INITIALIZATIONFUNCTIONS_H

#include "dataStructuresAndConstants.h"


void initializeOperationsArray(operationInfo* operationsArray);

void initializeOperandsArray(char operands[MAX_OPERANDS][MAX_OPERAND_LENGTH]);

void initializeDataArray(binaryWord* dataArray, int initialValue);

void initializeInstructionArray(binaryWord* instructionArray, int initialValue);

#endif
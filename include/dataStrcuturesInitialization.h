
#ifndef _DATASTRCUTURESINITIALIZATION_H
#define _DATASTRCUTURESINITIALIZATION_H

#include "dataStructuresAndConstants.h"


void initializeOperationsArray(operationInfo* operationsArray);

void initializeOperandsArray(char operands[MAXOPERANDS][MAXOPERANDLENGTH]);

void initializeDataArray(binaryWord* dataArray, int initialValue);

void initializeInstructionArray(binaryWord* instructionArray, int initialValue);

#endif

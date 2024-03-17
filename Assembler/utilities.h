#ifndef UTILITIES_H
#define UTILITIES_H

#include "dataStructuresAndConstants.h"
#include "syntaxAnalyzer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>


int findSymbolValue(symbolList **head, const char* name,char* type, int* value);

void markLabelAsEntry(symbolList** head, char* line, error** errorInfo);

char* removeColon(char* label);

int searchSymbolList(symbolList** head, char* name, char* type);

void addLabel(symbolList** head, char* name, char* type, int value, error** errorInfo);

void addExternAddress(symbolList** head, char* name, int address);

void initializeOperationsArray(operationInfo* operationsArray);

void closeFileAndExit(error** errorInfo, symbolList** symbolTable);

void deleteSymbolList(symbolList** head);

void insertFirstInstructionIntoArray(binaryWord* instructionArray, int IC, int opcode, int firstOperand, int secondOperand);

void convertOperandToBinaryAndInsertIntoArray(binaryWord* instructionArray, int IC, char* operand, symbolList** head, error** errorInfo, int source, int offset);

void insertOperandsIntoInstructionArray(binaryWord* instructionArray, int numOfLines, int *IC, char operands[MAXOPERANDS][MAXOPERANDLENGTH], symbolList** head, error** errorInfo);

void addValueToDataArray(binaryWord* dataArray, int DC, int value);

void printError(error** errorInfo, char* errorDescription);

void incrementDataSymbolValues(symbolList** head, int byValue);

void initializeOperandsArray(char operands[MAXOPERANDS][MAXOPERANDLENGTH]);

void initializeErrorInfo(error** errorInfo,symbolList** symbolTable, char* fileName, FILE* file);

void printSymbolList(const symbolList* head);
void printBits(int value);

#endif

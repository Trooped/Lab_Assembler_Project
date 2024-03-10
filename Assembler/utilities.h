#ifndef UTILITIES_H
#define UTILITIES_H

#include "assembler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "firstPass.h"


int findSymbolValue(symbolList **head, const char* name,char* type, int* value);

int searchSymbolList(symbolList** head, char* name, char* type);

symbolList* addLabel(symbolList* head, char* name, char* type, int value);

void deleteSymbolListNode(symbolList* node);

void deleteSymbolList(symbolList* head);

void insertInstructionIntoArray(word* instructionArray, int IC, int opcode, int firstOperand, int secondOperand);

void addValueToDataArray(word **dataArray, int DC, int value);

#endif

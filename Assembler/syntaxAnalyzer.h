#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include "dataStructuresAndConstants.h"
#include "utilities.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>


int getOperandCode(char* operand, symbolList** head, operationInfo* operationsArray, error** errorInfo);

void parseOperandsFirstPass(char *input, char operands[MAXOPERANDS][MAXOPERANDLENGTH]);

void parseOperandsSecondPass(const char* operand, char* outOperand, char* outLabelOrDefine);
int isSymbolExtern(symbolList** head, char* symbolName);

void trimWhitespace(char* str);

int isDefine(char* word);

int isData(char* word);

int isString(char* word);

int isExtern(char* word);

void checkEntrySyntax(symbolList** head, char* line, error** errorInfo, operationInfo* operationsArray);

int isEntry(char* word);

int isValidLabelName(char* name, operationInfo* operationsArray, symbolList** head, int colonFlag);

int isValidOperation(char* word, operationInfo* operationsArray);

int isValidInteger(char* str);

int isRegister(char* word);








#endif
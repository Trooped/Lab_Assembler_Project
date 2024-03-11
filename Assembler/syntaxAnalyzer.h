#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include "assembler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "firstPass.h"
#include "utilities.h"


int getOperandCode(char* operand, symbolList** head, operation* operationsArray, error* errorInfo);

void parseOperands(char *input, char operands[MAXOPERANDS][MAXOPERANDLENGTH]);

int isDefine(char* word);

int isData(char* word);

int isString(char* word);

int isExtern(char* word);

int isEntry(char* word);

int isValidLabelName(char* name, operation* operationsArray, symbolList** head, int colonFlag);

int isValidOperation(char* word, operation* operationsArray);

int isValidInteger(char* str);

int isRegister(char* word);








#endif
#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include "dataStructuresAndConstants.h"
#include "utilities.h"
#include "listFunctions.h"
#include "error.h"
#include "dataStrcuturesInitialization.h"
#include "dataConversion.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>


int getOperandCode(char* operand, symbolList** head, operationInfo* operationsArray, error** errorInfo);

int handleOperation(symbolList** head, binaryWord* instructionArray, int operation, char* line, int *IC, operationInfo* operationsArray, error** errorInfo, int isSecondPass);

void handleData(char* type, char* line, symbolList ** head, int *DC, binaryWord* dataArray, error** errorInfo);

void handleExtern(symbolList** head, char* line, error** errorInfo, operationInfo* operationsArray, int labelFlag);

void handleDefine(symbolList** head, operationInfo* operationsArray, char* line, error** errorInfo);

int parseOperandsFirstPass(char *input, char operands[MAXOPERANDS][MAXOPERANDLENGTH], error** errorInfo);

void analyzeOperandsAndInsertIntoArraySecondPass(binaryWord* instructionArray, int numOfLines, int *IC, char operands[MAXOPERANDS][MAXOPERANDLENGTH], symbolList** head, error** errorInfo);

void parseOperandsSecondPass(const char* operand, char* outOperand, char* outLabelOrDefine);

void trimWhitespace(char* str);

int isDefine(char* word);

int isData(char* word);

int isString(char* word);

int isExtern(char* word);

void checkEntrySyntax(symbolList** head, char* line, error** errorInfo, operationInfo* operationsArray, int labelFlag);

int isEntry(char* word);

int isValidLabelName(char* name, operationInfo* operationsArray, symbolList** head, int colonFlag);

int isValidOperation(char* word, operationInfo* operationsArray);

int isValidInteger(char* str);

int isRegister(char* word);

int checkLineLengthAndSkip(FILE *sourceFile, char *lineBuffer, error** errorInfo);








#endif
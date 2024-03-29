#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include "dataStructuresAndConstants.h"
#include "utilities.h"
#include "listFunctions.h"
#include "error.h"
#include "initializationFunctions.h"
#include "dataConversion.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#define NO_OPERAND (0) /*No operand constant*/

/*Constants for operation codes*/
#define MOV_OP 0 /*MOV operation code*/
#define CMP_OP 1 /*CMP operation code*/
#define ADD_OP 2 /*ADD operation code*/
#define SUB_OP 3 /*SUB operation code*/
#define NOT_OP 4 /*NOT operation code*/
#define CLR_OP 5 /*CLR operation code*/
#define LEA_OP 6 /*LEA operation code*/
#define INC_OP 7 /*INC operation code*/
#define DEC_OP 8 /*DEC operation code*/
#define JMP_OP 9 /*JMP operation code*/
#define BNE_OP 10 /*BNE operation code*/
#define RED_OP 11 /*RED operation code*/
#define PRN_OP 12 /*PRN operation code*/
#define JSR_OP 13 /*JSR operation code*/
#define RTS_OP 14 /*RTS operation code*/
#define HLT_OP 15 /*HLT operation code*/

int getOperandCode(char* operand, symbolList** head, operationInfo* operationsArray, error** errorInfo);

int handleOperation(symbolList** head, binaryWord* instructionArray, int operation, char* line, int *IC, operationInfo* operationsArray, error** errorInfo, int isSecondPass);

void handleData(char* type, char* labelName, char* line, symbolList ** head, int *DC, binaryWord* dataArray, error** errorInfo);

void handleExtern(symbolList** head, char* line, error** errorInfo, operationInfo* operationsArray, int labelFlag);

void handleDefine(symbolList** head, operationInfo* operationsArray, char* line, error** errorInfo);

int parseOperandsFirstPass(char *input, char operands[MAX_OPERANDS][MAX_OPERAND_LENGTH], error** errorInfo);

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

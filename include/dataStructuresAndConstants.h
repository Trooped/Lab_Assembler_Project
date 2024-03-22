#ifndef DATASTRUCTURESANDCONSTANTS_H
#define DATASTRUCTURESANDCONSTANTS_H


#include <stdio.h>

/*TODO order in this file, especially the defines!!*/

#define NUMOFOPERATIONS 16 /*Number of operations*/
#define MAXFILENAME 64 /*Maximum file name length*/
#define WORDSIZE 14 /*Word size*/
#define MAXDATA 2048 /*Maximum number of data in the array*/
#define MAXINSTRUCTIONS 2048 /*Maximum number of instructions in the array*/
#define MAXWORDS (MAXDATA + MAXINSTRUCTIONS - 100) /*Maximum number of words in the array*/
#define MAXDATAVALUESINARRAY 36 /*Maximum number of data values in an array*/
#define MAXLABELNAME 31 /*Maximum number of characters in a label name*/
#define MAXOPERANDS 4 /*Maximum number of operands in an instruction + 2 spaces for offsets (if relevant)*/
#define MAXOPERANDLENGTH (MAXLABELNAME+1+MAXLABELNAME+1) /*Maximum number of characters in an operand, calculated with maximum label name length, + '[' + maximum label name length + ']'*/
#define MININTEGER12BIT (-2048) /*Minimum number for 12 bits to represent*/
#define MAXINTEGER12BIT 2047 /*Maximum number for 12 bits to represent*/
#define INSTRUCTIONFAILCODE (-999) /*Specific code for when an instruction line fails (a line with operations and operands)*/
#define MAXCHARSPERLINE (80+1) /*Maximum number of characters per line*/
#define INITIAL_IC_VALUE 100 /*Initial value for the instruction counter, 0-99 are reserved*/
#define MAXEXTERNALADDRESSES 64 /*Maximum number of addresses reserved for each external symbol*/

typedef struct symbolList {
    char name[MAXLABELNAME]; /* Label name*/
    char type[MAXLABELNAME]; /*define, data, extern, or code*/
    int isEntry;
    int externalAddresses[MAXEXTERNALADDRESSES];
    int value; /* Label value*/
    struct symbolList* next; /* Pointer to the next node*/
} symbolList;


typedef struct binaryWord{
    int wordBits : WORDSIZE;
}binaryWord;

typedef struct operations{
    char* name;
    int code;
    int numOfOperands;
}operationInfo;

typedef struct errorInfo{
    int errorFlag;
    int errorCounter;
    char fileName[MAXFILENAME];
    char lineText[MAXCHARSPERLINE];
    int lineCounter;
    FILE* file;
}error;



#endif

#ifndef DATASTRUCTURESANDCONSTANTS_H
#define DATASTRUCTURESANDCONSTANTS_H


#include <stdio.h>


/*Memory image constants*/
#define WORD_SIZE 14 /*Word size*/
#define ENCRYPTED_WORD_SIZE 7 /*Encrypted word size*/
#define DATA_ARRAY_SIZE 3996 /*Maximum number of data in the array*/
#define INSTRUCTION_ARRAY_SIZE 3996 /*Maximum number of instructions in the array*/
#define MEMORY_IMAGE_SIZE (DATA_ARRAY_SIZE + INSTRUCTION_ARRAY_SIZE) /*Maximum number of words in the array*/
#define INITIAL_IC_VALUE 100 /*Initial value for the instruction counter, 0-99 are reserved*/

/*General constants*/
#define MAX_FILE_NAME 64 /*Maximum file name length*/
#define MAX_LABEL_NAME 31 /*Maximum number of characters in a label name*/
#define NUM_OF_OPERATIONS 16 /*Number of total operations in our assembly language*/
#define MAX_CHARS_PER_LINE (80+1) /*Maximum number of characters per line*/
#define MIN_INTEGER_12BIT (-2048) /*Minimum number for 12 bits to represent*/
#define MAX_INTEGER_12BIT 2047 /*Maximum number for 12 bits to represent*/
#define MAX_EXTERNAL_ADDRESSES 32 /*Maximum number of addresses reserved for each external symbol*/

/*Data and instruction constants*/
#define MAX_VALUES_PER_DATA_DIRECTIVE 36 /*Maximum number of data values in an array*/
#define MAX_OPERANDS 4 /*Maximum number of operands in an instruction + 2 spaces for offsets (if relevant)*/
#define MAX_OPERAND_LENGTH (MAX_LABEL_NAME+1+MAX_LABEL_NAME+1) /*Maximum number of characters in an operand, calculated with maximum label name length, + '[' + maximum label name length + ']'*/
#define INSTRUCTION_FAIL_CODE (-999) /*Specific code for when an instruction line fails (a line with operations and operands)*/


/*A struct for the symbol list, which will be used for a linked list consisting of the symbols in the program- "symbol table"*/
typedef struct symbolList {
    char name[MAX_LABEL_NAME]; /* Label name*/
    char type[MAX_LABEL_NAME]; /*define, data, extern, or code*/
    int isEntry; /* 1 if the label is an entry, 0 otherwise*/
    int externalAddresses[MAX_EXTERNAL_ADDRESSES]; /* Array of external addresses, per external symbol*/
    int value; /* Label value*/
    struct symbolList* next; /* Pointer to the next node*/
} symbolList;

/*A struct for the binary word, which will be used to store the binary words in the data and instruction arrays*/
typedef struct binaryWord{
    int wordBits : WORD_SIZE; /* The bits of the word*/
}binaryWord;

/*A struct for the operation info, which will be used to store the operations in the operations array*/
typedef struct operations{
    char* name; /* Operation name*/
    int code; /* Operation code*/
    int numOfOperands; /* Number of operands*/
}operationInfo;

/*A struct for the error info, which will be used to store the errors in the error struct and print them at the end of the program*/
typedef struct errorInfo{
    int errorFlag;
    int errorCounter;
    char fileName[MAX_FILE_NAME];
    char lineText[MAX_CHARS_PER_LINE];
    int lineCounter;
    FILE* file;
}error;


#endif

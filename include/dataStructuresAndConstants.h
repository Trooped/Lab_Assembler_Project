/**
 * dataStructuresAndConstants.h
 * This file contains the data structures and constants used in the assembler.
 * The data structures are used to store the data and instructions, as well as the symbols and errors.
 * The constants are used to define the sizes of the arrays and the maximum values for the data and instructions, as well as other general constants.
 *
 * The data structures include:
 * 1. symbolList - A struct for the symbol list, which will be used for a linked list consisting of the symbols in the program- "symbol table"
 * 2. memoryWord - A struct for the binary word, which will be used to store the binary words in the data and instruction arrays
 * 3. operationInfo - A struct for the operation info, which will be used to store the operations in the operations array
 * 4. errorInfo - A struct for the error info, which will be used to store the errors in the error struct and print them at the end of the program
 * */


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
#define TRUE 1 /*Boolean true value*/
#define FALSE 0 /*Boolean false value*/

/*Data and instruction constants*/
#define IMMEDIATE_ADDRESSING 0 /*Immediate addressing method*/
#define DIRECT_ADDRESSING 1 /*Direct addressing method*/
#define OFFSET_ADDRESSING 2 /*Relative addressing method*/
#define REGISTER_ADDRESSING 3 /*Register addressing method*/
#define MAX_VALUES_PER_DATA_DIRECTIVE 36 /*Maximum number of data values in an array*/
#define INSTRUCTION_FAIL_CODE (-999) /*Specific code for when an instruction line fails (a line with operations and operands)*/

/*Operation and operand constants*/
#define MAX_OPERANDS 4 /*Maximum number of operands in an instruction + 2 spaces for offsets (if relevant)*/
#define MAX_OPERAND_LENGTH (MAX_LABEL_NAME+1+MAX_LABEL_NAME+1) /*Maximum number of characters in an operand, calculated with maximum label name length, + '[' + maximum label name length + ']'*/
#define INVALID_OPERATION_CODE (-1) /*Specific code for invalid operations*/
#define NULL_OPERAND (0) /*Null operand constant*/
#define ZERO_OPERANDS (0) /*No operand constant*/
#define ONE_OPERAND (1) /*One operand constant*/
#define TWO_OPERANDS (2) /*Two operands constant*/

/*Constants for operation codes*/
#define MOV_OPCODE 0 /*MOV operation code*/
#define CMP_OPCODE 1 /*CMP operation code*/
#define ADD_OPCODE 2 /*ADD operation code*/
#define SUB_OPCODE 3 /*SUB operation code*/
#define NOT_OPCODE 4 /*NOT operation code*/
#define CLR_OPCODE 5 /*CLR operation code*/
#define LEA_OPCODE 6 /*LEA operation code*/
#define INC_OPCODE 7 /*INC operation code*/
#define DEC_OPCODE 8 /*DEC operation code*/
#define JMP_OPCODE 9 /*JMP operation code*/
#define BNE_OPCODE 10 /*BNE operation code*/
#define RED_OPCODE 11 /*RED operation code*/
#define PRN_OPCODE 12 /*PRN operation code*/
#define JSR_OPCODE 13 /*JSR operation code*/
#define RTS_OPCODE 14 /*RTS operation code*/
#define HLT_OPCODE 15 /*HLT operation code*/

/*A struct for the symbol list, which will be used for a linked list consisting of the symbols in the program- "symbol table"*/
typedef struct symbolList {
    char name[MAX_LABEL_NAME]; /* Label name*/
    char type[MAX_LABEL_NAME]; /*define, data, extern, or code*/
    short int dataCounter; /* Data counter for .data array elements, to check if we're out of .data array bounds*/
    int isEntry; /* 1 if the label is an entry, 0 otherwise*/
    int externalAddresses[MAX_EXTERNAL_ADDRESSES]; /* Array of external addresses, per external symbol*/
    int value; /* Label value*/
    struct symbolList* next; /* Pointer to the next node*/
} symbolList;

/*A struct for the binary word, which will be used to store the binary words in the data and instruction arrays*/
typedef struct binaryWord{
    int wordBits : WORD_SIZE; /* The bits of the word*/
}memoryWord;

/*A struct for the operation info, which will be used to store the operations in the operations array*/
typedef struct operations{
    char* name; /* Operation name*/
    int code; /* Operation code*/
    int numOfOperands; /* Number of operands*/
}operationInfo;

/*A struct for the error info, which will be used to store the errors in the error struct and print them at the end of the program*/
typedef struct errorInfo{
    int errorFlag; /* 1 if there is an error, 0 otherwise*/
    int errorCounter; /* Number of errors*/
    char currentFileName[MAX_FILE_NAME]; /* Current file name*/
    char currentLineContent[MAX_CHARS_PER_LINE]; /* Current line content*/
    int currentLineNumber; /* Current line number*/
    FILE* file; /* Pointer to the current file*/
}error;


#endif

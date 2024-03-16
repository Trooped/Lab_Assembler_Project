#ifndef DATASTRUCTURESANDCONSTANTS_H
#define DATASTRUCTURESANDCONSTANTS_H


#include <stdio.h>

#define NUMOFOPERATIONS 16 /*Number of operations*/
#define MAXFILENAME 64 /*Maximum file name length*/ /*TODO change the definitions that are more global to one file, maybe inside main.h??*/
#define MAXNAME 32 /*Maximum name length*/ /*TODO order in those fucking defines omg*/
#define WORDSIZE 14 /*Word size*/
#define MAXDATA 2048 /*Maximum number of data in the array*/
#define MAXINSTRUCTIONS 2048 /*Maximum number of instructions in the array*/
#define MAXDATAVALUESINARRAY 36 /*Maximum number of data values in an array*/
#define MAXLABELNAME 31 /*Maximum number of characters in a label name*/
#define MAXOPERANDS 4 /*Maximum number of operands in an instruction + 2 spaces for error handling*/
#define MAXOPERANDLENGTH 32 /*Maximum number of characters in an operand*/
#define MININTEGER (-2048)
#define MAXINTEGER 2047
#define MAXERRORDESCRIPTION 64 /*Maximum number of characters in an error description*/
#define MAXCHARSPERLINE (80+1) /*Maximum number of characters per line*/
#define MAXEXTERNALADDRESSES 64

typedef struct symbolList {
    char name[MAXNAME]; /* Label name*/
    char type[MAXNAME]; /*define, data, extern, or code*/
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
    int counter;
    char fileName[MAXFILENAME];
    char lineText[MAXCHARSPERLINE];
    FILE* file;
}error;



#endif

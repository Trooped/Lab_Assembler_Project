#ifndef DATASTRUCTURESANDCONSTANTS_H
#define DATASTRUCTURESANDCONSTANTS_H


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
#define MAXINTEGER 2048
#define MAXERRORDESCRIPTION 64 /*Maximum number of characters in an error description*/
#define MAXCHARSPERLINE (80+1) /*Maximum number of characters per line*/

typedef struct symbolList {
    char name[MAXNAME]; /* Label name*/
    char type[MAXNAME]; /*define, data, extern, entry or code*/
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
    char fileName[MAXFILENAME];
    char errorDescription[MAXERRORDESCRIPTION]; /*TODO maybe turn this into an array of chars with fixed size?*/
}error;



#endif

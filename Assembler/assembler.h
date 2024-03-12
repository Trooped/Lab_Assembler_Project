#ifndef LAB_PROJECT_ASSEMBLER_H
#define LAB_PROJECT_ASSEMBLER_H

#include "dataStructuresAndConstants.h"
#include "firstPass.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "errno.h"
#include "ctype.h"

void assembler(FILE* source, const char* fileName);

void initializeDataArray(binaryWord* dataArray, int initialValue);

void initializeInstructionArray(binaryWord* instructionArray, int initialValue);


#endif /*LAB_PROJECT_ASSEMBLER_H*/

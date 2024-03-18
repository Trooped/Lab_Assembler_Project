#ifndef LAB_PROJECT_ASSEMBLER_H
#define LAB_PROJECT_ASSEMBLER_H

#include "dataStructuresAndConstants.h"
#include "firstPass.h"
#include "secondPass.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "errno.h"
#include "ctype.h"

void assembler(FILE* source, const char* fileName);

void createEntFile(symbolList** head, FILE* entFile);

void createExtFile(symbolList** head, FILE* extFile);

void createObjectFile(FILE* objectFile, binaryWord* dataArray, binaryWord* instructionArray, int IC, int DC);

char* convertBinaryToBase4Symbols(int binary);

int entryLabelCounter(symbolList** head);

int externLabelCounter(symbolList** head);

void initializeDataArray(binaryWord* dataArray, int initialValue);

void initializeInstructionArray(binaryWord* instructionArray, int initialValue);


#endif /*LAB_PROJECT_ASSEMBLER_H*/

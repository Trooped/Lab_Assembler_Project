#ifndef LAB_PROJECT_ASSEMBLER_H
#define LAB_PROJECT_ASSEMBLER_H

#include "dataStructuresAndConstants.h"
#include "initializationFunctions.h"
#include "error.h"
#include "fileFunctions.h"
#include "listFunctions.h"
#include "PreAsmblr.h"
#include "../tests/testFunctions.h"
#include "error.h"
#include "firstPass.h"
#include "secondPass.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "errno.h"
#include "ctype.h"


int main(int argc, char** argv);

void assembler(FILE* sourceFile, char* fileName);


#endif /*LAB_PROJECT_ASSEMBLER_H*/

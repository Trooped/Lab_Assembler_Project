
#ifndef _TESTFUNCTIONS_H
#define _TESTFUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "../include/assembler.h"

void printFileContentFromPointer(FILE* file);

void deleteFile(const char* filePath);

void printFileContent(const char* filePath);

void testCopyMacrosIntoNewFile();


#endif

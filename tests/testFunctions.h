/**
 * @file testFunctions.h
 * @brief Header file for testFunctions.c
 * @details This file contains the prototypes of the functions used for testing the assembler.
 */
#ifndef _TESTFUNCTIONS_H
#define _TESTFUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include "../include/assembler.h"

void testPrintAndDeleteFile(char* baseFileName);

void deleteFile(const char* filePath, char* suffix);

void printFileContent(const char* filePath, char* suffix);



#endif

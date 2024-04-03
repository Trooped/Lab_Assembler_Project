/**
 * @file assembler.h
 * This file contains the main functions of the assembler.
 * Created by: Omri Peretz, as the final project in the course "Laboratory in C", Open University of Israel 2024A.
 *
 * the functions in this file are:
 * 1. main - The main function of the assembler, which calls the pre-assembly and assembly functions.
 * 2. assembler - The second main function of the assembler. It reads the source file (with macros spread) and creates the memory image.
 */

#ifndef LAB_PROJECT_ASSEMBLER_H
#define LAB_PROJECT_ASSEMBLER_H

#include "dataStructuresAndConstants.h"
#include "initializationFunctions.h"
#include "error.h"
#include "fileFunctions.h"
#include "listFunctions.h"
#include "preAsmblr.h"
/*#include "../tests/testFunctions.h"*/
#include "error.h"
#include "firstPass.h"
#include "secondPass.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "errno.h"
#include "ctype.h"

/**
 * @file main.c
 * The main function of the assembler, which calls the pre-assembly and assembly functions.
 * @param argc The number of arguments given to the program.
 * @param argv The arguments given to the program.
 * @return 0 if the program ran successfully.
 */
int main(int argc, char** argv);

/**
 * The second main function of the assembler program, which is actually the assembly process itself.
 * It reads the source file with macros and creates the memory image.
 * it also calls the firstPass and secondPass functions.
 * @param sourceFile The sourceFile file to be read.
 * @param fileName The name of the file.
 */
void assembler(FILE* sourceFile, char* fileName);


#endif /*LAB_PROJECT_ASSEMBLER_H*/

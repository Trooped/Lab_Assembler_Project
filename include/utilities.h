/**
 * @file utilities.h
 * This file contains the prototypes for the utility functions.
 * These functions are used to help with the parsing and processing of the input files.
 * These functions are used to remove the colon from a label, count the number of entry and external labels in the symbol table, and print the symbol table.
 */

#ifndef UTILITIES_H
#define UTILITIES_H

#include "dataStructuresAndConstants.h"
#include "syntaxAnalyzer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

/**
 * This function removes the colon from a label.
 *
 * @param label The label to remove the colon from.
 * @return char* The label without the colon.
 */
char* removeColon(char* label);



void printSymbolList(const symbolList* head);
void printBits(int value);

#endif

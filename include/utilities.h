#ifndef UTILITIES_H
#define UTILITIES_H

#include "dataStructuresAndConstants.h"
#include "syntaxAnalyzer.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

int entryLabelCounter(symbolList** head);

int externLabelCounter(symbolList** head);


char* removeColon(char* label);

void printSymbolList(const symbolList* head);
void printBits(int value);

#endif

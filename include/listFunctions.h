
#ifndef _LISTFUNCTIONS_H
#define _LISTFUNCTIONS_H

#include "dataStructuresAndConstants.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listFunctions.h"
#include "syntaxAnalyzer.h"

int findSymbolValue(symbolList **head, const char* name,char* type, int* value);

void markLabelAsEntry(symbolList** head, char* line, error** errorInfo);

int searchSymbolList(symbolList** head, char* name, char* type);

void addLabel(symbolList** head, char* name, char* type, int value, error** errorInfo);

void addExternAddress(symbolList** head, char* name, int address);


void deleteSymbolList(symbolList** head);


void incrementDataSymbolValues(symbolList** head, int byValue);

int isSymbolExtern(symbolList** head, char* symbolName);


/**
 * This function returns a pointer to a symbol in the symbol table.
 * @param head The symbol list.
 * @param symbolName The symbol name.
 * @return A pointer to the symbol in the symbol table.
 */
symbolList* getPointerToSymbol(symbolList** head, char* symbolName);

/**
 * This function counts the number of entry labels in the symbol table.
 * @param head The head of the symbol table.
 * @return int The number of entry labels.
 */
int entryLabelCounter(symbolList** head);

/**
 * This function counts the number of external labels in the symbol table.
 * @param head The head of the symbol table.
 * @return int The number of external labels.
 */
int externLabelCounter(symbolList** head);


#endif

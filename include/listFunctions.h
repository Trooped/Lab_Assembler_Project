/**
 * @file listFunctions.h
 * This file contains the functions that handle the symbol table, which is a linked list.
 *
 * The functions in this file are:
 * 1. searchSymbolList - This function searches the symbol table for a specific label.
 * 2. addLabel - This function adds a label to the symbol table.
 * 3. deleteSymbolList - This function deletes the symbol table and frees the memory.
 * 4. addExternAddress - This function adds an external address to a symbol in the symbol table.
 * 5. incrementDataSymbolValues - This function increments the values of the data symbols in the symbol table by a specific value.
 * 6. findSymbolValue - This function searches the symbol table for a specific label and returns its value.
 * 7. markLabelAsEntry - This function marks a label as an entry in the symbol table.
 * 8. isSymbolExtern - This function checks if a symbol is external.
 */

#ifndef LISTFUNCTIONS_H
#define LISTFUNCTIONS_H

#include "dataStructuresAndConstants.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listFunctions.h"
#include "syntaxAnalyzer.h"

/**
 * This function searches the symbol table for a specific label.
 *
 * @param head The head of the symbol table.
 * @param name The name of the label to search for.
 * @param type The type of the label to search for.
 * @return int 1 if the label wasn't found, 0 if it was found in the table.
 */
int searchSymbolList(symbolList** head, char* name, char* type);

/**
 * This function adds a label to the symbol table.
 *
 * @param head The head of the symbol table.
 * @param name The name of the label to add.
 * @param type The type of the label to add.
 * @param value The value of the label to add.
 * @param errorInfo A pointer to the errorInfo struct.
 */
void addLabel(symbolList** head, char* name, char* type, int value, error** errorInfo);

/**
 * This function deletes the symbol table and frees the memory.
 *
 * @param head The head of the symbol table.
 */
void deleteSymbolList(symbolList** head);

/**
 * This function adds an external address usage to an extern symbol in the symbol table.
 *
 * @param head The head of the symbol table.
 * @param name The name of the label to add the external address to.
 * @param address The address to add to the label.
 */
void addExternAddress(symbolList** head, char* name, int address);

/**
 * This function increments the values of the data symbols in the symbol table by a specific value.
 * This is used to update the data symbols after the first pass.
 *
 * @param head The head of the symbol table.
 * @param byValue The value to increment the data symbols by.
 */
void incrementDataSymbolValues(symbolList** head, int byValue);

/**
 * This function searches the symbol table for a specific label, by name and type- and returns its value.
 *
 * @param head The head of the symbol table.
 * @param name The name of the label to search for.
 * @param type The type of the label to search for.
 * @param value A pointer to an integer to store the value of the label.
 * @return int 1 if the label was found, 0 if it wasn't.
 */
int findSymbolValue(symbolList **head, const char* name,char* type, int* value);

/**
 * This function marks a label as an entry in the symbol table.
 *
 * @param head The head of the symbol table.
 * @param line The line to parse.
 * @param errorInfo A pointer to the errorInfo struct.
 */
void markLabelAsEntry(symbolList** head, char* line, error** errorInfo);

/**
 * This function checks if a symbol is external.
 * @param head The symbol list.
 * @param symbolName The symbol name.
 * @return 1 if the symbol is external, 0 otherwise.
 */
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

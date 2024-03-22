/**
 * @file listFunctions.c
 *
 * This file contains the functions for the symbol table (which is a linked list).
 * The symbol table is a linked list of symbolList structs.
 * Each symbolList struct contains the name, type, value, and a pointer to the next node
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

#include "include/listFunctions.h"



/**
 * This function searches the symbol table for a specific label.
 *
 * @param head The head of the symbol table.
 * @param name The name of the label to search for.
 * @param type The type of the label to search for.
 * @return int 1 if the label wasn't found, 0 if it was found in the table.
 */
int searchSymbolList(symbolList** head, char* name, char* type) {
    char tempLabel[MAXLABELNAME] = {0};
    symbolList* current = *head;
    trimWhitespace(name);
    if (name[strlen(name) - 1] == ':') {
        strncpy(tempLabel, name, strlen(name) - 1); /* Remove the colon*/
        tempLabel[strlen(name) - 1] = '\0'; /* Ensure null termination*/
    }
    else {
        strncpy(tempLabel, name, strlen(name)); /* Keep it as it is*/
        tempLabel[strlen(name)] = '\0'; /* Ensure null termination*/
    }

    /* Search for the name in the list*/
    while (current != NULL) {
        if ((strcmp(type, "entry")==0) && (strcmp(current->name, tempLabel)==0) && (current->isEntry==1)){
            /* Entry label was already found in the list)*/
            return 0;
        }
        else if (strcmp(type, "general") != 0) { /*Searching for a specific name AND type*/
            if (strcmp(current->name, tempLabel) == 0 && strcmp(current->type, type) == 0) {
                /* Label found in the list*/
                return 0;
            }
        }
        else if (strcmp(current->name, tempLabel) == 0) { /*Searching for a specific name only*/
            /* Label found in the list*/
            return 0;
        }
        current = current->next;
    }

    /* Name not found in the list, send 1 to signify creating a new node*/
    return 1;
}


/**
 * This function adds a label to the symbol table.
 *
 * @param head The head of the symbol table.
 * @param name The name of the label to add.
 * @param type The type of the label to add.
 * @param value The value of the label to add.
 * @param errorInfo A pointer to the errorInfo struct.
 */
void addLabel(symbolList** head, char* name, char* type, int value, error** errorInfo) {
    int i;
    symbolList* newNode = NULL;
    if (name == NULL || name[0] == '\0') {
        printError(errorInfo, "Empty label isn't allowed");
        return; /* Early return to avoid processing further*/
    }

    if (searchSymbolList(head, name, "external") == 0) {
        printf("WARNING: Double declaration- Label '%s' was already defined as external\n", name);
        return; /* Early return to avoid processing further*/
    }
    else if (searchSymbolList(head, name, "general") == 0) {
        printError(errorInfo, "Label already exists in the symbol table");
        return; /* Early return to avoid processing further*/
    }
    trimWhitespace(name);

    newNode = (symbolList*)malloc(sizeof(symbolList));
    if (newNode == NULL) {
        printError(errorInfo, "Failed to allocate memory for new Symbol Table node");
        closeFileAndExit(errorInfo, head);
    }

    /* Initialize the new node*/
    strncpy(newNode->name, name, MAXLABELNAME - 1);
    newNode->name[MAXLABELNAME - 1] = '\0'; /* Ensure null termination*/
    strncpy(newNode->type, type, MAXLABELNAME - 1);
    newNode->type[MAXLABELNAME - 1] = '\0'; /* Ensure null termination*/
    newNode->value = value;
    newNode->next = NULL;
    newNode->isEntry = 0;
    for (i = 0; i < MAXEXTERNALADDRESSES; i++) {
        newNode->externalAddresses[i] = -1;
    }

    if (*head == NULL) {
        *head = newNode; /* Set new node as the head if list is empty*/
    } else {
        /* Append the new node to the end of the list*/
        symbolList* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}


/**
 * This function deletes the symbol table and frees the memory.
 *
 * @param head The head of the symbol table.
 */
void deleteSymbolList(symbolList** head) {
    symbolList* current = *head;
    symbolList* nextNode = NULL;
    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
    *head = NULL; /* Ensure the caller's head pointer is set to NULL*/
}


void addExternAddress(symbolList** head, char* name, int address){
    int i=0;
    symbolList* current = *head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            while (current->externalAddresses[i] != -1) {
                i++;
            }
            current->externalAddresses[i] = address;
            return;
        }
        current = current->next;
    }
}


/**
 * This function increments the values of the data symbols in the symbol table by a specific value.
 * This is used to update the data symbols after the first pass.
 *
 * @param head The head of the symbol table.
 * @param byValue The value to increment the data symbols by.
 */
void incrementDataSymbolValues(symbolList** head, int byValue) {
    symbolList* current = (*head);
    while (current != NULL) {
        if (strcmp(current->type, "data") == 0) {
            current->value += byValue;
        }
        current = current->next;
    }
}

/**
 * This function searches the symbol table for a specific label and returns its value.
 *
 * @param head The head of the symbol table.
 * @param name The name of the label to search for.
 * @param type The type of the label to search for.
 * @param value A pointer to an integer to store the value of the label.
 * @return int 1 if the label was found, 0 if it wasn't.
 */
int findSymbolValue(symbolList **head, const char* name,char* type, int* value) {
    symbolList* current = *head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (strcmp(current->type, type) == 0) {
                *value = current->value;
                return 1;
            }
        }
        current = current->next;
    }
    return 0;
}

/**
 * This function marks a label as an entry in the symbol table.
 *
 * @param head The head of the symbol table.
 * @param line The line to parse.
 * @param errorInfo A pointer to the errorInfo struct.
 */
void markLabelAsEntry(symbolList** head, char* line, error** errorInfo) {
    char* entryLabelName;
    symbolList* current = *head;
    entryLabelName = strtok(line, " \n\r\t"); /* Get the next word.*/

    if (searchSymbolList(head, entryLabelName, "entry") == 0){
        printf("WARNING: Double declaration- label '%s' was already defined as entry\n", entryLabelName);
        return;
    }

    /* Search for the name in the list*/
    while (current != NULL) {
        if (strcmp(current->name, entryLabelName) == 0){
            if (strcmp(current->type, "code") == 0 || strcmp(current->type, "data") == 0){
                current->isEntry = 1;
                return;
            }
            else{
                printError(errorInfo, ".entry label is not of type code or data");
                return;
            }
        }
        current = current->next;
    }
    printError(errorInfo, ".entry Label not found in the symbol table");
}

/**
 * This function checks if a symbol is external.
 * @param head The symbol list.
 * @param symbolName The symbol name.
 * @return 1 if the symbol is external, 0 otherwise.
 */
int isSymbolExtern(symbolList** head, char* symbolName){
    if (searchSymbolList(head, symbolName, "external") == 0){
        return 1;
    }
    return 0;
}

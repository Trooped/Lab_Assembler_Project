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
    char tempLabel[MAX_LABEL_NAME] = {0}; /* Temporary label to store the name without the colon*/
    symbolList* current = *head; /* Set the current node to the head of the list*/
    trimWhitespace(name); /* Remove any leading or trailing whitespace*/
    if (name[strlen(name) - 1] == ':') { /* Check if the label has a colon at the end*/
        strncpy(tempLabel, name, strlen(name) - 1); /* Remove the colon*/
        tempLabel[strlen(name) - 1] = '\0'; /* Ensure null termination*/
    }
    else {
        strncpy(tempLabel, name, strlen(name)); /* Keep it as it is*/
        tempLabel[strlen(name)] = '\0'; /* Ensure null termination*/
    }

    /* Search for the name in the list*/
    while (current != NULL) {
        if ((strcmp(type, "entry")==0) && (strcmp(current->name, tempLabel)==0) && (current->isEntry==1)){ /*Searching for an entry label*/
            /* Entry label was already found in the list)*/
            return FALSE;
        }
        else if (strcmp(type, "general") != 0) { /*Searching for a specific name AND type*/
            if (strcmp(current->name, tempLabel) == 0 && strcmp(current->type, type) == 0) {
                /* Label found in the list*/
                return FALSE;
            }
        }
        else if (strcmp(current->name, tempLabel) == 0) { /*Searching for a specific name only*/
            /* Label found in the list*/
            return FALSE;
        }
        current = current->next;
    }
    /* Name not found in the list, send 1 to signify creating a new node*/
    return TRUE;
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
    int i; /* Loop index*/
    symbolList* newNode = NULL; /* The new node to add to the list*/
    if (name == NULL || name[0] == '\0') { /* Check if the label is empty*/
        printError(errorInfo, "Empty label isn't allowed");
        return; /* Early return to avoid processing further*/
    }

    if (searchSymbolList(head, name, "external") == 0) { /* Check if the label was already defined as external*/
        printf("WARNING: Double declaration- Label '%s' was already defined as external\n", name);
        return; /* Early return to avoid processing further*/
    }
    else if (searchSymbolList(head, name, "general") == 0) { /* Check if the label was already defined in the symbol table*/
        printError(errorInfo, "Label already exists in the symbol table");
        return; /* Early return to avoid processing further*/
    }
    trimWhitespace(name); /* Remove any leading or trailing whitespace*/

    newNode = (symbolList*)malloc(sizeof(symbolList)); /* Allocate memory for the new node*/
    if (newNode == NULL) { /* If the memory allocation failed, print an error message and exit*/
        printError(errorInfo, "Failed to allocate memory for new Symbol Table node");
        closeFileAndExit(errorInfo, head);
    }

    /* Initialize the new node*/
    strncpy(newNode->name, name, MAX_LABEL_NAME - 1); /* Copy the name to the new node*/
    newNode->name[MAX_LABEL_NAME - 1] = '\0'; /* Ensure null termination*/
    strncpy(newNode->type, type, MAX_LABEL_NAME - 1); /* Copy the type to the new node*/
    newNode->type[MAX_LABEL_NAME - 1] = '\0'; /* Ensure null termination*/
    newNode->value = value; /* Set the value of the new node*/
    newNode->next = NULL; /* Set the next pointer to NULL*/
    newNode->isEntry = FALSE; /* Set the isEntry flag to 0*/
    newNode->dataCounter = -1; /* Set the data counter to -1 (because we want it to represent the amount of integers in a .data label)*/
    for (i = 0; i < MAX_EXTERNAL_ADDRESSES; i++) { /* Initialize the external addresses array to -1*/
        newNode->externalAddresses[i] = -1;
    }

    if (*head == NULL) { /* Check if the list is empty*/
        *head = newNode; /* Set new node as the head if list is empty*/
    } else { /* Append the new node to the end of the list*/
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
    symbolList* current = *head; /* Set the current node to the head of the list*/
    symbolList* nextNode = NULL; /* The next node in the list*/
    while (current != NULL) {
        nextNode = current->next; /* Save the next node*/
        free(current); /* Free the current node*/
        current = nextNode; /* Move to the next node*/
    }
    *head = NULL; /* Ensure the caller's head pointer is set to NULL*/
}

/**
 * This function adds an external address usage to an extern symbol in the symbol table.
 *
 * @param head The head of the symbol table.
 * @param name The name of the label to add the external address to.
 * @param address The address to add to the label.
 */
void addExternAddress(symbolList** head, char* name, int address){
    int i = 0; /* Loop index*/
    symbolList* current = *head; /* Set the current node to the head of the list*/
    while (current != NULL) { /* Loop through the list*/
        if (strcmp(current->name, name) == 0) {
            while (current->externalAddresses[i] != -1) { /* Find the first empty slot in the external addresses array*/
                i++;
            }
            current->externalAddresses[i] = address; /* Add the address to the array*/
            return; /* Early return to avoid processing further*/
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
            current->value += byValue; /* Increment the value by the specified value*/
        }
        current = current->next;
    }
}

/**
 * This function searches the symbol table for a specific label, by name and type- and returns its value.
 *
 * @param head The head of the symbol table.
 * @param name The name of the label to search for.
 * @param type The type of the label to search for.
 * @param value A pointer to an integer to store the value of the label.
 * @return int 1 if the label was found, 0 if it wasn't.
 */
int findSymbolValue(symbolList **head, const char* name,char* type, int* value) {
    symbolList* current = *head; /* Set the current node to the head of the list*/
    while (current != NULL) { /* Loop through the list*/
        if (strcmp(current->name, name) == 0) { /* Check if the name matches*/
            if (strcmp(current->type, type) == 0 || strcmp(type, "general")==0) { /* Check if the type matches, or if the type is general*/
                *value = current->value; /* Set the value to the value of the current node*/
                return TRUE; /* Return 1 to signify the label was found*/
            }
        }
        current = current->next;
    }
    return FALSE;
}

/**
 * This function marks a label as an entry in the symbol table.
 *
 * @param head The head of the symbol table.
 * @param line The line to parse.
 * @param errorInfo A pointer to the errorInfo struct.
 */
void markLabelAsEntry(symbolList** head, char* line, error** errorInfo) {
    char* entryLabelName; /* The name of the entry label*/
    symbolList* current = *head; /* Set the current node to the head of the list*/
    entryLabelName = strtok(line, " \n\r\t"); /* Get the next word.*/

    if (searchSymbolList(head, entryLabelName, "entry") == 0){ /* Check if the same label was already defined as entry*/
        printf("WARNING: Double declaration- label '%s' was already defined as entry\n", entryLabelName);
        return;
    }

    /* Search for the name in the list*/
    while (current != NULL) {
        if (strcmp(current->name, entryLabelName) == 0){ /* Check if the name matches*/
            if (strcmp(current->type, "code") == 0 || strcmp(current->type, "data") == 0){ /* Check if the type is code or data*/
                current->isEntry = TRUE; /* Mark the label as an entry*/
                return; /* Early return to avoid processing further*/
            }
            else{ /* If the type is not code or data, print an error message and exit*/
                printError(errorInfo, ".entry label is not of type code or data");
                return;
            }
        }
        current = current->next;
    }
    printError(errorInfo, ".entry Label not found in the symbol table"); /* If the label wasn't found, print an error message*/
}

/**
 * This function checks if a symbol is external.
 * @param head The symbol list.
 * @param symbolName The symbol name.
 * @return 1 if the symbol is external, 0 otherwise.
 */
int isSymbolExtern(symbolList** head, char* symbolName){
    if (searchSymbolList(head, symbolName, "external") == 0){
        return TRUE;
    }
    return FALSE;
}


/**
 * This function returns a pointer to a symbol in the symbol table.
 * @param head The symbol list.
 * @param symbolName The symbol name.
 * @return A pointer to the symbol in the symbol table.
 */
symbolList* getPointerToSymbol(symbolList** head, char* symbolName){
    symbolList* current = *head; /* Set the current node to the head of the list*/
    while (current != NULL) { /* Loop through the list*/
        if (strcmp(current->name, symbolName) == 0) { /* Check if the name matches*/
            return current;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * This function counts the number of entry labels in the symbol table.
 * @param head The head of the symbol table.
 * @return int The number of entry labels.
 */
int entryLabelCounter(symbolList** head) {
    symbolList* current = *head; /* Start at the head of the list*/
    int counter = 0;
    while (current != NULL) {  /* Loop through the list*/
        if (current->isEntry == TRUE) {
            counter++; /* Increment the counter*/
        }
        current = current->next;
    }
    return counter; /* Return the counter*/
}

/**
 * This function counts the number of external labels in the symbol table.
 * It only counts the extern labels that have external addresses that are used.
 * @param head The head of the symbol table.
 * @return int The number of external labels.
 */
int externLabelCounter(symbolList** head) {
    int i = 0;  /* Index for the external addresses array*/
    symbolList* current = *head; /* Start at the head of the list*/
    int counter = 0;
    while (current != NULL) { /* Loop through the list*/
        if (strcmp(current->type, "external") == 0) { /* If the symbol is external*/
            while (current->externalAddresses[i] != -1 && i <= MAX_EXTERNAL_ADDRESSES) {
                i++; /* Increment the index*/
            }
            if(i > 0 && current->externalAddresses[i-1] != -1){ /* If there are external addresses for this extern*/
                counter++; /* Increment the counter*/
            }
        }
        current = current->next;
    }
    return counter; /* Return the counter*/
}

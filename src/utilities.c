/**
 * @file utilities.c
 *
 * This file contains general utility functions that are used in the assembler.
 * These functions are used to remove the colon from a label, count the number of entry and external labels in the symbol table, and print the symbol table.
 *
 * The functions in this file are:
 * 1. removeColon - This function removes the colon from a label.
 * 2. entryLabelCounter - This function counts the number of entry labels in the symbol table.
 * 3. externLabelCounter - This function counts the number of external labels in the symbol table.
 */

#include "include/utilities.h"


/**
 * This function removes the colon from a label.
 * /*TODO remove it?? it isn't used!!!!!!!!!!!
 * @param label The label to remove the colon from.
 * @return char* The label without the colon.
 */
char* removeColon(char* label) {
    char* newLabel = label;
    if (label[strlen(label) - 1] == ':') {
        label[strlen(label) - 1] = '\0'; /* Remove the colon*/
    }
    return newLabel;
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



/*TODO DELETE THIS FUNCTION after testing!!!!!!!!! AND ITS .H VARIANT*/
void printSymbolList(const symbolList* head) {
    int i;
    printf("Symbol Table Contents:\n");
    while (head != NULL) {
        printf("Name: %s, Type: %s, Value: %d\n", head->name, head->type, head->value);
        if (head->isEntry) {
            printf("This label is an entry\n");
        }
        if (head->externalAddresses[0] != -1) {
            printf("This label is external\n");
            for (i = 0; i < MAX_EXTERNAL_ADDRESSES; i++) {
                if (head->externalAddresses[i] != -1) {
                    printf("External address: %d\n", head->externalAddresses[i]);
                }
            }
        }
        head = head->next; /* Move to the next node*/
    }
}
/*TODO delete!!!!!!!!!!!!*/
void printBits(int value) {
    int i;
    for (i = WORD_SIZE - 1; i >= 0; i--) {
        int bit = (value >> i) & 1;
        printf("%d", bit);
    }
}


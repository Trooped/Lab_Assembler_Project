#include "include/utilities.h"


/**
 * This function removes the colon from a label.
 *
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


int entryLabelCounter(symbolList** head) {
    symbolList* current = *head;
    int counter = 0;
    while (current != NULL) {
        if (current->isEntry == 1) {
            counter++;
        }
        current = current->next;
    }
    return counter;
}

int externLabelCounter(symbolList** head) {
    symbolList* current = *head;
    int counter = 0;
    while (current != NULL) {
        if (strcmp(current->type, "external") == 0) {
            counter++;
        }
        current = current->next;
    }
    return counter;
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
            for (i = 0; i < MAXEXTERNALADDRESSES; i++) {
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
    for (i = WORDSIZE - 1; i >= 0; i--) {
        int bit = (value >> i) & 1;
        printf("%d", bit);
    }
}


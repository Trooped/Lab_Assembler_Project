#include "utilities.h"


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


int searchSymbolList(symbolList** head, char* name, char* type) {
    symbolList* current = *head;

    /* Search for the name in the list*/
    while (current != NULL) {
        if (strcmp(type, "general")!=0) {
            if (strcmp(current->name, name) == 0 && strcmp(current->type, type) == 0) {
                /* Label found in the list, return 0 = error TODO maybe add it as a constant?*/
                return 0;
            }
        }
        else if (strcmp(current->name, name) == 0) {
            /* Label found in the list, return 0 = error TODO maybe add it as a constant?*/
            return 0;
        }
        current = current->next;
    }

    /* Name not found in the list, send 1 to signify creating a new node*/
    return 1;
}



/*TODO maybe it can be a void function?*/
/* Function to add a new node at the end of the list */
symbolList* addLabel(symbolList* head, char* name, char* type, int value) {
    symbolList* newNode = (symbolList*)malloc(sizeof(symbolList));
    if(newNode == NULL) {
        /* If malloc fails, print an error and exit TODO DONT EXIT!!! FIND OUT WHAT TO DO DIFFERENTLY*/
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    /*Empty Label isn't allowed TODO add an error code*/
    if (strcmp(name, NULL) == 0){
        return;
    }

    /* Copy the name and value into the new node */
    strcpy(newNode->name, name);
    strcpy(newNode->type, type);
    newNode->value = value;
    newNode->next = NULL;

    if (head == NULL) {
        /* If the list is empty, the new node becomes the head of the list */
        head = newNode;
    } else {
        /* If the list is not empty, find the last node and link the new node to it */
        symbolList* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    /* Return the head of the list */
    return head;
}

/* Function to delete a node */
void deleteSymbolListNode(symbolList* node) {
    /* Free the memory allocated for the node */
    free(node);
}

/*TODO call this one in the end of the main function??*/
/* Function to delete the entire list */
void deleteSymbolList(symbolList* head) {
    /* Loop through the list and delete each node */
    symbolList* current = head;
    symbolList* nextNode;
    while (current != NULL) {
        nextNode = current->next;
        deleteSymbolListNode(current);
        current = nextNode;
    }
}

void insertInstructionIntoArray(word* instructionArray, int IC, int opcode, int firstOperand, int secondOperand) {
    word newWord;
    /*TODO remove all of that
    Start with the first 4 bits as 0000, which we can ignore as the bits are 0 by default.
    Then shift the opcode to its correct position (6 bits to the left)
    Then shift the firstOperand 4 bits to the left
    Then shift the secondOperand 2 bits to the left
    The last 2 bits are 00 and don't need to be explicitly set
    */
    newWord.wordBits = (opcode << 6) | (firstOperand << 4) | (secondOperand << 2);
    instructionArray[IC] = newWord;
}

void addValueToDataArray(word **dataArray, int DC, int value) {
    word newWord;
    newWord.wordBits = value;
    dataArray[DC] = &newWord;
}



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


/* Function to add a new node at the end of the list */
void addLabel(symbolList** head, char* name, char* type, int value, error** errorInfo) {
    symbolList* newNode = NULL;
    if (name == NULL || name[0] == '\0') {
        printError(errorInfo, "Empty label isn't allowed");
        return; /* Early return to avoid processing further*/
    }

    newNode = (symbolList*)malloc(sizeof(symbolList));
    if (newNode == NULL) {
        printError(errorInfo, "Out of memory");
        return; /*TODO Consider how to handle memory errors in your application context*/
    }

    /* Initialize the new node*/
    strncpy(newNode->name, name, MAXNAME - 1);
    newNode->name[MAXNAME - 1] = '\0'; /* Ensure null termination*/
    strncpy(newNode->type, type, MAXNAME - 1);
    newNode->type[MAXNAME - 1] = '\0'; /* Ensure null termination*/
    newNode->value = value;
    newNode->next = NULL;

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


/*TODO call this one in the end of the main function??*/
/* Function to delete the entire list */
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

void insertInstructionIntoArray(binaryWord* instructionArray, int IC, int opcode, int firstOperand, int secondOperand) {
    binaryWord newWord;
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

void addValueToDataArray(binaryWord* dataArray, int DC, int value) {
    binaryWord newWord;
    newWord.wordBits = value;
    dataArray[DC] = newWord;
}

void printError(error** errorInfo, char* errorDescription){
    (*errorInfo)->errorFlag = 1;
    snprintf((*errorInfo)->errorDescription, sizeof((*errorInfo)->errorDescription), "%s", errorDescription);
    printf("Error in file %s: %s\n", (*errorInfo)->fileName, (*errorInfo)->errorDescription);
}

void incrementDataSymbolValues(symbolList** head, int byValue) {
    symbolList* current = head;
    while (current != NULL) {
        if (strcmp(current->type, "data") == 0) {
            current->value += byValue;
        }
        current = current->next;
    }
}

void initializeOperandsArray(char operands[MAXOPERANDS][MAXOPERANDLENGTH]) {
    int i, j;
    for (i = 0; i < MAXOPERANDS; i++) {
        for (j = 0; j < MAXOPERANDLENGTH; j++) {
            operands[i][j] = '\0';
        }
    }
}

/*TODO DELETE THIS FUNCTION after testing!!!!!!!!! AND ITS .H VARIANT*/
void printSymbolList(const symbolList* head) {
    printf("Symbol Table Contents:\n");
    while (head != NULL) {
        printf("Name: %s, Type: %s, Value: %d\n", head->name, head->type, head->value);
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


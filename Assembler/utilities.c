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

void initializeOperationsArray(operationInfo* operationsArray) {
    /* Initialize operation 0 */
    operationsArray[0].name = "mov";
    operationsArray[0].code = 0;
    operationsArray[0].numOfOperands = 2;

    /* Initialize operation 1 */
    operationsArray[1].name = "cmp";
    operationsArray[1].code = 1;
    operationsArray[1].numOfOperands = 2;

    /* Initialize operation 2 */
    operationsArray[2].name = "add";
    operationsArray[2].code = 2;
    operationsArray[2].numOfOperands = 2;

    /* Initialize operation 3 */
    operationsArray[3].name = "sub";
    operationsArray[3].code = 3;
    operationsArray[3].numOfOperands = 2;

    /* Initialize operation 4 */
    operationsArray[4].name = "lea";
    operationsArray[4].code = 4;
    operationsArray[4].numOfOperands = 2;

    /* Initialize operation 5 */
    operationsArray[5].name = "not";
    operationsArray[5].code = 5;
    operationsArray[5].numOfOperands = 1;

    /* Initialize operation 6 */
    operationsArray[6].name = "clr";
    operationsArray[6].code = 6;
    operationsArray[6].numOfOperands = 1;

    /* Initialize operation 7 */
    operationsArray[7].name = "inc";
    operationsArray[7].code = 7;
    operationsArray[7].numOfOperands = 1;

    /* Initialize operation 8 */
    operationsArray[8].name = "dec";
    operationsArray[8].code = 8;
    operationsArray[8].numOfOperands = 1;

    /* Initialize operation 9 */
    operationsArray[9].name = "jmp";
    operationsArray[9].code = 9;
    operationsArray[9].numOfOperands = 1;

    /* Initialize operation 10 */
    operationsArray[10].name = "bne";
    operationsArray[10].code = 10;
    operationsArray[10].numOfOperands = 1;

    /* Initialize operation 11 */
    operationsArray[11].name = "red";
    operationsArray[11].code = 11;
    operationsArray[11].numOfOperands = 1;

    /* Initialize operation 12 */
    operationsArray[12].name = "prn";
    operationsArray[12].code = 12;
    operationsArray[12].numOfOperands = 1;

    /* Initialize operation 13 */
    operationsArray[13].name = "jsr";
    operationsArray[13].code = 13;
    operationsArray[13].numOfOperands = 1;

    /* Initialize operation 14 */
    operationsArray[14].name = "rts";
    operationsArray[14].code = 14;
    operationsArray[14].numOfOperands = 0;

    /* Initialize operation 15 */
    operationsArray[15].name = "hlt";
    operationsArray[15].code = 15;
    operationsArray[15].numOfOperands = 0;
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

void initializeErrorInfo(error** errorInfo, char* fileName) {
    *errorInfo = malloc(sizeof(error));
    if (*errorInfo == NULL) {
        /* Handle memory allocation failure TODO how to handle memory allocation fail?*/
        fprintf(stderr, "Failed to allocate memory for errorInfo\n");
        return;
    }
    /* Initialize errorInfo*/
    (*errorInfo)->errorFlag = 0;
    strcpy((*errorInfo)->fileName, fileName);
    (*errorInfo)->errorDescription[0] = '\0';
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


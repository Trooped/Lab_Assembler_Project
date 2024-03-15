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

/*TODO check if it works correctly!!*/
void markLabelAsEntry(symbolList** head, char* line, error** errorInfo) {
    char* entryLabelName;
    symbolList* current = *head;

    entryLabelName = strtok(line, " \n\r\t"); /* Get the next word.*/
    entryLabelName = strtok(NULL, " \n\r\t"); /* Get the next word.*/
    /* Search for the name in the list*/
    while (current != NULL) {
        if (strcmp(current->name, entryLabelName) == 0) {
            current->isEntry = 1;
        }
        current = current->next;
    }
    printError(errorInfo, ".entry Label not found in the symbol table");
}


int searchSymbolList(symbolList** head, char* name, char* type) {
    symbolList* current = *head;

    /* Search for the name in the list*/
    while (current != NULL) {
        if (strcmp(type, "general")!=0) { /*Searching for a specific name AND type*/
            if (strcmp(current->name, name) == 0 && strcmp(current->type, type) == 0) {
                /* Label found in the list*/
                return 0;
            }
        }
        else if (strcmp(current->name, name) == 0) {
            /* Label found in the list*/
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

void convertOperandToBinaryAndInsertIntoArray(binaryWord* instructionArray, int IC, char* operand, symbolList** head, error** errorInfo) {
    int val;
    binaryWord newWord;

    if (operand[0] == '#') {
        if (!isValidInteger(operand + 1)) {
            findSymbolValue(head, operand + 1, "define", &val);
        }
        else {
            val = atoi(operand + 1);
        }
        newWord.wordBits = val & 0xFFF;
    }
    else if(searchSymbolList(head, operand, "data")==0){
        findSymbolValue(head, operand, "data", &val);
        if (isSymbolExtern){
            /*TODO add 01 after the val*/
        }
        else{
            /*TODO add 10 after the val*/
        }

        newWord.wordBits = val & 0xFFF;
    }


    else if (operand[0] == 'r') { /*TODO handle the case where it's both of the registers!!*/
        if(source){ /*IF IT'S THE SOURCE REGISTER*/

        }
        val = atoi(operand + 1);
        addValueToDataArray(instructionArray, *IC, val);
        (*IC)++;
    }
    else if (operand[0] == '[') {
        char tempVal[MAXOPERANDLENGTH];
        int j = 0;
        int k = 1;
        while (operand[k] != ']' && operand[k] != '\0') {
            tempVal[j] = operand[k];
            j++;
            k++;
        }
        tempVal[j] = '\0'; /*Null terminating the temp String*/
        if (!isValidInteger(tempVal)) {
            int symbolValue;
            if (!findSymbolValue(head, tempVal, "define", &symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                printError(errorInfo, "Unvalid Integer or undefined symbol for offset operand");
                return;
            }
            val = symbolValue; /* Use the value from the symbol list*/
        }
        addValueToDataArray(instructionArray, *IC, val);
        (*IC)++;
    }
    else {
        if (!findSymbolValue(head, operand, "define", &val)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
            printError(errorInfo, "Undefined symbol for operand");
            return;
        }
        addValueToDataArray(instructionArray, *IC, val);
        (*IC)++;
    }


    newWord.wordBits = val;
    instructionArray[IC] = newWord;
}

void insertOperandsIntoInstructionArray(binaryWord* instructionArray, int numOfLines, int *IC, char operands[MAXOPERANDS][MAXOPERANDLENGTH], symbolList** head, error** errorInfo){
    int i;
    char currentWord[MAXOPERANDLENGTH];

    char* firstOperand;
    char* labelOrDefineFirst;
    char* secondOperand;
    char* labelOrDefineSecond;

    parseOperandsSecondPass(operands[0], &firstOperand, &labelOrDefineFirst);
    parseOperandsSecondPass(operands[1], &secondOperand, &labelOrDefineSecond);

    insertInstruction(instructionArray, IC+1, firstOperand, head, errorInfo);
    if (labelOrDefineFirst != NULL) {
        insertInstruction(instructionArray, IC+2, labelOrDefineFirst, head, errorInfo);
        insertInstruction(instructionArray, IC+3, secondOperand, head, errorInfo);
        if (labelOrDefineSecond != NULL) {
            insertInstruction(instructionArray, IC+4, labelOrDefineSecond, head, errorInfo);
        }
    }
    else{
        insertInstruction(instructionArray, IC+2, secondOperand, head, errorInfo);
        if (labelOrDefineSecond != NULL) {
            insertInstruction(instructionArray, IC+3, labelOrDefineSecond, head, errorInfo);
        }
    }






    for (i = 1; i <= numOfLines; i++){

        strncpy(currentWord, operands[i-1], MAXOPERANDLENGTH);
        currentWord[MAXOPERANDLENGTH - 1] = '\0'; /* Ensure null termination TODO IS THIS NEEDED?*/

        if (currentWord[0] == '#') {
            int val;
            if (!isValidInteger(currentWord + 1)) {
                findSymbolValue(head, currentWord + 1, "define", &val);
            }
            else {
                val = atoi(currentWord + 1);
            }
            addToArray(instructionArray, (*IC)+i, val);
        }
        else if (currentWord[0] == 'r') {
            if(isValidRegister(firstOperand) && isValidRegister(secondOperand)){
                /*TODO add it as one word and skip the loop*/
            }







            int val;
            if (!isValidRegister(currentWord)) {
                printError(errorInfo, "Invalid Register");
                return;
            }
            val = atoi(currentWord + 1);
            addValueToDataArray(instructionArray, *IC, val);
            (*IC)++;
        }
        else if (currentWord[0] == '[') {
            int val;
            char tempVal[MAXOPERANDLENGTH];
            int j = 0;
            int k = 1;
            while (currentWord[k] != ']' && currentWord[k] != '\0') {
                tempVal[j] = currentWord[k];
                j++;
                k++;
            }
            tempVal[j] = '\0'; /*Null terminating the temp String*/
            if (!isValidInteger(tempVal)) {
                int symbolValue;
                if (!findSymbolValue(head, tempVal, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                    printError(errorInfo, "Unvalid Integer or undefined symbol for offset operand");
                    return;
                }
                val = symbolValue; /* Use the value from the symbol list*/
            }
            addValueToDataArray(instructionArray, *IC, val);
            (*IC)++;
        }
        else {
            int val;
            if (!findSymbolValue(head, currentWord, "define",&val)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                printError(errorInfo, "Undefined symbol for operand");
                return;
            }
            addValueToDataArray(instructionArray, *IC, val);
            (*IC)++;
        }


    }

}


void addValueToDataArray(binaryWord* dataArray, int DC, int value) {
    binaryWord newWord;
    newWord.wordBits = value;
    dataArray[DC] = newWord;
}

void printError(error** errorInfo, char* errorDescription){
    (*errorInfo)->counter++;
    (*errorInfo)->errorFlag = 1;
    if ((*errorInfo)->lineText[strlen((*errorInfo)->lineText) - 1] == '\n') {
        (*errorInfo)->lineText[strlen((*errorInfo)->lineText) - 1] = '\0'; /* Remove the newline character*/
    }
    printf("Error %d found in file '%s'\nline content: '%s'\nerror description: %s\n",(*errorInfo)->counter, (*errorInfo)->fileName, (*errorInfo)->lineText, errorDescription);
}

void incrementDataSymbolValues(symbolList** head, int byValue) {
    symbolList* current = (*head);
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
    (*errorInfo)->counter = 0;
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


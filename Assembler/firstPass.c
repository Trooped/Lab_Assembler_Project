#include "firstPass.h"
#include "assembler.h"

/*TODO MAYBE create the lists in the asmblr.c file and send them here?*/
void firstPass(FILE *sourceFile, word *dataArray, word *instructionArray, operation *operationsArray, symbolList *symbolTable, symbolList *externalSymbols, symbolList *entrySymbols, int *IC, int *DC) {
    int labelFlag = 0;
    int operation = 0;
    int L;
    int value;
    char lineBuffer[MAXCHARSPERLINE]; /*TODO define a maxcharsperlines in this firstPass maybe?*/
    char word[MAXCHARSPERWORD]; /*TODO is it large enough??*/
    symbolList* head = NULL; /* Initialize the list to empty*/
    /*TODO maybe if i already define a head, i don't need to do the head = when calling the functyions?*/

    while (fgets(lineBuffer, sizeof(lineBuffer), sourceFile)) {
        labelFlag = 0;
        strcpy(word, strtok(lineBuffer, " \n\r\t")); /* Tokenize the line into words*/
        while (word != NULL) {
            if (isDefine(word)){
                handleDefine(&head, lineBuffer);
            }
            else if (isLabel(word)){ /*checks if the first word is a label name TODO CHANGE COMMENT*/
                labelFlag = 1;
                /*handleLabel(&head, lineBuffer, &DC);*/
            }
            else if (isData(word) || isString(word)){
                if (labelFlag) {
                    addLabel(&head, word, "data", DC);
                }

                if (isData(word)){
                    addToDataArray("data", lineBuffer, &head, &DC, dataArray);
                }
                else{
                    addToDataArray("string", lineBuffer, &head, &DC, dataArray);
                }
            }
            else if (isExtern(word)){
                handleExtern(&head, lineBuffer);
            }
            else if (isEntry(word)){ /*TODO do i even need this in the first pass?*/
                /*TODO according to line 11*/
            }
            else{ /*TODO the case where it's not data, entry or extern- meaning operation!!*/
                addLabel(&head, word, "code", IC+100);
                strtok(NULL, " \n\r\t"); /* Get the next word.*/
                operation = isValidOperation(word, operationsArray);

                if (operation == -1){
                    /*TODO print an error on wrong operation name and stuff and exit or something*/
                }
                else{
                    L = handleOperation(&head, instructionArray, operation, lineBuffer, IC, operationsArray);
                    if (L == -1){
                        /*TODO print an error on wrong operation name and stuff and exit or something*/
                    }
                    else{
                        IC += L;
                    }
                }



            }
            /*TODO maybe it's not any of the options, need to add an error option!*/


            /*TODO DO I NEED THIS STRTOK? seems useless*/
            strtok(NULL, " \n\r\t"); /* Get the next word.*/
        }
    }
    /*MOVE TO 16, ACCORDING TO THE COURSE'S ALGORITHM.*/



    /*TODO call the linked list memory free operation*/
}

int handleOperation(symbolList** head, word** instructionArray, int operation, char* line, int *IC, operation* operationsArray) {
    int L = 0;
    int opcode;
    int firstOperand;
    int secondOperand;
    int last2bits;/*TODO IS IT NECESSARY?*/

    char operands[MAXOPERANDS][MAXOPERANDLENGTH]; /*TODO define a maxcharsperlines in this firstPass maybe?*/
    fillOperandsArray(operands, MAXOPERANDS, MAXOPERANDLENGTH, '\0');


    parseOperands(line, operands); /*TODO change it to just separate the operands using a comma, the rest'll take care of itself*/

    if (operationsArray[operation].numOfOperands ==0) {
        if (operands[0][0] != '\0') {
            /*TODO add an error, too many operands*/
            /*TODO exit.*/
        }
        else{
            firstOperand = 0;
            secondOperand = 0;
        }
    }
    else if (operationsArray[operation].numOfOperands ==1) {
        if (operands[1][0] != '\0') {
            /*TODO add an error, too many operands*/
            /*TODO exit.*/
        }
        else{
            firstOperand = getOperandCode(operands[0], head); /*TODO am i sending this correctly?*/
            secondOperand = 0;
        }
    }
    else if (operationsArray[operation].numOfOperands ==2) {
        if (operands[2][0] != '\0') {
            /*TODO add an error, too many operands*/
            /*TODO exit.*/
        }
        else{
            firstOperand = getOperandCode(operands[0], head); /*TODO am i sending this correctly?*/
            secondOperand = getOperandCode(operands[1], head); /*TODO am i sending this correctly?*/
        }
    }
    /*TODO do i need another condition?*/

    /*TODO important!! everything stops here potentially!*/
    if (firstOperand == -999 || secondOperand == -999) {
        /*TODO add an error, wrong operand code*/
        /*TODO exit.*/
    }

    if (firstOperand == 2){
        L++;
    }
    if (secondOperand == 2){
        L++;
    }

    switch(operation){
        case 0: /*mov*/
            if (secondOperand == 0){
                /*TODO add an exit error!*/
            }
            else {

            }
            break;
        case 1: /*cmp*/
            /*TODO everything is legal here.*/
            break;
        case 2: /*add*/
            if (secondOperand == 0){
                /*TODO add an exit error!*/
            }
            else{
                /*TODO should i translate everything here?*/
            }
            break;
        case 3: /*sub*/
            if (secondOperand == 0){
                /*TODO add an exit error!*/
            }
            else{
                /*TODO should i translate everything here?*/
            }
            break;
        case 4: /*lea*/
            if (firstOperand == 3 || firstOperand == 0 || secondOperand == 0){
                /*TODO add an exit error!*/
            }
            else{
                /*TODO should i translate everything here?*/
            }
        case 5: /*not*/
            if (firstOperand == 0){
                /*TODO add an exit error!*/
            }
            else{
                /*TODO should i translate everything here?*/
            }
            break;
        case 6: /*clr*/
            if (firstOperand == 0){
                /*TODO add an exit error!*/
            }
            else{
                /*TODO should i translate everything here?*/
            }
            break;
        case 7: /*inc*/
            if (firstOperand == 0){
                /*TODO add an exit error!*/
            }
            else{
                /*TODO should i translate everything here?*/
            }
            break;
        case 8: /*dec*/
            if (firstOperand == 0){
                /*TODO add an exit error!*/
            }
            else{
                /*TODO should i translate everything here?*/
            }
            break;
        case 9: /*jmp*/
            if (firstOperand == 0 || firstOperand==2){
                /*TODO add an exit error!*/
            }
            else{
                /*TODO should i translate everything here?*/
            }
            break;
        case 10: /*bne*/
            if (firstOperand == 0 || firstOperand==2){
                /*TODO add an exit error!*/
            }
            else{
                /*TODO should i translate everything here?*/
            }
            break;
        case 11: /*red*/
            if (firstOperand == 0){
                /*TODO add an exit error!*/
            }
            else{
                /*TODO should i translate everything here?*/
            }
            break;
        case 12: /*prn*/
            /*TODO should i translate everything here?*/
            break;
        case 13: /*jsr*/
            if (firstOperand == 0 || firstOperand==2){
                /*TODO add an exit error!*/
            }
            else{
                /*TODO should i translate everything here?*/
            }
            break;
        case 14: /*rts*/
            /*TODO should i translate everything here?*/
            break;
        case 15: /*hlt*/
            /*TODO should i translate everything here?*/
            break;
        default:
            /*TODO add an error*/
            break;
    }

    insertInstruction(instructionArray, *IC, operation, firstOperand, secondOperand);

    L += operationsArray[operation].numOfOperands;
    return L;
}

void insertInstruction(word* instructionArray, int IC, int opcode, int firstOperand, int secondOperand) {
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

int getOperandCode(char* operand, symbolList** head){
    /* 0 = instant, constant (#number or #define)
     * 1 = direct, Label
     * 2 = constant index (meaning an offset number in an array (it must be of a label, and the offset must be a number / define THAT WAS DEFINED)
     * 3 = register, one of the registers.*/
    int i = 0, j=0;
    char tempVal[MAXOPERANDLENGTH]; /*TODO WOW I'VE GOTTA CHANGE THIS*/
    int val;
    char* endptr;
    char tempOperand[MAXOPERANDLENGTH];


    if (operand[0] == '#') {
        strcpy(tempOperand, operand+1);

        if (!isValidInteger){
            int symbolValue;
            if (!findSymbolValue(head, tempOperand, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                printf("Error: Undefined symbol '%s'\n", token); /*TODO change error!*/
                return -999; /*TODO add constant for failed function*/
            }
            val = symbolValue; /* Use the value from the symbol list*/
        }
        return 0;
    }
    else if (strcmp(operand, "r0") == 0 || strcmp(operand, "r1") == 0 || strcmp(operand, "r2") == 0 || strcmp(operand, "r3") == 0 || strcmp(operand, "r4") == 0 || strcmp(operand, "r5") == 0 || strcmp(operand, "r6") == 0 || strcmp(operand, "r7") == 0) {
        return 2;
    }
    else{
        while (operand[i] != '[' || operand[i] != '\0') {
            tempOperand[i] = operand[i];
            i++;
        }

        if (operand[i] == '\0') {
            if (isLabel(tempOperand) || isValidName(tempOperand)) {
                return 1;
            }
            else {
                /*TODO add error, wrong label name or w/e*/
                return -999; /*TODO add constant for failed function*/
            }
        }
        else if (operand[i] == '[') {
            if (isLabel(tempOperand) || isValidName(tempOperand)) {
                i++;
                while (operand[i] != ']' || operand[i] == '\0') {
                    tempVal[j] = operand[i];
                    i++;
                    j++;
                }
                if (operand[i]==']'){
                    val = strtol(tempVal, &endptr, 10); /* Converts to long and handles +/-*/

                    /* Check for invalid characters after the number*/
                    if (*endptr != '\0') { /* If strtol couldn't parse the whole token*/
                        int symbolValue;
                        if (!findSymbolValue(head, token, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                            printf("Error: Undefined symbol '%s'\n", token); /*TODO change this too*/
                            return -999; /*TODO add constant for failed function*/
                        }
                        val = symbolValue; /* Use the value from the symbol list*/
                    }
                    return 2;
                }
                else {
                    /*TODO add error, doesn't end with ] or something name or w/e*/
                    return -999; /*TODO add constant for failed function*/
                }
            }
            else {
                return -999; /*TODO add constant for failed function*/
            }
        }
        else {
            return -999; /*TODO add constant for failed function*/
        }
    }
    return -999; /*TODO add constant for failed function*/
}


void parseOperands(char *input, char operands[MAXOPERANDS][MAXOPERANDLENGTH]) {
    int operandIndex = 0, charIndex = 0;
    int i;

    for (i = 0; input[i] != '\0'; i++) {
        if (input[i] == ',' && charIndex != 0) { /* Check if comma is not at the start*/
            operands[operandIndex][charIndex] = '\0'; /* Terminate current operand string*/
            operandIndex++; /* Move to the next operand*/
            charIndex = 0; /* Reset character index for the new operand*/
            while (input[i + 1] && isspace((unsigned char)input[i + 1])) i++; /* Skip spaces after comma*/
        } else if (!isspace((unsigned char)input[i]) || (charIndex != 0 && isspace((unsigned char)input[i]))) {
            /* Copy character to the current operand if it's not a leading space*/
            operands[operandIndex][charIndex++] = input[i];
        }
    }

    if (charIndex != 0) { /* Check if there's a trailing operand*/
        operands[operandIndex][charIndex] = '\0'; /* Terminate the last operand*/
    }
}



#if 0
/*TODO i think i can delete this function*/
void handleLabel(symbolList** head, char* line, int *DC){
    validData *data = NULL;
    char name[MAXLABELNAME];
    char* currentWord; /*TODO define it as 30 or something? IDK*/

    /*Copying the Label name into the name string, without the colon (we know there's one colon because we've tested it earlier).*/
    char* colonPos = strchr(currentWord, ':'); /* Find the position of ':'*/
    int length = colonPos - currentWord; /* Calculate the length to copy*/
    strncpy(name, currentWord, length); /* Copy the characters*/
    name[length] = '\0'; /* Null-terminate the result string*/

    currentWord= strtok(line, " \n\r\t"); /* Tokenize the line into words*/
    if (isData(currentWord)){
        if (searchLabelList(&head, currentWord)){/*TODO make sure it means that if we didn't find anything*/
            addLabel(*head, name, "data", DC);/*TODO maybe i need to send the address?*/
            data = isDataValidAndAddToDataArray("data", line);
            if (data->valid){
                *DC += data->dataCounter;
            }
            else{
                /*TODO add an error and don't add it to the label list*/
            }
        }
        else{
            /*TODO return some error or something, label is already present.*/
        }
    }
    else if (isString(currentWord)){
        if (searchLabelList(&head, currentWord)){/*TODO make sure it means that if we didn't find anything*/
            addLabel(*head, name, "data", DC);/*TODO maybe i need to send the address?*/
            data = isDataValidAndAddToDataArray("string", line);
            if (data->valid){
                *DC += data->dataCounter;
            }
            else{
                /*TODO add an error and don't add it to the label list*/
            }
        }
        else{
            /*TODO return some error or something, label is already present.*/
        }
    }
    else if (isExtern(&head, line)) { /*TODO did I implement this correctly?*/
        printf("Warning, label is assigned to an external instruction\n");
        handleExtern(&head, line);
    }
    else{
        /*TODO add here according to line 10 in the algorithm*/
    }

}
#endif

void handleExtern(symbolList** head, char* line) {
    char* currentWord; /*TODO define it as 30 or something? IDK*/

    currentWord = strtok(NULL, " \n\r\t"); /* Get the next word.*/
    while (currentWord!= NULL) {
        addLabel(*head, currentWord, "external", 0);
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
    }
}


void handleDefine(symbolList** head, char* line) {
    char name[MAXLABELNAME]; /*TODO handle the maxname already*/
    char* endptr;
    int value;
    char* currentWord; /*TODO define it as 30 or something? IDK*/

    currentWord = strtok(line, " \n\r\t"); /* Tokenize the line into words*/
    while(currentWord != NULL) {
        if (isValidName){
            if (!searchLabelList(&head, currentWord)){ /*TODO explain and remember that it means it's returning 0 and not 1!*/
                strncpy(name, currentWord, MAXLABELNAME);
                currentWord = strtok(NULL, " \n\r\t"); /* Get the next word.*/
                if (strcmp(currentWord, "=")==0) {
                    currentWord = strtok(NULL, " \n\r\t"); /* Get the next word.*/
                    value = strtol(currentWord, &endptr, 10);/*TODO can i even use it in c90?*/
                    currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
                    if (*endptr == '\0' && currentWord == NULL){
                        addLabel(*head, name, "define", value);
                    }
                    else{
                        /*TODO add an error and don't add it to the label list*/
                    }
                }
            }
            else{
                /*TODO add an error and don't add it to the label list*/
            }
        }
        else{
            /*TODO add an error and don't add it to the label list*/
        }
    }

    // Call addLabel with head, name, and value
    addLabel(*head, name, "define", value);
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

int searchLabelList(symbolList** head, char* name, char* type) {
    symbolList* current = *head;

    /* Search for the name in the list*/
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && strcmp(current->type, type) == 0) {
            /* Label found in the list, return 0 = error TODO maybe add it as a constant?*/
            return 0;
        }
        current = current->next;
    }

    /* Name not found in the list, send 1 to signify creating a new node*/
    return 1;
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

void addToDataArray(char* type, char* line, symbolList ** head, int *DC, word** dataArray) {
    char* numbers;
    char* token;
    long val;
    int dataSize, currentSize, maxSize, maxString;
    char copiedLine[MAXCHARSPERLINE]; /*TODO define a maxcharsperlines in this firstPass maybe?*/
    char* endptr;
    if (strcmp(type, ".data") == 0) {
        /* Skip the ".data" part to get to the numbers*/
        numbers = strstr(line, ".data");
        if (!numbers) {
            printf("Error: Line does not start with .data\n");
            return;
        }
        numbers += strlen("data"); /* Move past ".data"*/

        /* Allocate or extend the dataArray initially or in chunks*/
        dataSize = 16; /* Define a reasonable data size*/
        currentSize = *DC; /* Current size of dataArray*/
        maxSize = currentSize + dataSize; /* Max size before next reallocation*/
        *dataArray = realloc(*dataArray, maxSize * sizeof(word));
        if (!*dataArray) {
            printf("Error: Memory allocation failed\n");
            return;
        }

        token = strtok(numbers, ",");
        while (token) {
            if (currentSize >= maxSize) {
                // Increase the dataArray size by another chunk
                maxSize += dataSize;
                *dataArray = realloc(*dataArray, maxSize * sizeof(word));
                if (!*dataArray) {
                    printf("Error: Memory allocation failed\n");
                    return;
                }
            }

            /* Trim leading spaces from the token*/
            while (isspace((unsigned char)*token)) token++;

            val = strtol(token, &endptr, 10); /* Converts to long and handles +/-*/

            /* Check for invalid characters after the number*/
            if (*endptr != '\0') { /* If strtol couldn't parse the whole token*/
                int symbolValue;
                if (!findSymbolValue(head, token, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                    printf("Error: Undefined symbol '%s'\n", token);
                    return;
                }
                val = symbolValue; /* Use the value from the symbol list*/
            }

            /* Store the integer and increment DC*/
            (*dataArray)[*DC].wordBits = (int)val;
            currentSize++;
            (*DC)++;
            *DC = currentSize;


            /* Get the next token*/
            token = strtok(NULL, ",");
        }
    }
    else if (strcmp(type, "string")==0) {
        int i;
        char *spacePos = strchr(line, ' '); /* Find the position of the first data element*/

        maxString = 66; /* Define a reasonable data size*/
        currentSize = *DC; /* Current size of dataArray*/
        maxSize = currentSize + dataSize; /* Max size before next reallocation*/
        *dataArray = realloc(*dataArray, maxSize * sizeof(word));
        if (!*dataArray) {
            printf("Error: Memory allocation failed\n");
            return;
        }

        strcpy(copiedLine, spacePos + 1); /* Copy the characters*/
        if (copiedLine == NULL) {/*TODO meaning that there's no space after .data or nothing?*/
            /*TODO add an error and don't add it to the label list*/
        }
        if (strlen(copiedLine) > 2 && copiedLine[0] == '"' && copiedLine[strlen(copiedLine) - 1] == '"') {
            for (i = 0; i < strlen(copiedLine); i++) {
                if (currentSize >= maxSize) {
                    // Increase the dataArray size by another chunk
                    maxSize += dataSize;
                    *dataArray = realloc(*dataArray, maxSize * sizeof(word));
                    if (!*dataArray) {
                        printf("Error: Memory allocation failed\n");
                        return;
                    }
                }

                if (copiedLine[i] != '"') {
                    (*dataArray)[*DC].wordBits = val; /*TODO i need to turn it into binary here?*/
                    currentSize++;
                    (*DC) = currentSize;
                }
                (*dataArray)[*DC].wordBits = '\0'; /*TODO should it just be 0??? adding 1 for the null terminator.*/
                (*DC)++;
            }
        } else {
            /*TODO add an error*/
            return;
        }
    }
    else{
        /*TODO add an error*/
        return;
    }
}

/*TODO add a free data struct function*/

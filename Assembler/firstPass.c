#include "firstPass.h"


/*TODO MAYBE create the lists in the asmblr.c file and send them here?*/
void firstPass(FILE *sourceFile, word *dataArray, word *instructionArray, operation *operationsArray, symbolList** symbolTable, int *IC, int *DC, error *errorInfo){
    int labelFlag = 0;
    int operation = 0;
    int L;
    char lineBuffer[MAXCHARSPERLINE]; /*TODO define a maxcharsperlines in this firstPass maybe?*/
    char word[MAXLABELNAME]; /*TODO is it large enough??*/
    /*TODO maybe if i already define a head, i don't need to do the head = when calling the functyions?*/

    while (fgets(lineBuffer, sizeof(lineBuffer), sourceFile)) {
        L = 0;
        labelFlag = 0;
        strcpy(word, strtok(lineBuffer, " \n\r\t")); /* Tokenize the line into words*/
        while (word != NULL) {
            if (isDefine(word)){
                handleDefine(symbolTable, &operationsArray, lineBuffer, &errorInfo);
            }
            else if (isValidLabelName(word, operationsArray, symbolTable, 1)){ /*checks if the first word is a valid label definition*/
                labelFlag = 1;
            }
            else if (isData(word) || isString(word)){
                if (labelFlag) {
                    addLabel(symbolTable, word, "data", *DC, errorInfo);
                }

                if (isData(word)){
                    handleData("data", lineBuffer, symbolTable, &DC, dataArray);
                }
                else{
                    handleData("string", lineBuffer, symbolTable, &DC, dataArray);
                }
            }
            else if (isExtern(word)){
                handleExtern(symbolTable, lineBuffer);
            }
            else if (isEntry(word)){ /*TODO do i even need this in the first pass?*/
                /*TODO according to line 11*/
            }
            else if (isValidOperation(word)){ /*TODO the case where it's not data, entry or extern- meaning operation!!*/
                addLabel(symbolTable, word, "code", *IC+100);
                strtok(NULL, " \n\r\t"); /* Get the next word.*/
                operation = isValidOperation(word, operationsArray);

                if (operation == -1){
                    printError(errorInfo, "Invalid operation");
                }
                else{
                    L = handleOperation(symbolTable, instructionArray, operation, lineBuffer, IC, operationsArray);
                    if (L == -1){
                        printError(errorInfo, "Invalid operation"); /*TODO do I even need this?*/
                    }
                    else{
                        IC += L;
                    }
                }

            }
            else{
                printError(errorInfo, "Invalid operation, label, or directive");
            }

            /*TODO DO I NEED THIS STRTOK? seems useless*/
            strtok(NULL, " \n\r\t"); /* Get the next word.*/
        }
    }
    /*MOVE TO 16, ACCORDING TO THE COURSE'S ALGORITHM.*/

    /*TODO call the linked list memory free operation, or actually do this after the second pass.*/
}

int handleOperation(symbolList** head, word** instructionArray, int opcode, char* line, int *IC, operation *operationsArray, error *errorInfo) {
    int L = 0;
    int firstOperand;
    int secondOperand;
    char operands[MAXOPERANDS][MAXOPERANDLENGTH]; /*TODO define a maxcharsperlines in this firstPass maybe?*/

    initializeOperandsArray(operands);
    parseOperands(line, operands);

    if (operationsArray[opcode].numOfOperands == 0) {
        if (operands[0][0] != '\0') {
            printError(errorInfo, "Error: Too many operands for %s operation", operationsArray[opcode].name);
        }
        else{
            firstOperand = 0;
            secondOperand = 0;
        }
    }
    else if (operationsArray[opcode].numOfOperands == 1) {
        if (operands[1][0] != '\0') {
            printError(errorInfo, "Error: Too many operands for %s operation", operationsArray[opcode].name);
        }
        else{
            firstOperand = getOperandCode(operands[0], head, operationsArray); /*TODO am i sending this correctly?*/
            secondOperand = 0;
        }
    }
    else if (operationsArray[opcode].numOfOperands == 2) {
        if (operands[2][0] != '\0') {
            printError(errorInfo, "Error: Too many operands for %s operation", operationsArray[opcode].name);
        }
        else{
            firstOperand = getOperandCode(operands[0], head, operationsArray); /*TODO am i sending this correctly?*/
            secondOperand = getOperandCode(operands[1], head, operationsArray); /*TODO am i sending this correctly?*/
        }
    }
    /*TODO do i need another condition? MAYBE I NEED SOMETHING ELSE?? LIKE TOO LITTLE OPERANDS?*/

    /*TODO important!! everything stops here potentially!*/
    /*TODO it seems like I've already taken care of this part in the operandCode function, but welp*/
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

    switch(opcode){
        case 0: /*mov*/
            if (secondOperand == 0){
                printError(errorInfo, "Error: Cannot move to an immediate operand");
            }
            break;
        case 1: /*cmp*/
            /*TODO everything is legal here.*/
            break;
        case 2: /*add*/
            if (secondOperand == 0){
                printError(errorInfo, "Error: Cannot add into an immediate operand");
            }
            break;
        case 3: /*sub*/
            if (secondOperand == 0){
                printError(errorInfo, "Error: Cannot subtract from an immediate operand");
            }
            else{
                /*TODO should i translate everything here?*/
            }
            break;
        case 4: /*lea*/
            if (firstOperand == 3 || firstOperand == 0 || secondOperand == 0){
                printError(errorInfo, "Error: Illegal operands for lea");
            }
            break; /*TODO maybe spread these^ for 3 different errors?*/
        case 5: /*not*/
            if (firstOperand == 0){
                printError(errorInfo, "Error: Cannot negate an immediate operand");
            }
            break;
        case 6: /*clr*/
            if (firstOperand == 0){
                printError(errorInfo, "Error: Cannot clear an immediate operand");
            }
            break;
        case 7: /*inc*/
            if (firstOperand == 0){
                printError(errorInfo, "Error: Cannot increment an immediate operand");
            }
            break;
        case 8: /*dec*/
            if (firstOperand == 0){
                printError(errorInfo, "Error: Cannot decrement an immediate operand");
            }
            break;
        case 9: /*jmp*/
            if (firstOperand == 0 || firstOperand==2){
                printError(errorInfo, "Error: Illegal operands for jmp");
            }
            break;
        case 10: /*bne*/
            if (firstOperand == 0 || firstOperand==2){
                printError(errorInfo, "Error: Illegal operands for bne");
            }
            else{
                /*TODO should i translate everything here?*/
            }
            break;
        case 11: /*red*/
            if (firstOperand == 0){
                printError(errorInfo, "Error: Cannot read into an immediate operand");
            }
            break;
        case 12: /*prn*/
            break;
        case 13: /*jsr*/
            if (firstOperand == 0 || firstOperand==2){
                printError(errorInfo, "Error: Illegal operands for jsr");
            }
            break;
        case 14: /*rts*/
            break;
        case 15: /*hlt*/
            break;
        default:
            printError(errorInfo, "Error: Invalid operation");
            break;
    }

    insertInstructionIntoArray(instructionArray, *IC, opcode, firstOperand, secondOperand);

    L += operationsArray[opcode].numOfOperands;
    return L;
}


void handleData(char* type, char* line, symbolList** head, int *DC, word** dataArray, error *errorInfo) {
    char* numbers;
    char* token;
    long val;
    int dataCounter = 0;
    int dataSize, currentSize, maxSize, maxString;
    char copiedLine[MAXCHARSPERLINE]; /*TODO define a maxcharsperlines in this firstPass maybe?*/
    char* endptr;
    if (strcmp(type, ".data") == 0) {
        /* Skip the ".data" part to get to the numbers*/
        numbers = strstr(line, ".data");
        if (!numbers) {
            printError(errorInfo, "Error: No numbers found after .data");
            return; /*TODO should I return here?*/
        }
        numbers += strlen(".data"); /* Move past ".data"*/

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
            if (dataCounter>= MAXDATAVALUESINARRAY){
                printError(errorInfo, "Error: Too many data values inserted, max is %s", MAXDATAVALUESINARRAY);
                return;
            }

            if (currentSize >= maxSize) {
                /* Increase the dataArray size by another chunk*/
                maxSize += dataSize;
                *dataArray = realloc(*dataArray, maxSize * sizeof(word));
                if (!*dataArray) {
                    printf("Error: Memory allocation failed\n"); /*TODO handle memory allocation errors differently!*/
                    return;
                }
            }

            /* Trim leading spaces from the token*/
            while (isspace((unsigned char)*token)) token++;

            if (!isValidInteger(token)){
                int symbolValue;
                if (!findSymbolValue(head, token, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                    printError(errorInfo, "Unvalid Integer or Undefined symbol '%s'", token);
                    return; /*TODO should i not return?*/
                }
                val = symbolValue; /* Use the value from the symbol list*/
            }

            /* Store the integer and increment DC*/
            addValueToDataArray(dataArray, *DC, val);
            currentSize++;
            (*DC)++;
            *DC = currentSize; /*TODO do i even need to do this? IT SEEMS WRONGGGGGGGGGGGG*/
            dataCounter++; /*how many data values is stored in the current array?*/


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
            printf("Error: Memory allocation failed\n"); /*TODO handle memory allocation bugs*/
            return;
        }

        strcpy(copiedLine, spacePos + 1); /* Copy the characters*/
        if (copiedLine == NULL) {/*TODO meaning that there's no space after .data or nothing?*/
            printError(errorInfo, "Error: No valid string was found after .string");
            return; /*TODO should I return here?*/
        }
        if (strlen(copiedLine) > 2 && copiedLine[0] == '"' && copiedLine[strlen(copiedLine) - 1] == '"') {
            for (i = 1; i < strlen(copiedLine-1); i++) {
                if (currentSize >= maxSize) {
                    // Increase the dataArray size by another chunk
                    maxSize += dataSize;
                    *dataArray = realloc(*dataArray, maxSize * sizeof(word));
                    if (!*dataArray) {
                        printf("Error: Memory allocation failed\n"); /*TODO handle memory managemebnt errorssssss*/
                        return;
                    }
                }

                addValueToDataArray(dataArray, *DC, copiedLine[i]);
                currentSize++;
                (*DC) = currentSize;
            }
            addValueToDataArray(dataArray, *DC, '\0'); /*TODO should it just be 0??? adding 1 for the null terminator.*/
            (*DC)++;
        }
        else {
            printError(errorInfo, "Error: No valid string was found after .string");
            return; /*TODO should I return?*/
        }
    }
    else{
        printError(errorInfo, "Error: No valid data directive was found");
        return;
    }
}




/*TODO I need to just add the next label after extern, and if there's mroe than 1 then error?*/
void handleExtern(symbolList** head, char* line) {
    char* currentWord;

    currentWord = strtok(NULL, " \n\r\t"); /* Get the next word.*/
    while (currentWord!= NULL) {
        addLabel(*head, currentWord, "external", 0);
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
    }
}


void handleDefine(symbolList** head, operation* operationsArray, char* line, error *errorInfo) {
    char name[MAXLABELNAME]; /*TODO handle the maxname already*/
    int value;
    char* currentWord; /*TODO define it as 30 or something? IDK*/

    currentWord = strtok(line, " \n\r\t"); /* Tokenize the line into words*/
    while(currentWord != NULL) {
        if (isValidLabelName(currentWord, operationsArray, head, 0)){
            if (!searchSymbolList(&head, currentWord, "define")){ /*TODO explain and remember that it means it's returning 0 and not 1!*/
                strncpy(name, currentWord, MAXLABELNAME);
                currentWord = strtok(NULL, " \n\r\t"); /* Get the next word.*/
                if (strcmp(currentWord, "=")==0) {
                    currentWord = strtok(NULL, " \n\r\t"); /* Get the next word.*/
                    if (!isValidInteger(currentWord)){
                        int symbolValue;
                        if (!findSymbolValue(head, currentWord, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                            printError(errorInfo, "Unvalid Integer or Undefined symbol '%s'", currentWord);
                            return; /*TODO do I need to return from here?*/
                        }
                        value = symbolValue; /* Use the value from the symbol list*/
                    }
                    else{
                        value = atoi(currentWord); /*TODO should I convert it like this?*/
                    }
                }
            }
            else{
                printError(errorInfo, ".define symbol already exists");
                return; /*TODO do I need to return from here?*/
            }
        }
        else{;
            printError(errorInfo, "Not a valid .define symbol name");
            return; /*TODO do I need to return from here?*/
        }
    }
    /* Passed all tests, call addLabel with head, name, type and value*/
    addLabel(*head, name, "define", value);
}


/*TODO add a free data struct function*/

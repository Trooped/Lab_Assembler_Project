#include "firstPass.h"


void firstPass(FILE *sourceFile, binaryWord *dataArray, binaryWord *instructionArray, operationInfo *operationsArray, symbolList** symbolTable, int *IC, int *DC, error** errorInfo){
    int labelFlag = 0;
    int operation = 0;
    int L;
    char lineBuffer[MAXCHARSPERLINE];
    char tempLine[MAXCHARSPERLINE];
    char* currentWord; /*TODO is it large enough??*/
    /*TODO maybe if i already define a head, i don't need to do the head = when calling the functyions?*/

    while (fgets(lineBuffer, sizeof(lineBuffer), sourceFile)) {
        tempLine[0] = '\0';
        strncpy(tempLine, lineBuffer, MAXCHARSPERLINE); /*TODO call all of the functions with tempLine, and not with lineBuffer.*/
        tempLine[MAXCHARSPERLINE - 1] = '\0'; /* Ensure null-termination*/

        /*TODO TESTING PURPOSES*/
        printf("lineBuffer: %s\n", lineBuffer);

        L = 0;
        labelFlag = 0;
        currentWord = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
        while (currentWord != NULL) {
            if (isDefine(currentWord)){
                handleDefine(symbolTable, operationsArray, tempLine, errorInfo);
                break;
            }
            else if (isValidLabelName(currentWord, operationsArray, symbolTable, 1)){ /*checks if the first binaryWord is a valid label definition*/
                labelFlag = 1;
            }
            else if (isData(currentWord) || isString(currentWord)){
                if (labelFlag) {
                    addLabel(symbolTable, currentWord, "data", *DC, errorInfo);
                }

                if (isData(currentWord)){
                    handleData("data", tempLine, symbolTable, DC, dataArray, errorInfo);
                    break;
                }
                else{
                    handleData("string", tempLine, symbolTable, DC, dataArray, errorInfo);
                    break;
                }
            }
            else if (isExtern(currentWord)){
                handleExtern(symbolTable, lineBuffer, errorInfo);
            }
            else if (isEntry(currentWord)){ /*TODO do i even need this in the first pass?*/
                /*TODO according to line 11*/
            }
            else if (isValidOperation(currentWord, operationsArray)!=-1){ /*TODO the case where it's not data, entry or extern- meaning operationInfo!!*/
                addLabel(symbolTable, currentWord, "code", *IC+100, errorInfo);
                strtok(NULL, " \n\r\t"); /* Get the next binaryWord.*/
                operation = isValidOperation(currentWord, operationsArray);

                /*
                if (operation == -1){
                    printError(errorInfo, "Invalid operation");
                }
                 */
                L = handleOperation(symbolTable, instructionArray, operation, tempLine, IC, operationsArray, errorInfo);
                if (L == -1){
                    printError(errorInfo, "Invalid operation"); /*TODO do I even need this?*/
                    break;
                }
                else{
                    IC += L;
                }

            }
            else{
                printError(errorInfo, "Invalid operation, label, or directive");
                break;
            }

            currentWord = strtok(NULL, " \n\r\t"); /* Get the next binaryWord.*/
        }
    }

    /*TODO return here*/

    /*TODO call the linked list memory free operationInfo, or actually do this after the second pass.*/
}

int handleOperation(symbolList** head, binaryWord* instructionArray, int opcode, char* line, int *IC, operationInfo *operationsArray, error** errorInfo) {
    int L = 0;
    char* colon;
    int firstOperand;
    int secondOperand;
    char operands[MAXOPERANDS][MAXOPERANDLENGTH]; /*TODO define a maxcharsperlines in this firstPass maybe?*/
    initializeOperandsArray(operands);


    /* Skip label if present*/
    colon = strchr(line, ':');
    if (colon) {
        line = colon + 1; /* Move past the colon*/
    }

    /* Move past any whitespace after the label or start of the line*/
    while (isspace((unsigned char)*line)) line++;

    /* Skip the operation (3 letters) and any space after it*/
    line += strlen(operationsArray[opcode].name);
    while (isspace((unsigned char)*line)) line++;

    /* Now 'line' should be positioned at the start of the operands*/
    parseOperands(line, operands);


    if (operationsArray[opcode].numOfOperands == 0) {
        if (operands[0][0] != '\0') {
            printError(errorInfo, "Error: Too many operands for a 0 operand operationInfo");
        }
        else{
            firstOperand = 0;
            secondOperand = 0;
        }
    }
    else if (operationsArray[opcode].numOfOperands == 1) {
        if (operands[1][0] != '\0') {
            printError(errorInfo, "Error: Too many operands for a 1 operand operationInfo");
        }
        else{
            firstOperand = getOperandCode(operands[0], head, operationsArray, errorInfo); /*TODO am i sending this correctly?*/
            secondOperand = 0;
        }
    }
    else if (operationsArray[opcode].numOfOperands == 2) {
        if (operands[2][0] != '\0') {
            printError(errorInfo, "Error: Too many operands for a 2 operand operationInfo");
        }
        else{
            firstOperand = getOperandCode(operands[0], head, operationsArray, errorInfo); /*TODO am i sending this correctly?*/
            secondOperand = getOperandCode(operands[1], head, operationsArray, errorInfo); /*TODO am i sending this correctly?*/
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
            printError(errorInfo, "Error: Invalid operationInfo");
            break;
    }

    insertInstructionIntoArray(instructionArray, *IC, opcode, firstOperand, secondOperand);

    L += operationsArray[opcode].numOfOperands;
    return L;
}


void handleData(char* type, char* line, symbolList** head, int *DC, binaryWord* dataArray, error** errorInfo) {
    char* numbers;
    char* token;
    long val;
    int dataCounter = 0;
    char copiedLine[MAXCHARSPERLINE]; /*TODO define a maxcharsperlines in this firstPass maybe?*/
    if (strcmp(type, ".data") == 0) {
        /* Skip the ".data" part to get to the numbers*/
        numbers = strstr(line, ".data");
        if (!numbers) {
            printError(errorInfo, "Error: No numbers found after .data");
            return; /*TODO should I return here?*/
        }
        numbers += strlen(".data"); /* Move past ".data"*/

        token = strtok(numbers, ",");
        while (token) {
            if (dataCounter>= MAXDATAVALUESINARRAY){
                printError(errorInfo, "Error: Too many data values inserted");
                return;
            }

            /* Trim leading spaces from the token*/
            while (isspace((unsigned char)*token)) token++;

            if (!isValidInteger(token)){
                int symbolValue;
                if (!findSymbolValue(head, token, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                    printError(errorInfo, ("Unvalid Integer or Undefined symbol '%s'", token));
                    return; /*TODO should i not return?*/
                }
                val = symbolValue; /* Use the value from the symbol list*/
            }

            /* Store the integer and increment DC*/
            addValueToDataArray(dataArray, *DC, val);
            (*DC)++;
            dataCounter++; /*how many data values is stored in the current array?*/

            /* Get the next token*/
            token = strtok(NULL, ",");
        }
    }
    else if (strcmp(type, "string")==0) {
        int i;
        char *spacePos = strchr(line, ' '); /* Find the position of the first data element*/

        strcpy(copiedLine, spacePos + 1); /* Copy the characters*/
        if (strcmp(copiedLine, NULL) == 0) {/*TODO meaning that there's no space after .data or nothing?*/
            printError(errorInfo, "Error: No valid string was found after .string");
            return; /*TODO should I return here?*/
        }
        if (strlen(copiedLine) > 2 && copiedLine[0] == '"' && copiedLine[strlen(copiedLine) - 1] == '"') {
            for (i = 1; i < strlen(copiedLine-1); i++) {
                addValueToDataArray(dataArray, *DC, copiedLine[i]);
                (*DC)++;
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
void handleExtern(symbolList** head, char* line, error** errorInfo){
    char* currentWord;

    currentWord = strtok(line, " \n\r\t"); /* Get the next binaryWord.*/
    while (currentWord!= NULL) {
        addLabel(head, currentWord, "external", 0, errorInfo);
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next binaryWord.*/
    }
}


int handleDefine(symbolList** head, operationInfo* operationsArray, char* line, error** errorInfo) {
    char name[MAXLABELNAME] = {0};
    int value = 0;
    char* ptr = line;

    /* Skip past ".define", assuming 'line' starts with this directive*/
    ptr += strlen(".define");

    /* Skip whitespace after ".define"*/
    while (*ptr == ' ' || *ptr == '\t') ptr++;

    /* Copy the name until we hit a space, tab, or '='*/
    char* startName = ptr;
    while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '=') ptr++;
    int nameLength = ptr - startName;
    if (nameLength >= MAXLABELNAME) nameLength = MAXLABELNAME - 1;
    strncpy(name, startName, nameLength);
    name[nameLength] = '\0';

    if (!isValidLabelName(name, operationsArray, head, 0)) {
        printError(errorInfo, "Not a valid .define symbol name");
        return 0;
    }

    if (!searchSymbolList(head, name, "define")) {
        printError(errorInfo, ".define symbol already exists");
        return 0;
    }

    /* Skip to '='*/
    while (*ptr && *ptr != '=') ptr++;
    if (!*ptr) {
        printError(errorInfo, "Missing '=' in .define statement");
        return 0;
    }
    ptr++; /* Move past '='*/

    /* Skip whitespace after '='*/
    while (*ptr == ' ' || *ptr == '\t') ptr++;

    /* ptr should now point at the start of the value*/
    char* valueStr = ptr;
    char* endValue = ptr;
    while (*endValue && *endValue != ' ' && *endValue != '\t' && *endValue != '\n' && *endValue != '\r') endValue++;
    *endValue = '\0'; /* Temporarily terminate the string for value conversion*/

    if (!isValidInteger(valueStr)) {
        int symbolValue;
        if (!findSymbolValue(head, valueStr,"define", &symbolValue)) {
            printError(errorInfo, "Invalid integer or undefined symbol for '.define'");
            return 0;
        }
        value = symbolValue; /* Use the value from the symbol list*/
    } else {
        value = atoi(valueStr);
    }

    /* Passed all tests, call addLabel with head, name, type, and value*/
    addLabel(head, name, "define", value, errorInfo);
    return 1;
}


#include "firstPass.h"


void firstPass(FILE *sourceFile, binaryWord *dataArray, binaryWord *instructionArray, operationInfo *operationsArray, symbolList** symbolTable, int *IC, int *DC, error** errorInfo){
    int labelFlag = 0;
    int operation = 0;
    int L;
    char lineBuffer[MAXCHARSPERLINE];
    char fullLine[MAXCHARSPERLINE];
    char tempLabelName[MAXLABELNAME];
    char* currentWord;

    while (fgets(lineBuffer, sizeof(lineBuffer), sourceFile)) {
        fullLine[0] = '\0';
        strncpy(fullLine, lineBuffer, MAXCHARSPERLINE);
        fullLine[MAXCHARSPERLINE - 1] = '\0'; /* Ensure null-termination*/
        strncpy((*errorInfo)->lineText, fullLine, MAXCHARSPERLINE); /* Copying the current line into the error struct*/

        /*TODO TESTING PURPOSES*/
        /*printf("lineBuffer: %s\n", lineBuffer);*/
        /* TODO TSTINGGGGGGG*/

        L = 0;
        labelFlag = 0;
        currentWord = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
        while (currentWord != NULL) {
            if (isDefine(currentWord)){
                handleDefine(symbolTable, operationsArray, fullLine, errorInfo);
                break;
            }
            else if (isValidLabelName(currentWord, operationsArray, symbolTable, 1)){ /*checks if the first word is a valid label definition*/
                labelFlag = 1;
                strncpy(tempLabelName, currentWord, MAXLABELNAME);
            }
            else if (isData(currentWord) || isString(currentWord)){
                if (labelFlag) {
                    addLabel(symbolTable, tempLabelName, "data", *DC, errorInfo);
                }

                if (isData(currentWord)){
                    handleData("data", fullLine, symbolTable, DC, dataArray, errorInfo);
                    break;
                }
                else{
                    handleData("string", fullLine, symbolTable, DC, dataArray, errorInfo);
                    break;
                }
            }
            else if (isExtern(currentWord)){
                handleExtern(symbolTable, fullLine, errorInfo, operationsArray);
                break;
            }
            else if (isEntry(currentWord)){
                checkEntrySyntax(symbolTable, fullLine, errorInfo, operationsArray);
                break;
            }
            else if (isValidOperation(currentWord, operationsArray)!=-1){
                if (labelFlag) {
                    addLabel(symbolTable, tempLabelName, "code", *IC + 100, errorInfo);
                }
                strtok(NULL, " \n\r\t"); /* Get the next binaryWord.*/
                operation = isValidOperation(currentWord, operationsArray);

                /* TODO do i need it?
                if (operation == -1){
                    printError(errorInfo, "Invalid operation");
                }
                 */
                L = handleOperation(symbolTable, instructionArray, operation, fullLine, IC, operationsArray, errorInfo, 0);
                if (L == -1){
                    printError(errorInfo, "Invalid operation"); /*TODO do I even need this?*/
                    break;
                }
                else{
                    (*IC) += L;
                }

            }
            else{
                printError(errorInfo, "Invalid operation, label, or directive");
                break;
            }

            currentWord = strtok(NULL, " \n\r\t"); /* Get the next binaryWord.*/
        }
    }
}

int handleOperation(symbolList** head, binaryWord* instructionArray, int opcode, char* line, int *IC, operationInfo *operationsArray, error** errorInfo, int isSecondPass) {
    int L = 0;
    int i;
    char* colon;
    int firstOperand;
    int secondOperand;
    char operands[MAXOPERANDS][MAXOPERANDLENGTH];
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
    parseOperandsFirstPass(line, operands);


    if (operationsArray[opcode].numOfOperands == 0) {
        if (operands[0][0] != '\0') {
            printError(errorInfo, "Too many operands for a 0 operand operationInfo");
        }
        else{
            firstOperand = 0;
            secondOperand = 0;
        }
    }
    else if (operationsArray[opcode].numOfOperands == 1) {
        if (operands[1][0] != '\0') {
            printError(errorInfo, "Too many operands for a 1 operand operationInfo");
        }
        else{
            firstOperand = 0;
            secondOperand = getOperandCode(operands[0], head, operationsArray, errorInfo);
        }
    }
    else if (operationsArray[opcode].numOfOperands == 2) {
        if (operands[2][0] != '\0') {
            printError(errorInfo, "Too many operands for a 2 operand operationInfo");
        }
        else{
            firstOperand = getOperandCode(operands[0], head, operationsArray, errorInfo);
            secondOperand = getOperandCode(operands[1], head, operationsArray, errorInfo);
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
                printError(errorInfo, "Cannot move to an immediate operand");
            }
            break;
        case 1: /*cmp*/
            break;
        case 2: /*add*/
            if (secondOperand == 0){
                printError(errorInfo, "Cannot add into an immediate operand");
            }
            break;
        case 3: /*sub*/
            if (secondOperand == 0){
                printError(errorInfo, "Cannot subtract from an immediate operand");
            }
            break;
        case 4: /*lea*/
            if (firstOperand == 3 || firstOperand == 0 || secondOperand == 0){
                printError(errorInfo, "Illegal operands for lea operation");
            }
            break;
        case 5: /*not*/
            if (secondOperand == 0){
                printError(errorInfo, "Cannot negate an immediate operand");
            }
            break;
        case 6: /*clr*/
            if (secondOperand == 0){
                printError(errorInfo, "Cannot clear an immediate operand");
            }
            break;
        case 7: /*inc*/
            if (secondOperand == 0){
                printError(errorInfo, "Cannot increment an immediate operand");
            }
            break;
        case 8: /*dec*/
            if (secondOperand == 0){
                printError(errorInfo, "Cannot decrement an immediate operand");
            }
            break;
        case 9: /*jmp*/
            if (secondOperand == 0 || secondOperand==2){
                printError(errorInfo, "Illegal operands for jmp");
            }
            break;
        case 10: /*bne*/
            if (secondOperand == 0 || secondOperand==2){
                printError(errorInfo, "Illegal operands for bne");
            }
            break;
        case 11: /*red*/
            if (secondOperand == 0){
                printError(errorInfo, "Cannot read into an immediate operand");
            }
            break;
        case 12: /*prn*/
            break;
        case 13: /*jsr*/
            if (secondOperand == 0 || secondOperand==2){
                printError(errorInfo, "Illegal operands for jsr");
            }
            break;
        case 14: /*rts*/
            break;
        case 15: /*hlt*/
            break;
        default:
            printError(errorInfo, "Invalid operationInfo");
            break;
    }

    if (firstOperand == 3 &&  secondOperand == 3){ /*Sharing one binary word with 2 registers*/
        L--;
    }

    L += operationsArray[opcode].numOfOperands + 1; /* The number of operands + the operation itself*/

    if (!isSecondPass){
        insertFirstInstructionIntoArray(instructionArray, *IC, opcode, firstOperand, secondOperand);
    }
    else if (operationsArray[opcode].numOfOperands != 0){
        insertOperandsIntoInstructionArray(instructionArray, L, IC, operands, head, errorInfo);
    }

    return L;
}


void handleData(char* type, char* line, symbolList** head, int *DC, binaryWord* dataArray, error** errorInfo) {
    char* numbers;
    char* token;
    long val;
    int dataCounter = 0;
    char copiedLine[MAXCHARSPERLINE];
    if (strcmp(type, "data") == 0) {
        /* Skip the ".data" part to get to the numbers*/
        numbers = strstr(line, ".data");
        if (!numbers) {
            printError(errorInfo, "No values found after .data");
            return;
        }
        numbers += strlen(".data"); /* Move past ".data"*/

        token = strtok(numbers, ",");
        while (token) {
            if (dataCounter>= MAXDATAVALUESINARRAY){
                printError(errorInfo, "Too many data values inserted");
                return;
            }

            trimWhitespace(token); /* Remove leading and trailing whitespace*/

            if (!isValidInteger(token)){
                int symbolValue;
                if (!findSymbolValue(head, token, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                    printError(errorInfo, ("Unvalid Integer or Undefined symbol '%s'", token));
                    return; /*TODO should i not return?*/
                }
                val = symbolValue; /* Use the value from the symbol list*/
            }
            else{
                val = atoi(token);
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
        char* stringPosition = strstr(line, ".string");
        int i;


        strcpy(copiedLine, stringPosition + strlen(".string") + 1); /* Copy the characters AFTER the .string declaration.*/
        if (copiedLine[0] == '\0') {
            printError(errorInfo, "No valid string was found after .string");
            return;
        }
        trimWhitespace(copiedLine); /* Remove leading and trailing whitespace*/
        if (strlen(copiedLine) > 2 && copiedLine[0] == '"' && copiedLine[strlen(copiedLine)-1] == '"') {
            for (i = 1; i < strlen(copiedLine)-1; i++) {
                addValueToDataArray(dataArray, *DC, copiedLine[i]);
                (*DC)++;
            }
            addValueToDataArray(dataArray, *DC, '\0');
            (*DC)++;
        }
        else {
            printError(errorInfo, "No valid string was found after .string");
            return;
        }
    }
}



void handleExtern(symbolList** head, char* line, error** errorInfo, operationInfo* operationsArray){
    char* currentWord;
    int flag = 0;

    currentWord = strtok(line, " \n\r\t"); /* Get the next word.*/
    currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
    while (currentWord!= NULL) {
        if (flag){
            printError(errorInfo, "Extraneous text after First label of .extern");
            return;
        }
        else if (!isValidLabelName(currentWord, operationsArray, head, 0)) {
            printError(errorInfo, "Not a valid .extern symbol name");
            return;
        }
        else {
            addLabel(head, currentWord, "external", 0, errorInfo);
            flag = 1;
        }
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
    }
}


int handleDefine(symbolList** head, operationInfo* operationsArray, char* line, error** errorInfo) {
    char name[MAXLABELNAME] = {0};
    int value = 0;
    char* ptr = line;
    char* startName;
    int nameLength;
    char* valueStr;
    char* endValue;

    /* Skip past ".define", assuming 'line' starts with this directive*/
    ptr += strlen(".define");

    /* Skip whitespace after ".define"*/
    while (*ptr == ' ' || *ptr == '\t') ptr++;

    /* Copy the name until we hit a space, tab, or '='*/
    startName = ptr;
    while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '=') ptr++;
    nameLength = ptr - startName;
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
    valueStr = ptr;
    endValue = ptr;
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


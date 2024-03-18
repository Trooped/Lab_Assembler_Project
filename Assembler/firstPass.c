#include "firstPass.h"

/**
 * This function is the first pass of the assembler. It goes through the source file and creates the symbol table, and the data and initial instruction arrays.
 * @param sourceFile The source file to be read.
 * @param dataArray The array to store the data.
 * @param instructionArray The array to store the instructions.
 * @param operationsArray The array to store the operations.
 * @param symbolTable The symbol table.
 * @param IC The instruction counter.
 * @param DC The data counter.
 * @param errorInfo The error struct.
 */
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
                if (labelFlag) { /*TODO do i need to add a warning? because this label is ignored*/
                    handleExtern(symbolTable, fullLine, errorInfo, operationsArray, 1);
                }
                else{
                    handleExtern(symbolTable, fullLine, errorInfo, operationsArray, 0);
                }
                break;
            }
            else if (isEntry(currentWord)){
                if (labelFlag) { /*TODO do i need to add a warning? because this label is ignored*/
                    checkEntrySyntax(symbolTable, fullLine, errorInfo, operationsArray,1);
                }
                else{
                    checkEntrySyntax(symbolTable, fullLine, errorInfo, operationsArray, 0);
                }
                break;
            }
            else if (isValidOperation(currentWord, operationsArray)!=-1){
                if (labelFlag) {
                    addLabel(symbolTable, tempLabelName, "code", *IC + 100, errorInfo);
                }
                strtok(NULL, " \n\r\t"); /* Get the next binaryWord.*/
                operation = isValidOperation(currentWord, operationsArray);

                L = handleOperation(symbolTable, instructionArray, operation, fullLine, IC, operationsArray, errorInfo, 0);
                if (L == INSTRUCTIONFAILCODE){
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

/**
 * This function handles the operations, and does different actions depending on if it's the first or second pass.
 * first pass - only checks for syntax errors, adds the first word only and updates the IC
 * second pass - adds the operands to the instruction array (to the reserved spaces after the first word)
 * @param head - the symbol table
 * @param instructionArray - the array of instructions
 * @param opcode - the operation code of our operation
 * @param line - the current line text
 * @param IC - the instruction counter
 * @param operationsArray - the array of operations
 * @param errorInfo - the error struct
 * @param isSecondPass - a flag to signal if it's the second pass, and use different actions accordingly
 * @return the length of the operation or INSTRUCTIONFAILCODE if the operation failed
 */
int handleOperation(symbolList** head, binaryWord* instructionArray, int opcode, char* line, int *IC, operationInfo *operationsArray, error** errorInfo, int isSecondPass) {
    int L = 0;
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
    if (!parseOperandsFirstPass(line, operands, errorInfo)) {
        return INSTRUCTIONFAILCODE;
    }

    if (operationsArray[opcode].numOfOperands == 0) {
        if (operands[0][0] != '\0') {
            printError(errorInfo, "Too many operands for a 0 operand operation");
            return INSTRUCTIONFAILCODE;
        }
        else{
            firstOperand = 0;
            secondOperand = 0;
        }
    }
    else if (operationsArray[opcode].numOfOperands == 1) {
        if (operands[1][0] != '\0') {
            printError(errorInfo, "Too many operands for a 1 operand operation");
            return INSTRUCTIONFAILCODE;
        }
        else if(operands[0][0] == '\0'){
            printError(errorInfo, "Too few operands for a 1 operand operation");
            return INSTRUCTIONFAILCODE;
        }
        else{
            firstOperand = 0;
            secondOperand = getOperandCode(operands[0], head, operationsArray, errorInfo);
            if (secondOperand == INSTRUCTIONFAILCODE){
                return INSTRUCTIONFAILCODE;
            }
        }
    }
    else if (operationsArray[opcode].numOfOperands == 2) {
        if (operands[2][0] != '\0') {
            printError(errorInfo, "Too many operands for a 2 operand operation");
            return INSTRUCTIONFAILCODE;
        }
        else if (operands[1][0] == '\0'){
            printError(errorInfo, "Too few operands for a 2 operand operation");
            return INSTRUCTIONFAILCODE;
        }
        else if (operands[0][0] == '\0'){
            printError(errorInfo, "Too few operands for a 2 operand operation");
            return INSTRUCTIONFAILCODE;
        }
        else{
            firstOperand = getOperandCode(operands[0], head, operationsArray, errorInfo);
            if (firstOperand != INSTRUCTIONFAILCODE){
                secondOperand = getOperandCode(operands[1], head, operationsArray, errorInfo);
            }
        }
    }
    /*TODO ADD ERROR FOR TOO LITTLE OPERANDS!*/

    /*Another test if anything failed*/
    if (firstOperand == INSTRUCTIONFAILCODE || secondOperand == INSTRUCTIONFAILCODE) {
        return INSTRUCTIONFAILCODE;
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
                return INSTRUCTIONFAILCODE;
            }
            break;
        case 1: /*cmp*/
            break;
        case 2: /*add*/
            if (secondOperand == 0){
                printError(errorInfo, "Cannot add into an immediate operand");
                return INSTRUCTIONFAILCODE;
            }
            break;
        case 3: /*sub*/
            if (secondOperand == 0){
                printError(errorInfo, "Cannot subtract from an immediate operand");
                return INSTRUCTIONFAILCODE;
            }
            break;
        case 4: /*lea*/
            if (firstOperand == 3 || firstOperand == 0 || secondOperand == 0){
                printError(errorInfo, "Illegal operands for lea operation");
                return INSTRUCTIONFAILCODE;
            }
            break;
        case 5: /*not*/
            if (secondOperand == 0){
                printError(errorInfo, "Cannot negate an immediate operand");
                return INSTRUCTIONFAILCODE;
            }
            break;
        case 6: /*clr*/
            if (secondOperand == 0){
                printError(errorInfo, "Cannot clear an immediate operand");
                return INSTRUCTIONFAILCODE;
            }
            break;
        case 7: /*inc*/
            if (secondOperand == 0){
                printError(errorInfo, "Cannot increment an immediate operand");
                return INSTRUCTIONFAILCODE;
            }
            break;
        case 8: /*dec*/
            if (secondOperand == 0){
                printError(errorInfo, "Cannot decrement an immediate operand");
                return INSTRUCTIONFAILCODE;
            }
            break;
        case 9: /*jmp*/
            if (secondOperand == 0 || secondOperand==2){
                printError(errorInfo, "Illegal operands for jmp");
                return INSTRUCTIONFAILCODE;
            }
            break;
        case 10: /*bne*/
            if (secondOperand == 0 || secondOperand==2){
                printError(errorInfo, "Illegal operands for bne");
                return INSTRUCTIONFAILCODE;
            }
            break;
        case 11: /*red*/
            if (secondOperand == 0){
                printError(errorInfo, "Cannot read into an immediate operand");
                return INSTRUCTIONFAILCODE;
            }
            break;
        case 12: /*prn*/
            break;
        case 13: /*jsr*/
            if (secondOperand == 0 || secondOperand==2){
                printError(errorInfo, "Illegal operands for jsr");
                return INSTRUCTIONFAILCODE;
            }
            break;
        case 14: /*rts*/
            break;
        case 15: /*hlt*/
            break;
        default:
            printError(errorInfo, "Invalid operation");
            return INSTRUCTIONFAILCODE;
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

/**
 * This function handles the .data and .string directives, and adds the data to the data array
 * @param type - the type of directive (data or string)
 * @param line - the current line text
 * @param head - the symbol table
 * @param DC - the data counter
 * @param dataArray - the array of data
 * @param errorInfo - the error struct
 */
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
                    printError(errorInfo, ("Invalid Integer or Undefined symbol"));
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
                if (isprint(copiedLine[i])){
                    addValueToDataArray(dataArray, *DC, copiedLine[i]);
                    (*DC)++;
                }
                else{
                    printError(errorInfo, "Invalid ASCII character in string");
                    return;
                }
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


/**
 * This function handles the .extern directive, and adds the label to the symbol table if it's valid
 * @param head - the symbol table
 * @param line - the current line text
 * @param errorInfo - the error struct
 * @param operationsArray - the array of operations
 * @param labelFlag - a flag to signal if the first word is a label
 */
void handleExtern(symbolList** head, char* line, error** errorInfo, operationInfo* operationsArray, int labelFlag){
    char* currentWord;
    int flag = 0;

    if(labelFlag){
        currentWord = strtok(line, " \n\r\t"); /* Get the next word.*/
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
    }
    else{
        currentWord = strtok(line, " \n\r\t"); /* Get the next word.*/
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
    }
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

/**
 * This function handles the .define directive, and adds the label to the symbol table if it's valid
 * @param head - the symbol table
 * @param operationsArray - the array of operations
 * @param line - the current line text
 * @param errorInfo - the error struct
 */
void handleDefine(symbolList** head, operationInfo* operationsArray, char* line, error** errorInfo) {
    char name[MAXLABELNAME] = {0};
    int value = 0;
    char* ptr = line;
    char* startName;
    int nameLength;
    char* equalPos;

    /* Skip past ".define" and any whitespace that immediately follows */
    ptr += strlen(".define");
    while (*ptr == ' ' || *ptr == '\t') ptr++;

    /* Now, 'ptr' should be at the start of the name */
    startName = ptr;
    equalPos = strchr(ptr, '='); /* Find the position of '=' */
    if (!equalPos) {
        printError(errorInfo, "Missing '=' in .define statement");
        return;
    }

    /* Calculate name length, ensuring name doesn't include spaces leading up to '=' */
    nameLength = equalPos - startName;
    while (nameLength > 0 && (startName[nameLength - 1] == ' ' || startName[nameLength - 1] == '\t')) {
        nameLength--; /* Exclude trailing spaces or tabs from the name */
    }

    if (nameLength >= MAXLABELNAME) nameLength = MAXLABELNAME - 1;
    strncpy(name, startName, nameLength);
    name[nameLength] = '\0'; /* Ensure null-termination */

    /* Check validity of the name */
    if (!isValidLabelName(name, operationsArray, head, 0)) {
        printError(errorInfo, "Not a valid .define symbol name");
        return;
    }

    /* Move 'ptr' past '=' and optional whitespace */
    ptr = equalPos + 1;
    while (*ptr == ' ' || *ptr == '\t') ptr++; /* Skip spaces or tabs after '=' */
    trimWhitespace(ptr); /* Remove leading and trailing whitespace*/

    /* 'ptr' should now be at the start of the value */
    if (!isValidInteger(ptr)) {
        printError(errorInfo, "Invalid integer or undefined symbol for '.define'");
        return;
    }
    value = atoi(ptr);

    /* Check for duplicate label name */
    if (!searchSymbolList(head, name, "define")) {
        printError(errorInfo, ".define symbol already exists");
        return;
    }

    /* If all checks pass, add the label */
    addLabel(head, name, "define", value, errorInfo);
}


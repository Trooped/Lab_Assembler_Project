/**
 * @file syntaxAnalyzer.c
 *
 * This file contains the functions for the syntax analyzer.
 * The different functions in the file, analyze the syntax of the assembly code, and handle the directives and operations.
 * It also checks if the label names are valid, and if the operands are valid.
 * The syntax analyzer is used in the first and second pass of the assembler.
 * It is by far the biggest file, with the most diverse functions.
 *
 * The functions in this file are:
 * 1. getOperandCode - This function gets the operand code for the instruction.
 * 2. handleOperation - This function handles the operations, and does different actions depending on if it's the first or second pass.
 * 3. handleData - This function handles the .data and .string directives, and adds the data to the data array.
 * 4. handleExtern - This function han   dles the .extern directive, and adds the label to the symbol table if it's valid.
 * 5. handleDefine - This function handles the .define directive, and adds the label to the symbol table if it's valid.
 * 6. parseOperandsFirstPass - This function parses the operands in the first pass of the assembler.
 * 7. parseOperandsSecondPass - This function parses the operands in the second pass of the assembler.
 * 8. trimWhitespace - This function trims the whitespace from a string.
 * 9. isDefine - This function checks if a string is a .define directive.
 * 10. isExtern - This function checks if a string is a .extern directive.
 * 11. isEntry - This function checks if a string is an .entry directive.
 * 12. isData - This function checks if a string is a .data directive.
 * 13. isString - This function checks if a string is a .string directive.
 * 14. isValidOperation - This function checks if a string is a valid operation.
 * 15. isValidLabelName - This function checks if a string is a valid label name.
 * 16. isValidInteger - This function checks if a string is a valid integer.
 * 17. isRegister - This function checks if a string is a register.
 * 18. checkEntrySyntax - This function checks the syntax of the .entry directive.
 * 19. checkLineLengthAndSkip - This function checks the length of the line and skips it if it's too long.
 */

#include "syntaxAnalyzer.h"


/**
 * This function gets the operand code for the instruction.
 * * 0 = instant, constant (#number or #define)
 * 1 = direct, Label
 * 2 = constant index (meaning an offset number in an array (it must be of a label, and the offset must be a number / define THAT WAS DEFINED)
 * 3 = register, one of the registers.
 *
 * @param operand The operand to be checked.
 * @param head The symbol list.
 * @param operationsArray The operations array.
 * @param errorInfo The error struct.
 * @return The operand code.
 */
int getOperandCode(char* operand, symbolList** head, operationInfo* operationsArray, error** errorInfo){
    int i = 0, j=0;
    char tempVal[MAX_OPERAND_LENGTH] = {0}; /* Temp string for the offset value*/
    char tempOperand[MAX_OPERAND_LENGTH] = {0}; /* Temp string for the operand*/

    if (operand[0] == '#') { /* Immediate operand*/
        strcpy(tempOperand, operand+1); /* Copy the operand without the '#'*/
        if (!isValidInteger(tempOperand)){ /* Check if the token is a valid integer*/
            int symbolValue;
            if (!findSymbolValue(head, tempOperand, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                printError(errorInfo, "Invalid Integer or undefined symbol for immediate operand");
                return INSTRUCTION_FAIL_CODE; /* Return fail code, as the operand is not a valid integer*/
            }
        }
        return IMMEDIATE_ADDRESSING; /* Immediate operand*/
    }
    else if (isRegister(operand)) { /* Register operand*/
        return REGISTER_ADDRESSING; /* Register operand*/
    }
    else { /* case of Label or Label offset */
        trimWhitespace(operand); /* Remove leading and trailing whitespace*/

        /* Extract the label part before the '['*/
        while (operand[i] != '[' && operand[i] != '\0') {
            tempOperand[i] = operand[i];
            i++;
        }
        tempOperand[i] = '\0'; /* Null terminating the temp String*/

        /* Check if there's a space between the label and the offset, if there is, send error*/
        if (isspace((unsigned char)tempOperand[i-1]) && operand[i] == '['){
            printError(errorInfo, "Label with offset cannot have spaces between the label and the offset value");
            return INSTRUCTION_FAIL_CODE; /* Return fail code, as the label with offset has a space between the label and the '[' char*/
        }

        /* Check if it's just a label without offset*/
        if (operand[i] == '\0') {
            if (isValidLabelName(tempOperand, operationsArray, head, 0)){
                return DIRECT_ADDRESSING; /* Valid label without offset*/
            } else {
                printError(errorInfo, "Undefined Label name");
                return INSTRUCTION_FAIL_CODE;
            }
        }
        /* Handle label with offset*/
        else if (operand[i] == '[') {
            i++; /* Move past '['*/
            /* Extract the integer value inside the brackets*/
            while (operand[i] != ']' && operand[i] != '\0') {
                tempVal[j++] = operand[i++];
            }
            tempVal[j] = '\0'; /* Null terminating the temp String*/

            /* Validate closing bracket and that it's the end of the string*/
            if (operand[i] == ']' && operand[i + 1] == '\0') {
                /* Empty offset value is not valid*/
                if (strlen(tempVal) == 0) {
                    printError(errorInfo, "Empty offset value is not valid");
                    return INSTRUCTION_FAIL_CODE;
                }
                trimWhitespace(tempVal);
                /* Check if the offset is a valid integer or a defined symbol*/
                if (!isValidInteger(tempVal)) {
                    int symbolValue;
                    if (!findSymbolValue(head, tempVal, "define", &symbolValue)) {
                        printError(errorInfo, "Invalid Integer or undefined symbol for offset operand");
                        return INSTRUCTION_FAIL_CODE;
                    }
                }
                return OFFSET_ADDRESSING; /* Valid label with offset*/
            } else { /* Invalid offset declaration, doesn't end with ']'*/
                printError(errorInfo, "Invalid offset declaration, doesn't end with ']'");
                return INSTRUCTION_FAIL_CODE;
            }
        } else { /*undefined operand*/
            printError(errorInfo, "Invalid operand");
            return INSTRUCTION_FAIL_CODE;
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
 * @param IC - the instruction errorCounter
 * @param operationsArray - the array of operations
 * @param errorInfo - the error struct
 * @param isSecondPass - a flag to signal if it's the second pass, and use different actions accordingly
 * @return the length of the operation or INSTRUCTION_FAIL_CODE if the operation failed
 */
int handleOperation(symbolList** head, memoryWord* instructionArray, int opcode, char* line, int *IC, operationInfo *operationsArray, error** errorInfo, int isSecondPass) {
    int L = 0; /* The number of lines to add to the IC*/
    char* colon; /* Pointer to the colon in the line*/
    int firstOperand; /* The first operand code*/
    int secondOperand; /* The second operand code*/
    char operands[MAX_OPERANDS][MAX_OPERAND_LENGTH]; /* Array for the operands*/
    int numOfOperands = operationsArray[opcode].numOfOperands; /* The number of operands for the current operation*/
    initializeOperandsArray(operands); /* Initialize the operands array*/

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
        return INSTRUCTION_FAIL_CODE;
    }

    /* Checking the number of operands and their validity*/
    if (numOfOperands == ZERO_OPERANDS) {
        if (operands[0][0] != '\0') {
            printError(errorInfo, "Too many operands for a 0 operand operation");
            return INSTRUCTION_FAIL_CODE;
        }
        else{ /* No operands for a 0 operand operation*/
            firstOperand = NULL_OPERAND;
            secondOperand = NULL_OPERAND;
        }
    }
    else if (numOfOperands == ONE_OPERAND) {
        if (operands[1][0] != '\0') {
            printError(errorInfo, "Too many operands for a 1 operand operation");
            return INSTRUCTION_FAIL_CODE;
        }
        else if(operands[0][0] == '\0'){
            printError(errorInfo, "Too few operands for a 1 operand operation");
            return INSTRUCTION_FAIL_CODE;
        }
        else{
            firstOperand = NULL_OPERAND; /* No first operand for a 1 operand operation*/
            secondOperand = getOperandCode(operands[0], head, operationsArray, errorInfo); /* Get the operand code*/
            if (secondOperand == INSTRUCTION_FAIL_CODE){
                return INSTRUCTION_FAIL_CODE;
            }
        }
    }
    else if (numOfOperands == TWO_OPERANDS) {
        if (operands[2][0] != '\0') {
            printError(errorInfo, "Too many operands for a 2 operand operation");
            return INSTRUCTION_FAIL_CODE;
        }
        else if (operands[1][0] == '\0'){
            printError(errorInfo, "Too few operands for a 2 operand operation");
            return INSTRUCTION_FAIL_CODE;
        }
        else if (operands[0][0] == '\0'){
            printError(errorInfo, "Too few operands for a 2 operand operation");
            return INSTRUCTION_FAIL_CODE;
        }
        else{
            firstOperand = getOperandCode(operands[0], head, operationsArray, errorInfo); /* Get the first operand code*/
            if (firstOperand != INSTRUCTION_FAIL_CODE){
                secondOperand = getOperandCode(operands[1], head, operationsArray, errorInfo); /* Get the second operand code*/
            }
        }
    }

    /*Another test to check if anything failed*/
    if (firstOperand == INSTRUCTION_FAIL_CODE || secondOperand == INSTRUCTION_FAIL_CODE) {
        return INSTRUCTION_FAIL_CODE;
    }

    /*If there's offset for an operand, increment the line counter for each offset*/
    if (firstOperand == OFFSET_ADDRESSING){
        L++;
    }
    if (secondOperand == OFFSET_ADDRESSING){
        L++;
    }

    /*switch case for the different operations, with specific errors for each operation*/
    switch(opcode){
        case MOV_OPCODE: /*mov*/
            if (secondOperand == IMMEDIATE_ADDRESSING){
                printError(errorInfo, "Cannot move to an immediate operand");
                return INSTRUCTION_FAIL_CODE;
            }
            break;
        case CMP_OPCODE: /*cmp*/
            break;
        case ADD_OPCODE: /*add*/
            if (secondOperand == IMMEDIATE_ADDRESSING){
                printError(errorInfo, "Cannot add into an immediate operand");
                return INSTRUCTION_FAIL_CODE;
            }
            break;
        case SUB_OPCODE: /*sub*/
            if (secondOperand == IMMEDIATE_ADDRESSING){
                printError(errorInfo, "Cannot subtract into an immediate operand");
                return INSTRUCTION_FAIL_CODE;
            }
            break;
        case NOT_OPCODE: /*not*/
            if (secondOperand == IMMEDIATE_ADDRESSING){
                printError(errorInfo, "Cannot negate an immediate operand");
                return INSTRUCTION_FAIL_CODE;
            }
            break;
        case CLR_OPCODE: /*clr*/
            if (secondOperand == IMMEDIATE_ADDRESSING){
                printError(errorInfo, "Cannot clear an immediate operand");
                return INSTRUCTION_FAIL_CODE;
            }
            break;
        case LEA_OPCODE: /*lea*/
            if (firstOperand == REGISTER_ADDRESSING || firstOperand == IMMEDIATE_ADDRESSING || secondOperand == IMMEDIATE_ADDRESSING){
                printError(errorInfo, "Invalid operands for lea operation");
                return INSTRUCTION_FAIL_CODE;
            }
            break;
        case INC_OPCODE: /*inc*/
            if (secondOperand == IMMEDIATE_ADDRESSING){
                printError(errorInfo, "Cannot increment an immediate operand");
                return INSTRUCTION_FAIL_CODE;
            }
            break;
        case DEC_OPCODE: /*dec*/
            if (secondOperand == IMMEDIATE_ADDRESSING){
                printError(errorInfo, "Cannot decrement an immediate operand");
                return INSTRUCTION_FAIL_CODE;
            }
            break;
        case JMP_OPCODE: /*jmp*/
            if (secondOperand == IMMEDIATE_ADDRESSING || secondOperand == OFFSET_ADDRESSING){
                printError(errorInfo, "Invalid operands for jmp");
                return INSTRUCTION_FAIL_CODE;
            }
            break;
        case BNE_OPCODE: /*bne*/
            if (secondOperand == IMMEDIATE_ADDRESSING || secondOperand == OFFSET_ADDRESSING){
                printError(errorInfo, "Invalid operands for bne");
                return INSTRUCTION_FAIL_CODE;
            }
            break;
        case RED_OPCODE: /*red*/
            if (secondOperand == IMMEDIATE_ADDRESSING){
                printError(errorInfo, "Cannot read into an immediate operand");
                return INSTRUCTION_FAIL_CODE;
            }
            break;
        case PRN_OPCODE: /*prn*/
            break;
        case JSR_OPCODE: /*jsr*/
            if (secondOperand == IMMEDIATE_ADDRESSING || secondOperand == OFFSET_ADDRESSING){
                printError(errorInfo, "Invalid operands for jsr");
                return INSTRUCTION_FAIL_CODE;
            }
            break;
        case RTS_OPCODE: /*rts*/
            break;
        case HLT_OPCODE: /*hlt*/
            break;
        default:
            printError(errorInfo, "Invalid operation");
            return INSTRUCTION_FAIL_CODE;
    }

    /*Special case where both operands are registers, sharing one binary word*/
    if (firstOperand == REGISTER_ADDRESSING &&  secondOperand == REGISTER_ADDRESSING){
        L--; /* Decrement the line counter, as the 2 operands are sharing one binary word*/
    }

    L += operationsArray[opcode].numOfOperands + 1; /* The number of operands + the operation line itself*/

    if (!isSecondPass){ /*If it's the first pass, only add the first word and update the IC*/
        insertFirstInstructionIntoArray(instructionArray, *IC, opcode, firstOperand, secondOperand);
    }
    else if (numOfOperands != ZERO_OPERANDS){ /*If it's the 2nd pass and there are operands(it's not a 0 operand operation like hlt)*/
        analyzeOperandsAndInsertIntoArraySecondPass(instructionArray, L, IC, operands, head, errorInfo); /*Analyze the operands and insert them into the instruction array*/
    }

    return L; /* Return the number of lines to add to the IC*/
}

/**
 * This function handles the .data and .string directives, and adds their data to the data array and to the symbol table.
 * @param type - the type of directive (data or string)
 * @param line - the current line text
 * @param head - the symbol table
 * @param DC - the data errorCounter
 * @param dataArray - the array of data
 * @param errorInfo - the error struct
 */
void handleData(char* type, char* labelName, char* line, symbolList** head, int *DC, memoryWord* dataArray, error** errorInfo) {
    symbolList *tempPtr = NULL; /* Temporary pointer for the symbol list*/
    char* numbers; /* Pointer to the numbers in the line*/
    char* ptr; /* Pointer for iterating through the line*/
    char* token; /* Token for the strtok function*/
    char* lastNonWhitespaceChar; /* Pointer to the last non-whitespace character in the line*/
    long val; /* The value of the current number*/
    short dataCounter = 0; /* Counter for the number of data values*/
    char copiedLine[MAX_CHARS_PER_LINE]; /* Copy of the line for further processing*/
    if (strcmp(type, "data") == 0) { /* If it's a .data directive*/
        numbers = strstr(line, ".data");  /* Find the .data in the line*/
        numbers += strlen(".data"); /* Move past ".data" */
        while (isspace((unsigned char)*numbers)) numbers++; /* Skip whitespace */

        if (*numbers == '\n' || *numbers == '\0') { /* If there are no values after .data*/
            printError(errorInfo, "No values found after .data.");
            return;
        }

        /* Initially check for a trailing comma and consecutive commas */
        ptr = numbers; /* Set the pointer to the numbers*/
        lastNonWhitespaceChar = NULL;
        while (*ptr) { /* Iterate through the line*/
            if (*ptr == ',' && (!lastNonWhitespaceChar || *lastNonWhitespaceChar == ',')) { /* If there are consecutive commas or a trailing comma*/
                printError(errorInfo, "Consecutive commas or trailing comma detected.");
                return; /* Return, as the line is invalid*/
            }
            if (!isspace((unsigned char)*ptr)) { /* If the character is not whitespace*/
                lastNonWhitespaceChar = ptr; /* Set the last non-whitespace character*/
            }
            ptr++;
        }

        /* If the last character before any trailing whitespace was a comma, it's a trailing comma */
        if (lastNonWhitespaceChar && *lastNonWhitespaceChar == ',') {
            printError(errorInfo, "Trailing comma detected.");
            return;
        }

        if (labelName) { /* If there is a label*/
            /* Find the symbol in the symbol list, (if it exists) */
            tempPtr = getPointerToSymbol(head, labelName);
        }

        /* Initial tests succeeded, Tokenization and value processing */
        token = strtok(numbers, ",");
        while (token) { /* Iterate through the tokens*/
            if (dataCounter >= MAX_VALUES_PER_DATA_DIRECTIVE) { /* If there are too many data values (more than 36)*/
                printError(errorInfo, "Too many data values inserted into the .data directive.");
                return;
            }

            trimWhitespace(token); /* Clean up token*/

            if (!isValidInteger(token)) { /* If the token is not a valid integer*/
                int symbolValue;
                if (!findSymbolValue(head, token, "define", &symbolValue)) {
                    printError(errorInfo, "Invalid integer or undefined symbol.");
                    return; /* Return, as the token is not a valid integer*/
                }
                val = symbolValue; /* Use the value from the symbol list*/
            } else {
                val = atoi(token);
            }

            convertValueToBinaryAndInsertToDataArray(dataArray, *DC, val); /* Convert the value to binary and insert it into the data array*/
            (*DC)++;
            dataCounter++;
            if (tempPtr) { /* If there is a label*/
                tempPtr->dataCounter++; /* Increment the data counter for the symbol*/
            }

            token = strtok(NULL, ",");
        }
    }
    else if (strcmp(type, "string")==0) { /* If it's a .string directive*/
        char* stringPosition = strstr(line, ".string"); /* Find the .string in the line*/
        int i; /* Iterator for the loop*/

        /* Copy the line content following the .string declaration.*/
        strcpy(copiedLine, stringPosition + strlen(".string") + 1);
        if (copiedLine[0] == '\0') { /* If there are no values after .string*/
            printError(errorInfo, "No valid string was found after .string");
            return;
        }

        if (labelName) { /* If there is a label*/
            /* Find the symbol in the symbol list, (if it exists) */
            tempPtr = getPointerToSymbol(head, labelName);
        }

        trimWhitespace(copiedLine); /* Remove leading and trailing whitespace*/
        if (strlen(copiedLine) > 2 && copiedLine[0] == '"' && copiedLine[strlen(copiedLine)-1] == '"') {
            for (i = 1; i < strlen(copiedLine)-1; i++) {
                if (isprint(copiedLine[i])){ /* If the character is printable*/
                    convertValueToBinaryAndInsertToDataArray(dataArray, *DC, copiedLine[i]); /* Convert the character to binary and insert it into the data array*/
                    (*DC)++; /* Increment the DC*/
                    if (tempPtr){
                        tempPtr->dataCounter++; /* Increment the char counter for the string symbol*/
                    }
                }
                else{
                    printError(errorInfo, "Invalid ASCII character in string"); /* If the character is not printable, print an error*/
                    return;
                }
            }
            convertValueToBinaryAndInsertToDataArray(dataArray, *DC, '\0'); /* Insert the null terminator into the data array*/
            (*DC)++; /* Increment the DC, but don't increment the data counter for the symbol (so we won't be able to access the \0 character)*/
        }
        else { /* If the string is not enclosed in double quotes or is empty*/
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
    char* currentWord; /* The current word in the line*/
    int flag = FALSE; /* Flag to check if the first label was already added*/

    if(labelFlag){ /* If the first word is a label, ignore it and print a warning*/
        currentWord = strtok(line, " \n\r\t"); /* Get the next word.*/
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
        printf("WARNING: Label is ignored in .extern directive, in line '%s'\n", (*errorInfo)->currentLineContent);
    }
    else{
        currentWord = strtok(line, " \n\r\t"); /* Get the next word.*/
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
    }

    if (!currentWord){  /* If there are no symbols after .extern*/
        printError(errorInfo, "No symbols found after .extern");
        return;
    }

    while (currentWord!= NULL) { /* Loop through the words in the line*/
        if (flag){ /* If the first label was already added, print an error because there are extraneous labels*/
            printError(errorInfo, "Extraneous text after First label of .extern");
            return;
        }
        else if (!isValidLabelName(currentWord, operationsArray, head, 0)) { /* If the label is not valid*/
            printError(errorInfo, "Not a valid .extern symbol name");
            return;
        }
        else { /* If the label is valid, add it to the symbol table*/
            addLabel(head, currentWord, "external", 0, errorInfo);
            flag = TRUE;
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
    char name[MAX_LABEL_NAME] = {0}; /* The name of the label*/
    int value = 0; /* The value of the .define directive*/
    int tempValue = 0; /* Temporary value for the .define directive*/
    char* ptr = line; /* Pointer to the line*/
    char* startName; /* Pointer to the start of the name*/
    int nameLength; /* The length of the name*/
    char* equalPos; /* Pointer to the position of the '='*/
    char* tempPtr; /* Temporary pointer for iterating through the line*/
    int equalCounter = 0; /* Counter for the number of '='*/

    /* Skip past ".define" and any whitespace that immediately follows */
    while (isspace((unsigned char)*ptr)) ptr++;
    ptr += strlen(".define");
    while (*ptr == ' ' || *ptr == '\t') ptr++;

    /* Now, 'ptr' should be at the start of the name */
    startName = ptr;
    equalPos = strchr(ptr, '='); /* Find the position of '=' */
    if (!equalPos) { /* If there's no '=' */
        printError(errorInfo, "Missing '=' in .define command");
        return;
    }

    /* Check for multiple '=' */
    tempPtr = equalPos + 1;
    while (*tempPtr) {
        if (*tempPtr == '=') {
            equalCounter++;
        }
        tempPtr++;
    }
    if (equalCounter > 0) { /*Meaning we've found another '=' after the first one*/
        printError(errorInfo, "Multiple '=' found in .define command");
        return;
    }

    /* Calculate name length, ensuring name doesn't include spaces leading up to '=' */
    nameLength = equalPos - startName;
    while (nameLength > 0 && (startName[nameLength - 1] == ' ' || startName[nameLength - 1] == '\t')) {
        nameLength--; /* Exclude trailing spaces or tabs from the name */
    }

    if (nameLength > MAX_LABEL_NAME){ /* If the name is too long*/
        printError(errorInfo, "Label name too long for .define");
        return;
    }
    strncpy(name, startName, nameLength);
    name[nameLength] = '\0'; /* Ensure null-termination */

    /* Check if the .define directive was already defined */
    findSymbolValue(head, name, "define", &tempValue); /* Trying to find the value (if it exists) */
    if (tempValue != 0){ /* If there's a value that was already initialized for the define label*/
        /* Move 'ptr' past '=' and optional whitespace */
        ptr = equalPos + 1;
        while (*ptr == ' ' || *ptr == '\t') ptr++; /* Skip spaces or tabs after '=' */
        trimWhitespace(ptr); /* Remove leading and trailing whitespace*/

        /* 'ptr' should now be at the start of the value */
        if (!isValidInteger(ptr)) { /* If the value is not a valid integer*/
            printError(errorInfo, "Invalid integer or undefined symbol for '.define'");
            return;
        }
        value = atoi(ptr);

        if (value == tempValue) { /* If the value is the same as the one that was already defined, print a warning*/
            printf("WARNING: Double declaration- Label '%s' was already defined as .define with the same value\n", name);
            return;
        }
        else{ /* If the define directive was initialized twice with different values, print an error*/
            printError(errorInfo, "Double declaration- Label was already defined as .define with a different value");
            return;
        }
    }
    else { /* If the define label wasn't found in the symbol table*/
        /* Check validity of the name */
        if (!isValidLabelName(name, operationsArray, head, 0)) {
            printError(errorInfo, "Invalid .define symbol name");
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
    }
    /* If all checks pass, add the label */
    addLabel(head, name, "define", value, errorInfo);
}


/**
 * This function checks the syntax of the .entry directive, as well as if it's been already been defined as .extern.
 * It does not add the label to the symbol table, as it's only used for checking the syntax.
 * @param head The symbol list.
 * @param line The line to be checked.
 * @param errorInfo The error struct.
 * @param operationsArray The operations array.
 */
void checkEntrySyntax(symbolList** head, char* line, error** errorInfo, operationInfo* operationsArray, int labelFlag) {
    char* currentWord; /* The current word in the line.*/
    int flag = FALSE; /* Flag to check if the first label was already added.*/

    if(labelFlag){ /* If the first word is a label, ignore it and print a warning.*/
        currentWord = strtok(line, " \n\r\t"); /* Get the next word.*/
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
        printf("WARNING: Label is ignored in .entry directive, in line '%s'\n", (*errorInfo)->currentLineContent);    }
    else{
        currentWord = strtok(line, " \n\r\t"); /* Get the next word.*/
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
    }

    if (!currentWord){ /* If there are no symbols after .entry*/
        printError(errorInfo, "No symbols found after .entry");
        return;
    }

    while (currentWord!= NULL) {
        if(flag){ /* If the first label was already added, print an error because there are extraneous labels*/
            printError(errorInfo, "Extraneous text after Label definition for .entry");
            return;
        }
        else if (!isValidLabelName(currentWord, operationsArray, head, 0)) { /* If the label is not valid*/
            printError(errorInfo, "Not a valid .entry symbol name");
            return;
        }
        else if (searchSymbolList(head, currentWord, "extern") == 0) { /* If the label is defined as .extern*/
            printError(errorInfo, "Invalid .entry symbol name, it's already defined as .extern");
            return;
        }
        else{
            flag = TRUE; /* If the label is valid, set the flag to 1*/
        }
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
    }
}


/**
 * This function parses the operands in the first pass of the assembler.
 * @param line The line to be parsed.
 * @param operands The array to store the operands.
 * @param errorInfo The error struct.
 * @return true if the parsing was successful, false otherwise.
 */
int parseOperandsFirstPass(char* line, char operands[MAX_OPERANDS][MAX_OPERAND_LENGTH], error** errorInfo) {
    char* token; /* The current token*/
    int operandIndex = 0; /* The index of the current operand*/
    char* nextToken; /* The next token*/

    /* Check for invalid syntax: comma before or after operands, or two consecutive commas */
    if (line[0] == ',' || strstr(line, ",,") != NULL || line[strlen(line) - 1] == ',') {
        printError(errorInfo, "Invalid syntax: comma before or after operands, or two consecutive commas found.");
        return FALSE;
    }

    token = strtok(line, ",");
    while (token != NULL && operandIndex < MAX_OPERANDS) { /* Iterate through the tokens*/
        trimWhitespace(token); /* Remove leading and trailing whitespace */

        /* Check for an empty token, indicating " , ," pattern */
        if (token[0] == '\0') {
            printError(errorInfo, "Invalid syntax: extraneous text after operands");
            return FALSE; /* Early return on finding an empty operand */
        }

        nextToken = strtok(NULL, ",");
        if (operandIndex == MAX_OPERANDS - 1 && nextToken != NULL) { /* If there are more than 2 operands*/
            printError(errorInfo, "Invalid syntax: too many operands provided.");
            return FALSE; /* Early return if there's content after the second operand */
        }

        strncpy(operands[operandIndex], token, MAX_OPERAND_LENGTH - 1); /* Copy the token to the operands array*/
        operands[operandIndex][MAX_OPERAND_LENGTH - 1] = '\0'; /* Ensure null termination */

        operandIndex++; /* Increment the operand index */
        token = nextToken; /* Proceed to the next token */
    }
    return TRUE; /* Return true if the parsing was successful */
}

/**
 * This function parses the operands in the second pass of the assembler.
 * @param operand The operand to be parsed.
 * @param outOperand The output operand.
 * @param outLabelOrDefine The output label or define.
 */
void parseOperandsSecondPass(const char* operand, char* outOperand, char* outLabelOrDefine) {
    char* bracketPos; /* The position of the bracket*/
    char* endBracketPos; /* The position of the end bracket*/

    if (operand == NULL) { /* If the operand is NULL*/
        outOperand[0] = '\0'; /* Indicate empty string*/
        outLabelOrDefine[0] = '\0'; /* Indicate empty string*/
        return;
    }

    /* Find the first occurrence of '[' which might indicate a label or define */
    bracketPos = strchr(operand, '[');

    if (bracketPos == NULL) {
        /* No bracket found, the entire operand is just the operand */
        strncpy(outOperand, operand, MAX_OPERAND_LENGTH - 1);
        outOperand[MAX_OPERAND_LENGTH - 1] = '\0'; /* Ensure null-termination*/
        outLabelOrDefine[0] = '\0'; /* Indicate empty string*/
    } else {
        /* Extract operand up to the bracket */
        int operandLen = bracketPos - operand;
        operandLen = (operandLen < MAX_OPERAND_LENGTH - 1) ? operandLen : MAX_OPERAND_LENGTH - 1;
        strncpy(outOperand, operand, operandLen);
        outOperand[operandLen] = '\0'; /* Null-terminate*/

        /* Check if there's a corresponding closing ']' */
        endBracketPos = strchr(bracketPos, ']');
        if (endBracketPos != NULL) {
            int labelLen = endBracketPos - bracketPos - 1;
            labelLen = (labelLen < MAX_OPERAND_LENGTH - 1) ? labelLen : MAX_OPERAND_LENGTH - 1;
            strncpy(outLabelOrDefine, bracketPos + 1, labelLen);
            outLabelOrDefine[labelLen] = '\0'; /* Null-terminate*/
        } else {
            outLabelOrDefine[0] = '\0'; /* Malformed operand, indicate empty string*/
        }
    }
}


/**
 * This function trims the whitespace before and after the string.
 * @param str The string to be trimmed.
 */
void trimWhitespace(char* str) {
    int start = 0, end = 0;
    int i, length;

    /* Trim leading space */
    while (isspace((unsigned char)str[start])) start++;

    /* Find end of string */
    length = strlen(str);
    end = length - 1;

    /* Trim trailing space */
    while (end > start && isspace((unsigned char)str[end])) end--; /* Find the last non-whitespace character */

    /* Shift everything to the left */
    length = end - start + 1;
    for (i = 0; i < length; i++) {
        str[i] = str[i + start];
    }
    str[i] = '\0';  /* Null-terminate the modified string*/
}

/**
 * This function checks if the word is a .define directive.
 * @param word The word to be checked.
 * @return true if the word is a .define directive, false otherwise.
 */
int isDefine(char* word) {
    if (strcmp(word, ".define") == 0) {
        return TRUE;
    }
    return FALSE;
}

/**
 * This function checks if the word is a .data directive.
 * @param word The word to be checked.
 * @return true if the word is a .data directive, false otherwise.
 */
int isData(char* word) {
    if (strcmp(word, ".data") == 0) {
        return TRUE;
    }
    return FALSE;
}

/**
 * This function checks if the word is a .string directive.
 * @param word The word to be checked.
 * @return true if the word is a .string directive, false otherwise.
 */
int isString(char* word) {
    if (strcmp(word, ".string") == 0) {
        return TRUE;
    }
    return FALSE;
}

/**
 * This function checks if the word is a .extern directive.
 * @param word The word to be checked.
 * @return true if the word is a .extern directive, false otherwise.
 */
int isExtern(char* word) {
    if (strcmp(word, ".extern") == 0) {
        return TRUE;
    }
    return FALSE;
}

/**
 * This function checks if the word is a .entry directive.
 * @param word The word to be checked.
 * @return true if the word is a .entry directive, false otherwise.
 */
int isEntry(char* word) {
    if (strcmp(word, ".entry") == 0) {
        return TRUE;
    }
    return FALSE;
}

/**
 * This function checks if the label name is valid.
 * The rules are:
 * Not a reserved word, not a register, not a number, not a defined symbol, not a label that already exists.
 * As well as the first character being an alphabetical character, and the rest of the characters being either uppercase or digits.
 * And the name being 31 characters at most.
 * @param name The label name to be checked.
 * @param operationsArray The operations array.
 * @param head The symbol list.
 * @param colonFlag A flag indicating if the label has a colon.
 * @return true if the label name is valid, false otherwise.
 */
int isValidLabelName(char* name, operationInfo* operationsArray, symbolList** head, int colonFlag){
    int i;
    char lastChar = name[strlen(name) - 1];

    /* Check if the label has a colon at the end */
    if (colonFlag && lastChar != ':') { /* If the label is supposed to have a colon, but the last character is not a colon*/
        return FALSE; /* Return FALSE, as the label is invalid*/
    }
    else if (colonFlag){ /*Remove the colon for testing!*/
        name[strlen(name) - 1] = '\0';
    }

    /* Check if the name is bigger than the maximum possible number (31), or if it's one of the operations*/
    if (strlen(name) > MAX_LABEL_NAME || isValidOperation(name, operationsArray) != INVALID_OPERATION_CODE){
        return FALSE;
    }
    /*check if the name is one of the registers or if it's "mcr/endmcr" keywords*/
    if (strcmp(name, "r0") == 0 || strcmp(name, "r1") == 0 || strcmp(name, "r2") == 0 || strcmp(name, "r3") == 0 || strcmp(name, "r4") == 0 || strcmp(name, "r5") == 0 || strcmp(name, "r6") == 0 || strcmp(name, "r7") == 0 || strcmp(name, "mcr")==0 || strcmp(name, "endmcr")==0){
        return FALSE;
    }
    /* Check if the first character is an alphabetical character */
    if (!isalpha(name[0])) {
        return FALSE;
    }

    /* Check if the rest of the characters are either uppercase or digits */
    for (i = 1; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && !isdigit(name[i])) {
            return FALSE;
        }
        if (isspace(name[i])){
            return FALSE;
        }
    }

    /* If all checks pass, the string is valid */
    return TRUE;
}

/**
 * This function checks if the word is a valid operation.
 * @param word The word to be checked.
 * @param operationsArray The operations array.
 * @return The index of the operation in the operations array if the word is a valid operation, -1 otherwise.
 */
int isValidOperation(char* word, operationInfo* operationsArray) {
    int i;
    for (i = 0; i < NUM_OF_OPERATIONS; i++) { /* Iterate through the operations array*/
        if (strcmp(word, operationsArray[i].name) == 0) {
            return i; /* Return the index of the operation if it's found*/
        }
    }
    return INVALID_OPERATION_CODE; /* Return -1 if the operation is not found*/
}


/**
 * This function checks if the string is a valid integer.
 * @param str The string to be checked.
 * @return true if the string is a valid integer, false otherwise.
 */
int isValidInteger(char* str) {
    int i;
    int len = strlen(str); /* Get the length of the string*/
    int start = 0;  /* The start index of the string*/
    int temp; /* Temporary variable for the integer value*/

    /* Check if the string starts with '+' or '-'*/
    if (str[0] == '+' || str[0] == '-') {
        if (len == 1) {
            /* If the string is only '+' or '-', it's not a valid integer*/
            return FALSE; /* Return false, as the string is not a valid integer*/
        }
        start = 1; /* Move the start index to the next character*/
    }

    /* Check if the rest of the characters are digits*/
    for (i = start; i < len; i++) { /* Iterate through the string*/
        if (!isdigit(str[i])) { /* If the character is not a digit*/
            return FALSE; /* Return false, as the string is not a valid integer*/
        }
    }

    /* Convert the string to an integer */
    temp = atoi(str);

    /* Check if the number is greater than the maximum 12-bit integer or less than the minimum 12-bit integer */
    if (temp > MAX_INTEGER_12BIT || temp < MIN_INTEGER_12BIT) {
        return FALSE; /* Return false, as the string is not a valid integer*/
    }

    /* If all checks pass, the string is a valid integer*/
    return TRUE;
}

/**
 * This function checks if the word is a register.
 * @param word The word to be checked.
 * @return true if the word is a register, false otherwise.
 */
int isRegister(char* word){
    if (strcmp(word, "r0") == 0 || strcmp(word, "r1") == 0 || strcmp(word, "r2") == 0 || strcmp(word, "r3") == 0 || strcmp(word, "r4") == 0 || strcmp(word, "r5") == 0 || strcmp(word, "r6") == 0 || strcmp(word, "r7") == 0 ){
        return TRUE;
    }
    return FALSE;
}

/**
 * This function checks if the current line exceeds the maximum allowed length.
 * If so, it consumes the rest of the line and prints an appropriate error.
 * Returns 0 if the line is too long (and was skipped), or 1 if the line length is valid.
 */
int checkLineLengthAndSkip(FILE* sourceFile, char* lineBuffer, error** errorInfo) {
    int ch; /* The current character*/
    size_t lineLen = strlen(lineBuffer); /* The length of the line*/
    if (lineBuffer[lineLen - 1] != '\n' && !feof(sourceFile)) {
        /* Line is longer than the buffer, so it's considered too long */
        printError(errorInfo, "Line exceeds the maximum allowed length.");
        /* Consume the rest of the line */
        while ((ch = fgetc(sourceFile)) != '\n' && ch != EOF);
        return FALSE; /* Indicate that the line is too long and was skipped */
    }
    return TRUE; /* Line is within the acceptable length */
}


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
 * 4. handleExtern - This function handles the .extern directive, and adds the label to the symbol table if it's valid.
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
 * 19. analyzeOperandsAndInsertIntoArraySecondPass - This function analyzes the operands and inserts them into the instruction array in the second pass.
 * 20. checkLineLengthAndSkip - This function checks the length of the line and skips it if it's too long.
 */

#include "include/syntaxAnalyzer.h"


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
    char tempVal[MAXOPERANDLENGTH] = {0};
    char tempOperand[MAXOPERANDLENGTH] = {0};

    if (operand[0] == '#') {
        strcpy(tempOperand, operand+1);

        if (!isValidInteger(tempOperand)){
            int symbolValue;
            if (!findSymbolValue(head, tempOperand, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                printError(errorInfo, "Invalid Integer or undefined symbol for immediate operand");
                return INSTRUCTIONFAILCODE;
            }
        }
        return 0;
    }
    else if (isRegister(operand)) {
        return 3;
    }
    else { /* case of Label or Label offset */
        trimWhitespace(operand);

        /* Extract the label part before the '['*/
        while (operand[i] != '[' && operand[i] != '\0') {
            tempOperand[i] = operand[i];
            i++;
        }
        tempOperand[i] = '\0'; /* Null terminating the temp String*/

        /* Check if there's a space between the label and the offset, if there is, send error*/
        if (isspace((unsigned char)tempOperand[i-1]) && operand[i] == '['){
            printError(errorInfo, "Label with offset cannot have spaces between the label and the offset value");
            return INSTRUCTIONFAILCODE;
        }

        /* Check if it's just a label without offset*/
        if (operand[i] == '\0') {
            if (isValidLabelName(tempOperand, operationsArray, head, 0)){
                return 1; /* Valid label without offset*/
            } else {
                printError(errorInfo, "Undefined Label name");
                return INSTRUCTIONFAILCODE;
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
                    return INSTRUCTIONFAILCODE;
                }
                trimWhitespace(tempVal);
                /* Check if the offset is a valid integer or a defined symbol*/
                if (!isValidInteger(tempVal)) {
                    int symbolValue;
                    if (!findSymbolValue(head, tempVal, "define", &symbolValue)) {
                        printError(errorInfo, "Invalid Integer or undefined symbol for offset operand");
                        return INSTRUCTIONFAILCODE;
                    }
                }
                return 2; /* Valid label with offset*/
            } else {
                printError(errorInfo, "Invalid offset declaration, doesn't end with ']'");
                return INSTRUCTIONFAILCODE;
            }
        } else {
            printError(errorInfo, "Invalid operand");
            return INSTRUCTIONFAILCODE;
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
    else if (operationsArray[opcode].numOfOperands != 0){ /*If it's the 2nd pass and there are operands(it's not a 0 operand operation like hlt)*/
        analyzeOperandsAndInsertIntoArraySecondPass(instructionArray, L, IC, operands, head, errorInfo);
    }

    return L;
}

/**
 * This function handles the .data and .string directives, and adds the data to the data array
 * @param type - the type of directive (data or string)
 * @param line - the current line text
 * @param head - the symbol table
 * @param DC - the data errorCounter
 * @param dataArray - the array of data
 * @param errorInfo - the error struct
 */
void handleData(char* type, char* line, symbolList** head, int *DC, binaryWord* dataArray, error** errorInfo) {
    char* numbers;
    char* ptr;
    char* token;
    char* lastNonWhitespaceChar;
    long val;
    short dataCounter = 0;
    char copiedLine[MAXCHARSPERLINE];
    if (strcmp(type, "data") == 0) {
        numbers = strstr(line, ".data");
        numbers += strlen(".data"); /* Move past ".data" */
        while (isspace((unsigned char)*numbers)) numbers++; /* Skip whitespace */

        if (*numbers == '\n' || *numbers == '\0') {
            printError(errorInfo, "No values found after .data.");
            return;
        }

        /* Initially check for a trailing comma and consecutive commas */
        ptr = numbers;
        lastNonWhitespaceChar = NULL;
        while (*ptr) {
            if (*ptr == ',' && (!lastNonWhitespaceChar || *lastNonWhitespaceChar == ',')) {
                printError(errorInfo, "Consecutive commas or trailing comma detected.");
                return;
            }
            if (!isspace((unsigned char)*ptr)) {
                lastNonWhitespaceChar = ptr;
            }
            ptr++;
        }

        /* If the last character before any trailing whitespace was a comma, it's a trailing comma */
        if (lastNonWhitespaceChar && *lastNonWhitespaceChar == ',') {
            printError(errorInfo, "Trailing comma detected.");
            return;
        }

        /* Tokenization and value processing */
        token = strtok(numbers, ",");
        while (token) {
            if (dataCounter >= MAXDATAVALUESINARRAY) {
                printError(errorInfo, "Too many data values inserted.");
                return;
            }

            trimWhitespace(token); /* Clean up token*/

            if (!isValidInteger(token)) {
                int symbolValue;
                if (!findSymbolValue(head, token, "define", &symbolValue)) {
                    printError(errorInfo, "Invalid integer or undefined symbol.");
                    return;
                }
                val = symbolValue; /* Use the value from the symbol list*/
            } else {
                val = atoi(token);
            }

            convertValueToBinaryAndInsertToDataArray(dataArray, *DC, val);
            (*DC)++;
            dataCounter++;

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
                    convertValueToBinaryAndInsertToDataArray(dataArray, *DC, copiedLine[i]);
                    (*DC)++;
                }
                else{
                    printError(errorInfo, "Invalid ASCII character in string");
                    return;
                }
            }
            convertValueToBinaryAndInsertToDataArray(dataArray, *DC, '\0');
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
        printf("WARNING: Label is ignored in .extern directive, in line '%s'\n", (*errorInfo)->lineText);
    }
    else{
        currentWord = strtok(line, " \n\r\t"); /* Get the next word.*/
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
    }

    if (!currentWord){
        printError(errorInfo, "No symbols found after .extern");
        return;
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
    int tempValue = 0;
    char* ptr = line;
    char* startName;
    int nameLength;
    char* equalPos;
    char* tempPtr;
    int equalCounter = 0;

    /* Skip past ".define" and any whitespace that immediately follows */
    while (isspace((unsigned char)*ptr)) ptr++;
    ptr += strlen(".define");
    while (*ptr == ' ' || *ptr == '\t') ptr++;

    /* Now, 'ptr' should be at the start of the name */
    startName = ptr;
    equalPos = strchr(ptr, '='); /* Find the position of '=' */
    if (!equalPos) {
        printError(errorInfo, "Missing '=' in .define statement");
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
        printError(errorInfo, "Multiple '=' found in .define statement");
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


    findSymbolValue(head, name, "define", &tempValue);
    if (tempValue != 0){
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

        if (value == tempValue) {
            printf("WARNING: Double declaration- Label '%s' was already defined as .define with the same value\n", name);
            return;
        }
        else{
            printError(errorInfo, "Double declaration- Label was already defined as .define with a different value");
            return;
        }
    }
    else { /* If the define label wasn't found in the symbol table*/
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
    }

    /* If all checks pass, add the label */
    addLabel(head, name, "define", value, errorInfo);
}


/**
 * This function checks the syntax of the .entry directive, as well as if it's been already been defined as .extern.
 * @param head The symbol list.
 * @param line The line to be checked.
 * @param errorInfo The error struct.
 * @param operationsArray The operations array.
 */
void checkEntrySyntax(symbolList** head, char* line, error** errorInfo, operationInfo* operationsArray, int labelFlag) {
    char* currentWord;
    int flag = 0;

    if(labelFlag){
        currentWord = strtok(line, " \n\r\t"); /* Get the next word.*/
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
        printf("WARNING: Label is ignored in .entry directive, in line '%s'\n", (*errorInfo)->lineText);    }
    else{
        currentWord = strtok(line, " \n\r\t"); /* Get the next word.*/
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
    }

    if (!currentWord){
        printError(errorInfo, "No symbols found after .entry");
        return;
    }

    while (currentWord!= NULL) {
        if(flag){
            printError(errorInfo, "Extraneous text after Label definition for .entry");
            return;
        }
        else if (!isValidLabelName(currentWord, operationsArray, head, 0)) {
            printError(errorInfo, "Not a valid .entry symbol name");
            return;
        }
        else if (searchSymbolList(head, currentWord, "extern") == 0) {
            printError(errorInfo, "Invalid .entry symbol name, it's already defined as .extern");
            return;
        }
        else{
            flag = 1;
        }
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
    }
}


/**
 * This function parses the operands in the first pass of the assembler.
 * @param line The line to be parsed.
 * @param operands The array to store the operands.
 * @param errorInfo The error struct.
 * @return 1 if the parsing was successful, 0 otherwise.
 */
int parseOperandsFirstPass(char* line, char operands[MAXOPERANDS][MAXOPERANDLENGTH], error** errorInfo) {
    char* token;
    int operandIndex = 0;
    char* nextToken;

    if (line[0] == ',' || strstr(line, ",,") != NULL || line[strlen(line) - 1] == ',') {
        printError(errorInfo, "Invalid syntax: comma before or after operands, or two consecutive commas found.");
        return 0;
    }

    token = strtok(line, ",");
    while (token != NULL && operandIndex < MAXOPERANDS) {
        trimWhitespace(token); /* Remove leading and trailing whitespace */

        /* Check for an empty token, indicating " , ," pattern */
        if (token[0] == '\0') {
            printError(errorInfo, "Invalid syntax: extraneous text after operands");
            return 0; /* Early return on finding an empty operand */
        }

        nextToken = strtok(NULL, ",");
        if (operandIndex == MAXOPERANDS - 1 && nextToken != NULL) {
            printError(errorInfo, "Invalid syntax: too many operands provided.");
            printf("Invalid syntax: too many operands provided.\n");
            return 0; /* Early return if there's content after the second operand */
        }

        strncpy(operands[operandIndex], token, MAXOPERANDLENGTH - 1);
        operands[operandIndex][MAXOPERANDLENGTH - 1] = '\0'; /* Ensure null termination */

        operandIndex++;
        token = nextToken; /* Proceed to the next token */
    }
    return 1;
}


/**
 * This function inserts the operands into the instruction array, calling the convertOperandToBinaryAndInsertIntoArray function for each operand.
 *
 * @param instructionArray The instruction array to insert the operands into.
 * @param numOfLines The number of lines in the file.
 * @param IC The instruction errorCounter.
 * @param operands The operands to insert into the instruction array.
 * @param head The head of the symbol table.
 * @param errorInfo A pointer to the errorInfo struct.
 */
void analyzeOperandsAndInsertIntoArraySecondPass(binaryWord* instructionArray, int numOfLines, int *IC, char operands[MAXOPERANDS][MAXOPERANDLENGTH], symbolList** head, error** errorInfo){
    binaryWord newWord;
    int regNumSource, regNumDest;

    char firstOperand[MAXOPERANDLENGTH];
    char firstOffset[MAXOPERANDLENGTH];
    char secondOperand[MAXOPERANDLENGTH];
    char secondOffset[MAXOPERANDLENGTH];
    firstOperand[0] = '\0';
    firstOffset[0] = '\0';
    secondOperand[0] = '\0';
    secondOffset[0] = '\0';

    /*If there are operands, parse them to get the first and second operands and offsets*/
    if (numOfLines>0) {
        parseOperandsSecondPass(operands[0], firstOperand, firstOffset);
    }
    if (numOfLines>2) { /*If there are two operands*/
        parseOperandsSecondPass(operands[1], secondOperand, secondOffset);
    }

    /*If the first operand is a register and the second is a register, special case where one word is being used*/
    if(isRegister(operands[0]) && isRegister(operands[1])){
        regNumSource = atoi(operands[0] + 1);
        regNumDest = atoi(operands[1] + 1);
        newWord.wordBits = (regNumSource << 5) | (regNumDest << 2);
        instructionArray[(*IC)+1] = newWord;
        return;
    }

    /*This part is quite complicated, so i'll write which case it is near each function call/ condition
     * In general, it checks how many operands and offsets we have in our operation.
     * It also converts them into binary accordingly (while maintaining other tests, for syntax accuracy)
     */
    /*There is always at least one operand, because we check it in an earlier condition. analyze and convert it*/
    convertOperandToBinaryAndInsertIntoArray(instructionArray, (*IC)+1, firstOperand, head, errorInfo,1, 0);
    if (firstOffset[0] != '\0') { /*If there is an offset for the first operand*/
        if (searchSymbolList(head, firstOperand, "data")!=0 && searchSymbolList(head, firstOperand, "string")!=0){
            printError(errorInfo, "Offset can only be used with data or string labels");
            return; /*If the label for which the offset is used is not data or string, error*/
        }
        convertOperandToBinaryAndInsertIntoArray(instructionArray, (*IC)+2, firstOffset, head, errorInfo, 0, 1);
        if (secondOperand[0] != '\0') { /*If there is a second operand, AFTER a first operand + offset*/
            convertOperandToBinaryAndInsertIntoArray(instructionArray, (*IC) + 3, secondOperand, head, errorInfo, 0, 0);
            if (secondOffset[0] != '\0') {/*If there is an offset for the second operand*/
                if (searchSymbolList(head, secondOperand, "data")!=0 && searchSymbolList(head, secondOperand, "string")!=0){
                    printError(errorInfo, "Offset can only be used with data or string labels");
                    return;/*If the label for which the offset is used is not data or string, error*/
                }
                convertOperandToBinaryAndInsertIntoArray(instructionArray, (*IC) + 4, secondOffset, head, errorInfo, 0, 1);
            }
        }
    }
    else if (secondOperand[0] != '\0'){ /*If there is a second operand, but no offset for the first operand*/
        convertOperandToBinaryAndInsertIntoArray(instructionArray, (*IC)+2, secondOperand, head, errorInfo,0, 0);
        if (secondOffset[0] != '\0') { /*If there is an offset for the second operand*/
            if (searchSymbolList(head, secondOperand, "data")!=0 && searchSymbolList(head, secondOperand, "string")!=0){
                printError(errorInfo, "Offset can only be used with data or string labels");
                return; /*If the label for which the offset is used is not data or string, error*/
            }
            convertOperandToBinaryAndInsertIntoArray(instructionArray, (*IC)+3, secondOffset, head, errorInfo, 0, 1);
        }
    }
}



/**
 * This function parses the operands in the second pass of the assembler.
 * @param operand The operand to be parsed.
 * @param outOperand The output operand.
 * @param outLabelOrDefine The output label or define.
 */
void parseOperandsSecondPass(const char* operand, char* outOperand, char* outLabelOrDefine) {
    char* bracketPos;
    char* endBracketPos;

    if (operand == NULL) {
        outOperand[0] = '\0'; /* Indicate empty string*/
        outLabelOrDefine[0] = '\0'; /* Indicate empty string*/
        return;
    }

    /* Find the first occurrence of '[' which might indicate a label or define */
    bracketPos = strchr(operand, '[');

    if (bracketPos == NULL) {
        /* No bracket found, the entire operand is just the operand */
        strncpy(outOperand, operand, MAXOPERANDLENGTH - 1);
        outOperand[MAXOPERANDLENGTH - 1] = '\0'; /* Ensure null-termination*/
        outLabelOrDefine[0] = '\0'; /* Indicate empty string*/
    } else {
        /* Extract operand up to the bracket */
        int operandLen = bracketPos - operand;
        operandLen = (operandLen < MAXOPERANDLENGTH - 1) ? operandLen : MAXOPERANDLENGTH - 1;
        strncpy(outOperand, operand, operandLen);
        outOperand[operandLen] = '\0'; /* Null-terminate*/

        /* Check if there's a corresponding closing ']' */
        endBracketPos = strchr(bracketPos, ']');
        if (endBracketPos != NULL) {
            int labelLen = endBracketPos - bracketPos - 1;
            labelLen = (labelLen < MAXOPERANDLENGTH - 1) ? labelLen : MAXOPERANDLENGTH - 1;
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
    while (end > start && isspace((unsigned char)str[end])) end--;

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
 * @return 1 if the word is a .define directive, 0 otherwise.
 */
int isDefine(char* word) {
    if (strcmp(word, ".define") == 0) {
        return 1;
    }
    return 0;
}

/**
 * This function checks if the word is a .data directive.
 * @param word The word to be checked.
 * @return 1 if the word is a .data directive, 0 otherwise.
 */
int isData(char* word) {
    if (strcmp(word, ".data") == 0) {
        return 1;
    }
    return 0;
}

/**
 * This function checks if the word is a .string directive.
 * @param word The word to be checked.
 * @return 1 if the word is a .string directive, 0 otherwise.
 */
int isString(char* word) {
    if (strcmp(word, ".string") == 0) {
        return 1;
    }
    return 0;
}

/**
 * This function checks if the word is a .extern directive.
 * @param word The word to be checked.
 * @return 1 if the word is a .extern directive, 0 otherwise.
 */
int isExtern(char* word) {
    if (strcmp(word, ".extern") == 0) {
        return 1;
    }
    return 0;
}

/**
 * This function checks if the word is a .entry directive.
 * @param word The word to be checked.
 * @return 1 if the word is a .entry directive, 0 otherwise.
 */
int isEntry(char* word) {
    if (strcmp(word, ".entry") == 0) {
        return 1;
    }
    return 0;
}

/**
 * This function checks if the label name is valid.
 * The rules are:
 * Not a reserved word, not a register, not a number, not a defined symbol, not a label that already exists.
 * As well as the first character being an alphabetical character, and the rest of the characters being either uppercase or digits.
 * And the name being less than 31 characters.
 * TODO maybe add a specific error for each case? can do this with a table of defines maybe
 * @param name The label name to be checked.
 * @param operationsArray The operations array.
 * @param head The symbol list.
 * @param colonFlag A flag indicating if the label has a colon.
 * @return 1 if the label name is valid, 0 otherwise.
 */
int isValidLabelName(char* name, operationInfo* operationsArray, symbolList** head, int colonFlag){
    int i;
    char lastChar = name[strlen(name) - 1];

    if (colonFlag && lastChar != ':') {
        return 0;
    }
    else if (colonFlag == 1){ /*Remove the colon for testing!*/
        name[strlen(name) - 1] = '\0';
    }

    /* Check if the name is bigger than the maximum possible number (31), or if it's one of the operations, or if it already exists in the symbol table.*/
    if (strlen(name) > MAXLABELNAME || isValidOperation(name, operationsArray) != -1){
        return 0;
    }
    /*check if the name is one of the registers*/
    if (strcmp(name, "r0") == 0 || strcmp(name, "r1") == 0 || strcmp(name, "r2") == 0 || strcmp(name, "r3") == 0 || strcmp(name, "r4") == 0 || strcmp(name, "r5") == 0 || strcmp(name, "r6") == 0 || strcmp(name, "r7") == 0 ){
        return 0;
    }
    /* Check if the first character is an alphabetical character */
    if (!isalpha(name[0])) {
        return 0;
    }

    /* Check if the rest of the characters are either uppercase or digits */
    for (i = 1; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && !isdigit(name[i])) {
            return 0;
        }
        if (isspace(name[i])){
            return 0;
        }
    }

    /* If all checks pass, the string is valid */
    return 1;
}

/**
 * This function checks if the word is a valid operation.
 * @param word The word to be checked.
 * @param operationsArray The operations array.
 * @return The index of the operation in the operations array if the word is a valid operation, -1 otherwise.
 */
int isValidOperation(char* word, operationInfo* operationsArray) {
    int i;
    for (i = 0; i < NUMOFOPERATIONS; i++) {
        if (strcmp(word, operationsArray[i].name) == 0) {
            return i;
        }
    }
    return -1;
}


/**
 * This function checks if the string is a valid integer.
 * @param str The string to be checked.
 * @return 1 if the string is a valid integer, 0 otherwise.
 */
int isValidInteger(char* str) {
    int i;
    int len = strlen(str);
    int start = 0;
    int temp;

    /* Check if the string starts with '+' or '-'*/
    if (str[0] == '+' || str[0] == '-') {
        if (len == 1) {
            /* If the string is only '+' or '-', it's not a valid integer*/
            return 0;
        }
        start = 1;
    }

    /* Check if the rest of the characters are digits*/
    for (i = start; i < len; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }

    /* Convert the string to an integer */
    temp = atoi(str);

    /* Check if the number is greater than the constant */
    if (temp > MAXINTEGER12BIT || temp < MININTEGER12BIT) {
        return 0;
    }

    /* If all checks pass, the string is a valid integer*/
    return 1;
}

/**
 * This function checks if the word is a register.
 * @param word The word to be checked.
 * @return 1 if the word is a register, 0 otherwise.
 */
int isRegister(char* word){
    if (strcmp(word, "r0") == 0 || strcmp(word, "r1") == 0 || strcmp(word, "r2") == 0 || strcmp(word, "r3") == 0 || strcmp(word, "r4") == 0 || strcmp(word, "r5") == 0 || strcmp(word, "r6") == 0 || strcmp(word, "r7") == 0 ){
        return 1;
    }
    return 0;
}

/**
 * Checks if the current line exceeds the maximum allowed length.
 * If so, it consumes the rest of the line and sets an appropriate error.
 * Returns 0 if the line is too long (and was skipped), or 1 if the line is okay.
 */
int checkLineLengthAndSkip(FILE* sourceFile, char* lineBuffer, error** errorInfo) {
    int ch;
    size_t lineLen = strlen(lineBuffer);
    if (lineBuffer[lineLen - 1] != '\n' && !feof(sourceFile)) {
        /* Line is longer than the buffer, so it's considered too long */
        printError(errorInfo, "Line exceeds the maximum allowed length.");
        /* Consume the rest of the line */
        while ((ch = fgetc(sourceFile)) != '\n' && ch != EOF);
        return 0; /* Indicate that the line is too long and was skipped */
    }
    return 1; /* Line is within the acceptable length */
}


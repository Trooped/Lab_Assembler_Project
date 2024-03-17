#include "syntaxAnalyzer.h"



int getOperandCode(char* operand, symbolList** head, operationInfo* operationsArray, error** errorInfo){
    /* 0 = instant, constant (#number or #define)
     * 1 = direct, Label
     * 2 = constant index (meaning an offset number in an array (it must be of a label, and the offset must be a number / define THAT WAS DEFINED)
     * 3 = register, one of the registers.*/
    int i = 0, j=0;
    char tempVal[MAXOPERANDLENGTH];
    int val;
    char* endptr;
    char tempOperand[MAXOPERANDLENGTH];

    if (operand[0] == '#') {
        strcpy(tempOperand, operand+1);

        if (!isValidInteger(tempOperand)){
            int symbolValue;
            if (!findSymbolValue(head, tempOperand, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                printError(errorInfo, "Invalid Integer or undefined symbol for immediate operand");
                return -999; /*TODO do I even need to retunr this??*/
            }
            val = symbolValue; /* Use the value from the symbol list*/
        }
        return 0;
    }
    else if (isRegister(operand)) {
        return 3;
    }
    else{ /*case of Label or Label offset*/
        trimWhitespace(operand);
        while (operand[i] != '[' && operand[i] != '\0') {
            tempOperand[i] = operand[i];
            i++;
        }
        tempOperand[i] = '\0'; /*Null terminating the temp String*/

        if (operand[i] == '\0') {
            if (isValidLabelName(tempOperand, operationsArray, head, 0)){
                return 1;
            }
            else {
                printError(errorInfo, "Undefined Label name");
                return -999; /*TODO add constant for failed function*/
            }
        }
        else if (operand[i] == '[') {
            if (isValidLabelName(tempOperand, operationsArray, head, 0)) {
                i++;
                while (operand[i] != ']' || operand[i] == '\0') {
                    tempVal[j] = operand[i];
                    i++;
                    j++;
                }
                tempVal[j] = '\0'; /*Null terminating the temp String*/
                if (operand[i]==']' && operand[i+1] == '\0'){
                    if (!isValidInteger(tempVal)){
                        int symbolValue;
                        if (!findSymbolValue(head, tempVal, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                            printError(errorInfo, "Unvalid Integer or undefined symbol for offset operand");
                            return -999; /*TODO add constant for failed function*/
                        }
                        val = symbolValue; /* Use the value from the symbol list*/
                    }
                    return 2;
                }
                else {
                    printError(errorInfo, "Unvalid offset declaration, doesn't end with ']'");
                    return -999; /*TODO add constant for failed function*/
                }
            }
            else {
                printError(errorInfo, "Undefined Label");
                return -999; /*TODO add constant for failed function*/
            }
        }
        else {
            printError(errorInfo, "Unvalid operand"); /*TODO what the hell is this case? need to test this*/
        }
    }
    printError(errorInfo, "Unvalid operand");
    return -999; /*TODO add constant for failed function*/
}


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

void parseOperandsSecondPass(const char* operand, char* outOperand, char* outLabelOrDefine) {
    char* bracketPos;

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
        char* endBracketPos = strchr(bracketPos, ']');
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

int isSymbolExtern(symbolList** head, char* symbolName){
    if (searchSymbolList(head, symbolName, "external") == 0){
        return 1;
    }
    return 0;
}

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


int isDefine(char* word) {
    if (strcmp(word, ".define") == 0) {
        return 1;
    }
    return 0;
}


int isData(char* word) {
    if (strcmp(word, ".data") == 0) {
        return 1;
    }
    return 0;
}

int isString(char* word) {
    if (strcmp(word, ".string") == 0) {
        return 1;
    }
    return 0;
}

int isExtern(char* word) {
    if (strcmp(word, ".extern") == 0) {
        return 1;
    }
    return 0;
}

int isEntry(char* word) {
    if (strcmp(word, ".entry") == 0) {
        return 1;
    }
    return 0;
}

/*TODO add errors!*/
int isValidLabelName(char* name, operationInfo* operationsArray, symbolList** head, int colonFlag){
    int i;
    char lastChar = name[strlen(name) - 1];

    if (colonFlag == 1 && lastChar != ':') {
        return 0;
        /*TODO add error*/
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



int isValidOperation(char* word, operationInfo* operationsArray) {
    int i;
    for (i = 0; i < NUMOFOPERATIONS; i++) {
        if (strcmp(word, operationsArray[i].name) == 0) {
            return i;
        }
    }
    return -1;
}

void checkEntrySyntax(symbolList** head, char* line, error** errorInfo, operationInfo* operationsArray) {
    char* currentWord;
    int flag = 0;

    currentWord = strtok(line, " \n\r\t"); /* Get the next word.*/
    currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
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


/*TODO add a relevant error message*/
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
    if (temp > MAXINTEGER || temp < MININTEGER) {
        return 0;
    }

    /* If all checks pass, the string is a valid integer*/
    return 1;
}

int isRegister(char* word){
    if (strcmp(word, "r0") == 0 || strcmp(word, "r1") == 0 || strcmp(word, "r2") == 0 || strcmp(word, "r3") == 0 || strcmp(word, "r4") == 0 || strcmp(word, "r5") == 0 || strcmp(word, "r6") == 0 || strcmp(word, "r7") == 0 ){
        return 1;
    }
    return 0;
}


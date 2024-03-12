#include "syntaxAnalyzer.h"



int getOperandCode(char* operand, symbolList** head, operationInfo* operationsArray, error** errorInfo){
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

        if (!isValidInteger(tempOperand)){
            int symbolValue;
            if (!findSymbolValue(head, tempOperand, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                printError(*errorInfo, "Unvalid Integer or undefined symbol for immediate operand");
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
        while (operand[i] != '[' || operand[i] != '\0') {
            tempOperand[i] = operand[i];
            i++;
        }

        if (operand[i] == '\0') {
            if (isValidLabelName(tempOperand, operationsArray, head, 0)){
                return 1;
            }
            else {
                printError(*errorInfo, "Undefined Label name");
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
                if (operand[i]==']'){
                    if (!isValidInteger(tempOperand)){
                        int symbolValue;
                        if (!findSymbolValue(head, tempOperand, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                            printError(*errorInfo, "Unvalid Integer or undefined symbol for offset operand");
                            return -999; /*TODO add constant for failed function*/
                        }
                        val = symbolValue; /* Use the value from the symbol list*/
                    }
                    return 2;
                }
                else {
                    printError(*errorInfo, "Unvalid offset declaration, doesn't end with ']'");
                    return -999; /*TODO add constant for failed function*/
                }
            }
            else {
                printError(*errorInfo, "Undefined Label");
                return -999; /*TODO add constant for failed function*/
            }
        }
        else {
            printError(*errorInfo, "Unvalid operand"); /*TODO what the hell is this case? need to test this*/
        }
    }
    printError(*errorInfo, "Unvalid operand");
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


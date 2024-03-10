#include "firstPass.h"


/*TODO MAYBE create the lists in the asmblr.c file and send them here?*/
void firstPass(FILE *sourceFile, word *dataArray, word *instructionArray, operation *operationsArray, symbolList *symbolTable, symbolList *externalSymbols, symbolList *entrySymbols, int *IC, int *DC) {
    int labelFlag = 0;
    int operation = 0;
    int L;
    int value;
    char lineBuffer[MAXCHARSPERLINE]; /*TODO define a maxcharsperlines in this firstPass maybe?*/
    char word[MAXLABELNAME]; /*TODO is it large enough??*/
    symbolList* head = NULL; /* Initialize the list to empty*/
    /*TODO maybe if i already define a head, i don't need to do the head = when calling the functyions?*/

    while (fgets(lineBuffer, sizeof(lineBuffer), sourceFile)) {
        labelFlag = 0;
        strcpy(word, strtok(lineBuffer, " \n\r\t")); /* Tokenize the line into words*/
        while (word != NULL) {
            if (isDefine(word)){
                handleDefine(&head, &operationsArray, lineBuffer);
            }
            else if (isValidLabelName(word, operationsArray, head, 1)){ /*checks if the first word is a label name TODO CHANGE COMMENT*/
                labelFlag = 1;
            }
            else if (isData(word) || isString(word)){
                if (labelFlag) {
                    addLabel(&head, word, "data", *DC);
                }

                if (isData(word)){
                    handleData("data", lineBuffer, &head, &DC, dataArray);
                }
                else{
                    handleData("string", lineBuffer, &head, &DC, dataArray);
                }
            }
            else if (isExtern(word)){
                handleExtern(&head, lineBuffer);
            }
            else if (isEntry(word)){ /*TODO do i even need this in the first pass?*/
                /*TODO according to line 11*/
            }
            else{ /*TODO the case where it's not data, entry or extern- meaning operation!!*/
                addLabel(&head, word, "code", *IC+100);
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

int handleOperation(symbolList** head, word** instructionArray, int opcode, char* line, int *IC, operation *operationsArray) {
    int L = 0;
    int firstOperand;
    int secondOperand;
    int last2bits;/*TODO IS IT NECESSARY?*/

    char operands[MAXOPERANDS][MAXOPERANDLENGTH]; /*TODO define a maxcharsperlines in this firstPass maybe?*/
    /*TODO write this function VVV*/
    fillOperandsArray(operands, MAXOPERANDS, MAXOPERANDLENGTH, '\0');


    parseOperands(line, operands); /*TODO change it to just separate the operands using a comma, the rest'll take care of itself*/

    if (operationsArray[opcode].numOfOperands == 0) {
        if (operands[0][0] != '\0') {
            /*TODO add an error, too many operands*/
            /*TODO exit.*/
        }
        else{
            firstOperand = 0;
            secondOperand = 0;
        }
    }
    else if (operationsArray[opcode].numOfOperands == 1) {
        if (operands[1][0] != '\0') {
            /*TODO add an error, too many operands*/
            /*TODO exit.*/
        }
        else{
            firstOperand = getOperandCode(operands[0], head, operationsArray); /*TODO am i sending this correctly?*/
            secondOperand = 0;
        }
    }
    else if (operationsArray[opcode].numOfOperands == 2) {
        if (operands[2][0] != '\0') {
            /*TODO add an error, too many operands*/
            /*TODO exit.*/
        }
        else{
            firstOperand = getOperandCode(operands[0], head, operationsArray); /*TODO am i sending this correctly?*/
            secondOperand = getOperandCode(operands[1], head, operationsArray); /*TODO am i sending this correctly?*/
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

    switch(opcode){
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

    insertInstructionIntoArray(instructionArray, *IC, opcode, firstOperand, secondOperand);

    L += operationsArray[opcode].numOfOperands;
    return L;
}


void handleData(char* type, char* line, symbolList ** head, int *DC, word** dataArray) {
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
            printf("Error: Line does not start with .data\n");
            return;
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
                /*TODO add an error*/
                return;
            }

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

            if (!isValidInteger(token)){
                int symbolValue;
                if (!findSymbolValue(head, token, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                    printf("Error: Undefined symbol '%s'\n", token); /*TODO change error!*/
                    return; /*TODO add constant for failed function*/
                }
                val = symbolValue; /* Use the value from the symbol list*/
            }

            /* Store the integer and increment DC*/
            addValueToDataArray(dataArray, *DC, val);
            currentSize++;
            (*DC)++;
            *DC = currentSize; /*TODO do i even need to do this?*/
            dataCounter++; /*how many data is saved in the current array?*/


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
            for (i = 1; i < strlen(copiedLine-1); i++) {
                if (currentSize >= maxSize) {
                    // Increase the dataArray size by another chunk
                    maxSize += dataSize;
                    *dataArray = realloc(*dataArray, maxSize * sizeof(word));
                    if (!*dataArray) {
                        printf("Error: Memory allocation failed\n");
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
            /*TODO add an error*/
            return;
        }
    }
    else{
        /*TODO add an error*/
        return;
    }
}




/*TODO I need to just add the next label after extern, and if there's mroe than 1 then error?*/
void handleExtern(symbolList** head, char* line) {
    char* currentWord; /*TODO define it as 30 or something? IDK*/

    currentWord = strtok(NULL, " \n\r\t"); /* Get the next word.*/
    while (currentWord!= NULL) {
        addLabel(*head, currentWord, "external", 0);
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
    }
}


void handleDefine(symbolList** head, operation* operationsArray, char* line) {
    char name[MAXLABELNAME]; /*TODO handle the maxname already*/
    char* endptr;
    int value;
    char* currentWord; /*TODO define it as 30 or something? IDK*/

    currentWord = strtok(line, " \n\r\t"); /* Tokenize the line into words*/
    while(currentWord != NULL) {
        if (isValidLabelName(currentWord, NULL, head, 1)){
            if (!searchSymbolList(&head, currentWord, "define")){ /*TODO explain and remember that it means it's returning 0 and not 1!*/
                strncpy(name, currentWord, MAXLABELNAME);
                currentWord = strtok(NULL, " \n\r\t"); /* Get the next word.*/
                if (strcmp(currentWord, "=")==0) {
                    currentWord = strtok(NULL, " \n\r\t"); /* Get the next word.*/
                    if (!isValidInteger(currentWord)){
                        int symbolValue;
                        if (!findSymbolValue(head, currentWord, "define",&symbolValue)) { /* Token wasn't a valid integer, check if it's a defined symbol*/
                            printf("Error: Undefined symbol '%s'\n", token); /*TODO change error!*/
                            return; /*TODO add constant for failed function*/
                        }
                        value = symbolValue; /* Use the value from the symbol list*/
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


/*TODO add a free data struct function*/

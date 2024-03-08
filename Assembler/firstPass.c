#include "firstPass.h"
#include "assembler.h"

/*TODO MAYBE create the lists in the asmblr.c file and send them here?*/
void firstPass(FILE *sourceFile, word *dataArray, word *instructionArray, operation *operationsArray, symbolList *symbolTable, symbolList *externalSymbols, symbolList *entrySymbols, int *IC, int *DC) {
    int labelFlag = 0;
    int value;
    char lineBuffer[MAXCHARSPERLINE]; /*TODO define a maxcharsperlines in this firstPass maybe?*/
    char word[MAXCHARSPERWORD]; /*TODO define it as 30 or something? IDK*/
    symbolList* head = NULL; /* Initialize the list to empty*/
    /*TODO maybe if i already define a head, i don't need to do the head = when calling the functyions?*/

    while (fgets(lineBuffer, sizeof(lineBuffer), sourceFile)) {
        labelFlag = 0;
        word = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
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
                    addDataToDataArray("data", lineBuffer,&head, &DC, dataArray);
                }
                else{
                    addDataToDataArray("string", lineBuffer,&head, &DC, dataArray);
                }
            }
            else if (isExtern(word)){
                handleExtern(&head, lineBuffer);
            }
            else if (isEntry(word)){ /*TODO do i even need this in the first pass?*/
                /*TODO according to line 11*/
            }
            else{ /*TODO the case where it's not data, entry or extern- meaning operation!!*/
                /*TODO do i need to do something here? lol*/
            }
            /*TODO DO I NEED THIS STRTOK? seems useless*/
            word = strtok(NULL, " \n\r\t"); /* Get the next word.*/
        }
    }
    /*MOVE TO 16, ACCORDING TO THE COURSE'S ALGORITHM.*/
    /*TODO call the linked list memory free operation*/
}


void handleLabel(symbolList** head, char* line, int *DC){
    validData *data = NULL;
    char name[MAXNAME];
    char currentWord[MAXCHARSPERWORD]; /*TODO define it as 30 or something? IDK*/

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

void handleExtern(symbolList** head, char* line) {
    char currentWord[MAXCHARSPERWORD]; /*TODO define it as 30 or something? IDK*/

    currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
    while (currentWord!= NULL) {
        addLabel(*head, currentWord, "external", 0);
        currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
    }
}

void handleDefine(symbolList** head, char* line) {
    char name[MAXNAME]; /*TODO handle the maxname already*/
    int value;
    char currentWord[MAXCHARSPERWORD]; /*TODO define it as 30 or something? IDK*/

    currentWord = strtok(line, " \n\r\t"); /* Tokenize the line into words*/
    while(currentWord != NULL) {
        if (isValidName){
            if (!searchLabelList(&head, currentWord)){ /*TODO explain and remember that it means it's returning 0 and not 1!*/
                strncpy(name, currentWord, MAXNAME);
                currentWord = strtok(NULL, " \n\r\t"); /* Get the next word.*/
                if (strcmp(currentWord, "=")==0) {/*TODO maybe it's !=0?*/
                    currentWord = strtok(NULL, " \n\r\t"); /* Get the next word.*/
                    value = strtol(currentWord);/*TODO can i even use it in c90?*/
                    currentWord= strtok(NULL, " \n\r\t"); /* Get the next word.*/
                    if (isValidValue && currentWord == NULL){
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
    *head = addLabel(*head, name, "define", value);
}

/* Function to add a new node at the end of the list */
symbolList* addLabel(symbolList* head, char* name, char* type, int value) {
    symbolList* newNode = (symbolList*)malloc(sizeof(symbolList));
    if(newNode == NULL) {
        /* If malloc fails, print an error and exit TODO DONT EXIT!!! FIND OUT WHAT TO DO DIFFERENTLY*/
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    /* Copy the name and value into the new node */
    strncpy(newNode->name, name, MAXNAME);
    strncpy(newNode->type, type, MAXNAME);/*TODO CHANGE THE CONSTANTS HERE OMNG*/
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
            /* Name found in the list, return 0 = error TODO maybe add it as a constant?*/
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

/* Function to delete the entire list */
void deleteLabelList(symbolList* head) {
    /* Loop through the list and delete each node */
    symbolList* current = head;
    symbolList* nextNode;
    while (current != NULL) {
        nextNode = current->next;
        deleteSymbolListNode(current);
        current = nextNode;
    }
}

void addDataToDataArray(char* type, char* line, symbolList ** head, int *DC, word** dataArray) {
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

int findSymbolValue(symbolList **head, const char* name,char* type, int* value) {
    symbolList* current = *head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if (strcmp(current->type, type) == 0) {
                *value = current->value;
                return 1;
            }
        }
        current = current->next;
    }
    return 0;
}




/*TODO add a free data struct function*/
/*TODO do i need to return &data or just data?*/
void isDataValidAndAddToDataArray(char* type, char* line, symbolList ** head, int *DC, word** dataArray){
    int i;
    int sign = 0;
    int numbers = 0;
    int comma = 0;
    int counter = 0;
    char token[MAXCHARSPERWORD]; /*TODO define a maxcharsperlines in this firstPass maybe?*/
    char copiedLine[MAXCHARSPERLINE]; /*TODO define a maxcharsperlines in this firstPass maybe?*/



    validData *data = malloc(sizeof(validData));
    if(data == NULL) {
        /* If malloc fails, print an error and exit TODO DONT EXIT!!! FIND OUT WHAT TO DO DIFFERENTLY*/
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }



    /*LETS START FROM HERE*/

    if (strcmp(type, ".data")==0) {
        char* spacePos = strchr(line, ' ');
        strcpy(copiedLine, spacePos+1);

        token = strtok(copiedLine, " ,");
        while(token != NULL) {
            if(token[0] == '+' || token[0] == '-') {
                token++;  /* Skip the sign*/
            }

            /* If the token is empty, it means there were consecutive commas*/
            if(strlen(token) == 0) {
                data->valid = 0;
                return data;
            }

            strcpy(token,trimSpaces(token)); /* TODO need to implement this function*/

            if(isInteger(token)) {
                counter++;
            }
            else if(searchLabelList(head, token, "data")) {/*TODO need to add a way to add the data that was found instead of this lol*/
                counter++;
            }
            else {
                /*TODO printf an error*/
                data->valid = 0;
                return data;
            }

            token = strtok(NULL, " ,");
        }
        data->dataCounter = counter; /*TODO maybe i need to add 1 more because it doesnt check the last integer?*/
        data->valid = 1;
        return data;

    #if 0
        char* spacePos = strchr(line, ' '); /* Find the position of the first data element*/
        strcpy(copiedLine, spacePos+1); /* Copy the characters*/
        if (copiedLine == NULL) {/*TODO meaning that there's no space after .data*/
            /*TODO add an error and don't add it to the label list*/
        }

        if (copiedLine[0] == ',') {
            /*TODO add an exit error, because of first char comma*/
            data->valid = 0;
            return data;
        }
        for (i = 0; i < strlen(copiedLine); i++) {
            if (copiedLine[i] == ' ') {
                continue;
            }
            if (copiedLine[i] == '-' || copiedLine[i] == '+') {
                if (numbers == 0 && sign == 0) {
                    sign = 1;
                } else {
                    /*TODO add an error*/
                    data->valid = 0;
                    return data;
                }
            } else if (copiedLine[i] == ',') {
                if (numbers == 1) {
                    numbers = 0;
                    comma = 1;
                    sign = 0;
                    counter++;
                } else {
                    /*TODO add an error AND EXIT!!!*/
                    data->valid = 0;
                    return data;
                }
            } else if (isdigit(copiedLine[i])) {
                if (numbers == 0 && copiedLine[i] != '0' && isdigit(copiedLine[i + 1])) {
                    comma = 0;
                    numbers = 1;
                } else if (numbers == 1) {
                    continue;
                } else {
                    /*TODO add an error*/
                    data->valid = 0;
                    return data;
                }
            } else {
                /*TODO add an error, not according to format or w/e*/
                data->valid = 0;
                return data;
            }
        }

        if (copiedLine[strlen(copiedLine) - 1] == ',') {
            /*TODO add an error, because of last char comma*/
            data->valid = 0;
            return data;
        } else {
            counter++;
            data->dataCounter = counter;
            data->valid = 1;
        }
        return data;
    }
    #endif
    }
    else if (strcmp(type, ".string")==0) {
        char *spacePos = strchr(line, ' '); /* Find the position of the first data element*/
        strcpy(copiedLine, spacePos + 1); /* Copy the characters*/
        if (copiedLine == NULL) {/*TODO meaning that there's no space after .data*/
            /*TODO add an error and don't add it to the label list*/
        }
        if (strlen(copiedLine) > 2 && copiedLine[0] == '"' && copiedLine[strlen(copiedLine) - 1] == '"') {
            data->valid = 1;
            for (i = 0; i < strlen(copiedLine); i++) {
                if (copiedLine[i] != '"') {
                    counter++;
                }
                counter++; /*adding 1 for the null terminator.*/
                data->dataCounter = counter;
                return data;
            }
        } else {
            /*TODO add an error*/
            data->valid = 0;
            return data;
        }
    }
    else{
        data->valid = 0;
        return data;
    }
    return data;
}


char* trimSpaces(char* str) {
    char* end;

    /* Trim leading space*/
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  /* All spaces?*/
        return str;

    /* Trim trailing space*/
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    /* Write new null terminator character*/
    end[1] = '\0';

    return str;
}
#include "analyzationAndUtilities.h"

int isDefine(char* word) {
    if (strcmp(word, ".define") == 0) {
        return 1;
    }
    return 0;
}

int isLabel(char* word) {
    char* colonPos = strchr(word, ':'); /* Find the position of ':'*/
    if (colonPos != NULL && strlen(word) <= MAXLABELNAME) {
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

int isValidName(char* name){
    /*TODO add tests to see if the name here isn't already used, and not one of the constants that are out of limit!*/
}



int isValidOperation(char* word, operation* operationsArray) {
    int i;
    for (i = 0; i < OPERATIONS; i++) {
        if (strcmp(word, operationsArray[i].name) == 0) {
            return i;
        }
    }
    return -1;
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

int isValidInteger(char* str) {
    int i;
    int len = strlen(str);
    int start = 0;

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

    /* If all checks pass, the string is a valid integer*/
    return 1;
}

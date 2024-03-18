#include "assembler.h"

/**
 * This function is the main function of the assembler. It reads the source file and creates the memory image.
 * it also calls the firstPass and secondPass functions.
 * @param source The source file to be read.
 * @param fileName The name of the file.
 */
void assembler(FILE* source, const char* fileName){
    binaryWord dataArray[MAXDATA]; /* array for data to be put in the memory image */
    binaryWord instructionArray[MAXINSTRUCTIONS]; /* array for the instructions to be put in the memory image.*/
    symbolList* symbolTable = NULL; /* linked list for labels */
    error* errorInfo; /* Empty errorInfo struct for errors!*/
    operationInfo operationsArray[NUMOFOPERATIONS];
    int IC = 0, DC = 0; /* instruction counter and data counter */

    int i; /*TODO FOR TESTING, DELETE LATER!!!*/

    initializeOperationsArray(operationsArray); /*initialize operations array according to the known operations*/



    /*TODO I want it here??*/
    char newFileName[MAXFILENAME];
    FILE* objectFile; /* *entFile, *extFile;*/





    /*TODO real start of assembler function!*/

    initializeErrorInfo(&errorInfo,&symbolTable, fileName, source); /*initialize errorInfo struct*/

    /*initializing both of the arrays*/
    initializeDataArray(dataArray, 0);
    initializeInstructionArray(instructionArray, 0);

    /*call firstPass function*/

    firstPass(source, dataArray, instructionArray, operationsArray, &symbolTable, &IC, &DC, &errorInfo);

    if (errorInfo->errorFlag == 1) {
        printf("%d Errors were found in your program, exiting the process\n", errorInfo->counter);
        closeFileAndExit(&errorInfo, &symbolTable);
    }

    incrementDataSymbolValues(&symbolTable, (IC)+100);

    rewind(source); /*reset the file pointer to the beginning of the file*/
    printf("SECOND PASS\n");
    secondPass(source, dataArray, instructionArray, operationsArray, &symbolTable, &IC, &DC, &errorInfo);


    /*TODO TESTINGGGGGGGGG
    printSymbolList(symbolTable);
    printf("\nIC: %d\n", IC);
    printf("DC: %d\n", DC);
    for (i = 0; i < DC; i++) {
        printf("dataArray[%d]: ", i);
        printBits(dataArray[i].wordBits);
        printf("\t decimal: %d", instructionArray[i].wordBits);
        printf("\n");
    }
    for (i = 0; i < IC; i++) {
        printf("instructionArray[%d]: ", i);
        printBits(instructionArray[i].wordBits);
        printf("\t decimal: %d", instructionArray[i].wordBits);
        printf("\n");
    }

    if (errorInfo->errorFlag == 1) {
        printf("%d Errors were found in your program, exiting the process\n", errorInfo->counter);
        closeFileAndExit(&errorInfo, &symbolTable);
    }
     */

    /*TODO testinggggggggggggggggggggg*/


    if (entryLabelCounter(&symbolTable) > 0) {
        sprintf(newFileName, "%s.ent", fileName);
        objectFile = fopen(newFileName, "w");
        if (objectFile == NULL) {
            printf("Failed to open file %s\n", newFileName);
            closeFileAndExit(&errorInfo, &symbolTable);
        }
        createEntFile(&symbolTable, objectFile);
        fclose(objectFile);
    }
    if (externLabelCounter(&symbolTable) > 0) {
        sprintf(newFileName, "%s.ext", fileName);
        objectFile = fopen(newFileName, "w");
        if (objectFile == NULL) {
            printf("Failed to open file %s\n", newFileName);
            closeFileAndExit(&errorInfo, &symbolTable);
        }
        createExtFile(&symbolTable, objectFile);
        fclose(objectFile);
    }

    sprintf(newFileName, "%s.ob", fileName);
    objectFile = fopen(newFileName, "w");
    if (objectFile == NULL) {
        printf("Failed to open file %s\n", newFileName);
        closeFileAndExit(&errorInfo, &symbolTable);
    }
    createObjectFile(objectFile, dataArray, instructionArray, IC, DC);
    fclose(objectFile);


    deleteSymbolList(&symbolTable);
    free(errorInfo);
}

void createObjectFile(FILE* objectFile, binaryWord* dataArray, binaryWord* instructionArray, int IC, int DC) {
    int i;
    fprintf(objectFile, "%d %d\n", IC, DC);
    for (i = 0; i < IC; i++) {
        char* base4String = convertBinaryToBase4Symbols(instructionArray[i].wordBits);
        if (base4String != NULL) {
            fprintf(objectFile, "0%d %s\n", i+100, base4String);
            free(base4String);
        } else {
            fprintf(stderr, "Memory allocation failed for value at index %d.\n", i);
            break;
        }
    }
    for (i = 0; i < DC; i++) {
        char* base4String = convertBinaryToBase4Symbols(dataArray[i].wordBits);
        if (base4String != NULL) {
            fprintf(objectFile, "0%d %s\n", i+IC+100, base4String);
            free(base4String);
        } else {
            fprintf(stderr, "Memory allocation failed for value at index %d.\n", i);
            break;
        }
    }
}

char* convertBinaryToBase4Symbols(int binary) {
    int i;
    char symbols[4] = {'*', '#', '%', '!'};
    int mask = 0x3;  /* Binary 11, to extract two bits at a time. */
    char* result = (char*)malloc(8);  /* Allocate on the heap. */
    int tempIndex = 7;  /* Start from the end of the result string. */

    if (result == NULL) return NULL;  /* Check for malloc failure. */

    result[7] = '\0';  /* Null-terminate the string. */

    /* Iterate 7 times to process all 14 bits, accounting for leading zeros. */
    for (i = 0; i < 7; i++) {
        int currentDigit = binary & mask;
        binary >>= 2;  /* Move to the next base 4 digit. */
        /* Fill the result from the end to start */
        result[6 - i] = symbols[currentDigit];
    }

    return result;
}


void createEntFile(symbolList** head, FILE* entFile) {
    symbolList* current = *head;
    while (current != NULL) {
        if (current->isEntry == 1) {
            fprintf(entFile, "%s 0%d\n", current->name, current->value);
        }
        current = current->next;
    }
}

void createExtFile(symbolList** head, FILE* extFile) {
    int i = 0;
    symbolList* current = *head;
    while (current != NULL) {
        if (strcmp(current->type, "external") == 0) {
            i = 0;
            while (current->externalAddresses[i] != -1) {
                fprintf(extFile, "%s\t0%d\n", current->name, current->externalAddresses[i]);
                i++;
            }
        }
        current = current->next;
    }
}


int entryLabelCounter(symbolList** head) {
    symbolList* current = *head;
    int counter = 0;
    while (current != NULL) {
        if (current->isEntry == 1) {
            counter++;
        }
        current = current->next;
    }
    return counter;
}

int externLabelCounter(symbolList** head) {
    symbolList* current = *head;
    int counter = 0;
    while (current != NULL) {
        if (strcmp(current->type, "external") == 0) {
            counter++;
        }
        current = current->next;
    }
    return counter;
}

/**
 * This function initializes the data array with a given value.
 * @param dataArray The array to be initialized.
 * @param initialValue The value to initialize the array with.
 */
void initializeDataArray(binaryWord* dataArray, int initialValue) {
    int i;

    /* Initialize each element*/
    for (i = 0; i < MAXDATA; i++) {
        dataArray[i].wordBits = initialValue;
    }
}

/**
 * This function initializes the instruction array with a given value.
 * @param instructionArray The array to be initialized.
 * @param initialValue The value to initialize the array with.
 */
void initializeInstructionArray(binaryWord* instructionArray, int initialValue) {
    int i;

    /* Initialize each element*/
    for (i = 0; i < MAXINSTRUCTIONS; i++) {
        instructionArray[i].wordBits = initialValue;
    }
}

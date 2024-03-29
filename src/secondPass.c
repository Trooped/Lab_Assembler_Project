/**
 * @file secondPass.c
 *
 * This file contains the second pass of the assembler.
 * The second pass goes through the source file and creates the binary words for the instructions.
 */

#include "include/secondPass.h"


/**
 * This function is the second pass of the assembler. It goes through the source file and creates the binary words for the instructions.
 * In the end of the second pass function, the program checks if the program is too large for the memory we have, and if it is- ends the program prematurely.
 * @param sourceFile The source file to be read.
 * @param dataArray The array to store the data.
 * @param instructionArray The array to store the instructions.
 * @param operationsArray The array to store the operations.
 * @param symbolTable The symbol table.
 * @param IC The instruction errorCounter.
 * @param DC The data errorCounter.
 * @param errorInfo The error struct.
 */
void secondPass(FILE *sourceFile, binaryWord *dataArray, binaryWord *instructionArray, operationInfo *operationsArray, symbolList** symbolTable, int *IC, int *DC, error** errorInfo){
    char lineBuffer[MAX_CHARS_PER_LINE]; /* Buffer for the current line*/
    char fullLine[MAX_CHARS_PER_LINE]; /* The full line*/
    char* currentWord; /* The current word in the line*/
    int L; /* The current L value = the number of lines to add to the IC*/
    int operation; /* The current operation number*/
    (*IC) = 0; /* Reset the IC*/
    (*errorInfo)->currentLineNumber = 0;
    rewind(sourceFile); /*reset the file pointer to the beginning of the file*/


    /* Loop through the source file line by line */
    while (fgets(lineBuffer, sizeof(lineBuffer), sourceFile)) {
        strncpy(fullLine, lineBuffer, MAX_CHARS_PER_LINE);  /* Copy the current line into fullLine (for further parsing)*/
        fullLine[MAX_CHARS_PER_LINE - 1] = '\0'; /* Ensure null-termination*/
        (*errorInfo)->currentLineNumber++; /* Increment the line counter*/
        strncpy((*errorInfo)->currentLineContent, fullLine, MAX_CHARS_PER_LINE); /* Copying the current line into the error struct*/


        L = 0; /* Reset the L value*/
        currentWord = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
        while(currentWord != NULL){ /* Loop through the words in the line*/
            if (isDefine(currentWord)) { /*checks if the first word is a define directive*/
                break; /* Skip the define line*/
            }
            if (searchSymbolList(symbolTable, currentWord, "general") == 0) { /* Label found in the list*/
                currentWord = strtok(NULL, " \n\r\t"); /* Get the next word.*/
            }
            else if (isValidLabelName(currentWord, operationsArray, symbolTable, 1)){
                currentWord = strtok(NULL, " \n\r\t"); /*If for example, we have a valid Label name before extern or entry that was ignored in the first pass*/
            }
            if (isData(currentWord) || isString(currentWord)  || isExtern(currentWord)) { /*checks if the first word is a data, string or extern*/
                break; /* Skip the data or string line*/
            }
            else if (isEntry(currentWord)) { /*checks if the first word is an entry directive*/
                currentWord = strtok(NULL, " \n\r\t"); /* Get the next word.*/
                markLabelAsEntry(symbolTable, currentWord, errorInfo); /*Calling the function with ONLY the label name, all other tests have been made in the first pass.*/
                break; /* Skip the entry line*/
            }

            operation = isValidOperation(currentWord, operationsArray); /*checks if the first word is a valid operation*/
            if (operation == INVALID_OPERATION_CODE) { /* If the operation is invalid*/
                break; /* Break the loop, we already found this error in the first pass*/
            }
            /* If the operation is valid, handle the operation*/
            L = handleOperation(symbolTable, instructionArray, operation, fullLine, IC, operationsArray, errorInfo,1);
            if (L == INSTRUCTION_FAIL_CODE){
                break; /* Break the loop, we already found this error in the first pass*/
            }
            (*IC) += L; /* Increment the IC by L*/
            break; /* Break the loop*/
        }
    }

    /*Checking if the program is too large for the memory we have, which is 4096-100 words of memory
     * if it is- end the program prematurely.*/
    if ((*IC)+(*DC) > MEMORY_IMAGE_SIZE) {
        printError(errorInfo, "The program is too large for the available memory, exiting the process");
        closeFileAndExit(errorInfo, symbolTable);
    }

}

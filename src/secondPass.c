/**
 * @file secondPass.c
 *
 * This file contains the second pass of the assembler.
 * The second pass goes through the source file and creates the binary words for the instructions.
 */

#include "include/secondPass.h"


/**
 * This function is the second pass of the assembler. It goes through the source file and creates the binary words for the instructions.
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
    char lineBuffer[MAXCHARSPERLINE];
    char fullLine[MAXCHARSPERLINE];
    char* currentWord;
    int L;
    int operation;
    (*IC) = 0;

    while (fgets(lineBuffer, sizeof(lineBuffer), sourceFile)) {
        fullLine[0] = '\0';
        strncpy(fullLine, lineBuffer, MAXCHARSPERLINE);
        fullLine[MAXCHARSPERLINE - 1] = '\0'; /* Ensure null-termination*/
        strncpy((*errorInfo)->lineText, fullLine, MAXCHARSPERLINE); /* Copying the current line into the error struct*/


        L = 0;
        currentWord = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
        while(currentWord != NULL){
            if (isDefine(currentWord)) {
                break;
            }
            if (searchSymbolList(symbolTable, currentWord, "general") == 0) { /* Label found in the list*/
                currentWord = strtok(NULL, " \n\r\t"); /* Get the next binaryWord.*/
            }
            else if (isValidLabelName(currentWord, operationsArray, symbolTable, 1)){
                currentWord = strtok(NULL, " \n\r\t"); /*If for example, we have a valid Label name before extern or entry that was ignored in the first pass*/
            }
            if (isData(currentWord) || isString(currentWord)  || isExtern(currentWord)) {
                break;
            }
            else if (isEntry(currentWord)) {
                currentWord = strtok(NULL, " \n\r\t");
                markLabelAsEntry(symbolTable, currentWord, errorInfo); /*Calling the function with ONLY the label name, all other tests have been made in the first pass.*/
                break;
            }

            operation = isValidOperation(currentWord, operationsArray);
            L = handleOperation(symbolTable, instructionArray, operation, fullLine, IC, operationsArray, errorInfo,1);
            (*IC) += L;
            break;
        }
    }
}

#include "secondPass.h"


/**
 * This function is the second pass of the assembler. It goes through the source file and creates the binary words for the instructions.
 * @param sourceFile The source file to be read.
 * @param dataArray The array to store the data.
 * @param instructionArray The array to store the instructions.
 * @param operationsArray The array to store the operations.
 * @param symbolTable The symbol table.
 * @param IC The instruction counter.
 * @param DC The data counter.
 * @param errorInfo The error struct.
 */
void secondPass(FILE *sourceFile, binaryWord *dataArray, binaryWord *instructionArray, operationInfo *operationsArray, symbolList** symbolTable, int *IC, int *DC, error** errorInfo){
    char lineBuffer[MAXCHARSPERLINE];
    char fullLine[MAXCHARSPERLINE];
    char* currentWord;
    int L;
    (*IC) = 0;

    while (fgets(lineBuffer, sizeof(lineBuffer), sourceFile)) {
        fullLine[0] = '\0';
        strncpy(fullLine, lineBuffer, MAXCHARSPERLINE);
        fullLine[MAXCHARSPERLINE - 1] = '\0'; /* Ensure null-termination*/
        strncpy((*errorInfo)->lineText, fullLine, MAXCHARSPERLINE); /* Copying the current line into the error struct*/

        /*TODO TESTING PURPOSES*/
        /*printf("lineBuffer: %s\n", lineBuffer);*/
        /* TODO TSTINGGGGGGG*/


        L = 0;
        currentWord = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
        while(currentWord != NULL){
            /*TODO check first if it's like a define or something, if it is i can skip this fucker*/
            /*TODO in general, don't forget about the first command, which is usually not one of the operands or w/e*/
            if (isDefine(currentWord)){
                break;
            }
            if (searchSymbolList(symbolTable, removeColon(currentWord), "general") == 0) { /* Label found in the list*/
                currentWord = strtok(NULL, " \n\r\t"); /* Get the next binaryWord.*/
            }
            if (isData(currentWord) || isString(currentWord)  || isExtern(currentWord)) {
                break;
            }
            else if (isEntry(currentWord)) {
                markLabelAsEntry(symbolTable, fullLine, errorInfo);
                break;
            }

            int operation = isValidOperation(currentWord, operationsArray);
            L = handleOperation(symbolTable, instructionArray, operation, fullLine, IC, operationsArray, errorInfo,1);
            (*IC) += L;
            break;
        }
    }
}

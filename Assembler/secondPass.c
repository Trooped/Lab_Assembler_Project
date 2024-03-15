#include "secondPass.h"


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

        L = 0;
        currentWord = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
        while(currentWord != NULL){
            /*TODO check first if it's like a define or something, if it is i can skip this fucker*/
            /*TODO in general, don't forget about the first command, which is usually not one of the operands or w/e*/
            if (searchSymbolList(symbolTable, currentWord, "general") == 0) { /* Label found in the list*/
                currentWord = strtok(NULL, " \n\r\t"); /* Get the next binaryWord.*/
                break; /*MAYBE STAY IN THE LOOP?*/
            }
            else if (isData(currentWord) || isString(currentWord)  || isExtern(currentWord)) {
                break;
            }
            else if (isEntry(currentWord)) {
                markLabelAsEntry(symbolTable, fullLine, errorInfo);
                break;
            }
            int operation = isValidOperation(currentWord, operationsArray);
            L = handleOperation(symbolTable, instructionArray, operation, fullLine, IC, operationsArray, errorInfo, 1);
            (*IC) += L;

            currentWord = strtok(NULL, " \n\r\t"); /* Get the next binaryWord.*/
        }



    }



}

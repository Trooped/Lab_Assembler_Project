#include "include/firstPass.h"

/**
 * This function is the first pass of the assembler. It goes through the source file and creates the symbol table, and the data and initial instruction arrays.
 * @param sourceFile The source file to be read.
 * @param dataArray The array to store the data.
 * @param instructionArray The array to store the instructions.
 * @param operationsArray The array to store the operations.
 * @param symbolTable The symbol table.
 * @param IC The instruction errorCounter.
 * @param DC The data errorCounter.
 * @param errorInfo The error struct.
 */
void firstPass(FILE *sourceFile, binaryWord *dataArray, binaryWord *instructionArray, operationInfo *operationsArray, symbolList** symbolTable, int *IC, int *DC, error** errorInfo){
    int labelFlag = 0;
    int operation = 0;
    int L;
    char lineBuffer[MAXCHARSPERLINE];
    char fullLine[MAXCHARSPERLINE];
    char tempLabelName[MAXLABELNAME];
    char* currentWord;

    while (fgets(lineBuffer, sizeof(lineBuffer), sourceFile)) {
        size_t lineLen = strlen(lineBuffer);
        int ch;
        (*errorInfo)->lineCounter++; /* Increment the line counter*/

        /* Copy the current line into a fullLine (for further parsing) and to the errorInfo struct*/
        fullLine[0] = '\0';
        strncpy(fullLine, lineBuffer, MAXCHARSPERLINE);
        fullLine[MAXCHARSPERLINE - 1] = '\0'; /* Ensure null-termination*/
        strncpy((*errorInfo)->lineText, fullLine, MAXCHARSPERLINE); /* Copying the current line into the error struct*/

        /* Check if we've read a complete line, if it's longer - then it exceeds the limit of 80 chars.
         * Throw an error, skip the line and check for other errors, if they exist.*/
        if (lineBuffer[lineLen - 1] != '\n') {
            /* We didn't find a newline, which means the line is longer than our buffer */
            printError(errorInfo, "Line exceeds the maximum allowed length.");

            /* Consume the rest of the line to get back in sync for the next iteration */
            while ((ch = fgetc(sourceFile)) != '\n' && ch != EOF);

            /* Skip processing this line since it's too long */
            continue;
        }


        /*TODO TESTING PURPOSES*/
        /*printf("lineBuffer: %s\n", lineBuffer);*/
        /* TODO TSTINGGGGGGG*/

        L = 0;
        labelFlag = 0;
        currentWord = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
        while (currentWord != NULL) {
            if (isDefine(currentWord)){
                handleDefine(symbolTable, operationsArray, fullLine, errorInfo);
                break;
            }
            else if (isValidLabelName(currentWord, operationsArray, symbolTable, 1)){ /*checks if the first word is a valid label definition*/
                labelFlag = 1;
                strncpy(tempLabelName, currentWord, MAXLABELNAME);
            }
            else if (isData(currentWord) || isString(currentWord)){
                if (labelFlag) {
                    addLabel(symbolTable, tempLabelName, "data", *DC, errorInfo);
                }

                if (isData(currentWord)){
                    handleData("data", fullLine, symbolTable, DC, dataArray, errorInfo);
                    break;
                }
                else{
                    handleData("string", fullLine, symbolTable, DC, dataArray, errorInfo);
                    break;
                }
            }
            else if (isExtern(currentWord)){
                if (labelFlag) { /*TODO do i need to add a warning? because this label is ignored*/
                    handleExtern(symbolTable, fullLine, errorInfo, operationsArray, 1);
                }
                else{
                    handleExtern(symbolTable, fullLine, errorInfo, operationsArray, 0);
                }
                break;
            }
            else if (isEntry(currentWord)){
                if (labelFlag) { /*TODO do i need to add a warning? because this label is ignored*/
                    checkEntrySyntax(symbolTable, fullLine, errorInfo, operationsArray,1);
                }
                else{
                    checkEntrySyntax(symbolTable, fullLine, errorInfo, operationsArray, 0);
                }
                break;
            }
            else if (isValidOperation(currentWord, operationsArray)!=-1){
                if (labelFlag) {
                    addLabel(symbolTable, tempLabelName, "code", *IC + 100, errorInfo);
                }
                strtok(NULL, " \n\r\t"); /* Get the next binaryWord.*/
                operation = isValidOperation(currentWord, operationsArray);

                L = handleOperation(symbolTable, instructionArray, operation, fullLine, IC, operationsArray, errorInfo, 0);
                if (L == INSTRUCTIONFAILCODE){
                    break;
                }
                else{
                    (*IC) += L;
                    break;
                }
            }
            else{
                printError(errorInfo, "Invalid label, operation or directive");
                break;
            }
            currentWord = strtok(NULL, " \n\r\t"); /* Get the next binaryWord.*/

            /*Checking if there was an empty label declaration*/
            if (labelFlag && currentWord == NULL){
                printError(errorInfo, "Empty label declaration");
                break;
            }

        }
    }
}


/**
 * @file firstPass.c
 * This file contains the implementation of the first pass of the assembler.
 */

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
    int labelFlag = 0; /* Flag to check if the current line begins with a label*/
    int operation = 0; /* The current operation number*/
    int L; /* The current L value = the number of lines to add to the IC*/
    char lineBuffer[MAX_CHARS_PER_LINE]; /* Buffer for the current line*/
    char fullLine[MAX_CHARS_PER_LINE]; /* Buffer for the full line*/
    char tempLabelName[MAX_LABEL_NAME]; /* String for the temporary label name (the label name to add in the current line)*/
    char* currentWord; /* The current word in the line*/

    rewind(sourceFile); /* Reset the file pointer to the beginning of the file*/

    /* Loop through the source file line by line */
    while (fgets(lineBuffer, sizeof(lineBuffer), sourceFile)) {
        /* Copy the current line into fullLine (for further parsing)*/
        strncpy(fullLine, lineBuffer, MAX_CHARS_PER_LINE);
        fullLine[MAX_CHARS_PER_LINE - 1] = '\0'; /* Ensure null-termination*/

        /* Updating the errorInfo struct with the current line information */
        (*errorInfo)->lineCounter++; /* Increment the line counter*/
        (*errorInfo)->lineText[0] = '\0'; /* Reset the line text*/
        strncpy((*errorInfo)->lineText, fullLine, MAX_CHARS_PER_LINE); /* Copying the current line into the error struct*/


        /* Check if the line is too long, and skip it if it is */
        if (!checkLineLengthAndSkip(sourceFile, lineBuffer, errorInfo)) {
            /* Line is too long and has been skipped, so continue to the next iteration */
            continue;
        }

        L = 0; /* Reset the L value*/
        labelFlag = 0; /* Reset the label flag*/
        currentWord = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
        while (currentWord != NULL) {
            if (isDefine(currentWord)){ /*checks if the first word is a define directive*/
                handleDefine(symbolTable, operationsArray, fullLine, errorInfo);
                break;
            }
            else if (isValidLabelName(currentWord, operationsArray, symbolTable, 1)){ /*checks if the first word is a valid label definition*/
                labelFlag = 1; /* Set the label flag*/
                strncpy(tempLabelName, currentWord, MAX_LABEL_NAME);  /* Copy the label name to the tempLabelName, for use with addLabel later*/
            }
            else if (isData(currentWord) || isString(currentWord)){ /*checks if the first word is a data or string directive*/
                if (labelFlag) { /* If there is a label, add it to the symbol table*/
                    addLabel(symbolTable, tempLabelName, "data", *DC, errorInfo);
                }
                if (isData(currentWord)){ /* If the directive is .data*/
                    handleData("data", fullLine, symbolTable, DC, dataArray, errorInfo);
                    break;
                }
                else{ /* If the directive is .string*/
                    handleData("string", fullLine, symbolTable, DC, dataArray, errorInfo);
                    break;
                }
            }
            else if (isExtern(currentWord)){ /*checks if the first word is an extern directive*/
                if (labelFlag) { /* If there was label before it, send '1' to the function to ignore the label*/
                    handleExtern(symbolTable, fullLine, errorInfo, operationsArray, 1);
                }
                else{ /* If there was no label before it, send '0' to the function to handle it normally*/
                    handleExtern(symbolTable, fullLine, errorInfo, operationsArray, 0);
                }
                break;
            }
            else if (isEntry(currentWord)){ /*checks if the first word is an entry directive*/
                if (labelFlag) { /* If there was label before it, send '1' to the function to ignore the label*/
                    checkEntrySyntax(symbolTable, fullLine, errorInfo, operationsArray,1);
                }
                else{ /* If there was no label before it, send '0' to the function to handle it normally*/
                    checkEntrySyntax(symbolTable, fullLine, errorInfo, operationsArray, 0);
                }
                break;
            }
            else if (isValidOperation(currentWord, operationsArray)!=-1){ /*checks if the first word is a valid operation*/
                if (labelFlag) { /* If there was label before it, add it to the symbol table*/
                    addLabel(symbolTable, tempLabelName, "code", ((*IC) + INITIAL_IC_VALUE), errorInfo);
                }
                strtok(NULL, " \n\r\t"); /* Get the next word.*/
                operation = isValidOperation(currentWord, operationsArray); /* Get the operation number*/
                /* Handle the operation and get the L value*/
                L = handleOperation(symbolTable, instructionArray, operation, fullLine, IC, operationsArray, errorInfo, 0);
                if (L == INSTRUCTION_FAIL_CODE){
                    break; /* If the operation failed, break the loop*/
                }
                else{ /* If the operation succeeded, increment the IC by L*/
                    (*IC) += L;
                    break;
                }
            }
            else{ /* If the first word is not a label, operation or directive, print an error and break the loop*/
                printError(errorInfo, "Invalid label, operation or directive");
                break;
            }
            currentWord = strtok(NULL, " \n\r\t"); /* Get the next word.*/

            /* If we reached here, there was an empty label declaration, throw an error!*/
            if (labelFlag && currentWord == NULL){
                printError(errorInfo, "Empty label declaration isn't allowed");
                break;
            }
        }/* End of the inner while loop (to parse words in line)*/
    } /* End of the main while loop (to parse the source code line by line)*/

    /*increment the data symbols by the initial IC value- 100*/
    incrementDataSymbolValues(symbolTable, (*IC) + INITIAL_IC_VALUE);
} /* End of the firstPass function*/


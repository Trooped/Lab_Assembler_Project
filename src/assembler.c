/**
 * @file assembler.c
 * This file contains the main functions of the assembler.
 * --------------------------------------------------------------------------------------------------------------
 * Created by: Omri Peretz, as the final project in the course "Laboratory in C", Open University of Israel 2024A.
 *---------------------------------------------------------------------------------------------------------------
 * the functions in this file are:
 * 1. main - The main function of the assembler, which calls the pre-assembly and assembly functions.
 * 2. assembler - The second main function of the assembler. It reads the source file (with macros spread) and creates the memory image.
 */

#include "include/assembler.h"

/**
 * @file main.c
 * The main function of the assembler, which calls the pre-assembly and assembly functions.
 * @param argc The number of arguments given to the program.
 * @param argv The arguments given to the program.
 * @return 0 if the program ran successfully.
 */
int main(int argc, char** argv) {
    int fileCount; /*Counter for the files*/
    FILE* initialFile, *macrosSpreadFile; /*File pointers for the old and new files*/
    char fileName[MAX_FILE_NAME]; /*The name of the file*/
    char baseFileName[MAX_FILE_NAME]; /*The base file name without the suffix*/

    if (argc >= 2) {
        for (fileCount=1; fileCount < argc; fileCount++) {
            sprintf(baseFileName, "%s", argv[fileCount]); /*Keep a copy of the original file name, without a suffix*/
            sprintf(fileName, "%s.as", argv[fileCount]); /*Add the suffix to the file name, so we can access it.*/

            /*TODO maybe change the opening of this file to inside of the createFileWithMacros?*/
            initialFile = fopen(fileName, "r");
            if (initialFile != NULL) {
                macrosSpreadFile = createFileWithMacros(initialFile, baseFileName); /*create a new file with the macros from the old file*/
                fclose(initialFile);

                if (macrosSpreadFile == NULL) { /*If there were errors in the pre-assembly process (they were printed already), the file pointer is NULL*/
                    continue; /*Continue to the next file*/
                }

                /*Call the assembler function*/
                assembler(macrosSpreadFile, baseFileName);

                testPrintAndDeleteFile(baseFileName); /*TODO delete this before submitting!!!*/
            }
            else { /*If the file couldn't be opened, print an error message*/
                fprintf(stderr,"Failed to open file %s\n", argv[fileCount]);
            }
        }
    }
    else { /*If no files were given as arguments, print an error message*/
        fprintf(stderr,"no files were given as arguments to the assembler\n");
    }
    return 0;
}


/**
 * The second main function of the assembler program, which is actually the assembly process itself.
 * It reads the source file with macros and creates the memory image.
 * it also calls the firstPass and secondPass functions.
 * @param sourceFile The sourceFile file to be read.
 * @param fileName The name of the file.
 */
void assembler(FILE* sourceFile, char* fileName){
    char baseFileName[MAX_FILE_NAME]; /* The base file name without the suffix - copy string */
    char tempFileName[MAX_FILE_NAME]; /* The temporary file name */
    memoryWord instructionArray[INSTRUCTION_ARRAY_SIZE]; /* Array for the instructions to be put in the memory image.*/
    memoryWord dataArray[DATA_ARRAY_SIZE]; /* Array for data to be put in the memory image */
    symbolList* symbolTable = NULL; /* Linked list for symbols, be it Labels or Defines */
    error* errorInfo; /* Empty errorInfo struct for errors!*/
    operationInfo operationsArray[NUM_OF_OPERATIONS]; /* Array for the operations */
    int IC = 0, DC = 0; /* instruction Counter and data Counter, for each of the arrays. */

    strncpy(tempFileName, fileName, MAX_FILE_NAME - 1); /* Keep a copy of the original file name, without a suffix*/
    strcat(tempFileName, ".am"); /* Add the suffix to the file name, so we can access print it specifically.*/
    /*initialize the arrays, error Struct and the symbol table*/
    initializeErrorInfo(&errorInfo, &symbolTable, tempFileName, sourceFile); /*initialize errorInfo struct*/
    initializeOperationsArray(operationsArray); /*initialize operations array according to the known operations*/
    initializeDataArray(dataArray, 0);
    initializeInstructionArray(instructionArray, 0);

    strncpy(baseFileName, fileName, MAX_FILE_NAME - 1); /* Keep a copy of the original file name, without a suffix*/

    /*Initiating the first pass*/
    firstPass(sourceFile, dataArray, instructionArray, operationsArray, &symbolTable, &IC, &DC, &errorInfo);

    /*Initiating the second pass*/
    secondPass(sourceFile, dataArray, instructionArray, operationsArray, &symbolTable, &IC, &DC, &errorInfo);

    /*Check if there are any errors. If there are errors, then print the number of errors and exit the process*/
    if (errorInfo->errorFlag == TRUE) {
        fprintf(stderr,"%d Errors were found in your program, exiting the process\n", errorInfo->errorCounter);
        closeFileAndExit(&errorInfo, &symbolTable);
    }

    /************************************************************************
    *If there are no errors, create the object file and the entry and external files
    *************************************************************************/

    /*If there are entry labels, create the entry file*/
    if (entryLabelCounter(&symbolTable) > 0) {
        createEntFile(&symbolTable, baseFileName, &errorInfo);
    }
    /*if there are external labels and they are used in operations, create the external file*/
    if (externLabelCounter(&symbolTable) > 0) {
        createExtFile(&symbolTable, baseFileName, &errorInfo);
    }
    /*Create the object file*/
    createObjectFile(dataArray, instructionArray, IC, DC, baseFileName, &errorInfo, &symbolTable);


    /*Delete the symbol table and the error struct*/
    deleteSymbolList(&symbolTable);
    free(errorInfo);
    fclose(sourceFile); /*close the sourceFile file and return to the main function.*/
}



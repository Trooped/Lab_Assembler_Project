/**
 * @file assembler.c
 * This file contains the main functions of the assembler.
 *
 * the functions in this file are:
 * 1. main - The main function of the assembler, which calls the pre-assembly and assembly functions.
 * 2. assembler - The second main function of the assembler. It reads the source file and creates the memory image.
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
    error* error; /*Empty error struct for the preAsmblr*/
    FILE* oldFIle, *newFile; /*File pointers for the old and new files*/
    char fileName[MAXFILENAME]; /*The name of the file*/
    char baseFileName[MAXFILENAME]; /*The base file name without the suffix*/

    if (argc >= 2) {
        for (fileCount=1; fileCount < argc; fileCount++) {
            sprintf(baseFileName, "%s", argv[fileCount]); /*Keep a copy of the original file name, without a suffix*/
            sprintf(fileName, "%s.as", argv[fileCount]); /*Add the suffix to the file name, so we can access it.*/
            initializeErrorInfo(&error, NULL, fileName, NULL); /*initialize empty errorInfo struct for the preAsmblr*/

            /*TODO maybe change the opening of this file to inside of the createFileWithMacros?*/
            oldFIle = fopen(fileName, "r");
            if (oldFIle) {
                newFile = createFileWithMacros(oldFIle, baseFileName, &error); /*create a new file with the macros from the old file*/
                fclose(oldFIle);

                /*TODO do i need to stop the run if there are errors in pre assembly??*/
                /*If there are errors in pre-assembly, stop the run*/
                if (error->errorFlag == 1) {
                    printf("Errors found in pre-assembly, stopping..\n");
                    fclose(newFile);
                    continue;
                }
                else {
                    free(error); /*free the error struct*/
                }

                /*Call the assembler function*/
                assembler(newFile, baseFileName);
                /*Close the file*/
                fclose(newFile);

                testPrintAndDeleteFile(baseFileName);
            }
            else { /*If the file couldn't be opened, print an error message*/
                printf("Failed to open %s\n", argv[fileCount]);
            }
        }
    }
    else { /*If no files were given as arguments, print an error message*/
        printf("no files were given as arguments to the assembler\n");
    }
    return 0;
}


/**
 * This function is the main function of the assembler. It reads the source file and creates the memory image.
 * it also calls the firstPass and secondPass functions.
 * @param source The source file to be read.
 * @param fileName The name of the file.
 */
void assembler(FILE* source, char* fileName){
    char baseFileName[MAXFILENAME]; /* The base file name without the suffix - copy string */
    binaryWord instructionArray[MAXINSTRUCTIONS]; /* Array for the instructions to be put in the memory image.*/
    binaryWord dataArray[MAXDATA]; /* Array for data to be put in the memory image */
    symbolList* symbolTable = NULL; /* Linked list for symbols, be it Labels or Defines */
    error* errorInfo; /* Empty errorInfo struct for errors!*/
    operationInfo operationsArray[NUMOFOPERATIONS]; /* Array for the operations */
    int IC = 0, DC = 0; /* instruction Counter and data Counter, for each of the arrays. */

    /*initialize the arrays, error Struct and the symbol table*/
    initializeErrorInfo(&errorInfo,&symbolTable, fileName, source); /*initialize errorInfo struct*/
    initializeOperationsArray(operationsArray); /*initialize operations array according to the known operations*/
    initializeDataArray(dataArray, 0);
    initializeInstructionArray(instructionArray, 0);


    strncpy(baseFileName, fileName, MAXFILENAME - 1); /* Keep a copy of the original file name, without a suffix*/

    /*Initiating the first pass*/
    rewind(source); /*reset the file pointer to the beginning of the file*/
    firstPass(source, dataArray, instructionArray, operationsArray, &symbolTable, &IC, &DC, &errorInfo);
    incrementDataSymbolValues(&symbolTable, (IC) + INITIAL_IC_VALUE); /*increment the data symbols by the initial IC value- 100*/

    /*Initiating the second pass*/
    rewind(source); /*reset the file pointer to the beginning of the file*/
    secondPass(source, dataArray, instructionArray, operationsArray, &symbolTable, &IC, &DC, &errorInfo);

    /*Check if there are any errors, if there are- print the number of errors and exit the process*/
    if (errorInfo->errorFlag == 1) {
        printf("%d Errors were found in your program, exiting the process\n", errorInfo->errorCounter);
        closeFileAndExit(&errorInfo, &symbolTable);
    }

    /****************
    *If there are no errors, create the object file and the entry and external files
    *****************/

    /*If there are entry labels, create the entry file*/
    if (entryLabelCounter(&symbolTable) > 0) {
        createEntFile(&symbolTable, baseFileName, &errorInfo);
    }
    /*if there are external labels and they are mentioned in another line, create the external file*/
    if (externLabelCounter(&symbolTable) > 0) {
        createExtFile(&symbolTable, baseFileName, &errorInfo);
    }
    /*Create the object file*/
    createObjectFile(dataArray, instructionArray, IC, DC, baseFileName, &errorInfo, &symbolTable);

    /*Delete the symbol table and the error struct, and return to main function*/
    deleteSymbolList(&symbolTable);
    free(errorInfo);
}



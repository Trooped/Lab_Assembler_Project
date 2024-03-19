#include "include/assembler.h"

int main(int argc, char** argv) {
    int fileCount;
    error* error;
    FILE* oldFIle, *newFile;
    char fileName[MAXFILENAME];
    char baseFileName[MAXFILENAME];

    if (argc >= 2) {
        for (fileCount=1; fileCount < argc; fileCount++) {
            sprintf(fileName, "%s.as", argv[fileCount]);
            sprintf(baseFileName, "%s", argv[fileCount]); /*Keep a copy of the original file name, without a suffix*/
            initializeErrorInfo(&error, NULL, fileName, NULL); /*initialize errorInfo struct*/

            /*TODO maybe change the opening of this file to inside of the createFileWithMacros?*/
            oldFIle = fopen(fileName, "r");
            if (oldFIle) {
                newFile = createFileWithMacros(oldFIle, argv[fileCount], &error); /*create a new file with the macros from the old file*/
                fclose(oldFIle);

                /*TODO do i need to stop the run if there are errors in pre assembly??*/
                if (error->errorFlag == 1) {
                    printf("Errors found in pre-assembly, stopping..\n");
                    fclose(newFile);
                    continue;
                }
                else {
                    free(error);
                }

                assembler(newFile, baseFileName);

                printFileContent("ps.ob");
                printf("\n");
                printFileContent("ps.ent");
                printf("\n");
                printFileContent("ps.ext");
                printf("\n");
                deleteFile("ps.ob");
                deleteFile("ps.ent");
                deleteFile("ps.ext");
                deleteFile("ps.am");
            }
            else
                printf("Failed to open %s\n", argv[fileCount]);
        }
    }
    else {
        printf("not enough arguments\n");
    }

    return 0;

}


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
    int IC = 0, DC = 0; /* instruction errorCounter and data errorCounter */

    int i; /*TODO FOR TESTING, DELETE LATER!!!*/


    initializeOperationsArray(operationsArray); /*initialize operations array according to the known operations*/


    /*TODO real start of assembler function!*/

    initializeErrorInfo(&errorInfo,&symbolTable, fileName, source); /*initialize errorInfo struct*/

    /*initializing both of the arrays*/
    initializeDataArray(dataArray, 0);
    initializeInstructionArray(instructionArray, 0);

    /*call firstPass function*/

    rewind(source); /*reset the file pointer to the beginning of the file*/
    firstPass(source, dataArray, instructionArray, operationsArray, &symbolTable, &IC, &DC, &errorInfo);

    if (errorInfo->errorFlag == 1) {
        printf("%d Errors were found in your program, exiting the process\n", errorInfo->errorCounter);
        closeFileAndExit(&errorInfo, &symbolTable);
    }

    incrementDataSymbolValues(&symbolTable, (IC) + INITIAL_IC_VALUE); /*increment the data symbols by the IC value*/

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
        printf("%d Errors were found in your program, exiting the process\n", errorInfo->errorCounter);
        closeFileAndExit(&errorInfo, &symbolTable);
    }
     */

    /*TODO testinggggggggggggggggggggg*/

    fclose(source); /*close the source file*/

    if (entryLabelCounter(&symbolTable) > 0) {
        createEntFile(&symbolTable, fileName, &errorInfo);
    }
    if (externLabelCounter(&symbolTable) > 0) {
        createExtFile(&symbolTable, fileName, &errorInfo);
    }

    createObjectFile(dataArray, instructionArray, IC, DC, fileName, &errorInfo, &symbolTable);

    deleteSymbolList(&symbolTable);
    free(errorInfo);
}



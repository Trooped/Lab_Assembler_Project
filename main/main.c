/*TODO add the correct files!*/

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "main.h"

/*----------------------------------------TESTING------------------------------*/
#include <assert.h>

void printFileContent1(const char* filePath) {
    int c;
    FILE* file = fopen(filePath, "r"); /* Open the file for reading.*/
    if (file == NULL) {
        perror("Error opening file");
        return;
    }


    while ((c = fgetc(file)) != EOF) { /* Read and print each character.*/
        putchar(c);
    }

    fclose(file); /* Close the file.*/
}

void deleteFile(const char* filePath) {
    if (remove(filePath) == 0) {
        printf("File %s deleted successfully.\n", filePath);
    } else {
        perror("Error deleting file");
    }
}


void printFileContent(const char* fileName) {
    FILE* file = fopen(fileName, "r");
    char line[MAXCHARSPERLINE];

    if (file != NULL) {
        printf("Content of %s:\n", fileName);
        while (fgets(line, sizeof(line), file) != NULL) {
            printf("%s", line);
        }
        fclose(file);
    } else {
        printf("Failed to open file %s\n", fileName);
    }
}

void testCopyMacrosIntoNewFile() {
    /* Create a test file with some macros*/
    const char* testFileName = "testFile.as";
    const char* newFileName = "testFile.am";
    FILE* testFile = fopen(testFileName, "w");
    FILE* newFile;
    if (testFile != NULL) {
        fputs("mcr 1testMacro\nline1\nline2\nline3\nendmcr\nHello!\n; this is a comment\nwhat's up?\n\n\n\n1testMacro\nfinish\n", testFile);
        fclose(testFile);
    } else {
        printf("Failed to open file %s for writing\n", testFileName);
        return;
    }

    /* Call your function with the test file*/
    FILE* oldFile = fopen(testFileName, "r");
    if (oldFile != NULL) {
        newFile = createNewFileAndWriteMacros(oldFile, "testFile");
        fclose(newFile);
    } else {
        printf("Failed to open file %s for reading\n", testFileName);
        return;
    }

    /* Print the contents of the old and new files*/
    printFileContent(testFileName);
    printFileContent(newFileName);

    /* Clean up*/
    fclose(newFile);
    remove(testFileName);
    remove("testFilenewFileFormat");
}

void testAssembler() {

    const char* testFileName = "testFile";
    FILE* testFile = fopen(testFileName, "r");
    if (testFile != NULL) {
        assembler(testFile, testFileName);
        fclose(testFile);
    } else {
        printf("Failed to open file %s\n", testFileName);
    }

}


/*----------------------------------------TESTING------------------------------*/



int main(int argc, char** argv) {
    /*testCopyMacrosIntoNewFile();*/
    testAssembler();
    printFileContent1("testFile.ob");
    printFileContent1("testFile.ent");
    printFileContent1("testFile.ext");
    deleteFile("testFile.ob");
    deleteFile("testFile.ent");
    deleteFile("testFile.ext");

    return 0;
/*----------------------------------------TESTING------------------------------*/

#if 0
    int fileCount;
    FILE* oldFIle, *newFile; /* TODO WHY IS THIS- NECESSARY?? ->>>am: for the file returned from pre-assembly */
    char* fileName = (char*) calloc(MAXFILENAME, sizeof(char));

    if (argc >= 2) {
        for (fileCount=1; fileCount < argc; fileCount++) {
            error=0; /*TODO what is this error?*/
            sprintf(fileName, "%s.as", argv[fileCount]); /*TODO should i use sprintf or just strcpy?*/
            oldFIle = fopen(fileName, "r");
            if (oldFIle) {
                /*newFile = createNewFileAndWriteMacros(oldFIle, argv[fileCount]);*/
                fclose(oldFIle);


                /*TODO KEEP DOING THIS LATER, CHANGE EVERYTHING AFTER THIS POINT*/
                if (error) {
                    printf("Errors found in pre-assembly, stopping..\n");
                    fclose(oldFIle);
                    continue;
                }

                /*compile(newFile, argv[fileCount]);*/
            }
            else
                printf("File pointer is null\n");
        }
    }
    else
        printf("not enough arguments\n");

    free(fileName);

    return 0;
#endif
}

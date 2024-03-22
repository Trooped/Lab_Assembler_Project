
#include "testFunctions.h"


void printFileContentFromPointer(FILE* file) {
    int c;
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    while ((c = fgetc(file)) != EOF
            ) { /* Read and print each character.*/
        putchar(c);
    }
}

void printFileContent(const char* filePath) {
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



void testCopyMacrosIntoNewFile() {
    /* Create a test file with some macros*/
    const char* testFileName = "testFile.as";
    const char* newFileName = "testFile.am";
    FILE* oldFile;
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
    oldFile = fopen(testFileName, "r");
    if (oldFile != NULL) {
        newFile = createFileWithMacros(oldFile, "testFile", NULL);
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

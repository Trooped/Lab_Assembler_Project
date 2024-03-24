
#include "testFunctions.h"

void testPrintAndDeleteFile(char* baseFileName){
    printf("MACROS FILE AFTER SPREADING\n");
    printFileContent(baseFileName, "am");
    printf("\n");


    printf("OBJECT FILE\n");
    printFileContent(baseFileName, "ob");
    printf("\n");


    printf("ENTRIES FILE\n");
    printFileContent(baseFileName, "ent");
    printf("\n");



    printf("EXTERNALS FILE\n");
    printFileContent(baseFileName, "ext");
    printf("\n");


    deleteFile(baseFileName, "am");
    deleteFile(baseFileName, "ob");
    deleteFile(baseFileName, "ent");
    deleteFile(baseFileName, "ext");
}


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

void printFileContent(const char* filePath, char* suffix) {
    int c;
    char fileName[MAX_FILE_NAME];
    FILE* file;
    sprintf(fileName, "%s.%s", filePath, suffix);

    file = fopen(fileName, "r"); /* Open the file for reading.*/
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    while ((c = fgetc(file)) != EOF) { /* Read and print each character.*/
        putchar(c);
    }

    fclose(file); /* Close the file.*/
}

void deleteFile(const char* filePath, char* suffix) {
    char fileName[MAX_FILE_NAME];
    sprintf(fileName, "%s.%s", filePath, suffix);
    if (remove(fileName) == 0) {
        printf("File %s deleted successfully.\n", fileName);
    } else {
        perror("Error deleting file");
    }
}

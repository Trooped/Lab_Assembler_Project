/**
 * @file dataConversion.c
 * This file contains functions that are used to convert data to binary and insert it into the instruction and data arrays.
 * These functions are used to insert the first instruction into the instruction array, insert operands into the instruction array, and add a value in binary to the data array.
 *
 * The functions in this file are:
 * 1. insertFirstInstructionIntoArray - This function inserts the first instruction into the instruction array.
 * 2. convertOperandToBinaryAndInsertIntoArray - This function converts an operand to binary and inserts it into the instruction array.
 * 3. convertValueToBinaryAndInsertToDataArray - This function adds a value in binary to the data array.
 * 4. convertBinaryToBase4Symbols - This function converts a binary number to base 4 symbols.
 */

#include "include/dataConversion.h"

/**
 * This function inserts the first instruction into the instruction array.
 * meaning - the first word of each line in the instruction array.
 *
 * @param instructionArray The instruction array to insert the instruction into.
 * @param IC The instruction errorCounter.
 * @param opcode The opcode of the instruction.
 * @param firstOperand The first operand of the instruction.
 * @param secondOperand The second operand of the instruction.
 */
void insertFirstInstructionIntoArray(binaryWord* instructionArray, int IC, int opcode, int firstOperand, int secondOperand) {
    binaryWord newWord;
    newWord.wordBits = (opcode << 6) | (firstOperand << 4) | (secondOperand << 2);
    instructionArray[IC] = newWord;
}

/**
 * This function converts an operand to binary and inserts it into the instruction array.
 * it first checks it's type, and then converts it to binary accordingly.
 *
 * @param instructionArray The instruction array to insert the operand into.
 * @param IC The instruction errorCounter.
 * @param operand The operand to convert and insert.
 * @param head The head of the symbol table.
 * @param errorInfo A pointer to the errorInfo struct.
 * @param source A flag to indicate if the operand is a source operand (1) or a destination operand (0).
 * @param offset A flag to indicate if the operand is an offset operand.
 */
void convertOperandToBinaryAndInsertIntoArray(binaryWord* instructionArray, int IC, char* operand, symbolList** head, error** errorInfo, int source, int offset) {
    int val;
    binaryWord newWord;

    if (operand[0] == '#') {
        if (!isValidInteger(operand + 1)) {
            findSymbolValue(head, operand + 1, "define", &val);
        }
        else {
            val = atoi(operand + 1);
        }
        newWord.wordBits = val << 2 | 0;
    }
    else if (offset){
        if (!isValidInteger(operand)) {
            findSymbolValue(head, operand, "define", &val);
        }
        else {
            val = atoi(operand);
        }

        if (val < 0){
            printError(errorInfo, "Offset value can't be negative");
            return;
        }
        newWord.wordBits = val << 2 | 0;
    }
    else if (isRegister(operand)) { /*if it's a register*/
        int regNum = atoi(operand + 1);
        if(source){ /*if it's the source register*/
            newWord.wordBits = regNum << 5;
        }
        else{ /*if it's the destination register*/
            newWord.wordBits = regNum << 2;
        }
    }
    else if(searchSymbolList(head, operand, "general")==0){
        if (isSymbolExtern(head, operand)){
            addExternAddress(head, operand, IC+100);
            newWord.wordBits = 0x0001;
        }
        else{
            findSymbolValue(head, operand, "data", &val);
            newWord.wordBits = ( val << 2) | 0x0002;
        }
    }
    else if (searchSymbolList(head, operand, "define")==0){
        printError(errorInfo, ".define label can't be used as an operand");
    }
    else{
        printError(errorInfo, "Label not found in the symbol table");
    }

    instructionArray[IC] = newWord;
}


/**
 * This function adds a value in binary to the data array.
 *
 * @param dataArray The data array to add the value to.
 * @param DC The data errorCounter.
 * @param value The value to add to the data array.
 */
void convertValueToBinaryAndInsertToDataArray(binaryWord* dataArray, int DC, int value) {
    binaryWord newWord;
    newWord.wordBits = value;
    dataArray[DC] = newWord;
}


/**
 * This function converts a binary number to base 4 symbols, which are encrypted symbols.
 * The function converts the binary number two bits at a time, and assigns a symbol to each pair of bits.
 * The symbols are '*' for 00, '#' for 01, '%' for 10, and '!' for 11.
 *
 * @param binary The binary number to convert.
 * @return The base 4 symbols representing the binary number.
 */
char* convertBinaryToBase4Symbols(int binary) {
    int i;
    char symbols[4] = {'*', '#', '%', '!'};
    int mask = 0x3;  /* Binary 11, to extract two bits at a time. */
    char* result = (char*)malloc(8);  /* Allocate on the heap. */
    int tempIndex = ENCRYPTEDWORDSIZE;  /* Start from the end of the result string. */

    if (result == NULL) return NULL;  /* Check for malloc failure. */

    result[ENCRYPTEDWORDSIZE] = '\0';  /* Null-terminate the string. */

    /* Iterate 7 times to process all 14 bits, accounting for leading zeros. */
    for (i = 0; i < ENCRYPTEDWORDSIZE; i++) {
        int currentDigit = binary & mask;
        binary >>= 2;  /* Move to the next base 4 digit. */
        /* Fill the result from the end to start */
        result[(ENCRYPTEDWORDSIZE - 1) - i] = symbols[currentDigit];
    }

    return result;
}

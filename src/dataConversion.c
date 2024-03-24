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
    binaryWord newWord; /* Create a new binary word. */
    newWord.wordBits = (opcode << 6) | (firstOperand << 4) | (secondOperand << 2); /* Create the binary word with the opcode and operands. */
    instructionArray[IC] = newWord; /* Insert the word into the instruction array. */
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
    int val; /* The value of the operand, if it's a number. */
    binaryWord newWord; /* Create a new binary word. */

    if (operand[0] == '#') { /*if it's an immediate number*/
        if (!isValidInteger(operand + 1)) { /*if it's not a valid number*/
            findSymbolValue(head, operand + 1, "define", &val); /*find the value of the symbol in the table, with a define directive*/
        }
        else {
            val = atoi(operand + 1); /*convert the number to an integer*/
        }
        newWord.wordBits = val << 2 | 0; /*shift the value by 2 bits and insert it into the word*/
    }
    else if (offset){ /*if it's an offset*/
        if (!isValidInteger(operand)) { /*if it's not a valid number*/
            findSymbolValue(head, operand, "define", &val); /*find the value of the symbol in the table, with a define directive*/
        }
        else {
            val = atoi(operand); /*convert the number to an integer*/
        }

        if (val < 0){ /*if the offset is negative, which isn't allowed.*/
            printError(errorInfo, "Offset value can't be negative");
            return;
        }
        newWord.wordBits = val << 2 | 0; /*shift the value by 2 bits and insert it into the word*/
    }
    else if (isRegister(operand)) { /*if it's a register*/
        int regNum = atoi(operand + 1); /*get the register number*/
        if(source){ /*if it's the source register*/
            newWord.wordBits = regNum << 5; /*shift the register number by 5 bits and insert it into the word*/
        }
        else{ /*if it's the destination register*/
            newWord.wordBits = regNum << 2; /*shift the register number by 2 bits and insert it into the word*/
        }
    }
    else if(searchSymbolList(head, operand, "general")==0){ /*if it's a label*/
        if (isSymbolExtern(head, operand)){ /*if it's an external label*/
            addExternAddress(head, operand, (IC+INITIAL_IC_VALUE)); /*add the address of the label to the extern list*/
            newWord.wordBits = 0x0001; /*insert the word with the A bit set to 1*/
        }
        else{ /*if it's not an external label*/
            findSymbolValue(head, operand, "data", &val); /*find the value of the symbol in the table, with a data directive*/
            newWord.wordBits = ( val << 2) | 0x0002; /*shift the value by 2 bits and insert it into the word, with the E bit set to 1*/
        }
    }
    else if (searchSymbolList(head, operand, "define")==0){ /*if it's a label that was declared as .define*/
        printError(errorInfo, ".define label can't be used as an operand");
    }
    else{ /* none of the above*/
        printError(errorInfo, "Label not found in the symbol table");
    }
    /*insert the word into the instruction array*/
    instructionArray[IC] = newWord;
}


/**
 * This function adds a value in binary to the data array.
 * used with .data and .string values.
 *
 * @param dataArray The data array to add the value to.
 * @param DC The data errorCounter.
 * @param value The value to add to the data array.
 */
void convertValueToBinaryAndInsertToDataArray(binaryWord* dataArray, int DC, int value) {
    binaryWord newWord; /* Create a new binary word. */
    newWord.wordBits = value; /* Insert the value into the word. */
    dataArray[DC] = newWord; /* Insert the word into the data array. */
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
    int i; /* Loop index. */
    char symbols[4] = {'*', '#', '%', '!'}; /* The symbols to represent the base 4 digits. */
    int mask = 0x3;  /* Binary 11, to extract two bits at a time. */
    int tempIndex = ENCRYPTED_WORD_SIZE;  /* Start from the end of the result string, which is 7. */
    char* result = (char*)malloc(8);  /* Allocate on the heap. */
    if (result == NULL){ /* Check for malloc failure. */
        return NULL;  /* Return NULL to the main function (which will call the closeFileAndExit function) */
    }

    result[ENCRYPTED_WORD_SIZE] = '\0';  /* Null-terminate the string. */

    /* Iterate 7 times to process all 14 bits, accounting for leading zeros. */
    for (i = 0; i < ENCRYPTED_WORD_SIZE; i++) {
        int currentDigit = binary & mask; /* Extract the two bits. */
        binary >>= 2;  /* Move to the next base 4 digit. */
        /* Fill the result from the end to start */
        result[(ENCRYPTED_WORD_SIZE - 1) - i] = symbols[currentDigit]; /* Assign the symbol to the result string. */
    }

    return result; /* Return the result string. */
}

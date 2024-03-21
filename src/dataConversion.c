/**
 * @file dataConversion.c
 * This file contains functions that are used to convert data to binary and insert it into the instruction and data arrays.
 * These functions are used to insert the first instruction into the instruction array, insert operands into the instruction array, and add a value in binary to the data array.
 *
 * The functions in this file are:
 * 1. insertFirstInstructionIntoArray - This function inserts the first instruction into the instruction array.
 * 2. convertOperandToBinaryAndInsertIntoArray - This function converts an operand to binary and inserts it into the instruction array.
 * 3. insertOperandsIntoInstructionArray - This function inserts the operands into the instruction array, calling the convertOperandToBinaryAndInsertIntoArray function for each operand.
 * 4. addValueToDataArray - This function adds a value in binary to the data array.
 * 5. convertBinaryToBase4Symbols - This function converts a binary number to base 4 symbols.
 */

#include "include/dataConversion.h"

/**
 * This function inserts the first instruction into the instruction array.
 * meaning - the first word of each instruction.
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
        newWord.wordBits = val << 2 | 0;
    }
    else if (operand[0] == 'r') {
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
    else{
        printError(errorInfo, "Label not found in the symbol table");
    }

    instructionArray[IC] = newWord;
}



/**
 * This function inserts the operands into the instruction array, calling the convertOperandToBinaryAndInsertIntoArray function for each operand.
 *
 * @param instructionArray The instruction array to insert the operands into.
 * @param numOfLines The number of lines in the file.
 * @param IC The instruction errorCounter.
 * @param operands The operands to insert into the instruction array.
 * @param head The head of the symbol table.
 * @param errorInfo A pointer to the errorInfo struct.
 */
void insertOperandsIntoInstructionArray(binaryWord* instructionArray, int numOfLines, int *IC, char operands[MAXOPERANDS][MAXOPERANDLENGTH], symbolList** head, error** errorInfo){
    binaryWord newWord;
    int regNumSource, regNumDest;

    char firstOperand[MAXOPERANDLENGTH];
    char labelOrDefineFirst[MAXOPERANDLENGTH];
    char secondOperand[MAXOPERANDLENGTH];
    char labelOrDefineSecond[MAXOPERANDLENGTH];
    firstOperand[0] = '\0';
    labelOrDefineFirst[0] = '\0';
    secondOperand[0] = '\0';
    labelOrDefineSecond[0] = '\0';


    if (numOfLines>0) {
        parseOperandsSecondPass(operands[0], firstOperand, labelOrDefineFirst);
    }
    if (numOfLines>2) {
        parseOperandsSecondPass(operands[1], secondOperand, labelOrDefineSecond);
    }

    /*If the first operand is a register and the second is a register, special case where one word is being used*/
    if(isRegister(operands[0]) && isRegister(operands[1])){
        regNumSource = atoi(operands[0] + 1);
        regNumDest = atoi(operands[1] + 1);
        newWord.wordBits = (regNumSource << 5) | (regNumDest << 2);
        instructionArray[(*IC)+1] = newWord;
        return;
    }

    convertOperandToBinaryAndInsertIntoArray(instructionArray, (*IC)+1, firstOperand, head, errorInfo,1, 0);
    if (labelOrDefineFirst[0] != '\0') {
        convertOperandToBinaryAndInsertIntoArray(instructionArray, (*IC)+2, labelOrDefineFirst, head, errorInfo,0, 1);
        if (secondOperand[0] != '\0') {
            convertOperandToBinaryAndInsertIntoArray(instructionArray, (*IC) + 3, secondOperand, head, errorInfo, 0, 0);
            if (labelOrDefineSecond[0] != '\0') {
                convertOperandToBinaryAndInsertIntoArray(instructionArray, (*IC) + 4, labelOrDefineSecond, head, errorInfo, 0, 1);
            }
        }
    }
    else if (secondOperand[0] != '\0'){
        convertOperandToBinaryAndInsertIntoArray(instructionArray, (*IC)+2, secondOperand, head, errorInfo,0, 0);
        if (labelOrDefineSecond[0] != '\0') {
            convertOperandToBinaryAndInsertIntoArray(instructionArray, (*IC)+3, labelOrDefineSecond, head, errorInfo,0, 1);
        }
    }

}


/**
 * This function adds a value in binary to the data array.
 *
 * @param dataArray The data array to add the value to.
 * @param DC The data errorCounter.
 * @param value The value to add to the data array.
 */
void addValueToDataArray(binaryWord* dataArray, int DC, int value) {
    binaryWord newWord;
    newWord.wordBits = value;
    dataArray[DC] = newWord;
}


char* convertBinaryToBase4Symbols(int binary) {
    int i;
    char symbols[4] = {'*', '#', '%', '!'};
    int mask = 0x3;  /* Binary 11, to extract two bits at a time. */
    char* result = (char*)malloc(8);  /* Allocate on the heap. */
    int tempIndex = 7;  /* Start from the end of the result string. TODO make it constant?? */

    if (result == NULL) return NULL;  /* Check for malloc failure. */

    result[7] = '\0';  /* Null-terminate the string. */

    /* Iterate 7 times to process all 14 bits, accounting for leading zeros. */
    for (i = 0; i < 7; i++) {
        int currentDigit = binary & mask;
        binary >>= 2;  /* Move to the next base 4 digit. */
        /* Fill the result from the end to start */
        result[6 - i] = symbols[currentDigit];
    }

    return result;
}

/**
 * @file syntaxAnalyzer.h
 *
 * This file contains the prototypes for the syntax analyzer.
 * The different functions in the file, analyze the syntax of the assembly code, and handle the directives and operations.
 * It also checks if the label names are valid, and if the operands are valid.
 * The syntax analyzer is used in the first and second pass of the assembler.
 * It is by far the biggest file, with the most diverse functions.
 *
 * The functions in this file are:
 * 1. getOperandCode - This function gets the operand code for the instruction.
 * 2. handleOperation - This function handles the operations, and does different actions depending on if it's the first or second pass.
 * 3. handleData - This function handles the .data and .string directives, and adds the data to the data array.
 * 4. handleExtern - This function han   dles the .extern directive, and adds the label to the symbol table if it's valid.
 * 5. handleDefine - This function handles the .define directive, and adds the label to the symbol table if it's valid.
 * 6. parseOperandsFirstPass - This function parses the operands in the first pass of the assembler.
 * 7. parseOperandsSecondPass - This function parses the operands in the second pass of the assembler.
 * 8. trimWhitespace - This function trims the whitespace from a string.
 * 9. isDefine - This function checks if a string is a .define directive.
 * 10. isExtern - This function checks if a string is a .extern directive.
 * 11. isEntry - This function checks if a string is an .entry directive.
 * 12. isData - This function checks if a string is a .data directive.
 * 13. isString - This function checks if a string is a .string directive.
 * 14. isValidOperation - This function checks if a string is a valid operation.
 * 15. isValidLabelName - This function checks if a string is a valid label name.
 * 16. isValidInteger - This function checks if a string is a valid integer.
 * 17. isRegister - This function checks if a string is a register.
 * 18. checkEntrySyntax - This function checks the syntax of the .entry directive.
 * 19. checkLineLengthAndSkip - This function checks the length of the line and skips it if it's too long.
 */

#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include "dataStructuresAndConstants.h"
#include "listFunctions.h"
#include "error.h"
#include "initializationFunctions.h"
#include "dataConversion.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

/**
 * This function gets the operand code for the instruction.
 * * 0 = instant, constant (#number or #define)
 * 1 = direct, Label
 * 2 = constant index (meaning an offset number in an array (it must be of a label, and the offset must be a number / define THAT WAS DEFINED)
 * 3 = register, one of the registers.
 *
 * @param operand The operand to be checked.
 * @param head The symbol list.
 * @param operationsArray The operations array.
 * @param errorInfo The error struct.
 * @return The operand code.
 */
int getOperandCode(char* operand, symbolList** head, operationInfo* operationsArray, error** errorInfo);

/**
 * This function handles the operations, and does different actions depending on if it's the first or second pass.
 * first pass - only checks for syntax errors, adds the first word only and updates the IC
 * second pass - adds the operands to the instruction array (to the reserved spaces after the first word)
 * @param head - the symbol table
 * @param instructionArray - the array of instructions
 * @param opcode - the operation code of our operation
 * @param line - the current line text
 * @param IC - the instruction errorCounter
 * @param operationsArray - the array of operations
 * @param errorInfo - the error struct
 * @param isSecondPass - a flag to signal if it's the second pass, and use different actions accordingly
 * @return the length of the operation or INSTRUCTION_FAIL_CODE if the operation failed
 */
int handleOperation(symbolList** head, memoryWord* instructionArray, int operation, char* line, int *IC, operationInfo* operationsArray, error** errorInfo, int isSecondPass);

/**
 * This function handles the .data and .string directives, and adds their data to the data array and to the symbol table.
 * @param type - the type of directive (data or string)
 * @param line - the current line text
 * @param head - the symbol table
 * @param DC - the data errorCounter
 * @param dataArray - the array of data
 * @param errorInfo - the error struct
 */
void handleData(char* type, char* labelName, char* line, symbolList ** head, int *DC, memoryWord* dataArray, error** errorInfo);

/**
 * This function handles the .extern directive, and adds the label to the symbol table if it's valid
 * @param head - the symbol table
 * @param line - the current line text
 * @param errorInfo - the error struct
 * @param operationsArray - the array of operations
 * @param labelFlag - a flag to signal if the first word is a label
 */
void handleExtern(symbolList** head, char* line, error** errorInfo, operationInfo* operationsArray, int labelFlag);

/**
 * This function handles the .define directive, and adds the label to the symbol table if it's valid
 * @param head - the symbol table
 * @param operationsArray - the array of operations
 * @param line - the current line text
 * @param errorInfo - the error struct
 */
void handleDefine(symbolList** head, operationInfo* operationsArray, char* line, error** errorInfo);

/**
 * This function checks the syntax of the .entry directive, as well as if it's been already been defined as .extern.
 * It does not add the label to the symbol table, as it's only used for checking the syntax.
 * @param head The symbol list.
 * @param line The line to be checked.
 * @param errorInfo The error struct.
 * @param operationsArray The operations array.
 */
void checkEntrySyntax(symbolList** head, char* line, error** errorInfo, operationInfo* operationsArray, int labelFlag);

/**
 * This function parses the operands in the first pass of the assembler.
 * @param line The line to be parsed.
 * @param operands The array to store the operands.
 * @param errorInfo The error struct.
 * @return true if the parsing was successful, false otherwise.
 */
int parseOperandsFirstPass(char *input, char operands[MAX_OPERANDS][MAX_OPERAND_LENGTH], error** errorInfo);

/**
 * This function parses the operands in the second pass of the assembler.
 * @param operand The operand to be parsed.
 * @param outOperand The output operand.
 * @param outLabelOrDefine The output label or define.
 */
void parseOperandsSecondPass(const char* operand, char* outOperand, char* outLabelOrDefine);

/**
 * This function trims the whitespace before and after the string.
 * @param str The string to be trimmed.
 */
void trimWhitespace(char* str);

/**
 * This function checks if the word is a .define directive.
 * @param word The word to be checked.
 * @return true if the word is a .define directive, false otherwise.
 */
int isDefine(char* word);

/**
 * This function checks if the word is a .data directive.
 * @param word The word to be checked.
 * @return true if the word is a .data directive, false otherwise.
 */
int isData(char* word);

/**
 * This function checks if the word is a .string directive.
 * @param word The word to be checked.
 * @return true if the word is a .string directive, false otherwise.
 */
int isString(char* word);

/**
 * This function checks if the word is a .extern directive.
 * @param word The word to be checked.
 * @return true if the word is a .extern directive, false otherwise.
 */
int isExtern(char* word);

/**
 * This function checks if the word is an .entry directive.
 * @param word The word to be checked.
 * @return true if the word is an .entry directive, false otherwise.
 */
int isEntry(char* word);

/**
 * This function checks if the label name is valid.
 * The rules are:
 * Not a reserved word, not a register, not a number, not a defined symbol, not a label that already exists.
 * As well as the first character being an alphabetical character, and the rest of the characters being either uppercase or digits.
 * And the name being 31 characters at most.
 * @param name The label name to be checked.
 * @param operationsArray The operations array.
 * @param head The symbol list.
 * @param colonFlag A flag indicating if the label has a colon.
 * @return true if the label name is valid, false otherwise.
 */
int isValidLabelName(char* name, operationInfo* operationsArray, symbolList** head, int colonFlag);

/**
 * This function checks if the word is a valid operation.
 * @param word The word to be checked.
 * @param operationsArray The operations array.
 * @return The index of the operation in the operations array if the word is a valid operation, -1 otherwise.
 */
int isValidOperation(char* word, operationInfo* operationsArray);

/**
 * This function checks if the string is a valid integer.
 * @param str The string to be checked.
 * @return true if the string is a valid integer, false otherwise.
 */
int isValidInteger(char* str);

/**
 * This function checks if the word is a register.
 * @param word The word to be checked.
 * @return true if the word is a register, false otherwise.
 */
int isRegister(char* word);

/**
 * This function checks if the current line exceeds the maximum allowed length.
 * If so, it consumes the rest of the line and prints an appropriate error.
 * Returns 0 if the line is too long (and was skipped), or 1 if the line length is valid.
 */
int checkLineLengthAndSkip(FILE *sourceFile, char *lineBuffer, error** errorInfo);

#endif

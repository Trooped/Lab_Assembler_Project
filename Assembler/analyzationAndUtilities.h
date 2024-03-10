#ifndef ANALYZATIONANDUTILITIES_H
#define ANALYZATIONANDUTILITIES_H

#include "assembler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "firstPass.h"


int isDefine(char* word);
int isLabel(char* word);
int isData(char* word);
int isString(char* word);
int isExtern(char* word);
int isEntry(char* word);
int isValidName(char* name);
int isValidOperation(char* word, operation* operationsArray);
int isValidInteger(char* word);





#endif

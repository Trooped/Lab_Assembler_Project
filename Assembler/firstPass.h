#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "../PreAsmblr/PreAsmblr.h"

#ifndef firstPass_H
#define firstPass_H

typedef struct LabelNode {
    char name[MAXNAME]; /* Label name TODO define MAXNAME already omg*/
    int value; /* Label value*/
    struct LabelNode* next; /* Pointer to the next node*/
} LabelNode;










#endif



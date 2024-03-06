#include "firstPass.h"


/*TODO rename this function.*/
void mainFunction(FILE *sourceFile, char *fileName){
    int IC =0;
    int DC =0;
    char lineBuffer[MAXCHARSPERLINE]; /*TODO define a maxcharsperlines in this firstPass maybe?*/
    char word[MAXCHARSPERWORD]; /*TODO define it as 30 or something? IDK*/

    while (fgets(lineBuffer, sizeof(lineBuffer), sourceFile)) {
        word = strtok(lineBuffer, " \n\r\t"); /* Tokenize the line into words*/
        while (word != NULL) {
            if (isDefine(word)){
                /*TODO according to line 3*/
            }
            else if (isLabel(word)){
                /*TODO according to line 5*/
            }
            else if (isDataOrString(word)){
                /*TODO according to line 7*/
            }
            else if (isExtern(word)){
                /*TODO according to line 9*/
            }
            else if (isEntry(word)){
                /*TODO according to line 11*/
            }
            else{
                /*TODO do i need to do something here? lol*/
            }
            word = strtok(NULL, " \n\r\t"); /* Get the next word.*/
        }
    }
    /*MOVE TO 16, ACCORDING TO THE COURSE'S ALGORITHM.*/


}


/* Function to add a new node at the end of the list */
LabelNode* addLabelNode(LabelNode* head, char* name, int value) {
    LabelNode* newNode = (LabelNode*)malloc(sizeof(LabelNode));
    if(newNode == NULL) {
        /* If malloc fails, print an error and exit TODO DONT EXIT!!! FIND OUT WHAT TO DO DIFFERENTLY*/
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    /* Copy the name and value into the new node */
    strncpy(newNode->name, name, MAXNAME);
    newNode->value = value;
    newNode->next = NULL;

    if (head == NULL) {
        /* If the list is empty, the new node becomes the head of the list */
        head = newNode;
    } else {
        /* If the list is not empty, find the last node and link the new node to it */
        LabelNode* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    /* Return the head of the list */
    return head;
}

/* Function to delete a node */
void deleteLabelNode(LabelNode* node) {
    /* Free the memory allocated for the node */
    free(node);
}

/* Function to delete the entire list */
void deleteLabelList(LabelNode* head) {
    /* Loop through the list and delete each node */
    LabelNode* current = head;
    LabelNode* nextNode;
    while (current != NULL) {
        nextNode = current->next;
        deleteLabelNode(current);
        current = nextNode;
    }
}

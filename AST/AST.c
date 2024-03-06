#include "AST.h"




AST* createASTNode(char* line) {
    /* Allocate memory for a new AST node*/
    AST* node = (AST*)malloc(sizeof(AST));
    if(node == NULL) {
        fprintf(stderr, "Out of memory\n");
        exit(1);/*TODO different exit or something*/
    }

    // Parse the line and fill in the details of the node
    // This is a simplified example and you'll need to expand upon it based on your specific needs
    sscanf(line, "%s %s", node->name, node->wordSummary.command.opcode);






    // Return the node
    return node;
}


void buildAST(FILE* source, AST** root) {
    char line[MAX_LINE_LENGTH];

    // Read the source file line by line
    while (fgets(line, sizeof(line), source)) {
        // Create an AST node for the line
        AST* node = createASTNode(line);

        // Add the node to the AST
        // In this example, we're simply setting the root to the node for simplicity
        // In a real scenario, you would need to properly link the nodes to form the AST
        *root = node;
    }
}

void traverseAST(AST* root) {
    // Check if the root is NULL
    if (root == NULL) {
        return;
    }

    // Perform operations on the node
    // In this example, we're simply printing the node's name and opcode for simplicity
    printf("Name: %s, Opcode: %s\n", root->name, root->wordSummary.command.opcode);

    // Recursively traverse the rest of the AST
    // In this example, we're assuming the AST is a linked list for simplicity
    // In a real scenario, you would need to traverse the AST based on its actual structure
    traverseAST(root->next);
}


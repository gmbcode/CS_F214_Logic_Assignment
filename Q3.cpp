#include "Q3.h"
#include <iostream>
#include <cctype> // For isalnum
// Helper function to check if a character is a binary operator
static bool isBinaryOperator(char c) {
    return (c == '+' || c == '*' || c == '>');
}

// Helper function to check if a character is a unary operator
static bool isUnaryOperator(char c) {
    return c == '~';
}
void printInfix(TreeNode* root) {
    if (root == nullptr) {
        return;
    }

    // If it's an operator, print an opening parenthesis
    if (isBinaryOperator(root->data) || isUnaryOperator(root->data)) {
        std::cout << "(";
    }

    // Recurse on the left child
    printInfix(root->left);

    // Print the node's data
    std::cout << root->data;

    // Recurse on the right child
    printInfix(root->right);

    // If it's an operator, print a closing parenthesis
    if (isBinaryOperator(root->data) || isUnaryOperator(root->data)) {
        std::cout << ")";
    }
}
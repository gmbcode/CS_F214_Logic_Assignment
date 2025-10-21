// Q3.cpp (updated)
#include "Q3.h"
#include "Q2.h"
#include <iostream>

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
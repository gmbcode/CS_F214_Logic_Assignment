/**
* @file Q3.cpp
 * @brief Implementation file for printing an expression parse tree in infix notation.
 *
 * This file contains the functionality to traverse a parse tree, constructed
 * from a logical expression, and print its corresponding fully-parenthesized
 * infix representation to standard output.
 */

#include "Q3.h"
#include "Q2.h" // For TreeNode, isBinaryOperator, isUnaryOperator
#include <iostream>

/**
 * @brief Prints the infix representation of a parse tree using in-order traversal.
 *
 * This function recursively traverses the given expression tree. It prints an opening
 * parenthesis before visiting the children of an operator node, then prints the left child,
 * the operator itself, the right child, and finally a closing parenthesis. This ensures
 * that the resulting expression is fully parenthesized to preserve the original
 * operator precedence.
 *
 * @param root A pointer to the root node of the parse tree (or subtree) to be printed.
 */
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
    std::cout << " " << root->data << " ";

    // Recurse on the right child
    printInfix(root->right);

    // If it's an operator, print a closing parenthesis
    if (isBinaryOperator(root->data) || isUnaryOperator(root->data)) {
        std::cout << ")";
    }
}
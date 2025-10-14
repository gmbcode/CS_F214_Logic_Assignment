#include "Q2.h"
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

// Recursive function to build the tree.
// It processes the string and advances the index.
static TreeNode* buildTreeRecursive(const std::string& prefix, int& index) {
    // Base case: if we are out of bounds, return null
    if (index >= prefix.length()) {
        return nullptr;
    }

    // 1. Get the current character and advance the index
    char currentChar = prefix[index];
    index++;

    // 2. Create a new node with this character
    TreeNode* node = new TreeNode(currentChar);

    // 3. Recursively build the rest of the tree
    if (isBinaryOperator(currentChar)) {
        // If it's a binary operator, it must have two children.
        node->left = buildTreeRecursive(prefix, index);
        node->right = buildTreeRecursive(prefix, index);
    } else if (isUnaryOperator(currentChar)) {
        // If it's a unary operator, it has one child (we'll use the left).
        node->left = nullptr;
        node->right = buildTreeRecursive(prefix, index);; // Right child is always null for unary ops.
    }
    // else, it's an operand (a leaf node), and its children are already nullptr.

    return node;
}

// Public-facing function to start the tree-building process.
TreeNode* buildParseTree(const std::string& prefix) {
    if (prefix.empty()) {
        return nullptr;
    }
    int index = 0;
    return buildTreeRecursive(prefix, index);
}

// Prints the tree using an in-order traversal to get infix notation.


// Deletes the tree using a post-order traversal to avoid dangling pointers.
void deleteTree(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

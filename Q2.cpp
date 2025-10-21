// Q2.cpp
#include "Q2.h"
#include <iostream>
#include <cctype>

// Helper function to check if a string is a binary operator
bool isBinaryOperator(const string& s) {
    return (s == "+" || s == "*" || s == ">");
}

// Helper function to check if a string is a unary operator
bool isUnaryOperator(const string& s) {
    return s == "~";
}

// Helper function to extract the next token from prefix string
static string getNextToken(const string& prefix, int& index) {
    if (index >= prefix.length()) {
        return "";
    }

    // Skip whitespace
    while (index < prefix.length() && isspace(prefix[index])) {
        index++;
    }

    if (index >= prefix.length()) {
        return "";
    }

    // Check for operators (single character)
    char c = prefix[index];
    if (c == '+' || c == '*' || c == '>' || c == '~' || c == '(' || c == ')') {
        index++;
        return string(1, c);
    }

    // Extract variable name (alphanumeric sequence)
    string token = "";
    while (index < prefix.length() && (isalnum(prefix[index]) || prefix[index] == '_')) {
        token += prefix[index];
        index++;
    }

    return token;
}

// Recursive function to build the tree
static TreeNode* buildTreeRecursive(const string& prefix, int& index) {
    // Get the next token
    string token = getNextToken(prefix, index);

    if (token.empty()) {
        return nullptr;
    }

    // Create a new node with this token
    TreeNode* node = new TreeNode(token);

    // Recursively build the rest of the tree
    if (isBinaryOperator(token)) {
        // Binary operator: has two children
        node->left = buildTreeRecursive(prefix, index);
        node->right = buildTreeRecursive(prefix, index);
    } else if (isUnaryOperator(token)) {
        // Unary operator: has one child (stored in right)
        node->left = nullptr;
        node->right = buildTreeRecursive(prefix, index);
    }
    // else, it's an operand (leaf node), children are already nullptr

    return node;
}

// Public-facing function to start the tree-building process
TreeNode* buildParseTree(const string& prefix) {
    if (prefix.empty()) {
        return nullptr;
    }
    int index = 0;
    return buildTreeRecursive(prefix, index);
}

// Deletes the tree using post-order traversal
void deleteTree(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}
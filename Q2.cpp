/**
 * @file Q2.cpp
 * @brief Implementation file for building and managing a parse tree from a prefix propositional logic expression.
 *
 * This file contains the logic to parse a prefix expression string and construct
 * an corresponding expression tree. It includes helper functions for tokenizing the
 * string and identifying operator types, as well as functions to build and delete the tree structure.
 */

#include "Q2.h"
#include <iostream>
#include <cctype>

/**
 * @brief Checks if a given string is a binary logical operator.
 *
 * Binary operators in this context are `+` (OR), `*` (AND), and `>` (Implication).
 *
 * @param s The string token to check.
 * @return true if the token is a binary operator.
 * @return false otherwise.
 */
bool isBinaryOperator(const string& s) {
    return (s == "+" || s == "*" || s == ">");
}

/**
 * @brief Checks if a given string is a unary logical operator.
 *
 * The only unary operator in this context is `~` (Negation).
 *
 * @param s The string token to check.
 * @return true if the token is a unary operator.
 * @return false otherwise.
 */
bool isUnaryOperator(const string& s) {
    return s == "~";
}

/**
 * @brief Extracts the next token from a prefix expression string.
 *
 * This is a static helper function that scans the string from the current index,
 * skips any leading whitespace, and extracts the next complete token, which can be
 * an operator, parenthesis, or a variable name. The index is advanced past the
 * extracted token.
 *
 * @param prefix The input prefix expression string.
 * @param index A reference to the current position (index) in the string. This value is modified by the function.
 * @return string The next token found in the string. Returns an empty string if the end of the string is reached.
 */
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

/**
 * @brief Recursively builds a parse tree from a prefix expression.
 *
 * This static helper function reads one token from the prefix string to create a node.
 * If the token is an operator, it recursively calls itself to build the required
 * child subtrees (two for a binary operator, one for a unary operator).
 * If the token is an operand, it creates a leaf node.
 *
 * @param prefix The input prefix expression string.
 * @param index A reference to the current parsing position in the string.
 * @return TreeNode* A pointer to the root of the constructed subtree.
 */
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

/**
 * @brief Builds a parse tree from a given prefix expression string.
 *
 * This is the public-facing function that initiates the tree-building process.
 * It handles empty input and initializes the starting index for the recursive helper function.
 *
 * @param prefix The complete prefix expression string (e.g., "> p * q ~ r").
 * @return TreeNode* A pointer to the root of the newly constructed parse tree. Returns nullptr if the input string is empty.
 */
TreeNode* buildParseTree(const string& prefix) {
    if (prefix.empty()) {
        return nullptr;
    }
    int index = 0;
    return buildTreeRecursive(prefix, index);
}

/**
 * @brief Deletes an entire parse tree and frees its memory.
 *
 * This function traverses the tree in a post-order fashion to ensure that
 * child nodes are deleted before their parent node, preventing memory leaks.
 *
 * @param root A pointer to the root node of the tree to be deleted.
 */
void deleteTree(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}
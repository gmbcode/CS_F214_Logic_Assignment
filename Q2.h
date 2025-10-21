// Q2.h
#ifndef Q2_H
#define Q2_H
#include <string>
#include <cctype>

using std::string;

// Structure for a node in the binary parse tree
struct TreeNode {
    string data;  // Changed from char to string
    TreeNode* left;
    TreeNode* right;

    TreeNode(const string& val) : data(val), left(nullptr), right(nullptr) {}
    TreeNode(char val) : data(1, val), left(nullptr), right(nullptr) {}
};

// Helper functions
bool isBinaryOperator(const string& s);
bool isUnaryOperator(const string& s);

TreeNode* buildParseTree(const std::string& prefix);
void deleteTree(TreeNode* root);

#endif //Q2_H
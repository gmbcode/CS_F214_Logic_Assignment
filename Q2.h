#ifndef Q2_H
#define Q2_H
#include<string>
#include <cctype>

using std::string;
// Structure for a node in the binary parse tree
struct TreeNode {
    char data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(char val) : data(val), left(nullptr), right(nullptr) {}
    TreeNode(string v) {
        data = v[0];
        left = nullptr;
        right = nullptr;
    }
};
TreeNode* buildParseTree(const std::string& prefix);
void deleteTree(TreeNode* root);
#endif //Q2_H

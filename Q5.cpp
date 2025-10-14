#include "Q2.h" // Include our Tree Struct
#include "Q5.h"
#include <stdexcept> // For std::runtime_error
#include <cctype>    // For isalnum
#include <string>
#include <map>

bool evaluateTruthValue(TreeNode* root, const std::map<char, bool>& truthValues) {
    if (root == nullptr) {
        return 0;
    }


    if (isalnum(root->data)) {
        try {
            return truthValues.at(root->data);
        }
        catch (const std::out_of_range& e) {
            throw std::runtime_error("Truth value for atom '" + std::string(1, root->data) + "' not found in map.");
        }
    }


    bool rightValue = evaluateTruthValue(root->right, truthValues);
    if (root->data == '~') {
        return !rightValue;
    }
    bool leftValue = evaluateTruthValue(root->left, truthValues);
    switch (root->data) {
        case '+':
            return leftValue || rightValue;
        case '*':
            return leftValue && rightValue;
        case '>':
            return !leftValue || rightValue;
        default:
            throw std::runtime_error("Unknown operator in tree: '" + std::string(1, root->data) + "'");
    }
}
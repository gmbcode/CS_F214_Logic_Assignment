#include "Q2.h" // Include our Tree Struct
#include "Q5.h"
#include <stdexcept> // For std::runtime_error
#include <cctype>    // For isalnum
#include <string>
#include <map>

using std::string;
using std::map;

bool evaluateTruthValue(TreeNode* root, const map<string, bool>& truthValues) {
    if (root == nullptr) {
        return false;
    }

    // Check if it's an operator
    if (root->data == "~" || root->data == "+" ||
        root->data == "*" || root->data == ">") {

        // Handle unary negation operator
        if (root->data == "~") {
            bool rightValue = evaluateTruthValue(root->right, truthValues);
            return !rightValue;
        }

        // Handle binary operators
        bool leftValue = evaluateTruthValue(root->left, truthValues);
        bool rightValue = evaluateTruthValue(root->right, truthValues);

        if (root->data == "+") {
            return leftValue || rightValue;
        } else if (root->data == "*") {
            return leftValue && rightValue;
        } else if (root->data == ">") {
            return !leftValue || rightValue;
        }
        }

    // It's a variable/atom - look up its truth value
    try {
        return truthValues.at(root->data);
    }
    catch (const std::out_of_range& e) {
        throw std::runtime_error("Truth value for atom '" + root->data + "' not found in map.");
    }
}


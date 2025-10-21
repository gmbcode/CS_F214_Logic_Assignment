#include "Q2.h" // Include our Tree Struct
#include "Q5.h"
#include <stdexcept> // For std::runtime_error
#include <cctype>    // For isalnum
#include <string>
#include <map>
#include<iostream>
#include <unordered_map>
#include <set>
#include <vector>
using std::string;
using std::map;
using std::set;
using std::vector;
using std::unordered_map;
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
// Evaluate truth table value
bool evaluate(TreeNode* r, unordered_map<string, bool>& val) {
    if (!r) return false;

    string data = r->data;

    // Check if it's an operator
    if (data == "~") {
        // Negation operator - operand in right child
        return !evaluate(r->right, val);
    }
    if (data == "*") {
        return evaluate(r->left, val) && evaluate(r->right, val);
    }
    if (data == "+") {
        return evaluate(r->left, val) || evaluate(r->right, val);
    }
    if (data == ">") {
        return (!evaluate(r->left, val)) || evaluate(r->right, val);
    }

    // It's a variable - look up its value
    if (val.find(data) != val.end()) {
        return val[data];
    }

    // Variable not found in map
    return false;
}
// ---------- Truth Table -----------
void printTruthTable(TreeNode *r, set<string> &vars) {
    using std::cout;
    cout<<"Truth Table :- "<<std::endl;
    vector<string> varList(vars.begin(), vars.end());
    int n = varList.size();
    for (string c : varList){
        cout << "   " << c << "   |";
    }
    cout << "  value   " << std::endl;
    cout << "---------------------------------" << std::endl;

    int rows = 1<<n; // 2^n
    unordered_map<string,bool> val;

    for(int i=0; i<rows; i++){
        for (int j=n-1; j>=0; j--){
            int x = ((i>>j) & 1);
            cout << "   " << x << "   |";
            val[varList[n-j-1]] = x;
        }
        cout << "    " << evaluate(r, val) << "   " << std::endl;
    }
}


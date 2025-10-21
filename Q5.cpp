/**
 * @file Q5.cpp
 * @brief Implementation file for evaluating and generating truth tables for expression parse trees.
 *
 * This file contains functions to recursively evaluate the boolean value of a
 * parse tree given a specific assignment of truth values to its variables.
 * It also includes functionality to generate and print a complete truth table for the expression.
 */

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

/**
 * @brief Evaluates the truth value of an expression tree for a single assignment of truth values.
 *
 * This function recursively traverses the tree. For operator nodes, it computes the result
 * based on the values of its children. For variable (leaf) nodes, it looks up the value
 * in the provided map.
 *
 * @param root A pointer to the root of the parse tree (or subtree) to be evaluated.
 * @param truthValues A constant map where keys are variable names (strings) and values are their assigned boolean truth values.
 * @return true if the expression evaluates to true for the given assignment.
 * @return false otherwise.
 * @throw std::runtime_error if a variable encountered in the tree is not found in the `truthValues` map.
 */
bool evaluateTruthValue(TreeNode* root, const map<string, bool>& truthValues) {
    if (root == nullptr) {
        // Base case for empty subtree, behavior can be defined as needed. Here, returning false.
        return false;
    }

    // Check if the node's data represents an operator
    if (root->data == "~" || root->data == "+" ||
        root->data == "*" || root->data == ">") {

        // Handle the unary negation operator
        if (root->data == "~") {
            bool rightValue = evaluateTruthValue(root->right, truthValues);
            return !rightValue;
        }

        // Handle binary operators by evaluating left and right children
        bool leftValue = evaluateTruthValue(root->left, truthValues);
        bool rightValue = evaluateTruthValue(root->right, truthValues);

        if (root->data == "+") { // OR
            return leftValue || rightValue;
        } else if (root->data == "*") { // AND
            return leftValue && rightValue;
        } else if (root->data == ">") { // Implication
            return !leftValue || rightValue;
        }
    }

    // If not an operator, it must be a variable/atom. Look up its truth value.
    try {
        return truthValues.at(root->data);
    }
    catch (const std::out_of_range& e) {
        // Throw an error if a variable's truth value isn't provided in the map.
        throw std::runtime_error("Truth value for atom '" + root->data + "' not found in map.");
    }
    return false; // Should not be reached
}

/**
 * @brief Helper function to evaluate the tree for a specific row in the truth table.
 *
 * This is a recursive helper function used by `printTruthTable`. It evaluates the
 * expression tree based on the current assignment of boolean values in the `val` map.
 * It assumes all variables in the tree exist in the map.
 *
 * @param r A pointer to the root of the tree (or subtree).
 * @param val A reference to an unordered_map containing the current truth value assignments for variables.
 * @return true if the expression evaluates to true.
 * @return false otherwise.
 */
bool evaluate(TreeNode* r, unordered_map<string, bool>& val) {
    if (!r) return false;

    string data = r->data;

    // Evaluate based on the operator type
    if (data == "~") {
        // Negation operator - operand is in the right child
        return !evaluate(r->right, val);
    }
    if (data == "*") { // AND
        return evaluate(r->left, val) && evaluate(r->right, val);
    }
    if (data == "+") { // OR
        return evaluate(r->left, val) || evaluate(r->right, val);
    }
    if (data == ">") { // Implication
        return (!evaluate(r->left, val)) || evaluate(r->right, val);
    }

    // If not an operator, it's a variable. Look up its value.
    if (val.find(data) != val.end()) {
        return val[data];
    }

    // Should not happen if all variables are in the map.
    return false;
}

/**
 * @brief Generates and prints a complete truth table for the given expression tree.
 *
 * This function first prints a header with all the variable names. It then iterates
 * through all possible combinations of truth values (2^n rows, where n is the number of variables).
 * For each combination, it evaluates the expression and prints the result.
 *
 * @param r A pointer to the root of the expression tree.
 * @param vars A reference to a set of strings containing all unique variable names in the expression.
 */
void printTruthTable(TreeNode *r, set<string> &vars) {
    using std::cout;
    cout<<"Truth Table :- "<<std::endl;
    vector<string> varList(vars.begin(), vars.end());
    int n = varList.size();

    // Print table header
    for (const string& c : varList){
        cout << "   " << c << "   |";
    }
    cout << "  Result  " << std::endl;
    cout << "---------------------------------" << std::endl;

    int rows = 1 << n; // 2^n combinations
    unordered_map<string, bool> val;

    // Iterate through each row of the truth table
    for(int i = 0; i < rows; i++){
        // Generate truth values for this row
        for (int j = n - 1; j >= 0; j--){
            bool current_val = ((i >> j) & 1);
            cout << "   " << current_val << "   |";
            val[varList[n-j-1]] = current_val;
        }
        // Evaluate and print the result for this row
        cout << "    " << evaluate(r, val) << "   " << std::endl;
    }
}
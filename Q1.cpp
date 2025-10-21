// Q1.cpp
/**
 * @file Q1.cpp
 * @brief Implementation file for converting infix propositional logic expressions to prefix notation.
 *
 * Contains helper functions for operator priority, operator identification,
 * token extraction, and the main infix-to-prefix conversion algorithm.
 */

#include "Q1.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <stack>
#include <cctype> // For isalnum

using std::string;
using std::vector;
using std::stack;
using std::reverse;

/**
 * @brief Returns the priority (precedence) of a logical operator.
 *
 * Higher numbers indicate higher priority.
 * - `~` (Negation): 4
 * - `*` (AND): 3
 * - `+` (OR): 2
 * - `>` (Implication): 1
 *
 * @param op The operator string (e.g., "~", "*", "+", ">").
 * @return int The priority level of the operator, or -1 if the string is not a recognized operator.
 */
int priority_order(const string& op) {
    if (op == "~") return 4; // Negation
    if (op == "*") return 3; // AND
    if (op == "+") return 2; // OR
    if (op == ">") return 1; // Implication
    return -1; // Not an operator
}

/**
 * @brief Checks if a given string is a recognized logical operator.
 *
 * Recognized operators are: `~` (Negation), `*` (AND), `+` (OR), `>` (Implication).
 *
 * @param s The string to check.
 * @return true if the string is one of the recognized operators, false otherwise.
 */
bool isOperator(const string& s) {
    return (s == "~" || s == "*" || s == "+" || s == ">");
}

/**
 * @brief Extracts the next token (variable, operator, or parenthesis) from an infix expression string.
 *
 * This function handles whitespace skipping and can scan either left-to-right or right-to-left.
 * Variables are strings of alphanumeric characters and underscores.
 * Operators and parentheses are single-character tokens.
 *
 * @param infix The infix expression string.
 * @param index A reference to the current position (index) in the string. This index will be modified
 * to point past the extracted token (or before it, if scanning in reverse).
 * @param reverse A boolean flag. If false (default), scans left-to-right (index increases).
 * If true, scans right-to-left (index decreases).
 * @return string The extracted token. Returns an empty string if the end (or beginning, if reverse)
 * of the string is reached.
 */
string getNextToken(const string& infix, int& index, bool reverse = false) {
    if (reverse) {
        // For right-to-left scanning
        if (index < 0) return "";

        // Skip whitespace
        while (index >= 0 && isspace(infix[index])) {
            index--;
        }

        if (index < 0) return "";

        char c = infix[index];

        // Check for operators or parentheses (single character)
        if (c == '+' || c == '*' || c == '>' || c == '~' || c == '(' || c == ')') {
            index--;
            return string(1, c);
        }

        // Extract variable name (reading backwards)
        string token = "";
        while (index >= 0 && (isalnum(infix[index]) || infix[index] == '_')) {
            token = infix[index] + token;  // Prepend character
            index--;
        }

        return token;
    } else {
        // For left-to-right scanning
        if (index >= infix.length()) return "";

        // Skip whitespace
        while (index < infix.length() && isspace(infix[index])) {
            index++;
        }

        if (index >= infix.length()) return "";

        char c = infix[index];

        // Check for operators or parentheses (single character)
        if (c == '+' || c == '*' || c == '>' || c == '~' || c == '(' || c == ')') {
            index++;
            return string(1, c);
        }

        // Extract variable name
        string token = "";
        while (index < infix.length() && (isalnum(infix[index]) || infix[index] == '_')) {
            token += infix[index];
            index++;
        }

        return token;
    }
}

/**
 * @brief Converts a propositional logic expression from infix notation to prefix (Polish) notation.
 *
 * This implementation uses a modified shunting-yard algorithm that scans the
 * infix string from right to left to produce the prefix notation.
 * Tokens (operators, operands, parentheses) are expected to be separated by spaces
 * in the output string.
 *
 * @param infix The infix expression string (e.g., "p1 + ~ q1").
 * @return string The equivalent prefix expression string (e.g., "+ p1 ~ q1").
 */
string infixToPrefix(string infix) {
    stack<string> st;
    vector<string> prefix_tokens;

    // 1. Scan the infix expression from right to left
    int i = infix.length() - 1;
    while (i >= 0) {
        string token = getNextToken(infix, i, true);  // true = reverse scanning

        if (token.empty()) break;

        // If the token is an operand (a variable), add it to the result
        if (!isOperator(token) && token != "(" && token != ")") {
            prefix_tokens.push_back(token);
        }
        // If the token is a closing parenthesis ')', push it onto the stack
        else if (token == ")") {
            st.push(token);
        }
        // If the token is an opening parenthesis '(', pop from stack until ')' is found
        else if (token == "(") {
            while (!st.empty() && st.top() != ")") {
                prefix_tokens.push_back(st.top());
                st.pop();
            }
            if (!st.empty()) {
                st.pop(); // Pop the ')'
            }
        }
        // If an operator is encountered
        else if (isOperator(token)) {
            // CORRECTED: Pop operators with GREATER OR EQUAL priority
            // For right-to-left scan, we need >= for correct associativity
            while (!st.empty() &&
                   st.top() != ")" &&
                   priority_order(st.top()) >= priority_order(token)) {
                prefix_tokens.push_back(st.top());
                st.pop();
            }
            st.push(token);
        }
    }

    // Pop all remaining operators from the stack
    while (!st.empty()) {
        prefix_tokens.push_back(st.top());
        st.pop();
    }

    // 2. Reverse the result to get the final prefix expression
    reverse(prefix_tokens.begin(), prefix_tokens.end());

    // 3. Join tokens with spaces
    string prefix_expr = "";
    for (size_t j = 0; j < prefix_tokens.size(); j++) {
        prefix_expr += prefix_tokens[j];
        if (j < prefix_tokens.size() - 1) {
            prefix_expr += " ";  // Add space between tokens
        }
    }

    return prefix_expr;
}
// Q1.cpp
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

// Returns the priority of a logical operator
int priority_order(const string& op) {
    if (op == "~") return 4; // Negation
    if (op == "*") return 3; // AND
    if (op == "+") return 2; // OR
    if (op == ">") return 1; // Implication
    return -1; // Not an operator
}

// Checks if a string is a logical operator
bool isOperator(const string& s) {
    return (s == "~" || s == "*" || s == "+" || s == ">");
}

// Helper function to extract the next token from infix string
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

// Infix to Prefix conversion for Propositional Logic expressions
string infixToPrefix(string infix) {
    stack<string> st;  // Changed from stack<char> to stack<string>
    vector<string> prefix_tokens;  // Store tokens instead of characters

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
            // Pop operators with higher priority
            while (!st.empty() && priority_order(st.top()) > priority_order(token)) {
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
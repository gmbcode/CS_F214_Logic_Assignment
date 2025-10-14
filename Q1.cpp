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


int priority_order(char op) {// Returns the priority_order of a logical operator.
    if (op == '~') return 4; // Negation
    if (op == '*') return 3; // AND
    if (op == '+') return 2; // OR
    if (op == '>') return 1; // Implication
    return -1; // Not an operator
}

// Checks if a character is a logical operator.
bool isOperator(char c) {
    return (c == '~' || c == '*' || c == '+' || c == '>');
}

// Infix to Prefix conversion for Propositional Logic expressions.
string infixToPrefix(string infix) {
    stack<char> st;
    string prefix_expr;

    // 1. Scan the infix expression from right to left.
    for (int i = infix.length() - 1; i >= 0; i--) {
        char c = infix[i];

        // If the character is an operand (a proposition), add it to the result.
        if (isalnum(c)) {
            prefix_expr += c;
        }
        // If the character is a closing parenthesis ')', push it onto the stack.
        else if (c == ')') {
            st.push(c);
        }
        // If the character is an opening parenthesis '(', pop from stack until ')' is found.
        else if (c == '(') {
            while (!st.empty() && st.top() != ')') {
                prefix_expr += st.top();

                st.pop();
            }
            if (!st.empty()) {
                st.pop(); // Pop the ')'
            }
        }
        // If an operator is encountered.
        else if (isOperator(c)) {
            // NOTE: For a right-to-left scan, we pop when the operator on the stack
            // has HIGHER priority_order than the current operator.
            // For operators with equal priority_order, we consider associativity.
            // ~, *, +, > are all left-associative, so we pop. Negation is unary but its
            // position makes this rule work.
            while (!st.empty() && priority_order(st.top()) > priority_order(c)) {
                prefix_expr += st.top();
                st.pop();
            }
            st.push(c);
        }
        //std::cout << "Prefix_expr: at index  " << i << " is " << prefix_expr << std::endl;
    }

    // Pop all remaining operators from the stack.
    while (!st.empty()) {
        prefix_expr += st.top();
        st.pop();
    }

    // 2. Reverse the result to get the final prefix expression.
    reverse(prefix_expr.begin(), prefix_expr.end());
    return prefix_expr;
}



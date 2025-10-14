#include <iostream>
#include<string>
#include<vector>
#include "Q1.h"
#include "Q2.h"
#include "Q3.h"
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;

int main() {

    cout << "Task 1 : Converting infix to prefix propositional logic formula" << endl;
    string infix;
    cin >> infix;
    cout << "Prefix propositional logic formula : " << infixToPrefix(infix) << endl;

    std::cout << "--- Test Case 1 ---" << std::endl;
    std::string prefix1 = "+p~q";

    std::cout << "Prefix Expression: " << prefix1 << std::endl;

    // Build the parse tree
    TreeNode* root1 = buildParseTree(prefix1);

    // Print the infix expression to verify the tree structure
    std::cout << "Resulting Infix:   ";
    printInfix(root1);
    std::cout << std::endl;
    std::cout << "Expected Infix:    (p+(~q))" << std::endl;

    // Clean up the allocated memory
    deleteTree(root1);

    std::cout << "\n--- Test Case 2 ---" << std::endl;
    std::string prefix2 = "*+pq>~rs";

    std::cout << "Prefix Expression: " << prefix2 << std::endl;

    TreeNode* root2 = buildParseTree(prefix2);

    std::cout << "Resulting Infix:   ";
    printInfix(root2);
    std::cout << std::endl;
    std::cout << "Expected Infix:    ((p+q)*((~r)>s))" << std::endl;

    deleteTree(root2);

    return 0;
}
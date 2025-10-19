#include <iostream>
#include <map>
#include<string>
#include<vector>
#include "Q1.h"
#include "Q2.h"
#include "Q3.h"
#include "Q4.h"
#include "Q5.h"
#include "Q6.h"
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::map;
int main() {
    /*
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
    std::cout << "Height of parse tree is : " << getParseTreeHeight(root1)<< std::endl;
    deleteTree(root2);

    map<char, bool> truthValues;
    truthValues['p'] = false;
    string test = "p+~p";
    cout << "Infix to prefix is : "<< infixToPrefix(test) << endl;
    TreeNode* root3 = buildParseTree(infixToPrefix(test));
    printInfix(root3);
    cout << "Height of parse tree is : "<< getParseTreeHeight(root3) << endl;
    cout << "Truth value is : " << evaluateTruthValue(root3, truthValues) << endl;
    */
    string testcnf = "((p+q)*((~r)>s))";
    TreeNode* root4= buildParseTree(infixToPrefix(testcnf));
    printInfix(root4);
    cout << endl;
    root4 = toCNF(root4);
    printInfix(root4);
    return 0;
}
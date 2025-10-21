#include <iostream>
#include <map>
#include<string>
#include<vector>
#include<chrono>
#include "Q1.h"
#include "Q2.h"
#include "Q3.h"
#include "Q4.h"
#include "Q5.h"
#include "Q6.h"
#include "Q7.h"
#include "FileUtility.h"
using std::string;
using std::vector;
using std::cout;
using std::cin;
using std::endl;
using std::map;
int main() {

    cout << "Task 1 : Converting infix to prefix propositional logic formula" << endl;
    string infix;
    cin >> infix;
    cout << "Prefix propositional logic formula : " << infixToPrefix(infix) << endl;

    std::cout << "--- Test Case 1 ---" << std::endl;
    std::string prefix1 = "+ p1 ~ q1";

    std::cout << "Prefix Expression: " << prefix1 << std::endl;

    // Build the parse tree
    TreeNode* root1 = buildParseTree(prefix1);

    // Print the infix expression to verify the tree structure
    std::cout << "Resulting Infix:   ";
    printInfix(root1);
    std::cout << std::endl;
    std::cout << "Expected Infix:    (p1+(~q1))" << std::endl;
    set<string> vars = {"p1","q1"};
    printTruthTable(root1,vars);
    // Clean up the allocated memory
    deleteTree(root1);
    /*
    std::cout << "\n--- Test Case 2 ---" << std::endl;
    std::string prefix2 = "* + p q > ~ r s";

    std::cout << "Prefix Expression: " << prefix2 << std::endl;

    TreeNode* root2 = buildParseTree(prefix2);

    std::cout << "Resulting Infix:   ";
    printInfix(root2);
    std::cout << std::endl;
    std::cout << "Expected Infix:    ((p+q)*((~r)>s))" << std::endl;
    std::cout << "Height of parse tree is : " << getParseTreeHeight(root2)<< std::endl;
    deleteTree(root2);

    map<string, bool> truthValues;
    truthValues["p"] = false;
    string test = "p+~p";
    cout << "Infix to prefix is : "<< infixToPrefix(test) << endl;
    TreeNode* root3 = buildParseTree(infixToPrefix(test));
    printInfix(root3);
    cout << "Height of parse tree is : "<< getParseTreeHeight(root3) << endl;
    cout << "Truth value is : " << evaluateTruthValue(root3, truthValues) << endl;

    string testcnf = "((p+~p)*((~r)>s))";
    TreeNode* root4= buildParseTree(infixToPrefix(testcnf));
    printInfix(root4);
    cout << endl;
    root4 = toCNF(root4);
    printInfix(root4);
    std::pair<int,int> val = isCNF(root4);
    cout << endl;
    cout <<" Number of valid clauses : " << val.first << endl <<  "Number of invalid clauses : " << val.second << endl;

    // Testing on file
    auto start = std::chrono::high_resolution_clock::now();
    TreeNode* root5 = buildParseTreeFromFile("C:\\Users\\gmbha\\OneDrive\\Desktop\\compute\\Logic_Assignment\\test2.cnf");
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "Parse tree built in " << duration.count() << " ms" << std::endl;
    start = std::chrono::high_resolution_clock::now();
    cout << "Checking number of valid clauses " << endl;
    root5 = toCNF(root5);
    //printInfix(root5);
    std::pair<int,int> val1 = isCNF(root5);
    cout <<" Number of valid clauses : " << val1.first << endl <<  "Number of invalid clauses : " << val1.second << endl;
    end = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    cout << "Clauses calculated in " << duration1.count() << " s" << std::endl;
    deleteTree(root5);
    */
    return 0;

}
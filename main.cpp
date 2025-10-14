#include <iostream>
#include<string>
#include<vector>
#include "Q1.h"
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

    return 0;
}
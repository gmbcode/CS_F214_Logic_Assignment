// CNFUtility.cpp
#include "FileUtility.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// Read CNF file and extract clauses
bool readCNFFile(const string& filename,
                 vector<vector<int>>& clauses,
                 int& numVars,
                 int& numClauses) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return false;
    }

    clauses.clear();
    numVars = 0;
    numClauses = 0;

    string line;
    while (getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == 'c') {
            continue;
        }

        // Parse problem line: p cnf <numVars> <numClauses>
        if (line[0] == 'p') {
            istringstream iss(line);
            string p, cnf;
            iss >> p >> cnf >> numVars >> numClauses;
            continue;
        }

        // Parse clause
        istringstream iss(line);
        vector<int> clause;
        int literal;

        while (iss >> literal) {
            if (literal == 0) break;  // End of clause
            clause.push_back(literal);
        }

        if (!clause.empty()) {
            clauses.push_back(clause);
        }
    }

    file.close();
    return true;
}

// Create variable mapping
map<int, string> createVarMap(int numVars, const string& prefix) {
    map<int, string> varMap;
    for (int i = 1; i <= numVars; i++) {
        varMap[i] = prefix + to_string(i);
    }
    return varMap;
}

// Build a literal node (variable or negated variable)
TreeNode* buildLiteralNode(int literal, const map<int, string>& varMap) {
    int varNum = abs(literal);
    string varName = varMap.at(varNum);

    if (literal < 0) {
        // Negated literal: ~x
        TreeNode* negNode = new TreeNode("~");
        negNode->left = nullptr;
        negNode->right = new TreeNode(varName);
        return negNode;
    } else {
        // Positive literal: x
        return new TreeNode(varName);
    }
}

// Build a clause (disjunction of literals)
TreeNode* buildClauseNode(const vector<int>& clause, const map<int, string>& varMap) {
    if (clause.empty()) {
        return nullptr;
    }

    if (clause.size() == 1) {
        // Single literal
        return buildLiteralNode(clause[0], varMap);
    }

    // Multiple literals: build right-associative disjunction tree
    // (a + b + c) is built as (a + (b + c))
    TreeNode* root = buildLiteralNode(clause[0], varMap);

    for (size_t i = 1; i < clause.size(); i++) {
        TreeNode* orNode = new TreeNode("+");
        orNode->left = root;
        orNode->right = buildLiteralNode(clause[i], varMap);
        root = orNode;
    }

    return root;
}

// Build parse tree from clauses (efficient direct construction)
TreeNode* buildParseTreeFromClauses(const vector<vector<int>>& clauses,
                                     const map<int, string>& varMap) {
    if (clauses.empty()) {
        return nullptr;
    }

    if (clauses.size() == 1) {
        // Single clause
        return buildClauseNode(clauses[0], varMap);
    }

    // Multiple clauses: build right-associative conjunction tree
    // (c1 * c2 * c3) is built as (c1 * (c2 * c3))
    TreeNode* root = buildClauseNode(clauses[0], varMap);

    for (size_t i = 1; i < clauses.size(); i++) {
        TreeNode* andNode = new TreeNode("*");
        andNode->left = root;
        andNode->right = buildClauseNode(clauses[i], varMap);
        root = andNode;
    }

    return root;
}

// Main function: Build parse tree directly from CNF file
TreeNode* buildParseTreeFromFile(const string& filename) {
    vector<vector<int>> clauses;
    int numVars, numClauses;

    // Read CNF file
    if (!readCNFFile(filename, clauses, numVars, numClauses)) {
        return nullptr;
    }

    // Create variable mapping
    map<int, string> varMap = createVarMap(numVars);

    // Build parse tree
    return buildParseTreeFromClauses(clauses, varMap);
}

// Alternative: Convert CNF file to infix string (for compatibility with old code)
string cnfFileToInfix(const string& filename) {
    vector<vector<int>> clauses;
    int numVars, numClauses;

    if (!readCNFFile(filename, clauses, numVars, numClauses)) {
        return "";
    }

    map<int, string> varMap = createVarMap(numVars);

    if (clauses.empty()) {
        return "";
    }

    string result = "";

    for (size_t i = 0; i < clauses.size(); i++) {
        // Build clause (disjunction of literals)
        string clauseStr = "(";

        for (size_t j = 0; j < clauses[i].size(); j++) {
            int lit = clauses[i][j];

            if (lit < 0) {
                clauseStr += "(~" + varMap[abs(lit)] + ")";
            } else {
                clauseStr += varMap[lit];
            }

            if (j < clauses[i].size() - 1) {
                clauseStr += "+";
            }
        }

        clauseStr += ")";
        result += clauseStr;

        if (i < clauses.size() - 1) {
            result += "*";
        }
    }

    return result;
}
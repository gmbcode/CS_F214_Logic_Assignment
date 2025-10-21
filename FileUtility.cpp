/**
 * @file FileUtility.cpp
 * @brief Implementation file for utilities that read DIMACS CNF files and build expression parse trees.
 *
 * This file provides a set of functions to parse standard DIMACS CNF (Conjunctive Normal Form)
 * files, which represent boolean satisfiability problems. It includes functionality to
 * read the file, create a mapping from integer variables to string names, and then
 * construct an in-memory expression tree (`TreeNode`) that represents the CNF formula.
 * It also provides a function to convert the CNF file directly to an infix string.
 */

#include "FileUtility.h"
#include "Q2.h" // For TreeNode definition
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdlib> // For abs()

using namespace std;

/**
 * @brief Reads a DIMACS CNF file and parses its clauses.
 *
 * This function opens and reads a file specified in the DIMACS CNF format. It parses the
 * 'p' (problem) line to get the number of variables and clauses, and then reads each
 * line of literals, grouping them into clauses.
 *
 * @param filename The path to the CNF file.
 * @param clauses A reference to a vector of vectors of integers, which will be populated with the clauses.
 * @param numVars A reference to an integer that will store the number of variables.
 * @param numClauses A reference to an integer that will store the number of clauses.
 * @return true if the file was successfully read and parsed.
 * @return false if the file could not be opened.
 */
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

        // Parse a clause line
        istringstream iss(line);
        vector<int> clause;
        int literal;

        while (iss >> literal) {
            if (literal == 0) break;  // 0 terminates a clause line
            clause.push_back(literal);
        }

        if (!clause.empty()) {
            clauses.push_back(clause);
        }
    }

    file.close();
    return true;
}

/**
 * @brief Creates a mapping from integer variable IDs to string names.
 *
 * DIMACS files use integers to represent variables. This function generates a map
 * to associate each integer (e.g., 1, 2, 3) with a string name (e.g., "v1", "v2", "v3").
 *
 * @param numVars The total number of variables.
 * @param prefix A string prefix to use for variable names (default is "v").
 * @return map<int, string> A map where the key is the integer ID and the value is the string name.
 */
map<int, string> createVarMap(int numVars, const string& prefix) {
    map<int, string> varMap;
    for (int i = 1; i <= numVars; i++) {
        varMap[i] = prefix + to_string(i);
    }
    return varMap;
}

/**
 * @brief Builds a TreeNode for a single literal.
 *
 * A literal can be positive (e.g., 5) or negative (e.g., -5). This function creates
 * a simple TreeNode for a positive literal or a negation ('~') node with an atom as its
 * right child for a negative literal.
 *
 * @param literal The integer representing the literal.
 * @param varMap The map from integer IDs to string variable names.
 * @return TreeNode* A pointer to the newly created literal node.
 */
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

/**
 * @brief Builds a clause subtree from a vector of literals.
 *
 * A clause is a disjunction ('+') of literals. This function constructs a right-associative
 * tree of OR nodes. For example, (a + b + c) is built as (a + (b + c)).
 *
 * @param clause A vector of integers representing the literals in the clause.
 * @param varMap The map from integer IDs to string variable names.
 * @return TreeNode* A pointer to the root of the clause subtree. Returns nullptr if the clause is empty.
 */
TreeNode* buildClauseNode(const vector<int>& clause, const map<int, string>& varMap) {
    if (clause.empty()) {
        return nullptr;
    }

    if (clause.size() == 1) {
        // A clause with a single literal is just that literal.
        return buildLiteralNode(clause[0], varMap);
    }

    // Build right-associative disjunction tree for multiple literals.
    TreeNode* root = buildLiteralNode(clause[0], varMap);

    for (size_t i = 1; i < clause.size(); i++) {
        TreeNode* orNode = new TreeNode("+");
        orNode->left = root;
        orNode->right = buildLiteralNode(clause[i], varMap);
        root = orNode;
    }

    return root;
}

/**
 * @brief Builds a complete parse tree from a vector of clauses.
 *
 * A CNF formula is a conjunction ('*') of clauses. This function constructs a right-associative
 * tree of AND nodes. For example, (c1 * c2 * c3) is built as (c1 * (c2 * c3)).
 *
 * @param clauses A vector of vectors, where each inner vector is a clause.
 * @param varMap The map from integer IDs to string variable names.
 * @return TreeNode* A pointer to the root of the complete CNF parse tree.
 */
TreeNode* buildParseTreeFromClauses(const vector<vector<int>>& clauses,
                                     const map<int, string>& varMap) {
    if (clauses.empty()) {
        return nullptr;
    }

    if (clauses.size() == 1) {
        // A formula with a single clause is just that clause's tree.
        return buildClauseNode(clauses[0], varMap);
    }

    // Build right-associative conjunction tree for multiple clauses.
    TreeNode* root = buildClauseNode(clauses[0], varMap);

    for (size_t i = 1; i < clauses.size(); i++) {
        TreeNode* andNode = new TreeNode("*");
        andNode->left = root;
        andNode->right = buildClauseNode(clauses[i], varMap);
        root = andNode;
    }

    return root;
}

/**
 * @brief Main function to build a parse tree directly from a CNF file.
 *
 * This function orchestrates the entire process: reading the file, creating the
 * variable map, and building the final parse tree.
 *
 * @param filename The path to the CNF file.
 * @return TreeNode* A pointer to the root of the generated parse tree, or nullptr on failure.
 */
TreeNode* buildParseTreeFromFile(const string& filename) {
    vector<vector<int>> clauses;
    int numVars, numClauses;

    if (!readCNFFile(filename, clauses, numVars, numClauses)) {
        return nullptr;
    }

    map<int, string> varMap = createVarMap(numVars);
    return buildParseTreeFromClauses(clauses, varMap);
}

/**
 * @brief Converts a CNF file into a single infix expression string.
 *
 * This function provides an alternative way to process a CNF file by converting it
 * into a string representation like "((~v1+v2))*(v3+v4))". This can be useful
 * for compatibility with parsers that expect infix notation.
 *
 * @param filename The path to the CNF file.
 * @return string The infix expression string, or an empty string on failure.
 */
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
        // Build clause string (disjunction of literals)
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
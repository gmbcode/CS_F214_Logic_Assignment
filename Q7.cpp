#include "Q7.h"
#include <set>
#include <string>
using std::pair, std::make_pair;
using std::string;

// Helper function to check if node is a literal (atom or negated atom)
bool isLiteral(TreeNode* node) {
    if (!node) return false;

    // Check if it's an atom (not an operator)
    if (node->data != "~" && node->data != "*" && node->data != "+" && node->data != ">") {
        return true; // Atom
    }

    // Check if it's a negated atom (~A)
    // Negation stores operand in RIGHT child
    if (node->data == "~" && node->right &&
        node->right->data != "~" && node->right->data != "*" &&
        node->right->data != "+" && node->right->data != ">") {
        return true; // Negated atom
    }

    return false;
}

// Helper function to collect all literals in a clause
void collectLiterals(TreeNode* node, std::set<string>& positiveAtoms, std::set<string>& negativeAtoms) {
    if (!node) return;

    // If it's a negated atom (~A)
    // Negation stores operand in RIGHT child
    if (node->data == "~" && node->right && isLiteral(node)) {
        negativeAtoms.insert(node->right->data);
        return;
    }

    // If it's a positive atom (A, B, C, etc.)
    if (isLiteral(node) && node->data != "~") {
        positiveAtoms.insert(node->data);
        return;
    }

    // If it's a disjunction (+), recursively collect from both sides
    if (node->data == "+") {
        collectLiterals(node->left, positiveAtoms, negativeAtoms);
        collectLiterals(node->right, positiveAtoms, negativeAtoms);
    }
}

// Check if a clause is satisfiable (NOT a tautology)
// A clause is UNSATISFIABLE (tautology) if it contains both A and ~A
// A clause is SATISFIABLE if it doesn't contain complementary literals
bool isSatisfiableClause(TreeNode* clause) {
    std::set<string> positiveAtoms;
    std::set<string> negativeAtoms;

    collectLiterals(clause, positiveAtoms, negativeAtoms);

    // Check if any atom appears both positively and negatively
    // If yes, it's a tautology (always true, so unsatisfiable/trivial)
    for (const string& atom : positiveAtoms) {
        if (negativeAtoms.count(atom) > 0) {
            return false; // Contains A and ~A, so it's a tautology
        }
    }

    return true; // Satisfiable
}

// Helper function to check if a node represents a valid clause structure
bool isClause(TreeNode* node) {
    if (!node) {
        return true;
    }
    if (isLiteral(node)) {
        return true;
    }
    if (node->data == "+") {
        return isClause(node->left) && isClause(node->right);
    }
    return false;
}

// Helper function for counting clauses (recursive)
void countClauses(TreeNode* root, int &validNum, int &invalidNum) {
    if (!root) return;

    // If conjunction, check both sides
    if (root->data == "*") {
        countClauses(root->left, validNum, invalidNum);
        countClauses(root->right, validNum, invalidNum);
        return;
    }

    // If disjunction (a clause)
    if (root->data == "+") {
        if (isClause(root)) {
            // Check if the clause is satisfiable
            if (isSatisfiableClause(root)) {
                validNum++; // Satisfiable clause
            } else {
                invalidNum++; // Unsatisfiable/Tautology clause
            }
        } else {
            invalidNum++;
        }
        return;
    }

    // Single literal is a satisfiable clause
    if (isLiteral(root)) {
        validNum++; // Single literal is always satisfiable
    } else {
        invalidNum++;
    }
}

// Main function to check CNF and return pair of (validNum, invalidNum)
pair<int, int> isCNF(TreeNode* root) {
    int validNum = 0;
    int invalidNum = 0; // Number of tautologies

    countClauses(root, validNum, invalidNum);

    return make_pair(invalidNum, validNum);
}
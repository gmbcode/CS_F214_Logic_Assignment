/**
 * @file Q7.cpp
 * @brief Implementation file for validating and analyzing a tree in Conjunctive Normal Form (CNF).
 *
 * This file contains functions to check if a given expression tree conforms to the
 * structure of CNF. It also counts the number of valid (satisfiable) clauses
 * and invalid or tautological clauses within the expression.
 */

#include "Q2.h" // For TreeNode definition
#include "Q7.h"
#include <set>
#include <string>
#include <utility> // For std::pair and std::make_pair

using std::pair;
using std::string;
using std::make_pair;


/**
 * @brief Checks if a given TreeNode represents a literal.
 *
 * A literal is defined as either an atomic proposition (e.g., "p", "q") or its
 * negation (e.g., "~p"). This function identifies atoms by checking if they are not operators,
 * and negated atoms by checking for a '~' node whose right child is an atom.
 *
 * @param node A pointer to the TreeNode to check.
 * @return true if the node is a literal.
 * @return false otherwise.
 */
bool isLiteral(TreeNode* node) {
    if (!node) return false;

    // Check if it's an atom (a variable, not an operator)
    if (node->data != "~" && node->data != "*" && node->data != "+" && node->data != ">") {
        return true; // Atom
    }

    // Check if it's a negated atom (~A), where A is not an operator
    if (node->data == "~" && node->right &&
        (node->right->data != "~" && node->right->data != "*" &&
         node->right->data != "+" && node->right->data != ">")) {
        return true; // Negated atom
    }

    return false;
}

/**
 * @brief Recursively collects all literals from a clause subtree.
 *
 * This function traverses a tree representing a single clause (a disjunction of literals)
 * and separates the atomic variables into two sets: one for positive literals (e.g., "p")
 * and one for negative literals (e.g., the "p" from "~p").
 *
 * @param node A pointer to the root of the clause subtree.
 * @param positiveAtoms A reference to a set where the names of positive atoms will be stored.
 * @param negativeAtoms A reference to a set where the names of negated atoms will be stored.
 */
void collectLiterals(TreeNode* node, std::set<string>& positiveAtoms, std::set<string>& negativeAtoms) {
    if (!node) return;

    // If it's a negated atom (~A)
    if (node->data == "~" && node->right && isLiteral(node)) {
        negativeAtoms.insert(node->right->data);
        return;
    }

    // If it's a positive atom (A, B, C, etc.)
    if (isLiteral(node) && node->data != "~") {
        positiveAtoms.insert(node->data);
        return;
    }

    // If it's a disjunction (+), recursively collect from both children.
    if (node->data == "+") {
        collectLiterals(node->left, positiveAtoms, negativeAtoms);
        collectLiterals(node->right, positiveAtoms, negativeAtoms);
    }
}

/**
 * @brief Determines if a clause is satisfiable by checking for tautology.
 *
 * A clause is a tautology (and thus trivially satisfiable, but often considered invalid in CNF contexts)
 * if it contains complementary literals, i.e., both an atom and its negation (e.g., `p + ~p`).
 * This function returns `false` if the clause is a tautology and `true` otherwise.
 *
 * @param clause A pointer to the root of the clause subtree.
 * @return true if the clause is satisfiable (not a tautology).
 * @return false if the clause is a tautology (contains complementary literals).
 */
bool isSatisfiableClause(TreeNode* clause) {
    std::set<string> positiveAtoms;
    std::set<string> negativeAtoms;

    collectLiterals(clause, positiveAtoms, negativeAtoms);

    // Check if any atom appears in both sets.
    // If so, the clause contains (A + ~A), which is a tautology.
    for (const string& atom : positiveAtoms) {
        if (negativeAtoms.count(atom) > 0) {
            return false; // Tautology found
        }
    }

    return true; // Satisfiable
}

/**
 * @brief Recursively checks if a subtree has the structure of a valid clause.
 *
 * A valid clause is defined as a literal or a disjunction ('+') of other valid clauses.
 * It must not contain conjunctions ('*') or implications ('>').
 *
 * @param node A pointer to the root of the subtree to check.
 * @return true if the subtree has a valid clause structure.
 * @return false otherwise.
 */
bool isClause(TreeNode* node) {
    if (!node) {
        return true; // An empty child is acceptable.
    }
    if (isLiteral(node)) {
        return true; // Base case: a literal is a valid clause.
    }
    // Recursive step: an OR node is a clause if its children are clauses.
    if (node->data == "+") {
        return isClause(node->left) && isClause(node->right);
    }
    // Any other operator (like '*') is not allowed within a clause.
    return false;
}

/**
 * @brief Recursively traverses a CNF tree to count valid and invalid clauses.
 *
 * This function walks the tree. When it finds a conjunction ('*'), it continues down
 * both branches. When it encounters a potential clause (a literal or a disjunction root),
 * it validates its structure and checks if it's a tautology, then increments the
 * appropriate counter.
 *
 * @param root A pointer to the root of the CNF expression tree.
 * @param validNum A reference to an integer to count valid, satisfiable clauses.
 * @param invalidNum A reference to an integer to count invalid/tautological clauses.
 */
void countClauses(TreeNode* root, int &validNum, int &invalidNum) {
    if (!root) return;

    // If it's a conjunction, recurse on both sides (the conjuncts).
    if (root->data == "*") {
        countClauses(root->left, validNum, invalidNum);
        countClauses(root->right, validNum, invalidNum);
        return;
    }

    // At this point, the root of the subtree should be a clause.
    // We check if it has the correct structure (is a disjunction of literals).
    if (isClause(root)) {
        // If the structure is correct, check if it's a tautology.
        if (isSatisfiableClause(root)) {
            validNum++; // Valid, satisfiable clause.
        } else {
            invalidNum++; // Tautology (e.g., p + ~p).
        }
    } else {
        // If the structure is not a valid clause (e.g., contains '*').
        invalidNum++;
    }
}


/**
 * @brief Checks if a tree is in CNF and counts its valid and invalid clauses.
 *
 * This function serves as the entry point for CNF validation. It initializes counters
 * and calls a recursive helper function to traverse the tree and perform the counts.
 *
 * @param root A pointer to the root of the expression tree to be checked.
 * @return pair<int, int> A pair where the first element is the number of invalid/tautological
 * clauses and the second element is the number of valid, satisfiable clauses.
 */
pair<int, int> isCNF(TreeNode* root) {
    int validNum = 0;
    int invalidNum = 0;

    // A single node that is not a conjunction is treated as a single clause.
    if (root && root->data != "*") {
         if (isClause(root)) {
            if (isSatisfiableClause(root)) {
                validNum++;
            } else {
                invalidNum++;
            }
        } else {
            invalidNum++;
        }
    } else {
        // If the root is a conjunction or null, start the recursive counting.
        countClauses(root, validNum, invalidNum);
    }

    return make_pair(invalidNum, validNum);
}

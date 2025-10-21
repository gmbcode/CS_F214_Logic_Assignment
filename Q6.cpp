/**
 * @file Q6.cpp
 * @brief Implementation file for converting a propositional logic expression tree into Conjunctive Normal Form (CNF).
 *
 * This file contains the functions that perform the multi-step process of CNF conversion:
 * 1. IMPL_FREE: Eliminate all implication operators.
 * 2. NNF: Convert the expression into Negation Normal Form by moving negations inward.
 * 3. CNF (with DISTR): Distribute ORs over ANDs to create a conjunction of disjunctions.
 * The final `toCNF` function orchestrates this entire pipeline.
 */

#include "Q2.h"
#include <iostream>
using namespace std;

/**
 * @brief Recursively creates a deep copy of a TreeNode and its entire subtree.
 *
 * This utility is used to create new nodes and subtrees without modifying the
 * original tree structure during transformations.
 *
 * @param node A pointer to the root of the tree (or subtree) to be cloned.
 * @return TreeNode* A pointer to the root of the new, cloned tree.
 */
TreeNode* cloneNode(TreeNode* node) {
    if (!node) return nullptr;
    TreeNode* c = new TreeNode(node->data);
    c->left = cloneNode(node->left);
    c->right = cloneNode(node->right);
    return c;
}

/**
 * @brief Helper function to safely get the child of a negation node.
 *
 * In the parse tree structure used, the operand of a unary negation operator ('~')
 * is always stored in the `right` child. This function provides a clear way to access it.
 *
 * @param node A pointer to the node to check.
 * @return TreeNode* A pointer to the child operand if the input is a negation node, otherwise `nullptr`.
 */
TreeNode* getNegationChild(TreeNode* node) {
    if (!node || node->data != "~") return nullptr;
    return node->right;
}

/**
 * @brief Step 1 of CNF conversion: Eliminates all implication operators from the tree.
 *
 * It recursively traverses the tree and replaces every subtree of the form `A > B`
 * with its logical equivalent, `~A + B`.
 *
 * @param root A pointer to the root of the expression tree.
 * @return TreeNode* A pointer to the root of a new tree that is logically equivalent
 * but contains no implication ('>') operators.
 */
TreeNode* IMPL_FREE(TreeNode* root) {
    if (!root) return nullptr;

    // Leaf node: just return a copy.
    if (!root->left && !root->right) {
        return new TreeNode(root->data);
    }

    // Handle negation (unary operator with operand in right child)
    if (root->data == "~") {
        TreeNode* child = getNegationChild(root);
        TreeNode* transformedChild = IMPL_FREE(child);

        TreeNode* notNode = new TreeNode("~");
        notNode->right = transformedChild;
        notNode->left = nullptr;
        return notNode;
    }

    // Handle implication: p > q  is equivalent to  ~p + q
    if (root->data == ">") {
        TreeNode* L = IMPL_FREE(root->left);
        TreeNode* R = IMPL_FREE(root->right);

        TreeNode* notLeft = new TreeNode("~");
        notLeft->right = L;
        notLeft->left = nullptr;

        TreeNode* orNode = new TreeNode("+");
        orNode->left = notLeft;
        orNode->right = R;
        return orNode;
    }

    // For other binary operators (*, +), recursively process their children.
    TreeNode* out = new TreeNode(root->data);
    out->left = IMPL_FREE(root->left);
    out->right = IMPL_FREE(root->right);
    return out;
}

/**
 * @brief Step 2 of CNF conversion: Converts an implication-free tree to Negation Normal Form (NNF).
 *
 * In NNF, negations ('~') only appear directly in front of atomic propositions (variables),
 * forming literals. This is achieved by applying De Morgan's laws and eliminating double negations.
 * The input tree must already be implication-free.
 *
 * @param root A pointer to the root of the implication-free expression tree.
 * @return TreeNode* A pointer to the root of a new, logically equivalent tree in NNF.
 */
TreeNode* NNF(TreeNode* root) {
    if (!root) return nullptr;

    // Base case: Leaf (literal) - just copy it.
    if (!root->left && !root->right) {
        return new TreeNode(root->data);
    }

    // Handle negation node
    if (root->data == "~") {
        TreeNode* child = getNegationChild(root);

        if (!child) return new TreeNode("~"); // Should not happen in a valid tree

        // Double negation: ~~p -> p
        if (child->data == "~") {
            TreeNode* grandchild = getNegationChild(child);
            return NNF(grandchild); // Recursively process the inner expression
        }

        // De Morgan's Law: ~(p * q) -> (~p + ~q)
        if (child->data == "*") {
            TreeNode* notLeft = new TreeNode("~");
            notLeft->right = child->left;

            TreeNode* notRight = new TreeNode("~");
            notRight->right = child->right;

            TreeNode* orNode = new TreeNode("+");
            orNode->left = NNF(notLeft);
            orNode->right = NNF(notRight);
            return orNode;
        }

        // De Morgan's Law: ~(p + q) -> (~p * ~q)
        if (child->data == "+") {
            TreeNode* notLeft = new TreeNode("~");
            notLeft->right = child->left;

            TreeNode* notRight = new TreeNode("~");
            notRight->right = child->right;

            TreeNode* andNode = new TreeNode("*");
            andNode->left = NNF(notLeft);
            andNode->right = NNF(notRight);
            return andNode;
        }

        // Negation of a literal: ~p (where p is an atom). This is already in NNF.
        TreeNode* neg = new TreeNode("~");
        neg->right = new TreeNode(child->data);
        return neg;
    }

    // For AND/OR operators, recursively process children.
    TreeNode* out = new TreeNode(root->data);
    out->left = NNF(root->left);
    out->right = NNF(root->right);
    return out;
}

/**
 * @brief Helper function for CNF that distributes OR ('+') over AND ('*').
 *
 * This function recursively applies the distribution law:
 * - A + (B * C) <=> (A + B) * (A + C)
 * - (B * C) + A <=> (B + A) * (C + A)
 * This is the core step in transforming an NNF expression into a conjunction of disjunctions.
 *
 * @param root A pointer to the root of a tree (or subtree) in NNF.
 * @return TreeNode* A pointer to the root of a new tree where OR has been distributed over AND.
 */
TreeNode* DISTR(TreeNode* root) {
    if (!root) return nullptr;

    // Base case: leaf node
    if (!root->left && !root->right) {
        return new TreeNode(root->data);
    }

    // A literal (~p) is already a clause.
    if (root->data == "~") {
        TreeNode* out = new TreeNode("~");
        out->right = cloneNode(root->right);
        return out;
    }

    // If root is AND, its children are conjuncts. Just recurse.
    if (root->data == "*") {
        TreeNode* out = new TreeNode("*");
        out->left = DISTR(root->left);
        out->right = DISTR(root->right);
        return out;
    }

    // If root is OR, check if distribution is needed.
    if (root->data == "+") {
        TreeNode* L = DISTR(root->left);
        TreeNode* R = DISTR(root->right);

        // Case 1: (p * q) + r  =>  (p + r) * (q + r)
        if (L && L->data == "*") {
            // New left branch: (p + r)
            TreeNode* leftOr = new TreeNode("+");
            leftOr->left = cloneNode(L->left); // p
            leftOr->right = cloneNode(R);      // r

            // New right branch: (q + r)
            TreeNode* rightOr = new TreeNode("+");
            rightOr->left = cloneNode(L->right); // q
            rightOr->right = cloneNode(R);       // r

            // Create the new AND node and recurse
            TreeNode* andNode = new TreeNode("*");
            andNode->left = DISTR(leftOr);
            andNode->right = DISTR(rightOr);
            return andNode;
        }

        // Case 2: p + (q * r)  =>  (p + q) * (p + r)
        if (R && R->data == "*") {
            // New left branch: (p + q)
            TreeNode* leftOr = new TreeNode("+");
            leftOr->left = cloneNode(L);        // p
            leftOr->right = cloneNode(R->left); // q

            // New right branch: (p + r)
            TreeNode* rightOr = new TreeNode("+");
            rightOr->left = cloneNode(L);         // p
            rightOr->right = cloneNode(R->right); // r

            // Create the new AND node and recurse
            TreeNode* andNode = new TreeNode("*");
            andNode->left = DISTR(leftOr);
            andNode->right = DISTR(rightOr);
            return andNode;
        }

        // If no distribution is needed (e.g., p + q), just reconstruct the OR node.
        TreeNode* orNode = new TreeNode("+");
        orNode->left = L;
        orNode->right = R;
        return orNode;
    }

    // Fallback for any other case
    TreeNode* out = new TreeNode(root->data);
    out->left = DISTR(root->left);
    out->right = DISTR(root->right);
    return out;
}

/**
 * @brief Step 3 of CNF conversion: Converts a tree in NNF to Conjunctive Normal Form.
 *
 * This function recursively processes the tree. For OR nodes, it ensures its children
 * are in CNF first, then applies the `DISTR` function to guarantee the result is a
 * conjunction of clauses (disjunctions of literals).
 *
 * @param root A pointer to the root of a tree in NNF.
 * @return TreeNode* A pointer to the root of a new tree in CNF.
 */
TreeNode* CNF(TreeNode* root) {
    if (!root) return nullptr;

    // Base case: A literal is in CNF.
    if (!root->left && !root->right || root->data == "~") {
        return cloneNode(root);
    }

    // For an AND node, the children must be in CNF.
    if (root->data == "*") {
        TreeNode* out = new TreeNode("*");
        out->left = CNF(root->left);
        out->right = CNF(root->right);
        return out;
    }

    // For an OR node, convert children to CNF then distribute.
    if (root->data == "+") {
        TreeNode* L = CNF(root->left);
        TreeNode* R = CNF(root->right);

        TreeNode* orNode = new TreeNode("+");
        orNode->left = L;
        orNode->right = R;

        return DISTR(orNode);
    }

    // Fallback should not be reached with valid logical expressions.
    return cloneNode(root);
}

/**
 * @brief The main pipeline to convert a propositional logic expression tree to CNF.
 *
 * It executes the three main steps in sequence:
 * 1. `IMPL_FREE`: Eliminate implications.
 * 2. `NNF`: Convert to Negation Normal Form.
 * 3. `CNF`: Apply distribution recursively to get the final CNF structure.
 *
 * @param root A pointer to the root of the original expression tree.
 * @return TreeNode* A pointer to the root of a new, logically equivalent tree in CNF.
 */
TreeNode* toCNF(TreeNode* root) {
    TreeNode* step1 = IMPL_FREE(root);
    TreeNode* step2 = NNF(step1);
    TreeNode* step3 = CNF(step2);

    // Clean up intermediate trees
    deleteTree(step1);
    deleteTree(step2);

    return step3;
}
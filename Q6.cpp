// Q2.cpp
#include "Q2.h"
#include <iostream>
using namespace std;

// ---------- Utility ---------- (Recursive cloner)
TreeNode* cloneNode(TreeNode* node) {
    if (!node) return nullptr;
    TreeNode* c = new TreeNode(node->data);
    c->left = cloneNode(node->left);
    c->right = cloneNode(node->right);
    return c;
}

// Helper to get the child of a negation operator (stored in right)
TreeNode* getNegationChild(TreeNode* node) {
    if (!node || node->data != '~') return nullptr;
    return node->right;
}

// ---------- Step 1: Eliminate implications ----------
TreeNode* IMPL_FREE(TreeNode* root) {
    if (!root) return nullptr;

    // Leaf node
    if (!root->left && !root->right) {
        return new TreeNode(root->data);
    }

    // Handle negation (unary operator with operand in right)
    if (root->data == '~') {
        TreeNode* child = getNegationChild(root);
        TreeNode* transformedChild = IMPL_FREE(child);

        TreeNode* notNode = new TreeNode('~');
        notNode->right = transformedChild;  // Store in right for proper inorder
        notNode->left = nullptr;
        return notNode;
    }

    // Handle implication: p > q  ≡  ~p + q
    if (root->data == '>') {
        TreeNode* L = IMPL_FREE(root->left);
        TreeNode* R = IMPL_FREE(root->right);

        TreeNode* notLeft = new TreeNode('~');
        notLeft->right = L;  // Store in right
        notLeft->left = nullptr;

        TreeNode* orNode = new TreeNode('+');
        orNode->left = notLeft;
        orNode->right = R;
        return orNode;
    }

    // For other binary operators, recursively process children
    TreeNode* out = new TreeNode(root->data);
    out->left = IMPL_FREE(root->left);
    out->right = IMPL_FREE(root->right);
    return out;
}

// ---------- Step 2: Negation Normal Form (NNF) ----------
TreeNode* NNF(TreeNode* root) {
    if (!root) return nullptr;

    // Leaf (literal) - just copy
    if (!root->left && !root->right) {
        return new TreeNode(root->data);
    }

    // Handle negation
    if (root->data == '~') {
        TreeNode* child = getNegationChild(root);

        if (!child) {
            TreeNode* neg = new TreeNode('~');
            neg->left = nullptr;
            return neg;
        }

        // Double negation: ~~p -> p
        if (child->data == '~') {
            TreeNode* grandchild = getNegationChild(child);
            if (grandchild) {
                return NNF(grandchild);
            }
            return nullptr;
        }

        // De Morgan: ~(p * q) -> (~p + ~q)
        if (child->data == '*') {
            TreeNode* notLeft = new TreeNode('~');
            notLeft->left = nullptr;
            notLeft->right = child->left;

            TreeNode* notRight = new TreeNode('~');
            notRight->left = nullptr;
            notRight->right = child->right;

            TreeNode* orNode = new TreeNode('+');
            orNode->left = NNF(notLeft);
            orNode->right = NNF(notRight);
            return orNode;
        }

        // De Morgan: ~(p + q) -> (~p * ~q)
        if (child->data == '+') {
            TreeNode* notLeft = new TreeNode('~');
            notLeft->left = nullptr;
            notLeft->right = child->left;

            TreeNode* notRight = new TreeNode('~');
            notRight->left = nullptr;
            notRight->right = child->right;

            TreeNode* andNode = new TreeNode('*');
            andNode->left = NNF(notLeft);
            andNode->right = NNF(notRight);
            return andNode;
        }

        // Negation of a literal: ~p (where p is an atom)
        TreeNode* neg = new TreeNode('~');
        neg->left = nullptr;
        neg->right = new TreeNode(child->data);
        return neg;
    }

    // For AND/OR operators, recursively process children
    TreeNode* out = new TreeNode(root->data);
    out->left = NNF(root->left);
    out->right = NNF(root->right);
    return out;
}

// ---------- Step 3: Distribution (DISTR) ----------
TreeNode* DISTR(TreeNode* root) {
    if (!root) return nullptr;

    // Leaf
    if (!root->left && !root->right) {
        return new TreeNode(root->data);
    }

    // Handle negation (literal ~p) - preserve structure
    if (root->data == '~') {
        TreeNode* out = new TreeNode('~');
        out->left = nullptr;
        out->right = cloneNode(root->right);
        return out;
    }

    // If root is AND, just recurse on children
    if (root->data == '*') {
        TreeNode* out = new TreeNode('*');
        out->left = DISTR(root->left);
        out->right = DISTR(root->right);
        return out;
    }

    // If root is OR, attempt distribution
    if (root->data == '+') {
        TreeNode* L = DISTR(root->left);
        TreeNode* R = DISTR(root->right);

        // (p * q) + r  =>  (p + r) * (q + r)
        if (L && L->data == '*') {
            TreeNode* andNode = new TreeNode('*');

            TreeNode* leftOr = new TreeNode('+');
            leftOr->left = cloneNode(L->left);
            leftOr->right = cloneNode(R);

            TreeNode* rightOr = new TreeNode('+');
            rightOr->left = cloneNode(L->right);
            rightOr->right = cloneNode(R);

            andNode->left = DISTR(leftOr);
            andNode->right = DISTR(rightOr);
            return andNode;
        }

        // p + (q * r)  =>  (p + q) * (p + r)
        if (R && R->data == '*') {
            TreeNode* andNode = new TreeNode('*');

            TreeNode* leftOr = new TreeNode('+');
            leftOr->left = cloneNode(L);
            leftOr->right = cloneNode(R->left);

            TreeNode* rightOr = new TreeNode('+');
            rightOr->left = cloneNode(L);
            rightOr->right = cloneNode(R->right);

            andNode->left = DISTR(leftOr);
            andNode->right = DISTR(rightOr);
            return andNode;
        }

        // No distribution needed
        TreeNode* orNode = new TreeNode('+');
        orNode->left = L;
        orNode->right = R;
        return orNode;
    }

    // Fallback for other operators
    TreeNode* out = new TreeNode(root->data);
    out->left = DISTR(root->left);
    out->right = DISTR(root->right);
    return out;
}

// ---------- Step 4: CNF (recursive structural) ----------
TreeNode* CNF(TreeNode* root) {
    if (!root) return nullptr;

    // Leaf
    if (!root->left && !root->right) {
        return new TreeNode(root->data);
    }

    // Negation (literal ~p) - preserve structure
    if (root->data == '~') {
        TreeNode* out = new TreeNode('~');
        out->left = nullptr;
        out->right = cloneNode(root->right);
        return out;
    }

    // AND: just recurse
    if (root->data == '*') {
        TreeNode* out = new TreeNode('*');
        out->left = CNF(root->left);
        out->right = CNF(root->right);
        return out;
    }

    // OR: recurse then distribute
    if (root->data == '+') {
        TreeNode* L = CNF(root->left);
        TreeNode* R = CNF(root->right);

        TreeNode* orNode = new TreeNode('+');
        orNode->left = L;
        orNode->right = R;

        return DISTR(orNode);
    }

    // Fallback
    TreeNode* out = new TreeNode(root->data);
    out->left = CNF(root->left);
    out->right = CNF(root->right);
    return out;
}

// ---------- Full pipeline ----------
TreeNode* toCNF(TreeNode* root) {
    TreeNode* step1 = IMPL_FREE(root);
    TreeNode* step2 = NNF(step1);
    TreeNode* step3 = CNF(step2);
    return step3;
}
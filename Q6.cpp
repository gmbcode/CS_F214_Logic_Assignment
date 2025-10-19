// q2_fixed.cpp
#include "Q2.h"
#include <iostream>
using namespace std;



// ---------- Utility ----------
TreeNode* cloneNode(TreeNode* node) {
    if (!node) return nullptr;
    TreeNode* c = new TreeNode(node->data);
    c->left = cloneNode(node->left);
    c->right = cloneNode(node->right);
    return c;
}

// ---------- Step 1: Eliminate implications ----------
TreeNode* IMPL_FREE(TreeNode* root) {
    if (!root) return nullptr;

    // Recursively process children first
    TreeNode* L = IMPL_FREE(root->left);
    TreeNode* R = IMPL_FREE(root->right);

    if (root->data == '>') {
        // p > q  ≡  ~p + q
        TreeNode* notLeft = new TreeNode('~');
        notLeft->left = L;          // L already transformed
        TreeNode* orNode = new TreeNode('+');
        orNode->left = notLeft;
        orNode->right = R;
        return orNode;
    }

    // Non-implication node: recreate using transformed children
    TreeNode* out = new TreeNode(root->data);
    out->left = L;
    out->right = R;
    return out;
}

// ---------- Step 2: Negation Normal Form (NNF) ----------
TreeNode* NNF(TreeNode* root) {
    if (!root) return nullptr;

    // Leaf (literal)
    if (!root->left && !root->right) {
        return new TreeNode(root->data);
    }

    // If it's a negation, push it inwards
    if (root->data == '~') {
        TreeNode* child = NNF(root->left); // normalize the child first

        if (!child) return new TreeNode('~');

        // Double negation: ~~p -> p
        if (child->data == '~') {
            return NNF(child->left);
        }

        // De Morgan: ~(p * q) -> ~p + ~q
        if (child->data == '*') {
            TreeNode* orNode = new TreeNode('+');

            TreeNode* notP = new TreeNode('~');
            notP->left = cloneNode(child->left);

            TreeNode* notQ = new TreeNode('~');
            notQ->left = cloneNode(child->right);

            orNode->left = NNF(notP);
            orNode->right = NNF(notQ);
            return orNode;
        }

        // De Morgan: ~(p + q) -> ~p * ~q
        if (child->data == '+') {
            TreeNode* andNode = new TreeNode('*');

            TreeNode* notP = new TreeNode('~');
            notP->left = cloneNode(child->left);

            TreeNode* notQ = new TreeNode('~');
            notQ->left = cloneNode(child->right);

            andNode->left = NNF(notP);
            andNode->right = NNF(notQ);
            return andNode;
        }

        // Negation of literal (or already atomic) -> ~child
        TreeNode* neg = new TreeNode('~');
        neg->left = NNF(child);
        return neg;
    }

    // Non-negation operator: recursively normalize children
    TreeNode* out = new TreeNode(root->data);
    out->left = NNF(root->left);
    out->right = NNF(root->right);
    return out;
}

// ---------- Step 3: Distribution (DISTR) ----------
TreeNode* DISTR(TreeNode* root) {
    if (!root) return nullptr;

    // Leaf or unary
    if (!root->left && !root->right) {
        return new TreeNode(root->data);
    }

    // If root is OR, attempt distribution
    if (root->data == '+') {
        TreeNode* L = DISTR(root->left);
        TreeNode* R = DISTR(root->right);

        // p + (q * r) => (p + q) * (p + r)
        if (R && R->data == '*') {
            TreeNode* andNode = new TreeNode('*');

            andNode->left = new TreeNode('+');
            andNode->left->left  = cloneNode(L);
            andNode->left->right = cloneNode(R->left);

            andNode->right = new TreeNode('+');
            andNode->right->left  = cloneNode(L);
            andNode->right->right = cloneNode(R->right);

            // recursively distribute the newly created AND
            return DISTR(andNode);
        }

        // (p * q) + r => (p + r) * (q + r)
        if (L && L->data == '*') {
            TreeNode* andNode = new TreeNode('*');

            andNode->left = new TreeNode('+');
            andNode->left->left  = cloneNode(L->left);
            andNode->left->right = cloneNode(R);

            andNode->right = new TreeNode('+');
            andNode->right->left  = cloneNode(L->right);
            andNode->right->right = cloneNode(R);

            return DISTR(andNode);
        }

        // Nothing to distribute
        TreeNode* orNode = new TreeNode('+');
        orNode->left = L;
        orNode->right = R;
        return orNode;
    }

    // If it's AND or other binary operator, just recurse
    TreeNode* out = new TreeNode(root->data);
    out->left = DISTR(root->left);
    out->right = DISTR(root->right);
    return out;
}

// ---------- Step 4: CNF (recursive structural) ----------
TreeNode* CNF(TreeNode* root) {
    if (!root) return nullptr;

    if (!root->left && !root->right) return new TreeNode(root->data);

    if (root->data == '*') {
        TreeNode* L = CNF(root->left);
        TreeNode* R = CNF(root->right);
        TreeNode* out = new TreeNode('*');
        out->left = L;
        out->right = R;
        return out;
    }

    if (root->data == '+') {
        TreeNode* L = CNF(root->left);
        TreeNode* R = CNF(root->right);
        TreeNode* orNode = new TreeNode('+');
        orNode->left  = L;
        orNode->right = R;
        return DISTR(orNode);
    }

    // Fallback: copy node
    TreeNode* out = new TreeNode(root->data);
    out->left = CNF(root->left);
    out->right = CNF(root->right);
    return out;
}

// ---------- Full pipeline ----------
TreeNode* toCNF(TreeNode* root) {
    TreeNode* a = IMPL_FREE(root);
    TreeNode* b = NNF(a);
    TreeNode* c = CNF(b);
    return c;
}


/**
* @file Q4.cpp
 * @brief Implementation file for calculating the height of an expression parse tree.
 *
 * This file contains the logic to determine the height of a given parse tree,
 * which is defined as the number of nodes along the longest path from the root
 * node down to the farthest leaf node.
 */

#include "Q2.h" // For TreeNode definition
#include "Q4.h"

#include <algorithm>
using std::max;

/**
 * @brief Recursively calculates the height of a parse tree.
 *
 * The height of a tree is the length of the longest path from the root to a leaf.
 * An empty tree (nullptr) has a height of 0. A tree with a single node has a height of 1.
 * The function works by finding the height of the left and right subtrees and
 * returning 1 (for the current node) plus the height of the taller subtree.
 *
 * @param root A pointer to the root node of the tree (or subtree).
 * @return int The height of the tree.
 */
int getParseTreeHeight(TreeNode* root) {

    if (root == nullptr) {
        return 0;
    }

    // Recursively find the height of the left subtree
    int leftHeight = getParseTreeHeight(root->left);
    // Recursively find the height of the right subtree
    int rightHeight = getParseTreeHeight(root->right);

    // The height of the tree is 1 (for the current node) + the height of the taller subtree
    return 1 + max(leftHeight, rightHeight);
}

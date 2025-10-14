#include "Q2.h"
#include "Q4.h"

#include <algorithm>
using std::max;

int getParseTreeHeight(TreeNode* root) {

    if (root == nullptr) {
        return -1;
    }

    int leftHeight = getParseTreeHeight(root->left);
    int rightHeight = getParseTreeHeight(root->right);

    return 1 + max(leftHeight, rightHeight);
}

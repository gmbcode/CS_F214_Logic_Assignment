#include "Q2.h"
#include <map>
#include <set>
using std::set;
#ifndef Q5_H
#define Q5_H
bool evaluateTruthValue(TreeNode* root, const std::map<string, bool>& truthValues);
void printTruthTable(TreeNode *r, set<string> &vars);
#endif //Q5_H

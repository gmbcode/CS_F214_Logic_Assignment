// CNFUtility.h
#ifndef FileUtility_H
#define FileUtility_H
#pragma once
#include "Q2.h"
#include <string>
#include <vector>
#include <map>

// Main function to build parse tree directly from CNF file
TreeNode* buildParseTreeFromFile(const std::string& filename);

// Helper: Build parse tree directly from CNF clauses (more efficient than string conversion)
TreeNode* buildParseTreeFromClauses(const std::vector<std::vector<int>>& clauses,
                                     const std::map<int, std::string>& varMap);

// Helper: Read CNF file and extract clauses
bool readCNFFile(const std::string& filename,
                 std::vector<std::vector<int>>& clauses,
                 int& numVars,
                 int& numClauses);

// Helper: Create variable mapping (1->x1, 2->x2, etc.)
std::map<int, std::string> createVarMap(int numVars, const std::string& prefix = "x");

// Alternative: Convert CNF file to infix string (for compatibility)
std::string cnfFileToInfix(const std::string& filename);

#endif // FileUtility

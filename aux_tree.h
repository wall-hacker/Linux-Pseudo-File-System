// Copyright 2022 Andrei Calin-Mihail & Bucur Vladimir 311CA

#include "tree.h"

FileTree createFileTree(char* rootFolderName);

void free_file(ListNode *node);

void free_folder(ListNode *node);

void freeTree(FileTree fileTree);

TreeNode* move_back(TreeNode* currentNode);

TreeNode* move_next(TreeNode* currentNode, char* p);

void tree_recursiv(TreeNode* currentNode, int lvl_number,
                   int* no_file, int* no_folder);

void rmrec_recursive(TreeNode *currentNode);

TreeNode* move_next_cp(TreeNode* currentNode, char* p);

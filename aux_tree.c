// Copyright 2022 Andrei Calin-Mihail & Bucur Vladimir 311CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."

#include "list.h"

// function that creates a file structure
FileTree createFileTree(char* rootFolderName)
{
    // we declare and allocate memory for a new tree
    /**
    * we declare and allocate memory for a new tree
    * 
    * we initialize each of its fields and in case it is needed we
    * allocate memory to it*
    */
    FileTree new_tree;
    new_tree.root = malloc(sizeof(TreeNode));
    DIE(!new_tree.root, "Malloc failed!");
    new_tree.root->parent = NULL;
    new_tree.root->name = malloc((strlen(rootFolderName) + 1) * sizeof(char));
    DIE(!new_tree.root->name, "Malloc failed!");
    memcpy(new_tree.root->name, rootFolderName,
           (strlen(rootFolderName) + 1) * sizeof(char));
    new_tree.root->type = FOLDER_NODE;
    new_tree.root->content = malloc(sizeof(FolderContent));
    DIE(!new_tree.root->content, "Malloc failed!");

    FolderContent *new_content = malloc(sizeof(FolderContent));
    DIE(!new_content, "Malloc failed!");
    new_content->children = malloc(sizeof(List));
    DIE(!new_content->children, "Malloc failed!");
    new_content->children->head = NULL;
    memcpy(new_tree.root->content, new_content, sizeof(FolderContent));

    free(new_content);

    return new_tree;
}

// function that frees up the memory of a file
void free_file(ListNode *node)
{
    // we free the memory of each field of the file
    TreeNode* node_info = node->info;
    free(node_info->name);

    FileContent* content = (FileContent*)node_info->content;
    free(content->text);
    free(content);

    free(node->info);
    free(node);
}

// function that frees up the memory of a folder
void free_folder(ListNode *node)
{
    TreeNode* node_info = node->info;
    free(node_info->name);

    FolderContent* content = (FolderContent*)node_info->content;
    List* list = content->children;

    /**
    * we go through the list and free the memory for each
    * file / folder we find
    * 
    * if we meet a folder we go into it and free the memory
    * to everything we find and so on*
    */
    ListNode* curr = list->head;
    while (curr) {
        ListNode* temp = curr;
        curr = curr->next;
        if (((TreeNode*)temp->info)->type == FILE_NODE) {
            free_file(temp);
        } else if (((TreeNode*)temp->info)->type == FOLDER_NODE) {
            free_folder(temp);
        }
    }

    free(list);
    free(content);
    free(node->info);
    free(node);
}

// functie ce elibereaza memoria intregii structuri de fisiere(tree)
void freeTree(FileTree fileTree) {
    List* list = ((FolderContent*)fileTree.root->content)->children;
    // the same procedure applies as for free_folder
    ListNode* curr = list->head;
    while (curr) {
        ListNode* temp = curr;
        curr = curr->next;
        if (((TreeNode*)temp->info)->type == FILE_NODE) {
            free_file(temp);
        } else if (((TreeNode*)temp->info)->type == FOLDER_NODE) {
            free_folder(temp);
        }
    }

    free(list);

    TreeNode* root = fileTree.root;
    free(root->name);
    free(root->content);
    free(root);
}

// function that returns the parent of the current file / folder
TreeNode* move_back(TreeNode* currentNode)
{
    if (!currentNode->parent)
        return NULL;
    currentNode = currentNode->parent;
    return currentNode;
}


/**
* function that searches for and returns the file / folder
* that has the name specified in the header*
*/
TreeNode* move_next(TreeNode* currentNode, char* file_name)
{
    List* list = ((FolderContent*)currentNode->content)->children;
    ListNode* curr = list->head;
    int ok = 0;
    while (curr) {
        TreeNode* f = curr->info;
        if (strcmp(f->name, file_name) == 0 && f->type == FOLDER_NODE) {
            ok = 1;
            currentNode = f;
            break;
        }
        curr = curr->next;
    }

    // if the desired file was not found, NULL is returned
    if (!ok)
        return NULL;
    return currentNode;
}

void tree_recursiv(TreeNode* currentNode, int lvl_number,
                   int* no_file, int* no_folder) {
    List* list = ((FolderContent*)currentNode->content)->children;
    /**
    * we go through the list of the current node and print
    * what we find in it*
    */
    ListNode* curr = list->head;
    while (curr) {
        ListNode* temp = curr;
        if (((TreeNode*)temp->info)->type == FILE_NODE) {
            for (int i = 0; i < lvl_number; i++)
                printf("\t");
            printf("%s\n", ((TreeNode*)temp->info)->name);

            (*no_file)++;

        /**
        * if what I found is a folder, go into it recursively and
        * proceed as before*
        */
        } else if (((TreeNode*)temp->info)->type == FOLDER_NODE) {
            for (int i = 0; i < lvl_number; i++)
                printf("\t");
            printf("%s\n", ((TreeNode*)temp->info)->name);

            (*no_folder)++;
            lvl_number++;

            tree_recursiv(((TreeNode*)temp->info), lvl_number, no_file,
                          no_folder);

            lvl_number--;
        }
        curr = curr->next;
    }
}

void rmrec_recursive(TreeNode *currentNode) {
    int index = 0;
    List* list = ((FolderContent*)currentNode->content)->children;
    /**
    * we go through the list of the current node and remove
    * what we find in it*
    */
    ListNode* curr = list->head;
    while (curr) {
        ListNode* temp = curr;
        curr = curr->next;
        if (((TreeNode*)temp->info)->type == FILE_NODE) {
            ll_remove_nth_node(list, index);
        } else if (((TreeNode*)temp->info)->type == FOLDER_NODE) {
            /**
            * if what I found is a folder, go into it recursively and
            * proceed as before*
            */
            TreeNode* aux = (TreeNode*)temp->info;
            if (((FolderContent*)aux->content)->children->head)
                rmrec_recursive(((TreeNode*)temp->info));

            ll_remove_nth_node(list, index);
        }
        index++;
    }
}

/**
* auxiliary function for cp command
* it returns the child with the name specified in the header*
*/
TreeNode* move_next_cp(TreeNode* currentNode, char* file_name)
{
    List* list = ((FolderContent*)currentNode->content)->children;
    ListNode* curr = list->head;

    int ok = 0;
    while (curr) {
        TreeNode* f = curr->info;
        if (strcmp(f->name, file_name) == 0) {
            ok = 1;
            currentNode = f;
            break;
        }
        curr = curr->next;
    }

    // if the desired file was not found, NULL is returned
    if (!ok)
        return NULL;
    return currentNode;
}

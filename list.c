// Copyright 2022 Andrei Calin-Mihail & Bucur Vladimir 311CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."

#include "list.h"

// function that creates a new node
ListNode* create_new_node(const void* new_data, int data_size)
{
    ListNode *new_node =  malloc(sizeof(ListNode));
    DIE(!new_node, "Malloc failed!");
    new_node->info =  malloc(data_size);
    DIE(!new_node->info, "Malloc failed!");
    memcpy(new_node->info, new_data, data_size);
    new_node->next = NULL;
    return new_node;
}

// function that adds a node to the list
void ll_add_nth_node(List* list, unsigned int n, const void* new_data)
{
    ListNode *new_node = create_new_node(new_data, sizeof(TreeNode));
    if (!list->head) {
        new_node->next = NULL;
        list->head = new_node;
    } else if (n == 0) {
        new_node->next = list->head;
        list->head = new_node;
    }
}

// function that removes a node from the list
void ll_remove_nth_node(List* list, int n)
{
    ListNode *curr = list->head;
    if (n == 0) {
        ListNode *garbage = list->head;
        curr = curr->next;
        garbage->next = NULL;
        list->head = curr;

        TreeNode* garbage_info = garbage->info;
        free(garbage_info->name);
        FolderContent* garbage_content = (FolderContent*)garbage_info->content;

        List* list = garbage_content->children;
        free(list);
        free(garbage_content);
        free(garbage_info);
        free(garbage);
    } else {
        for (int i = 0; i < n - 1; i++)
            curr = curr->next;
        ListNode *garbage = curr->next;
        curr->next = garbage->next;
        garbage->next = NULL;

        TreeNode* garbage_info = garbage->info;
        free(garbage_info->name);
        FolderContent* garbage_content = (FolderContent*)garbage_info->content;

        List* list = garbage_content->children;
        free(list);
        free(garbage_content);
        free(garbage_info);
        free(garbage);
    }
}

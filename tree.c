// Copyright 2022 Andrei Calin-Mihail & Bucur Vladimir 311CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."
#define MAX_STRING_SIZE 256

#include "list.h"

void ls(TreeNode* currentNode, char* arg) {
    /**
    * if arg lenght is 0 it means that we have no argument and the list
	* will be scrolled and all its contents will be printed*
    */
    if (strlen(arg) == 0) {
        List* list = ((FolderContent*)currentNode->content)->children;
        ListNode* curr = list->head;
        while (curr) {
            TreeNode* f = curr->info;
            printf("%s\n", f->name);
            curr = curr->next;
        }
    } else {
        // we are looking for the file / folder with the specified name
        List* list = ((FolderContent*)currentNode->content)->children;
        ListNode* curr = list->head;
        int ok = 0;
        while (curr) {
            TreeNode* f = curr->info;
            if (strcmp(f->name, arg) == 0) {
                ok = 1;
                // if what we are looking for is a file, we print its name
                if (f->type == FILE_NODE) {
                    printf("%s: %s\n", f->name,
                            ((FileContent*)f->content)->text);

                /**
                * if what we are looking for is a folder, we browse and
                * print what is in it*
                */
                } else if (f->type == FOLDER_NODE) {
                    List* f_list = ((FolderContent*)f->content)->children;
                    ListNode* f_curr = f_list->head;

                    while (f_curr) {
                        TreeNode* f_f = f_curr->info;
                        printf("%s\n", f_f->name);
                        f_curr = f_curr->next;
                    }
                }
            }
            curr = curr->next;
        }

        /**
        * in case we do not find what we are looking for we will print
        * an error message*
        */
        if (!ok)
            printf("ls: cannot access '%s': No such file or directory\n", arg);
    }
}

void pwd(TreeNode* treeNode) {
    char aux[MAX_STRING_SIZE][MAX_STRING_SIZE];
    int no_folder = 0;

    /**
    * we go from parent to parent until we get to the root and memorize in
    * aux the names of the folders we go through*
    */
    TreeNode* currentNode = treeNode;
    while (currentNode) {
        memcpy(aux[no_folder++], currentNode->name,
               (strlen(currentNode->name) + 1) * sizeof(char));
        currentNode = currentNode->parent;
    }

    // we go iterate through aux backwards ​​and print its contents
    for (int i = no_folder - 1; i >= 0; i--) {
        if (i == 0) {
            printf("%s", aux[i]);
        } else {
            printf("%s/", aux[i]);
        }
    }
    printf("\n");
}

TreeNode* cd(TreeNode* currentNode, char* path) {
    TreeNode* initial_folder = currentNode;
    char *temp = malloc((strlen(path) + 1) * sizeof(char));
    DIE(!temp, "Malloc failed!");
    memcpy(temp, path, (strlen(path) + 1) * sizeof(char));
    // we divide the path
    char *p;
    p = strtok(path, "/");
    while (p) {
        /**
        * if we encounter ".." we use the move_back function to move
        * to the parent of the current file*
        */
        if (!strcmp(p, "..")) {
            currentNode = move_back(currentNode);
        } else {
            /**
            * otherwise we move to the desired file / folder using the move_next
            * function*
            */
            currentNode = move_next(currentNode, p);
        }

        // in case the searched file was not found we print an error message
        if (!currentNode) {
            printf("cd: no such file or directory: %s\n", temp);
            free(temp);
            return initial_folder;
        }

        p = strtok(NULL, "/");
    }
    free(temp);

    return currentNode;
}

void tree(TreeNode* currentNode, char* arg)
{
    // if the path is specified, we proceed identically with the cd function
    if (strlen(arg) != 0) {
        char *temp = malloc((strlen(arg) + 1) * sizeof(char));
        DIE(!temp, "Malloc failed!");
        memcpy(temp, arg, (strlen(arg) + 1) * sizeof(char));

        // we divide the arg
        char *p;
        p = strtok(arg, "/");
        while (p) {
            /**
            * if we encounter ".." we use the move_back function to move
            * to the parent of the current file*
            */
            if (!strcmp(p, "..")) {
                currentNode = move_back(currentNode);
            } else {
                /**
                * otherwise we move to the desired file / folder using the move_next
                * function*
                */
                currentNode = move_next(currentNode, p);
            }

            // in case the searched file was not found we print an error message
            if (!currentNode) {
                printf("%s [error opening dir]\n\n0 directories, 0 files\n",
                       temp);
                free(temp);
                return;
            }

            p = strtok(NULL, "/");
        }
        free(temp);
    }

    int lvl_number = 0, no_file = 0, no_folder = 0;

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

            no_file++;

        /**
        * if what we found is a folder, we go into it recursively and
        * proceed as before*
        */
        } else if (((TreeNode*)temp->info)->type == FOLDER_NODE) {
            for (int i = 0; i < lvl_number; i++)
                printf("\t");
            printf("%s\n", ((TreeNode*)temp->info)->name);
            no_folder++;
            lvl_number++;

            tree_recursiv(((TreeNode*)temp->info), lvl_number,
                          &no_file, &no_folder);

            lvl_number--;
        }
        curr = curr->next;
    }

    printf("%d directories, %d files\n", no_folder, no_file);
}

void mkdir(TreeNode* currentNode, char* folderName) {
    List *list = ((FolderContent*)currentNode->content)->children;

    /**
    * we check if there is already a file with the specified name,
    * and if so we print an error message*
    */
    ListNode* curr = list->head;
    while (curr) {
        TreeNode* f = curr->info;
        if (strcmp(f->name, folderName) == 0) {
            printf("mkdir: cannot create directory '%s': File exists\n",
                   f->name);
            return;
        }

        curr = curr->next;
    }

    /**
    * we create a new folder by allocating memory and initializing
    * all left fields*
    */
    TreeNode *folder = malloc(sizeof(TreeNode));
    DIE(!folder, "Malloc failed!");
    folder->parent = currentNode;
    folder->name = malloc((strlen(folderName) + 1) * sizeof(char));
    DIE(!folder->name, "Malloc failed!");
    memcpy(folder->name, folderName, (strlen(folderName) + 1) * sizeof(char));
    folder->type = FOLDER_NODE;
    folder->content = malloc(sizeof(FolderContent));
    DIE(!folder->content, "Malloc failed!");

    FolderContent* new_folder_content = malloc(sizeof(FolderContent));
    DIE(!new_folder_content, "Malloc failed!");
    new_folder_content->children = malloc(sizeof(List));
    DIE(!new_folder_content->children, "Malloc failed!");
    new_folder_content->children->head = NULL;
    memcpy(folder->content, new_folder_content, sizeof(FolderContent));

    free(new_folder_content);

    ll_add_nth_node(list, 0, folder);

    free(folder);
}

void rmrec(TreeNode* currentNode, char* resourceName) {
    /**
    * we check if a file with the specified name already exists
    * and if not, we print an error message*
    */
    List* list = ((FolderContent*)currentNode->content)->children;
    ListNode* curr = list->head;
    TreeNode* f;
    int ok = 0, index = 0;

    while (curr) {
        f = curr->info;
        if (strcmp(f->name, resourceName) == 0) {
            ok = 1;
            break;
        }
        index++;

        curr = curr->next;
    }

    if (!ok) {
        printf("rmrec: failed to remove '%s': No such file or directory\n",
               resourceName);
    } else {
        /**
        * we delete the specified folder and use the rmrec_recursive function
        * to delete everything in it*
        */
         if (f->type == FILE_NODE) {
            ll_remove_nth_node(list, index);
        } else if (f->type == FOLDER_NODE) {
            if (((FolderContent*)f->content)->children->head)
                rmrec_recursive(f);

            ll_remove_nth_node(list, index);
        }
    }
}

void rm(TreeNode* currentNode, char* fileName) {
    List* list = ((FolderContent*)currentNode->content)->children;
    /**
    * we check if a file with the specified name already exists
    * and if not, we print an error message*
    */
    ListNode* curr = list->head;
    TreeNode* f;
    int ok = 0, index = 0;
    while (curr) {
        f = curr->info;
        if (strcmp(f->name, fileName) == 0 && f->type == FOLDER_NODE) {
            printf("rm: cannot remove '%s': Is a directory\n", fileName);
            return;
        }

        if (strcmp(f->name, fileName) == 0 && f->type == FILE_NODE) {
            ok = 1;
            break;
        }

        index++;

        curr = curr->next;
    }
    if (!ok) {
        printf("rm: failed to remove '%s': No such file or directory\n",
               fileName);
    } else {
        // we use ll_remove_nth_node function to remove the desired file
        ll_remove_nth_node(list, index);
    }
}

void rmdir(TreeNode* currentNode, char* folderName) {
    List* list = ((FolderContent*)currentNode->content)->children;
    /**
    * we check if a file with the specified name already exists
    * and if not, we print an error message*
    */
    ListNode* curr = list->head;
    TreeNode* f;
    int ok = 0, index = 0;
    while (curr) {
        f = curr->info;
        // if the found file is not a folder, an error message will be printed
        if (strcmp(f->name, folderName) == 0 && f->type == FILE_NODE) {
            printf("rmdir: failed to remove '%s': Not a directory\n",
                   folderName);
            return;
        }

        if (strcmp(f->name, folderName) == 0 && f->type == FOLDER_NODE) {
            ok = 1;
            break;
        }

        index++;
        curr = curr->next;
    }
    if (!ok) {
        printf("rmdir: failed to remove '%s': No such file or directory\n",
               folderName);
    } else {
        List* curr_list = ((FolderContent*)f->content)->children;

        /**
        * if the found file is not an empty folder, an error message
        * will be printed*
        */
        if (curr_list->head) {
            printf("rmdir: failed to remove '%s': Directory not empty\n",
                   folderName);
        } else {
            // we use ll_remove_nth_node function to remove the desired file
            ll_remove_nth_node(list, index);
        }
    }
}


void touch(TreeNode* currentNode, char* fileName, char* fileContent) {
    List *list = ((FolderContent*)currentNode->content)->children;

    // we check if a file with the specified name already exists
    ListNode* curr = list->head;
    while (curr) {
        TreeNode* f = curr->info;
        if (strcmp(f->name, fileName) == 0)
            return;
        curr = curr->next;
    }

    /**
    * we create a new file by allocating memory and initializing
    * all left fields*
    */
    TreeNode *file = malloc(sizeof(TreeNode));
    DIE(!file, "Malloc failed!");
    file->parent = currentNode;
    file->name = malloc((strlen(fileName) + 1) * sizeof(char));
    DIE(!file->name, "Malloc failed!");
    memcpy(file->name, fileName, (strlen(fileName) + 1) * sizeof(char));
    file->type = FILE_NODE;
    file->content = malloc(sizeof(FileContent));
    DIE(!file->content, "Malloc failed!");

    ((FileContent*)file->content)->text = malloc((strlen(fileContent) + 1)
                                                 * sizeof(char));
    DIE(!((FileContent*)file->content)->text, "Malloc failed!");
    memcpy(((FileContent*)file->content)->text, fileContent,
           (strlen(fileContent) + 1) * sizeof(char));

    // we use ll_remove_nth_node function to add the desired file
    ll_add_nth_node(list, 0, file);

    free(file);
}

void cp(TreeNode* currentNode, char* source, char* destination) {
    TreeNode* sourceNode = currentNode;
    char *temp_source = malloc((strlen(source) + 1) * sizeof(char));
    DIE(!temp_source, "Malloc failed!");
    memcpy(temp_source, source, (strlen(source) + 1) * sizeof(char));

    // we divide the source path
    char *p;
    p = strtok(source, "/");
    while (p) {
        /**
        * if we encounter ".." we use the move_back function to move
        * to the parent of the current file*
        */
        if (!strcmp(p, "..")) {
            sourceNode = move_back(sourceNode);
        } else {
            /**
            * otherwise we move to the desired file / folder using the move_next
            * function*
            */
            sourceNode = move_next_cp(sourceNode, p);
        }

        p = strtok(NULL, "/");
    }

    if (sourceNode->type == FOLDER_NODE) {
        printf("cp: -r not specified; omitting directory '%s'", temp_source);
        return;
    }
    free(temp_source);

    char* source_content = malloc((strlen(((FileContent*)sourceNode
                                  ->content)->text) + 1) * sizeof(char));
    DIE(!source_content, "Malloc failed!");
    memcpy(source_content, ((FileContent*)sourceNode->content)->text, (strlen((
           (FileContent*)sourceNode->content)->text) + 1) * sizeof(char));
    char *aux = malloc((strlen(destination) + 1) * sizeof(char));
    DIE(!aux, "Malloc failed!");
    memcpy(aux, destination, (strlen(destination) + 1) * sizeof(char));

    int counter = 0;

    p = strtok(aux, "/");
    while (p) {
        counter++;
        p = strtok(NULL, "/");
    }

    free(aux);

    TreeNode* destinationNode = currentNode;
    char *temp_destination = malloc((strlen(destination) + 1) * sizeof(char));
    DIE(!temp_destination, "Malloc failed!");
    memcpy(temp_destination, destination,
           (strlen(destination) + 1) * sizeof(char));

    // we divide the destination patn
    p = strtok(destination, "/");
    while (p) {
        /**
        * if we encounter ".." we use the move_back function to move
        * to the parent of the current file*
        */
        if (!strcmp(p, "..")) {
            destinationNode = move_back(destinationNode);
        } else {
            /**
            * otherwise we move to the desired file / folder using the move_next
            * function*
            */
            destinationNode = move_next_cp(destinationNode, p);
        }

        TreeNode* final_destination;
        if (counter == 2 && destinationNode)
            final_destination = destinationNode;

		/**
        * if the destination path is a non-existing file we copy the source
        * node's data (name,text) and make a node with the name given by the
        * last parameter of the destination path's parse*
        */
        if (counter == 1 && !destinationNode){
            touch(final_destination, p, source_content);
            break;
        }

        /**
        * if the destination path is an existing file we copy the source node's
        * data (name,text) directly into the destination node's data fields*
        */
        if (counter == 1 && destinationNode &&
            destinationNode->type == FILE_NODE){
            ((FileContent*)destinationNode->content)->text =
             realloc(((FileContent*)destinationNode->content)->text,
                     (strlen(source_content) + 1) * sizeof(char));

            memcpy(((FileContent*)destinationNode->content)->text,
                   source_content, (strlen(source_content) + 1) *
                   sizeof(char));
            break;
        }

        /**
        * if the destination path is an existing folder we copy the source
        * node's data (name,text) and call ll_add_nth_node*
        */
        if (counter == 1 && destinationNode &&
            destinationNode->type == FOLDER_NODE){
            touch(destinationNode, sourceNode->name, source_content);
            break;
        }

        // in any other case we print an error message
        if (!destinationNode) {
            printf("cp: failed to access '%s': Not a directory\n",
                   temp_destination);
            free(source_content);
            free(temp_destination);
            return;
        }

        counter--;

        p = strtok(NULL, "/");
    }

    free(source_content);
    free(temp_destination);
}

void mv(TreeNode* currentNode, char* source, char* destination) {
    TreeNode* sourceNode = currentNode;
    char *temp_source = malloc((strlen(source) + 1) * sizeof(char));
    DIE(!temp_source, "Malloc failed!");
    memcpy(temp_source, source, (strlen(source) + 1) * sizeof(char));

    // we divide the source path
    char *p;
    p = strtok(source, "/");
    while (p) {
        /**
        * if we encounter ".." we use the move_back function to move
        * to the parent of the current file*
        */
        if (!strcmp(p, "..")) {
            sourceNode = move_back(sourceNode);
        } else {
            /**
            * otherwise we move to the desired file / folder using the move_next
            * function*
            */
            sourceNode = move_next_cp(sourceNode, p);
        }

        p = strtok(NULL, "/");
    }

    free(temp_source);

    TreeNode* destinationNode = currentNode;
    char *temp_destination = malloc((strlen(destination) + 1) * sizeof(char));
    DIE(!temp_destination, "Malloc failed!");
    memcpy(temp_destination, destination,
           (strlen(destination) + 1) * sizeof(char));
    // we divide the destination path
    p = strtok(destination, "/");
    while (p) {
        /**
        * if we encounter ".." we use the move_back function to move
        * to the parent of the current file*
        */
        if (!strcmp(p, "..")) {
            destinationNode = move_back(destinationNode);
        } else {
            /**
            * otherwise we move to the desired file / folder using the move_next
            * function*
            */
            destinationNode = move_next_cp(destinationNode, p);
        }

        if (!destinationNode) {
            printf("mv: failed to access '%s': Not a directory\n",
                   temp_destination);
            free(temp_destination);
            return;
        }

        p = strtok(NULL, "/");
    }

    free(temp_destination);

    /**
    * if the destination path is a folder we change all existing connections
    * between nodes in such a way that the destination node cointains the
    * source node*
    */
    if (destinationNode->type == FOLDER_NODE) {
        List* list = ((FolderContent*)destinationNode->content)->children;

        List* list1 = ((FolderContent*)sourceNode->parent->content)->children;
        if (list1->head->info == sourceNode) {
            list->head = list1->head;
            ListNode* node = list1->head->next;
            list1->head->next = NULL;
            list1->head = node;
        }
    } else {
        /**
        * if both paths are pointing to files we delete the source file and copy
        * its text to the destination file*
        */
        char* source_content = malloc((strlen(((FileContent*)sourceNode->
                                      content)->text) + 1) * sizeof(char));
        DIE(!source_content, "Malloc failed!");
        memcpy(source_content, ((FileContent*)sourceNode->content)->text,
               (strlen(((FileContent*)sourceNode->content)->text) + 1) *
                sizeof(char));

        ((FileContent*)destinationNode->content)->text =
         realloc(((FileContent*)destinationNode->content)->text,
                 (strlen(source_content) + 1) * sizeof(char));
        memcpy(((FileContent*)destinationNode->content)->text, source_content,
               (strlen(source_content) + 1) * sizeof(char));
        List* list = ((FolderContent*)sourceNode->parent->content)->children;

        ll_remove_nth_node(list, 0);

        free(source_content);
    }
}

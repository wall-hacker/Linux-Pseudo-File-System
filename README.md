````
  _     _                  _____ _ _      ____            
 | |   (_)_ __  _   ___  _|  ___(_| | ___/ ___| _   _ ___ 
 | |   | | '_ \| | | \ \/ | |_  | | |/ _ \___ \| | | / __|
 | |___| | | | | |_| |>  <|  _| | | |  __/___) | |_| \__ \
 |_____|_|_| |_|\__,_/_/\_|_|   |_|_|\___|____/ \__, |___/
                                                |___/     
````

- - -
### Description
- - -  

A simulation of a simple linux file system where a number of
commands are read from stdin, and processed in the main file.
Every function called by a command deals with the respective
errors and requested task. In order to create a file system
like program we have to create an arborescent structure that
links several node, files and folders (files are nodes filled
with text, and folders and nodes that contain a list) alike
into a unified easy to manipulate form. The initial tree is 
composed of a single folder, named root.

- - -
### Commands && Implementation 
- - -

Here is a list of all implemented commands and their
respective description:

**1] 'touch':**
- in order to implement this function we have to add a file
type node to the list found inside our current tree node.
- first we access our folders list and check wheter there
isn't a file with the same name
- if we find that a file already exists the function ends
- if there isn't such a file, we create a new file type tree 
node and 'fill it' all of its fields including name and text
- last but not least we add the newly created node to the
list using our auxiliary function

**2] 'ls':**
- if the argument given is NULL we iterate through the list
found inside our current tree node and print all file names
- if the argument given is a folder we iterate through the
list found inside that given tree node and print all file
names
- if the argument given is a file we access its content
field and print the text found inside
- if the argument is a non-existent file or folder we print
an error message

**3] 'mkdir':**
- in order to implement this function we have to add a
folder type node to the list found inside our current tree
node.
- first we access our folders list and check wheter there
isn't a folder with the same name
- if we find that a folder already exists we print an error
message
- if there isn't such a folder, we create a new folder type
tree node and 'fill it' all of its fields including
allocating an empty linked list and pointing its head to
NULL
- last but not least we add the newly created node to the
list using our auxiliary function

**4] 'cd':**
- in order to implement this function we have decided to use
2 auxiliary functions one that mimics "cd .." and moves back
a folder, and one that moves into the next given folder
- first we save a copy of the initial folder, then we parse
the path
- we call auxiliary function depending on the parsed
"keyword"
- move_back changes the current folder node to its parent
and it returns said node or NULL if the current node does
not have a parent;
- move_next has to iterate through the current folders'
contents and in only in the case it finds a folder with the
given name the function returns it; if that folder does not
exist the function returns NULL
- last but not least if the returned pointer is NULL that
means we have been given an incorrect path; we printf an
error message and return the node that was given at the
begging of the cd function
- if the returned pointer is not NULL that means that we
have reached the desired folder and we can return that node

**5] 'tree':**
- if the argument given is NULL we have to show the entire
arborescent structure
- if the argument given is a path we parse it and change the
current node as we did in "cd" using move_back and
move_forward and also print the corresponding error message
- we then iterate through the arborescent structure
recursively and print all the nodes, while also counting the
number of files and folders

**6] 'pwd':**
- starting from the current node, we travel to the root of
the tree and keep the folder names in a string array
- we then print the path by iterating through the string
array

**7] 'rmdir':**
- we get the node for the specified folder and its index
inside its list (only if it exists and it is empty, else we 
print an error message)
- then we call ll_remove_nth_node and remove the node from
the newly found index

**8] 'rm':**
- we get the node for the specified file and its index inside
its list (only if it exists and is not a folder, else we
print an error message)
- then we call ll_remove_nth_node and remove the node from
the newly found index

**9] 'rmrec':**
- if the given node exists we either remove it in the case
of a file or call rmrec_rescursive in the case of a folder
- remrec_recursive calls itself until all files and
subfolders have been iterated through, and their respective
nodes have been removed

**10] 'cp':**
- we parse the 2 paths (source & destination) and get their
nodes
- if the destination path is an existing folder we copy the
source node's data (name,text) and call ll_add_nth_node
- if the destination path is an existing file we copy the
source node's data (name,text) directly into the destination
node's data fields
- if the destination path is a non-existing file we copy the
source node's data (name,text) and make a node with the name
given by the last parameter of  the destination path's parse
- in any other case we print an error message

**11] 'mv':**
- we parse the 2 paths (source & destination) and get their
nodes
- if both paths are pointing to files we delete the source
file and copy its text to the destination file
- if the destination path is a folder we change all existing
connections between nodes in such a way that the destination
node cointains the source node
- in any other case we print an error message

- - -
### Feedback && Comments
- - -

This was the most challenging project so far that had to do
with data structures and algorithms. It was a great
experience but also a very time consuming one. I have
learned a lot about how to implement a file system and how
to use linked lists in and arborescent structures. I have
also learned how to use recursion in order to iterate
through these arborescent structures and how to use pointers
in order to change the connections between nodes.
#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BST* Tree;
typedef struct BST {
    int value;
    struct BST* left;
    struct BST* right;
}BST;

Tree insertNode(Tree root, int key);
Tree findMin(Tree tree);
Tree deleteNode(Tree root, int key);
int findNode(Tree root, int key);
void printInorder(Tree root);
void deleteTree(Tree root);

void main(int argc, char* argv[]) {
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    char cv;
    int key;

    Tree root = NULL;

    while (1) {
        fscanf(fin, "%c", &cv);
        if (feof(fin)) {
            break;
        }
        switch (cv) {
            case 'i':
                fscanf(fin, "%d", &key);
                root = insertNode(root, key);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if (findNode(root, key)) {
                    fprintf(fout, "%d is in the tree\n", key);
                } else {
                    fprintf(fout, "finding error: %d is not in the tree\n", key);
                }
                break;
            case 'd':
                fscanf(fin, "%d", &key);
                if (findNode(root, key)) {
                    root = deleteNode(root, key);
                    fprintf(fout, "delete %d\n", key);
                } else {
                    fprintf(fout, "delete error : %d is not in the tree\n", key);
                }
                break;
            case 'p':
                fscanf(fin, "%c", &cv);
                if (cv == 'i') {
                    if (root == NULL) {
                        fprintf(fout, "tree is empty");
                    } else {
                        printInorder(root);
                    }
                }
                fprintf(fout, "\n");
                break;
        }
    }
    deleteTree(root);
}

Tree insertNode(Tree root, int key) {
    if (root == NULL) {
        root = (Tree)malloc(sizeof(BST));
        if (root == NULL) {
            fprintf(fout, "Out of space!!!\n");
            return NULL;
        }
        root -> value = key;
        root -> left = root -> right = NULL; 
        fprintf(fout, "insert %d\n", key);
    } else if (root -> value > key) {
        root -> left = insertNode(root -> left, key);
    } else if (root -> value < key) {
        root -> right = insertNode(root -> right, key);
    } else {
        fprintf(fout, "insertion error: %d is already in the tree\n", key);
        //return NULL;
    }
    return root;
}

Tree findMin(Tree tree) {
    if (tree == NULL) {
        return NULL;
    } else if (tree -> left == NULL) {
        return tree;
    }
    return findMin(tree -> left);
}

Tree deleteNode(Tree root, int key) {
    Tree tmp;
    if (root -> value > key) {
        root -> left = deleteNode(root -> left, key);
    } else if (root -> value < key) {
        root -> right = deleteNode(root -> right, key);
    } else if (root -> left && root -> right) { // 2 children
        tmp = findMin(root -> right);
        root -> value = tmp -> value;
        root -> right = deleteNode(root -> right, root -> value);
    } else { // 0 or 1 child
        tmp = root;
        if (root -> left == NULL) {
            root = root -> right;
        } else {
            root = root -> left;
        }
        free(tmp);
    }
    return root;
}

int findNode(Tree root, int key) {
    if (root == NULL) {
        return 0;
    } else if (root -> value > key) {
        return findNode(root -> left, key);
    } else if (root -> value < key) {
        return findNode(root -> right, key);
    }
    return 1;
}

void printInorder(Tree root) {
    if (root) {
        printInorder(root -> left);
        fprintf(fout, "%d ", root -> value);
        printInorder(root -> right);
    }
}

void deleteTree(Tree root) {
    if (root) {
        deleteTree(root -> left);
        Tree tmp = root -> right;
        free(root);
        deleteTree(tmp);
    }
}

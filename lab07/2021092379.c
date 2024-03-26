#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

FILE* fin;
FILE* fout;

struct AVLNode;
typedef struct AVLNode* Position;
typedef struct AVLNode* AVLTree;
typedef int ElementType;

typedef struct AVLNode {
    ElementType element;
    AVLTree left, right;
    int height;
} AVLNode;

AVLTree Insert(ElementType X, AVLTree T);
AVLTree Delete(ElementType X, AVLTree T);
Position SingleRotateWithLeft(Position node);
Position SingleRotateWithRight(Position node);
Position DoubleRotateWithLeft(Position node);
Position DoubleRotateWithRight(Position node);
void PrintInorder(AVLTree T);
void DeleteTree(AVLTree T);
int Height(Position node);
Position FindMin(AVLTree T);
int max(int a, int b);

int main(int argc, char* argv[]) {
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    AVLTree Tree = NULL;
    char cv;
    int key;
    while (1) {
        fscanf(fin, "%c", &cv);
        if (feof(fin)) {
            break;
        }
        switch (cv) {
            case 'i':
                fscanf(fin, "%d\n", &key);
                Tree = Insert(key, Tree);
                break;
            case 'd':
                fscanf(fin, "%d\n", &key);
                Tree = Delete(key, Tree);
                break;
        }
        PrintInorder(Tree);
        fprintf(fout, "\n");
    }
    DeleteTree(Tree);
    fclose(fin);
    fclose(fout);
    return 0;
}

// Function definitions

int Height(Position node) {
    if (node == NULL) {
        return -1;
    }
    return node -> height;
}

Position SingleRotateWithLeft(Position node) { //RR
    Position p;
    p = node -> right;
    node -> right = p -> left;
    p -> left = node;

    node -> height = max(Height(node -> left), Height(node -> right)) + 1;
    p -> height = max(Height(p -> left), Height(p -> right)) + 1;

    return p;
}

Position SingleRotateWithRight(Position node) { //LL
    Position p;
    p = node -> left;
    node -> left = p -> right;
    p -> right = node;

    node -> height = max(Height(node -> left), Height(node -> right)) + 1;
    p -> height = max(Height(p -> left), Height(p -> right)) + 1;

    return p; // new node
}

Position DoubleRotateWithLeft(Position node) { //LR
    node -> right = SingleRotateWithRight(node -> right);
    return SingleRotateWithLeft(node);
}

Position DoubleRotateWithRight(Position node) { //RL
    node -> left = SingleRotateWithLeft(node -> left);
    return SingleRotateWithRight(node);
}

AVLTree Insert(ElementType x, AVLTree tree) {
    if (tree == NULL) {
        tree = malloc(sizeof(struct AVLNode));
        assert(tree != NULL);
        tree -> element = x;
        tree -> height = 0;
        tree -> left = tree -> right = NULL;
    } else if (x < tree -> element) {
        tree -> left = Insert(x, tree -> left);
        if (Height(tree -> left) - Height(tree -> right) >= 2) {
            if (x < tree -> left -> element) { // LL
                tree = SingleRotateWithRight(tree);
            } else { // RL
                tree = DoubleRotateWithRight(tree);
            }
        }
    } else if (x > tree -> element) {
        tree -> right = Insert(x, tree -> right);
        if (Height(tree -> right) - Height(tree -> left) >= 2) {
            if (x > tree -> right -> element) { // RR
                tree = SingleRotateWithLeft(tree);
            } else { // LR
                tree = DoubleRotateWithLeft(tree);
            }
        }
    } else {
        fprintf(fout, "insertion error : %d is already in the tree!\n", x);
    }
    tree -> height = max(Height(tree -> left), Height(tree -> right)) + 1;
    return tree;
}

Position Rebalance(Position p) {
    if (Height(p -> left) - Height(p -> right) >= 2) {
        Position l = p -> left;
        if (Height(l-> left) < Height(l -> right)) {
            return DoubleRotateWithRight(p);
        } else {
            return SingleRotateWithRight(p);
        }
    } else if (Height(p -> right) - Height(p -> left) >= 2) {
        Position r = p -> right;
        if (Height(r -> left) > Height(r -> right)) {
            return DoubleRotateWithLeft(p);
        } else {
            return SingleRotateWithLeft(p);
        }
    } else {
        return p;
    }
}

AVLTree Delete(ElementType X, AVLTree T) {
    Position tmp;
    if (T == NULL) {
        fprintf(fout, "deletion error : %d is not in the tree!\n", X);
    } else if (T -> element > X) {
        T -> left = Delete(X, T -> left);
    } else if (T -> element < X) {
        T -> right = Delete(X, T -> right);
    } else if (T -> left && T -> right) {
        // has two children
        tmp = FindMin(T -> right);
        T -> element = tmp -> element;
        T -> right = Delete(T -> element, T -> right);
    } else {
        // 0 or 1 child.
        tmp = T;
        if (T -> left == NULL) {
            T = T -> right;
        } else {
            T = T -> left;
        }
        free(tmp);
    }

    if (T == NULL) {
        return NULL;
    } 
    T -> height = max(Height(T -> left), Height(T -> right)) + 1;
    return Rebalance(T);
}

void PrintInorder(AVLTree T) {
    if (T) {
        PrintInorder(T -> left);
        fprintf(fout, "%d(%d) ", T -> element, T -> height);
        PrintInorder(T -> right);
    }
}

void DeleteTree(AVLTree T) {
    if (T) {
        DeleteTree(T -> left);
        AVLTree tmp = T -> right;
        free(T);
        DeleteTree(tmp);
    }
}

Position FindMin(AVLTree T) {
    if (T == NULL) {
        return NULL;
    }
    if (T -> left == NULL) {
        return T;
    }
    return FindMin(T -> left);
}

int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}
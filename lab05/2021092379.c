#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

struct ThreadedTree {
    int left_thread;
    ThreadedPtr left_child;
    ElementType data;
    ThreadedPtr right_child;
    int right_thread;
} ThreadedTree;

ThreadedPtr CreateTree();
int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx);
void printInorder(ThreadedPtr root);
void DeleteTree(ThreadedPtr root);

// Stack
typedef struct Stack {
    int *key;
    int top;
    int max_stack_size;
} Stack;

Stack* CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);
void DeleteStack(Stack* S);
int IsEmpty(Stack* S);
int IsFull(Stack* S);

void PrintStack(Stack* S) {
    for (int i = 0; i <= S->top; ++i) {
        printf("%d ", S->key[i]);
    }
    printf("\n");
}


int main(int argc, char* argv[]) {
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    ThreadedPtr root = CreateTree();
    if (root == NULL) {
        return 0;
    }

    int NUM_NODES;
    fscanf(fin, "%d", &NUM_NODES);
    int root_idx = 0, idx = 0;

    while (++idx <= NUM_NODES) {
        ElementType data;
        fscanf(fin, "%d", &data);
        if (Insert(root, root_idx, data, idx) == 0) {
            return 0;
        }
    }

    printInorder(root);
    DeleteTree(root);

    fclose(fin);
    fclose(fout);
    return 0;
}

ThreadedPtr CreateTree() {
    ThreadedPtr tree = NULL;
    tree = (ThreadedPtr)malloc(sizeof(ThreadedTree));
    if (tree == NULL) {
        fprintf(fout, "Out of Space!\n");
        return NULL;
    }
    tree -> data = -1;
    tree -> left_thread = 1;
    tree -> left_child = tree;
    tree -> right_thread = 1;
    tree -> right_child = tree;
    return tree;
}

int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx) {
    ThreadedPtr temp = NULL;
    temp = (ThreadedPtr)malloc(sizeof(ThreadedTree));
    if (temp == NULL) {
        fprintf(fout, "Out of Space!\n");
        return 0;
    }
    temp -> data = data;
    temp -> left_thread = 0;
    temp -> right_thread = 0;

    Stack* st = CreateStack(100);

    while (idx / 2 != root_idx) {
        Push(st, idx % 2);
        idx /= 2;
    }
    Push(st, 1);

    //PrintStack(st);
    ThreadedPtr now = root;

    while (st -> top > 0) { 
        if (Top(st) == 0) { // left
            now = now -> left_child;
        } else { // right
            now = now -> right_child;
        }
        Pop(st);
    }
    if (Top(st) == 0) { // left
        temp -> left_child = now -> left_child;
        temp -> right_child = now;

        now -> left_thread = 1;
        now -> left_child = temp;

    } else { // right
        temp -> left_child = now;
        temp -> right_child = now -> right_child;

        now -> right_thread = 1;
        now -> right_child = temp;
    }

    DeleteStack(st);
    return 1;
}

ThreadedPtr InSucc(ThreadedPtr tree) {
    ThreadedPtr tmp;
    tmp = tree -> right_child;
    if (tree -> right_thread) {
        while (tmp -> left_thread) {
            tmp = tmp -> left_child;
        }
    }
    return tmp;
}

void printInorder(ThreadedPtr root) {
    fprintf(fout, "inorder traversal : ");
    ThreadedPtr tmp = root;
    for (;;) {
        tmp = InSucc(tmp);
        if (tmp == root) {
            break;
        }
        fprintf(fout, "%d ", tmp -> data);
    }
    fprintf(fout, "\n");
}

void DeleteTree(ThreadedPtr root) {
    ThreadedPtr node = root;
    ThreadedPtr tmp;
    for (;;) {
        tmp = node;
        node = InSucc(node);
        //printf("delete(%d)\n", tmp -> data);
        free(tmp);
        if (node == root) {
            break;
        }
    }
}

// Stack Function Definitions
Stack* CreateStack(int max) {
    Stack* stack = malloc(sizeof(Stack));
    stack -> max_stack_size = max;
    stack -> top = -1;
    stack -> key = malloc(sizeof(int)*max);
    return stack;
}

void Push(Stack* S, int X) {
    if (!IsFull(S)) {
        S -> key[++(S->top)] = X;
    }
}

int Pop(Stack* S) {
    if (IsEmpty(S)) {
        return -1;
    } else {
        S -> top--;
        return 0;
    }
}

int Top(Stack* S) {
    if (!IsEmpty(S)) {
        return S -> key[S->top];
    }
    return 0;
}

void DeleteStack(Stack* S) {
    free(S -> key);
    free(S);
}

int IsEmpty(Stack* S) {
    if (S -> top < 0) {
        return 1;
    }
    return 0;
}

int IsFull(Stack* S) {
    if (S -> top == S -> max_stack_size - 1) {
        return 1;
    } 
    return 0;
}
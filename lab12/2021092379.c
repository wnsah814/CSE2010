#include <stdio.h>
#include <stdlib.h>

FILE* fin;
FILE* fout;

typedef struct BNode* BNodePtr;

struct BNode {
    int order; // m: capacity
    int size; // number of keys
    BNodePtr* child; // # of children = order + 1
    int* key; // # of keys = order
    int is_leaf; // whether this node is leaf
} BNode;

BNodePtr CreateTree(int order);
BNodePtr SplitNode(int pos, BNodePtr node, BNodePtr parent, BNodePtr root);
BNodePtr InsertNode(int parent_pos, int key, BNodePtr node, BNodePtr parent, BNodePtr root);
void Insert(BNodePtr* root, int key);
int Find(BNodePtr root, int key);
void PrintTree(BNodePtr root);
void DeleteTree(BNodePtr root);

int main(int argc, char* argv[]) {
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    int order;
    fscanf(fin, "%d", &order);
    BNodePtr root = CreateTree(order);
    char cv;
    int key;
    while (1) {
        fscanf(fin, "%c", &cv);
        if (feof(fin)) {
            break;
        }
        switch (cv) {
            case 'i':
                fscanf(fin, "%d", &key);
                if (Find(root, key)) {
                    fprintf(fout, "insert error : key %d is already in the tree!\n", key);
                } else {
                    Insert(&root, key);
                }
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if (Find(root, key)) {
                    fprintf(fout, "key %d found\n", key);
                } else {
                    fprintf(fout, "finding error : key %d is not in the tree\n", key);
                }
                break;
            case 'p':
                if (root -> size == 0) {
                    fprintf(fout, "print error : tree is empty!");
                } else {
                    PrintTree(root);
                }
                fprintf(fout, "\n");
                break;
        }
    }
    DeleteTree(root);
    fclose(fin);
    fclose(fout);

    return 0;
}

BNodePtr CreateTree(int order) {
    BNodePtr root = (BNodePtr)malloc(sizeof(BNode));
    root -> order = order; // m-way: m == order
    root -> size = 0; // # of keys
    root -> child = (BNodePtr*)malloc(sizeof(BNodePtr) * root -> order + 1); // To make it easier to implement
    for (int i = 0; i < root->order + 1; ++i) {
        root->child[i] = NULL; // init to NULL
    }
    root -> key = (int*)malloc(sizeof(int) * (root -> order)); // m-way tree has m-1 keys but to make it easier to implement, I set maximum to m
    root -> is_leaf = 1; // init true
    return root;
}

BNodePtr SplitNode(int pos, BNodePtr node, BNodePtr parent, BNodePtr root) {
    int mid = node -> size / 2;
    BNodePtr right = CreateTree(node -> order);
    right -> is_leaf = node -> is_leaf; // Just follow 

    int keyIt = node -> size, childIt = node -> size + 1;
    for (int i = mid + 1; i < keyIt; ++i) {
        right -> key[right->size++] = node -> key[i];
        node -> size--;
    }

    if (!node -> is_leaf) { // If this node is not leaf, then gather children
        for (int i = mid + 1; i < childIt; ++i) {
            right -> child[i - (mid + 1)] = node -> child[i];
        }
    }
    
    if (node == root) { // When node is root, we need to make a new parent node
        BNodePtr newParent = CreateTree(node -> order);
        newParent -> is_leaf = 0;
        newParent -> key[newParent -> size++] = node -> key[mid];

        node -> size--;
        newParent -> child[0] = node;
        newParent -> child[1] = right;
        return newParent;
    } else { // If not, use existing node
        for (int i = parent -> size; i > pos; --i) {
            parent -> key[i] = parent -> key[i - 1];
            parent -> child[i + 1] = parent -> child[i];
        }
        parent -> key[pos] = node -> key[mid];
        parent -> size++;
        node -> size--;
        parent -> child[pos+1] = right;
    }
    return node;
}

BNodePtr InsertNode(int parent_pos, int key, BNodePtr node, BNodePtr parent, BNodePtr root) {
    int pos; // position to insert
    for (pos = 0; pos < node -> size; ++pos) {
        if (key < node -> key[pos]) break;
    }
    if (!node -> is_leaf) { // If this node is not a leaf node
        node -> child[pos] = InsertNode(pos, key, node -> child[pos], node, root);
        if (node -> size == node -> order) { // full
            node = SplitNode(parent_pos, node, parent, root);
        }
    } else { // If this node is a leaf node
        for (int i = node -> size; i > pos; --i) {
            node -> key[i] = node ->key[i-1]; // Move keys back
            node -> child[i + 1] = node -> child[i]; // children also
        }
        node -> key[pos] = key;
        node -> size++;
        if (node -> size == node -> order) {
            node = SplitNode(parent_pos, node, parent, root);
        }
    }
    return node;
}

void Insert(BNodePtr* root, int key) {
    *root = InsertNode(0, key, *root, *root, *root);
}

int Find(BNodePtr root, int key) {
    int i;
    if (root == NULL) return 0;
    for (i = 0; i < root->size; ++i) {
        if (key == root->key[i]) {
            return 1;
        }
        if (key < root -> key[i]) {
            return Find(root -> child[i], key);
        }
    }
    return Find(root->child[i], key);
}

void PrintTree(BNodePtr root) {
    int i;
    if (root == NULL) {
        return;
    }
    for (i = 0; i < root->size; ++i) {
        PrintTree(root->child[i]);
        fprintf(fout, "%d ", root -> key[i]);
    }
    PrintTree(root->child[i]);
}

void DeleteTree(BNodePtr root) {
    if (root == NULL) return;
    int i;
    for (i = 0; i < root -> size; ++i) {
        DeleteTree(root -> child[i]);
    }
    DeleteTree(root -> child[i]);
    free(root -> key);
    free(root -> child);
    free(root);
}
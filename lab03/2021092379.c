#include <stdio.h>
#include <stdlib.h>

typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

struct Node {
    ElementType element;
    Position next;
};

List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
void Insert(ElementType X, List L, Position P);
void PrintList(List L);
void Delete(ElementType X, List L);
Position Find(ElementType X, List L);
Position FindPrevious(ElementType X, List L);
void DeleteList(List L);

FILE *input;
FILE *output;

int main(int argc, char* argv[]) {
    char command;
    int key1, key2;
    FILE *input;
    Position header = NULL;
    Position tmp = NULL;
    if (argc == 1) {
        input = fopen("input.txt", "r");
        output = fopen("output.txt", "w");
    } else {
        input = fopen(argv[1], "r");
        output = fopen(argv[2], "w");
    }
    header = MakeEmpty(header);
    while (1) {
        command = fgetc(input);
        if (feof(input)) break;
        switch (command) {
            case 'i':
                fscanf(input, "%d%d", &key1, &key2);
                tmp = Find(key1, header);
                if (tmp != NULL) {
                    fprintf(output, "insertion(%d) failed : the key already exists in the list\n", key1);
                    break;
                }
                tmp = Find(key2, header);
                if (tmp == NULL) {
                    fprintf(output, "insertion(%d) failed : can not find the location to be inserted\n", key1);
                    break;
                }
                Insert(key1, header, tmp);
                break;

            case 'd':
                fscanf(input, "%d", &key1);
                Delete(key1, header);
                break;

            case 'f':
                fscanf(input, "%d", &key1);
                tmp = FindPrevious(key1, header);
                if (IsLast(tmp, header)) {
                    fprintf(output, "finding(%d) failed : element %d is not in the list\n", key1, key1);
                } else {
                    if (tmp -> element > 0) {
                        fprintf(output, "key of the previous node of %d is %d\n", key1, tmp -> element);
                    } else {
                        fprintf(output, "key of the previous node of %d is head\n", key1);
                    }
                }
                break;
            
            case 'p':
                PrintList(header);
                break;

            default:
                break;
        }
    }

    DeleteList(header);
    fclose(input);
    fclose(output);
    return 0;
}

// Function Definitions

List MakeEmpty(List L) {
    List list = (List)malloc(sizeof(struct Node));
    list -> element = -1; //header;
    list -> next = NULL;
    return list;
}

int IsEmpty(List L) {
    return L -> next == NULL;
}

int IsLast(Position P, List L) {
    return P -> next == NULL;
}

void Insert(ElementType X, List L, Position P) {
    List list = (List)malloc(sizeof(struct Node));

    if (list == NULL) {
        printf("FatalError : Out of space!!!");
    }

    list -> element = X;
    list -> next = P -> next;
    P -> next = list;
}

void PrintList(List L) {
    List now = L -> next;
    while (now -> next != NULL) {
        fprintf(output, "key:%d ", now -> element);        
        now = now -> next;
    }
    fprintf(output, "key:%d\n", now -> element);
}

void Delete(ElementType X, List L) {
    Position p = Find(X, L);
    if (p == NULL) {
        fprintf(output, "deletion(%d) failed : element %d is not in the list\n",X,X);
    } else {
        Position prev = FindPrevious(X, L);
        prev -> next = p -> next;
        free(p);
    }
}

Position Find(ElementType X, List L) {
    Position p = L;
    while (p != NULL && p -> element != X) {
        p = p -> next;
    }
    return p;
}

Position FindPrevious(ElementType X, List L) {
    Position p;
    p = L;
    while (p -> next != NULL && p -> next -> element != X) {
        p = p -> next;
    }
    return p;
}

void DeleteList(List L) {
    Position p, tmp;
    p = L -> next;
    while (p != NULL) {
        tmp = p -> next;
        free(p);
        p = tmp;
    }
    L -> next = NULL;
}
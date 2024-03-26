#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* fin;
FILE* fout;

typedef struct _Queue {
    int* key;
    int first, rear, qsize, max_queue_size;
} Queue;

typedef struct _Graph {
    int size;
    int* node;
    int** matrix;
} Graph;

Graph* CreateGraph(int* node, int n);
int Find(Graph* G, int val);
void InsertEdge(Graph* G, int a, int b);
void PrintGraph(Graph* G);
void DeleteGraph(Graph* G);
void Topsort(Graph* G);

Queue* MakeNewQueue(int X);
int isEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);

void countInput(int* n, char* str);
void parseInput(int* arr, char* str, int n);

int main(int argc, char* argv[]) {
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char numbers[100], inputs[150];
    fgets(numbers, 100, fin);
    int n = 0;
    countInput(&n, numbers);

    int* nodes = (int*)malloc(sizeof(int) * n);
    parseInput(nodes, numbers, n);
    Graph* G = CreateGraph(nodes, n);

    fgets(inputs, 100, fin);

    int len = strlen(inputs), i, j;
    for (i = 0; i < len; i += 4) {
        int a = inputs[i] - '0', b = inputs[i + 2] - '0';
        InsertEdge(G, a, b);
    }
    PrintGraph(G);
    Topsort(G);
    DeleteGraph(G);

    fclose(fin);
    fclose(fout);

    return 0;
}

void countInput(int* n, char* str) {
    int len = strlen(str), i;
    for (i = 0; i < len; ++i) {
        if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
    }
}

void parseInput(int* arr, char* str, int n) {
    int len = strlen(str), i;
    int cnt = 0;
    for (i = 0; i < len; ++i) {
        if (0 <= str[i] - '0' && str[i] - '0' < 10) {
            arr[cnt++] = str[i] - '0';
        }
    }
}

Graph* CreateGraph(int* node, int n) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph -> size = n;
    // sort nodes
    int i, j, min;
    for (i = 0; i < n - 1; ++i) {
        min = i;
        for (j = i + 1; j < n; ++j) {
            if (node[j] < node[min]) {
                min = j;
            }
        }
        if (i != min) {
            int tmp = node[i];
            node[i] = node[min];
            node[min] = tmp;
        }
    }
    graph -> node = node;
    graph -> matrix = (int**)malloc(sizeof(int*) * n);
    for (int i = 0; i < n; ++i) {
        graph -> matrix[i] = (int*)malloc(sizeof(int) * n);
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            graph -> matrix[i][j] = 0;
        }
    }
    return graph;
}

int Find(Graph* G, int val) {
    for (int i = 0; i < G -> size; ++i) {
        if (G -> node[i] == val) {
            return i;
        }
    }
    return -1; // well, it doesn't occurs
}

void InsertEdge(Graph* G, int a, int b) {
    int from = Find(G, a), to = Find(G, b);
    G -> matrix[from][to] += 1;
}

void PrintGraph(Graph* G) {
    fprintf(fout, "   ");
    for (int i = 0; i < G -> size; ++i) {
        fprintf(fout, "%d  ", G -> node[i]);
    }
    fprintf(fout, "\n");
    for (int i = 0; i < G -> size; ++i) {
        fprintf(fout, "%d  ", G -> node[i]);
        for (int j = 0; j < G -> size; ++j) {
            fprintf(fout, "%d  ", G -> matrix[i][j]);
        }
        fprintf(fout, "\n");
    }
    fprintf(fout, "\n");
}

void DeleteGraph(Graph* G) {
    free(G -> node);
    for (int i = 0; i < G -> size; ++i) {
        free(G -> matrix[i]);
    }
    free(G -> matrix);
    free(G);
}

void Topsort(Graph* G) {
    Queue* Q = MakeNewQueue(G -> size);
    int idx = 0;
    int* ans = (int*)malloc(sizeof(int) * G -> size);
    int* visit = (int*)malloc(sizeof(int) * G -> size);

    int i, j;
    for (i = 0; i < G -> size; ++i) {
        int flag = 0;
        for (j = 0; j < G -> size; ++j) {
            if (G -> matrix[j][i] > 0) {
                flag = 1;
            }
        }
        if (flag == 0) {
            // it means in-degree == 0
            visit[i] = 1;
            Enqueue(Q, i);
        }
    }

    while(!isEmpty(Q)) {
        int front = Dequeue(Q);
        ans[idx++] = front;

        for (i = 0; i < G -> size; ++i) {
            if (G -> matrix[front][i] > 0) {
                G -> matrix[front][i] -= 1;
            }
        }
        
        for (i = 0; i < G -> size; ++i) {
            int flag = 0;
            for (j = 0; j < G -> size; ++j) {
                if (G -> matrix[j][i] > 0) {
                    flag = 1;
                }
            }
            if (flag == 0 && visit[i] == 0) {
                visit[i] = 1;
                Enqueue(Q, i);
            }
        }
    }

    //check cycle
    if (idx != G -> size) {
        fprintf(fout, "sorting error : cycle!");
    } else {
        for (i = 0; i < idx; ++i) {
            fprintf(fout, "%d ", G -> node[ans[i]]);
        }
    }

    fprintf(fout, "\n");
    DeleteQueue(Q);
    free(ans);
    free(visit);
}

Queue* MakeNewQueue(int X) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue -> max_queue_size = X;
    queue -> key = (int*)malloc(sizeof(int) * X);
    queue -> rear = -1;
    queue -> first = 0;
    queue -> qsize = 0;
    return queue;
}

int isEmpty(Queue* Q) {
    return Q -> qsize == 0;
}

int Dequeue(Queue* Q) {
    if (isEmpty(Q)) {
        return -1;
    } 
    Q -> qsize--;
    return Q -> key[Q -> first++];
}

void Enqueue(Queue* Q, int X) {
    if (Q -> rear == Q -> max_queue_size - 1) {
        return;
    }
    Q -> key[++(Q -> rear)] = X;
    Q -> qsize++;
}

void DeleteQueue(Queue* Q) {
    free(Q -> key);
    free(Q);
}
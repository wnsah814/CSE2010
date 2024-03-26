#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF 1e9

FILE* fin;
FILE* fout;

typedef struct Node {
    int vertex;
    int dist;
    int prev; // to print path
} Node;

typedef struct Graph {
    int size;
    int** vertices; // adjacent matrix
    Node* nodes;
} Graph;

typedef struct Heap {
    int Capacity;
    int Size;
    Node* Element;
} Heap;

Graph* createGraph(int size);
void deleteGraph(Graph* g);

void printShortestPath(Graph* g);

Heap* createMinHeap(int heapSize);
int isEmpty(Heap* minHeap);
int isFull(Heap* minHeap);
int Find(Heap* minHeap, int vtx);
void deleteMinHeap(Heap* minHeap);
void insertToMinHeap(Heap* minHeap, int vertex, int distance);
Node deleteMin(Heap* minHeap);

void main(int argc, char* argv[]) {
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    Graph* g;
    int size;
    fscanf(fin, "%d\n", &size);
    g = createGraph(size + 1);

    char tmp = 0;
    while (tmp != '\n' && tmp != EOF) {
        int node1, node2, weight;
        fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
        g -> vertices[node1][node2] = weight;
        tmp = fgetc(fin);
    }

    printShortestPath(g);
    deleteGraph(g);
}

Graph* createGraph(int size) {
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g -> size = size;
    g -> vertices = (int**)malloc(sizeof(int*) * size);
    for (int i = 0; i < size; ++i) {
        g -> vertices[i] = (int*)malloc(sizeof(int) * size);
    }
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            g -> vertices[i][j] = 0;
        }
    }
    g -> nodes = (Node*)malloc(sizeof(Node) * size);
    for (int i = 0; i < size; ++i) {
        g -> nodes[i].vertex = i;
        g -> nodes[i].dist = INF;
        g -> nodes[i].prev = -1; // it means no path from 1 to i
    }
    return g;
}

void deleteGraph(Graph* g) {
    for (int i = 0; i < g -> size; ++i) { // from 0 to ?
        free(g -> vertices[i]);
    }
    free(g -> vertices);
    free(g -> nodes);
}

void printShortestPath(Graph* g) {
    Heap* heap = createMinHeap(g -> size * g -> size); // full enough
    
    g -> nodes[1].dist = 0;
    g -> nodes[1].vertex = 1;
    g -> nodes[1].prev = 0; // has path from 1 (use for later printing path)
    insertToMinHeap(heap, 1, 0);
    while (!isEmpty(heap)) {
        Node current = deleteMin(heap);
        int cur_node = current.vertex;
        int cur_dist = current.dist;

        for (int i = 1; i <= g -> size; ++i) {
            if(g -> vertices[cur_node][i] > 0 && g -> nodes[i].prev == -1) { // has edge && !visited
                int nxt_node = i;
                int nxt_dist = cur_dist + g -> vertices[cur_node][i];
                if (nxt_dist < g -> nodes[nxt_node].dist) {
                    g -> nodes[nxt_node].dist = nxt_dist;
                    g -> nodes[nxt_node].prev = cur_node;
                    insertToMinHeap(heap, nxt_node, nxt_dist);
                }
            }
        }        
    }
    for (int i = 2; i < g -> size; ++i) {
        int* visit = (int*)malloc(sizeof(int) * g -> size); // save path
        int idx = 0;
        for (int j = i; j != -1; j = g -> nodes[j].prev) {
            if (j == 0) {
                break;
            }
            visit[idx++] = j;
        }

        if (visit[idx - 1] == i) {
            fprintf(fout, "can not reach to node %d\n", i);
        } else {
            for (int k = idx - 1; k > 0; --k) {
                fprintf(fout, "%d->", visit[k]);
            }
            fprintf(fout, "%d (cost : %d)\n", i, g -> nodes[i].dist);
        }
        free(visit);
    }
}

Heap* createMinHeap(int heapSize) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap -> Capacity = heapSize;
    heap -> Size = 0;
    heap -> Element = (Node*)malloc(sizeof(Node) * (heapSize+1));
    heap -> Element[0].dist = -INF;
    for (int i = 1; i <= heapSize; ++i) {
        heap -> Element[i].dist = INF;
    }
    return heap;
}

int isEmpty(Heap* minHeap) {
    return minHeap -> Size <= 0;
}

int isFull(Heap* minHeap) {
    return minHeap -> Size >= minHeap -> Capacity;
}

int Find(Heap* minHeap, int vtx) {
    for (int i = 1; i <= minHeap -> Capacity; ++i) {
        if (minHeap -> Element[i].vertex == vtx) {
            return 1;
        }
    }
    return 0;
}

void deleteMinHeap(Heap* minHeap) {
    free(minHeap -> Element);
    free(minHeap);
}

void insertToMinHeap(Heap* minHeap, int vertex, int distance) {
    int i;
    if (isFull(minHeap)) {
        return;
    }
    if (Find(minHeap, vertex)) {
        return; // already in the heap
    }
    for (i = ++minHeap -> Size; minHeap -> Element[i/2].dist > distance; i /= 2) {
        minHeap -> Element[i] = minHeap -> Element[i/2];
    }
    minHeap -> Element[i].dist = distance;
    minHeap -> Element[i].vertex = vertex;
}

Node deleteMin(Heap* minHeap) {
    int i, child;
    Node min = minHeap -> Element[1];
    Node last = minHeap -> Element[minHeap -> Size];

    minHeap -> Element[minHeap -> Size--].dist = INF; 

    for (i = 1; i * 2 <= minHeap ->Size; i = child) {
        child = i * 2;
        if (child < minHeap -> Size && minHeap -> Element[child + 1].dist < minHeap -> Element[child].dist) {
            child++;
        }
        if (last.dist > minHeap -> Element[child].dist) {
            minHeap -> Element[i] = minHeap -> Element[child];
        } else {
            break;
        }
    }
    minHeap -> Element[i] = last;
    return min;
}

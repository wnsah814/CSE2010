#include <stdio.h>
#include <stdlib.h>
#define INF 1e9

FILE* fin;
FILE* fout;

typedef struct HeapStruct {
    int Capacity;
    int Size;
    int* Elements;
} Heap;

Heap* CreateHeap(int heapSize);
void Insert(Heap *heap, int value);
int Find(Heap* heap, int value);
int DeleteMax(Heap* heap);
void PrintHeap(Heap* heap);
int isFull(Heap* heap);
int isEmpty(Heap* heap);
void FreeHeap(Heap* heap);

int main(int argc, char* argv[]) {
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char cv;
    Heap* maxHeap;
    int heapSize, key, max_element;

    while (1) {
        fscanf(fin, "%c", &cv);
        if (feof(fin)) {
            break;
        }
    
        switch (cv) {
            case 'n':
                fscanf(fin, "%d", &heapSize);
                maxHeap = CreateHeap(heapSize);
                break;
            case 'i':
                fscanf(fin, "%d", &key);
                Insert(maxHeap, key);
                break;
            case 'd':
                max_element = DeleteMax(maxHeap);
                if (max_element != -INF) {
                    fprintf(fout, "max element : %d deleted\n", max_element);
                }
                break;
            case 'p':
                PrintHeap(maxHeap);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if (Find(maxHeap, key)) {
                    fprintf(fout, "%d is in the heap\n", key);
                } else {
                    fprintf(fout, "finding error : %d is not in the heap\n", key);
                }
                break;
        }
    }
    FreeHeap(maxHeap);
    return 0;
}

Heap* CreateHeap(int heapSize) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    if (heap == NULL) {
        // No more space;
    }
    heap -> Elements = (int*)malloc(sizeof(int)*(heapSize+1));
    heap->Elements[0] = INF; // we use index from 1 to capacity. so I make [0] inf.
    for (int i = 1; i <= heapSize + 1; ++i) {
        heap -> Elements[i] = -INF;
    }
    heap -> Size = 0;
    heap -> Capacity = heapSize;
    return heap;
}

void Insert(Heap *heap, int value) {
    int i;
    if (isFull(heap)) {
        fprintf(fout, "insert error : heap is full\n");
        return;
    }
    if (Find(heap, value)) {
        fprintf(fout, "insert error : %d is already in the heap\n", value);
        return;
    }
    for (i = ++heap->Size; heap->Elements[i/2] < value; i /= 2) { // if Elements[0] != INF, infinite loop happens..
        heap -> Elements[i] = heap -> Elements[i/2];
    }
    heap -> Elements[i] = value;
    fprintf(fout, "insert %d\n", value);
}

int Find(Heap* heap, int value) {
    for (int i = 1; i <= heap->Capacity; ++i) { // because Array is not sorted, I used linear search.
        if (heap->Elements[i] == value) {
            return 1;
        }
    }
    return 0;
}
int DeleteMax(Heap* heap) {
    if (isEmpty(heap)) {
        fprintf(fout, "delete error : heap is empty\n");
        return -INF;
    }
    int i, child;
    int max = heap->Elements[1];
    int last = heap->Elements[heap->Size];
    heap->Elements[heap->Size--] = -INF; // -INF is like free()
    
    for (i = 1; i * 2 <= heap -> Size; i = child) {
        child = i * 2;
        if (child < heap -> Size && heap -> Elements[child+1] > heap->Elements[child]) {
            child++;
        }
        if (last < heap->Elements[child]) {
            heap->Elements[i] = heap->Elements[child];
        } else {
            break;
        }
    }
    heap -> Elements[i] = last;
    return max;
}

void PrintHeap(Heap* heap) {
    if (isEmpty(heap)) {
        fprintf(fout, "print error : heap is empty\n");
        return;
    }
    for (int i = 1; i <= heap->Size; ++i) {
        fprintf(fout, "%d ", heap->Elements[i]);
    }
    fprintf(fout, "\n");
}

int isFull(Heap* heap) {
    return heap->Size >= heap->Capacity;
}

int isEmpty(Heap* heap) {
    return heap->Size <= 0;
}

void FreeHeap(Heap* heap) {
    free(heap -> Elements);
    free(heap);
}
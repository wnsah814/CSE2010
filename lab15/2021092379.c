#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* fin;
FILE* fout;

typedef int ElementType;
typedef ElementType List;
typedef struct HashTbl* HashTable;

typedef struct HashTbl {
    int TableSize;
    List *TheLists;
} HashTbl;

HashTable createTable(int TableSize);
void Insert(HashTable H, ElementType Key, int solution);
void Delete(HashTable H, ElementType Key, int solution);
int Find(HashTable H, ElementType Key, int solution);
void printTable(HashTable H);
void deleteTable(HashTable H);
int colFunc(int sol, int n);

int main(int argc, char* argv[]) {
    fin = fopen("input.txt", "r");
    fout = fopen("output.txt", "w");

    char solution_str[20];
    int solution, TableSize;

    fscanf(fin, "%s", solution_str);
    if (!strcmp(solution_str, "linear")) {
        solution = 1;
    } else if (!strcmp(solution_str, "quadratic")) {
        solution = 2;
    } else {
        fprintf(fout, "error: invalid hashing solution!");
        return 0;
    }

    fscanf(fin, "%d", &TableSize);

    HashTable H = createTable(TableSize);

    char cv;
    int key;
    while(1) {
        fscanf(fin, "%c", &cv);
        if (feof(fin)) {
            break;
        }
        switch (cv) {
            case 'i':
                fscanf(fin, "%d", &key);
                Insert(H, key, solution);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                int result = Find(H, key, solution);
                if (result) {
                    fprintf(fout, "%d is in the table\n", key);
                } else { 
                    fprintf(fout, "%d is not in the table\n", key);
                }
                break;

            case 'd':
                fscanf(fin, "%d", &key);
                Delete(H, key, solution);
                break;

            case 'p': 
                printTable(H);
                break;
        }
    }
    return 0;
}

HashTable createTable(int TableSize) {
    HashTable h = (HashTable)malloc(sizeof(HashTbl));
    h -> TableSize = TableSize;
    h -> TheLists = (List*)malloc(sizeof(List) * TableSize);
    for (int i = 0; i < TableSize; ++i) {
        h -> TheLists[i] = 0;
    }
    return h;
}

int colFunc(int sol, int n) {
    if (sol == 1) {
        return n;
    } else {
        return n * n;
    }
}

void Insert(HashTable H, ElementType Key, int solution) {
    int i = Key % H -> TableSize;
    int cnt = 0;
    while (H -> TheLists[i] > 0) {
        if (H -> TheLists[i] == Key) {
            fprintf(fout, "insertion error : %d already exists at address %d\n", Key ,i);
            return;
        }
        i = (Key + colFunc(solution, ++cnt)) % H -> TableSize;
        
        if (cnt == H -> TableSize) {
            fprintf(fout, "insertion error : table is full\n");
            return;
        }
    }
    fprintf(fout, "insert %d into address %d\n", Key, i);
    H -> TheLists[i] = Key;
}

void Delete(HashTable H, ElementType Key, int solution) {
    int i = Key % H -> TableSize;
    int idx = Find(H, Key, solution) - 1; // bacuase Find return (idx + 1) if the key exists
    if (idx == -1) { // so it means doesnt exist
        fprintf(fout, "deletion error : %d is not in the table\n", Key);
    } else {
        fprintf(fout, "delete %d\n", Key);
        H -> TheLists[idx] = -1; // to record this idx was deleted and can insert to this place
    }
}

int Find(HashTable H, ElementType Key, int solution) { // return index if the Key exists, while returns 0 if the key does not exists
    int i = Key % H -> TableSize;
    int cnt = 0;
    while (H -> TheLists[i] > 0 || H -> TheLists[i] == -1) {
        if (H -> TheLists[i] == Key) {
            return i + 1; // index can be 0, so when we return 0 for cannot find the Key, it conflicts later
        }
        i = (Key + colFunc(solution, ++cnt)) % H -> TableSize;
        if (cnt == H -> TableSize) {
            return 0;
        }
    }
    return 0;
}

void printTable(HashTable H) {
    for (int i = 0; i < H -> TableSize; ++i) {
        if (H -> TheLists[i] == -1) {
            fprintf(fout, "0 "); // -1 is not key but just record this place can be inserted
            continue;
        }
        fprintf(fout, "%d ", H -> TheLists[i]);
    }
    fprintf(fout, "\n");
}

void deleteTable(HashTable H) {
    free(H -> TheLists);
    free(H);
}
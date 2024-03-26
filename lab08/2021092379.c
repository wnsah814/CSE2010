#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
    int size_maze;
    int *ptr_arr;
} DisjointSets;

void init(DisjointSets *set, DisjointSets *maze_print, int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets  *sets, DisjointSets *maze_print, int num);
void printMaze(DisjointSets *maze_print, int num);
void freeMaze(DisjointSets *sets, DisjointSets *maze_print);

int main(int argc, char* argv[]) {
    srand((unsigned int)time(NULL));

    int num, i;
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    DisjointSets *sets, *maze_print;
    fscanf(fin, "%d", &num);

    sets = (DisjointSets*)malloc(sizeof(DisjointSets));
    maze_print = (DisjointSets*)malloc(sizeof(DisjointSets));

    init(sets, maze_print, num);
    
    createMaze(sets, maze_print, num);
    printMaze(maze_print, num);

    freeMaze(sets, maze_print);

    fclose(fin);
    fclose(fout);

    return 0;
}

void init(DisjointSets *sets, DisjointSets *maze_print, int num) {
    int i;

    sets -> size_maze = num * num;
    sets -> ptr_arr = (int*)malloc(sizeof(int) * (sets->size_maze+1));
    for (i = 1; i <= sets->size_maze; ++i) {
        sets -> ptr_arr[i] = i;
    }

    maze_print -> size_maze = num * num * 2;
    maze_print -> ptr_arr = (int*)malloc(sizeof(int) * (maze_print -> size_maze + 1));
    for (i = 1; i <= maze_print -> size_maze; ++i) {
        maze_print -> ptr_arr[i] = 1; // 1 : wall
    }
    // 2i : right edge
    // 2i - 1 : bottom edge for i >= 1
    maze_print -> ptr_arr[maze_print -> size_maze - 1] = 0;
}

void freeMaze(DisjointSets *sets, DisjointSets* maze_print) {
    free(sets->ptr_arr);
    free(sets);
    free(maze_print->ptr_arr);
    free(maze_print);
}

void Union(DisjointSets *sets, int i, int j) { // 
    if (sets == NULL) return;
    if (i == j) return; // Do nothing when i and j are in same set
    sets -> ptr_arr[i] = j; // because of path compression, dont care about the rank
}

int find(DisjointSets *sets, int i) {
    if (sets -> ptr_arr[i] == i) {
        return i;
    } else {
        return sets -> ptr_arr[i] = find(sets, sets->ptr_arr[i]);
    }
}

void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num) {
    while (find(sets, 1) != find(sets, sets->size_maze)) {
        int random = (rand()%(sets->size_maze))+1;
        int flip = rand()%2;
        if (flip == 0) { //bottom wall
            if (random > num*(num-1)) {
                continue; // bottom most walls
            } else {
                if (find(sets, random) != find(sets, random + num)) { // when they are in the same set, it will make a loop 
                    maze_print -> ptr_arr[2*random] = 0;
                    Union(sets, find(sets, random), find(sets, random+num));
                }
            }
        } else { //right wall
            if (random%num == 0) {
                continue; // right most walls
            } else {
                if (find(sets, random) != find(sets, random+1)) { // same as above
                    maze_print -> ptr_arr[2*random-1] = 0;
                    Union(sets,find(sets, random), find(sets, random+1));
                }
            } 
        }
    }       
}

void printMaze(DisjointSets *maze_print, int num) {
    // first row
    fprintf(fout, "+");
    for (int i = 0; i < num; ++i) {
        fprintf(fout, "---+");
    }
    
    // others
    int ver = 1, hor = 2;
    for (int k = 0; k < num; ++k) {
        if (k == 0) {
            fprintf(fout, "\n ");
        } else {
            fprintf(fout, "\n|");
        }
        for (int i = 0; i < num; ++i) {
            fprintf(fout, "   ");
            if (maze_print -> ptr_arr[ver] == 0) {
                fprintf(fout, " ");
            } else {
                fprintf(fout, "|");
            }
            ver += 2;
        }

        fprintf(fout, "\n+");
        for (int i = 0; i < num; ++i) {
            if (maze_print -> ptr_arr[hor]== 0) {
                fprintf(fout, "   ");
            } else {
                fprintf(fout, "---");
            }
            fprintf(fout, "+");
            hor += 2;
        }
    }
    fprintf(fout, "\n");
}
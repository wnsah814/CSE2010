#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ISFULL 1
#define ISEMPTY 2
#define DIVIDEZERO 3

FILE *fin;
FILE *fout;

typedef struct Stack {
    int *key;
    int top;
    int max_stack_size;
} Stack;

// Function Declarations
Stack* CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);
void DeleteStack(Stack* S);
int IsEmpty(Stack* S);
int IsFull(Stack* S);

// Main
int main(int argc, char *argv[]) {
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    Stack* stack;
    char input_str[101];
    int max = 20, i = 0, a, b, result, error_flag = 0;

    fgets(input_str, 101, fin);
    stack = CreateStack(max);

    fprintf(fout, "top numbers : ");
    while (input_str[i] != '#') {
        if (input_str[i] == '+') {
            int sec = Top(stack);
            error_flag = Pop(stack);
            int fir = Top(stack);
            error_flag = Pop(stack);
            if (!error_flag) {
                Push(stack, fir+sec);
            } else {
                break;
            }
        } else if (input_str[i] == '-') {
            int sec = Top(stack);
            error_flag = Pop(stack);
            int fir = Top(stack);
            error_flag = Pop(stack);
            if (!error_flag) {
                Push(stack, fir-sec);
            } else {
                break;
            }
        } else if (input_str[i] == '*') {
            int sec = Top(stack);
            error_flag = Pop(stack);
            int fir = Top(stack);
            error_flag = Pop(stack);
            if (!error_flag) {
                Push(stack, fir*sec);
            } else {
                break;
            }
        } else if (input_str[i] == '/') {
            int sec = Top(stack);
            error_flag = Pop(stack);
            int fir = Top(stack);
            error_flag = Pop(stack);
            if (!error_flag) {
                if (sec != 0) {
                    Push(stack, fir/sec);
                } else {
                    error_flag = DIVIDEZERO;
                    break;
                }
            } else {
                break;
            }
        } else if (input_str[i] == '%') {
            int sec = Top(stack);
            error_flag = Pop(stack);
            int fir = Top(stack);
            error_flag = Pop(stack);
            if (!error_flag) {
                if (sec != 0) {
                    Push(stack, fir%sec);
                } else {
                    error_flag = DIVIDEZERO;
                    break;
                }
            } else {
                break;
            }
        } else {
            if (!IsFull(stack)) {
                Push(stack, input_str[i]-'0');
            } else {
                error_flag = ISFULL;
                break;
            }
        }
        int t = Top(stack);
        fprintf(fout, "%d ", t);
        result = t;

        i++;
    }
    if (error_flag == ISFULL) {
        fprintf(fout, "\nerror : invalid postfix expression, stack is full!\n");
    } else if (error_flag == ISEMPTY) {
        fprintf(fout, "\nerror : invalid postfix expression, stack is empty!\n");
    } else if (error_flag == DIVIDEZERO) {
        fprintf(fout, "\nerror : invalid postfix expression, divide by zero!\n");
    } else {
        if (stack -> top + 1 > 1) {
            fprintf(fout, "\nerror : invalid postfix expression, %d elements are left!\n", stack -> top+1);
        } else {
            fprintf(fout, "\nevaluation result : %d\n", result);
        }
    }

    fclose(fin);
    fclose(fout);
    DeleteStack(stack);
}

// Function Definitions
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
        return ISEMPTY;
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

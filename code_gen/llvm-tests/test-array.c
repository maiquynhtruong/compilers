// Run with llvm-gcc -S -emit-llvm test-array.c for LLVM IR code

#include <stdio.h>

int global_numbers[10];
int global_res[10];
int global_i;
float global_f;
int i;

void square_array(int array[10], int *result, int param_i, char *s, float *param_f) {
    int i;
    i = 0;
    global_i = 0;
    global_f = 2.5;
    i = global_i;
    float f;
    *param_f = global_f;
    f = *param_f;

    for ( ; i < 10; i = i + 1)
        result[i] = array[i] * array[i];
    i = param_i + 1;
}

int main() {
    int numbers[10];
    int res[10];
    // int i;
    char *s;
    i = 0;
    for (; i < 10; i = i + 1)
        numbers[i] = i;
    float f;
    square_array(numbers, res, 0, s, &f);
    i = 0;
    for (; i < 10; i = i + 1) {
        printf("%d\n", res[i]);
        printf(" ");
    }
}

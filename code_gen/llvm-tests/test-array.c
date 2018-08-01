// Run with llvm-gcc -S -emit-llvm test-array.c for LLVM IR code

#include <stdio.h>

int global_numbers[10];
int global_res[10];
int global_i;
float global_f;
int i;

void square_array(int array[10], int result[10]) {
    int i;
    i = 0;
    for ( ; i < 10; i = i + 1)
        result[i] = array[i] * array[i];
}

int main() {
    int numbers[10];
    int res[10];
    // int i;
    float f;
    char *s;
    i = 0;
    for (; i < 10; i = i + 1)
        numbers[i] = i;
    square_array(numbers, res);
    i = 0;
    for (; i < 10; i = i + 1) {
        printf("%d\n", res[i]);
        printf(" ");
    }
}

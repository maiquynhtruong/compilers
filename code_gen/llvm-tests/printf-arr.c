// Run with llvm-gcc -S -emit-llvm printf-arr.c for LLVM IR code
#include <stdio.h>
#include <stdbool.h>

int main() {
    int arr[3];
    arr[0] = 132456;
    arr[1] = 98765;
    arr[2] = 65738;
    printf("%d\n", arr[0]);
    printf("%d\n", arr[1]);
    printf("%d\n", arr[2]);
}

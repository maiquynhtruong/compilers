// Run with llvm-gcc -S -emit-llvm if-statements.c for LLVM IR code
#include <stdio.h>

int factorial(int n) {
    if (n == 0) {
        return 1;
    } else {
        return n*factorial(n-1);
    }
}

int main() {
    int res = factorial(5);
    printf("%d\n", res);
}

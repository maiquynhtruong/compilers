// Run with llvm-gcc -S -emit-llvm printf-var.c for LLVM IR code
#include <stdio.h>
#include <stdbool.h>

int main() {
    int i = 1;
    printf("The integer is %d\n", i);
    float f = 0.01234;
    printf("The float is %f\n", f);
    char c = 'c';
    printf("The char is %c\n", c);
    char *s = "a string";
    printf("The string is %s\n", s);
    bool b = true;
    printf("The bool is %d\n", b);
}

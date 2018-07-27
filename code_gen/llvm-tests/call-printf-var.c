// Run with llvm-gcc -S -emit-llvm call-printf-var.c for LLVM IR code
#include <stdio.h>
#include <stdbool.h>

void print_variables_4(int i, float f, char c) {
    printf("The integer is %d\n", i);
    printf("The float is %f\n", f);
    printf("The char is %c\n", c);
}

void print_variables() {
    int i = 1;
    printf("The integer is %d\n", i);
    float f = 0.01234;
    // printf("The float is %f\n", f);
    // char c = 'c';
    // printf("The char is %c\n", c);
    // char *s = "a string";
    // printf("The string is %s\n", s);
    // bool b = true;
    // printf("The bool is %d\n", b);
    print_variables_4(1, 2.0, '3');
}

void print_variables_2(int i, bool b) {
    // int i = 1;
    float f = 0.01234;
    // char c = i + 'a';
    // char *s = "a string";
    // bool b = true;
    // printf("The integer is %d\n", i);
    // printf("The float is %f\n", f);
    // printf("The char is %c\n", c);
    // printf("The string is %s\n", s);
    // printf("The bool is %d\n", b);
    print_variables_4(i, f, 'a');
}

void print_variables_3(int i1, float f1, char c1, char *s1, bool b1) {
    int i;
    float f;
    char c;
    char *s;
    bool b;
    // i = 1;
    // f = 0.01234;
    // c = 'c';
    // s = "a string";
    // b = true;
    // printf("The integer is %d\n", i);
    // printf("The float is %f\n", f);
    // printf("The char is %c\n", c);
    // printf("The string is %s\n", s);
    // printf("The bool is %d\n", b);
}

int main() {
    print_variables();
    return 0;
}

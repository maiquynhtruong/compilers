// Run with llvm-gcc -S -emit-llvm call-scanf.c for LLVM IR code
#include <stdio.h>
#define MAX_STRING_LENGTH 100

void scan_variables() {
    char s[MAX_STRING_LENGTH];
    printf("Enter a string: ");
    scanf("%s", s);
    printf("The string entered is: %s\n", s);
}

void scan_variables_1(int i) {
    float f;
    char c = 'a';
    printf("Enter integer: ");
    scanf("%d", &i);
    printf("Enter float: ");
    scanf("%f", &f);
    printf("Expression is %f\n", i + f + c);
}

void scan_variables_2() {

}

int main() {
    scan_variables();
    scan_variables_1(4);
    return 0;
}

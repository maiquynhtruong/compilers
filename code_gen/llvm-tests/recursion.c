// Run with llvm-gcc -S -emit-llvm recursion.c for LLVM IR code
#include <stdio.h>

void recursion(int level) {
    if (level >= 3) {
        printf("It's time to return\n");
        return;
    }
    // } else {
        printf("Recursion level: %d\n", level);
        level++;
        recursion(level);
        // return;
    // }
}

int main() {
    recursion(0);
}

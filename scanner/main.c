#include "scanner.c"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("%s\n", "Error! No input file...");
        exit(-1);
    }

    if (scan(argv[1]) == IO_ERROR) {
        printf("%s\n", "Can't read input file");
        exit(-1);
    }
    return 0;
}

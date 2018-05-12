#include <stdio.h>
#include <stdlib.h>
#include "reader.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("%s\n", "Error! No input file...");
        exit(-1);
    }

    if (parse(argv[1]) == IO_ERROR) {
        printf("%s\n", "Can't read input file");
        exit(-1);
    }
    return 0;
}

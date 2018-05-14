#include <stdio.h>
#include "reader.h"

FILE *input;
int lineNo, columnNo, prev_columnNo;
int cur_char;

int read_char() {
    cur_char = getc(input);
    columnNo++;
    if (cur_char == '\n') {
      lineNo++;
      columnNo = 0;
    }
    return cur_char;
}

int open_input_stream(char *file_name) {
    input = fopen(file_name, "r");
    if (input == NULL)
        return IO_ERROR;

    lineNo = 1;
    columnNo = 0;
    read_char();

    return IO_SUCCESS;
}
void close_input_stream() {
    fclose(input);
}

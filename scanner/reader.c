#include <stdio.h>
#include "reader.h"

FILE *input_stream;
int lineNo, columnNo;
int cur_char;

int read_char() {
    cur_char = getc(input_stream);
    columnNo++;
    if (cur_char == '\n') {
      lineNo++;
      columnNo = 0;
    }
    return cur_char;
}

int openInputStream(char *file_name) {
    input_stream = fopen(file_name, "r");
    if (input_stream == NULL)
        return IO_ERROR;

    lineNo = 1;
    columnNo = 0;
    read_char();

    return IO_SUCCESS;
}
void closeInputStream() {
    fclose(input_stream);
}

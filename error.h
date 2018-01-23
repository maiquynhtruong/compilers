#include <stdio.h>
#define ERROR_COUNT 1
typedef enum {
    E_INVALID_CHAR
} ErrorCode;

void throw_error(ErrorCode code);

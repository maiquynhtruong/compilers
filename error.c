#include "error.h"
#include <stdlib.h>
typedef struct {
    ErrorCode error_code;
    char *error_mesg;
} Error; 

Error errors[ERROR_COUNT] = {
    {E_INVALID_CHAR, "Invalid character!"}
};

void throw_error(ErrorCode code) {
    switch(code) {
        case E_INVALID_CHAR:
            printf("%s\n", "Illegal character!"); break;
    }
    exit(1);
}

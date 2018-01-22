#include "error.h"

typedef struct {
    ErrorCode error_code;
    char *error_mesg;
} Error; 

Error errors[ERROR_COUNT] = {
    {E_INVALID, "Invalid character!"}
};

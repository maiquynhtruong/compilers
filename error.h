#define ERROR_COUNT 1
typedef enum {
    E_INVALID
} ErrorCode;

void throw_error(ErrorCode code);

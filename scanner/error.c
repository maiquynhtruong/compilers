#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void throw_error(ErrorCode code, int lineNo, int columnNo) {
    switch(code) {
        case E_END_OF_COMMENT:
            printf("%d:%d: %s\n", lineNo, columnNo, E_MES_ENDOFCOMMENT); break;
        case E_IDENT_TOO_LONG:
            printf("%d:%d: %s\n", lineNo, columnNo, E_MES_IDENTTOOLONG); break;
        case E_INVALID_CHAR:
            printf("%d:%d: %s\n", lineNo, columnNo, E_MES_INVALIDCHAR); break;
        case E_INVALID_SYMBOL:
            printf("%d:%d: %s\n", lineNo, columnNo, E_MES_INVALIDSYMBOL); break;
    }
    exit(-1);
}

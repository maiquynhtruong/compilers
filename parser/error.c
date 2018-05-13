#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void throw_error(ErrorCode code, int lineNo, int columnNo) {
    switch(code) {
        case E_END_OF_COMMENT:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_ENDOFCOMMENT); break;
        case E_IDENT_TOO_LONG:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_IDENTTOOLONG); break;
        case E_INVALID_CHAR:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDCHAR); break;
        case E_INVALID_SYMBOL:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDSYMBOL); break;
        case E_INVALID_TYPE:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDTYPE); break;
        case E_INVALID_PARAM_TYPE:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDPARAMTYPE); break;
        case E_INVALID_STATEMENT:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDSTATEMENT); break;
        case E_INVALID_ARGUMENT:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDARGUMENT); break;
        case E_INVALID_OPERATOR:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDOPERATOR); break;
        case E_INVALID_EXPRESSION:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDEXPRESSION); break;
        case E_INVALID_TERM:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDTERM); break;
        case E_INVALID_FACTOR:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDFACTOR); break;
    }
    exit(0);
}

void missing_token(TokenType type, int lineNo, int columnNo) {
    printf("%d:%d: Parse Erorr! Expecting %s\n", lineNo, columnNo, print_token_type(type));
    exit(0);
}

void assert(char *mesg) {
    printf("%s\n", mesg);
}

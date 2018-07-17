#include "error.h"
#include <stdio.h>
#include <stdlib.h>

void throw_error(ErrorCode code, int lineNo, int columnNo) {
    switch(code) {
        case E_END_OF_COMMENT:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_ENDOFCOMMENT); break;
        case E_IDENT_TOO_LONG:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_IDENTTOOLONG); break;
        case E_STRING_TOO_LONG:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_STRINGTOOLONG); break;
        case E_INVALID_CHAR:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDCHAR); break;
        case E_INVALID_STRING:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDSTRING); break;
        case E_INVALID_SYMBOL:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDSYMBOL); break;
        case E_INVALID_IDENT:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDIDENT); break;
        case E_INVALID_CONSTANT:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDCONSTANT);
        case E_INVALID_TYPE:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDTYPE); break;
        case E_INVALID_PARAM_TYPE:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDPARAMTYPE); break;
        case E_INVALID_VARIABLE:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDVARIABLE); break;
        case E_INVALID_PROCEDURE:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDPROCEDURE); break;
        case E_INVALID_PARAM:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDPARAM); break;
        case E_INVALID_STATEMENT:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDSTATEMENT); break;
        case E_INVALID_ARGUMENT:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDARGUMENT); break;
        case E_INVALID_ARITH_OPERATOR:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDARITHOPERATOR); break;
        case E_INVALID_RELATION:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDRELATION); break;
        case E_INVALID_EXPRESSION:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDEXPRESSION); break;
        case E_INVALID_TERM:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDTERM); break;
        case E_INVALID_FACTOR:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDFACTOR); break;
        case E_INVALID_LVALUE:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INVALIDLVALUE); break;
        case E_UNDECLARED_IDENT:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_UNDECLAREDIDENT); break;
        case E_UNDECLARED_CONST:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_UNDECLAREDCONST); break;
        case E_UNDECLARED_TYPE:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_UNDECLAREDTYPE); break;
        case E_UNDECLARED_VARIABLE:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_UNDECLAREDVARIABLE); break;
        case E_UNDECLARED_PROCEDURE:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_UNDECLAREDPROCEDURE); break;
        case E_DUPLICATE_IDENT:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_DUPLICATEIDENT); break;
        case E_INCOSISTENT_TYPE:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INCONSISENTTYPE); break;
        case E_INCONSISTENT_PARAM_ARGS:
            printf("%d:%d: Syntax Error: %s\n", lineNo, columnNo, E_MES_INCONSISENTPARAMARGS); break;
    }
    exit(0);
}

void missing_token(TokenType type, int lineNo, int columnNo) {
    printf("%d:%d: Parse Erorr! Expecting %s\n", lineNo, columnNo, print_token_type(type));
    exit(0);
}

/** Assert functions:
Comment the function body to print logging info
Uncomment to silence
**/
void assert_scanner(const char *mesg) {
    printf("%s", mesg);
}

void assert_parser(const char *mesg) {
    printf("%s", mesg);
}

void assert_symbol_table(const char *mesg) {
    printf("%s", mesg);
}

void assert_ast(const char *mesg) {
    printf("%s", mesg);
}

void assert_semantics(const char *mesg) {
    printf("%s", mesg);
}

void assert_codegen(const char *mesg) {
    printf("%s", mesg);
}

//https://stackoverflow.com/questions/36836337/need-clarification-on-ifndef-define
//https://stackoverflow.com/questions/1653958/why-are-ifndef-and-define-used-in-c-header-files
#ifndef ERROR_H
#define ERROR_H

#include "token.h"

typedef enum {
    E_END_OF_COMMENT,
    E_IDENT_TOO_LONG,
    E_INVALID_CHAR,
    E_INVALID_SYMBOL,
    E_INVALID_TYPE,
    E_INVALID_PARAM,
    E_INVALID_STATEMENT,
    E_INVALID_ARGUMENT,
    E_INVALID_OPERATOR,
    E_INVALID_EXPRESSION,
    E_INVALID_TERM,
    E_INVALID_FACTOR
} ErrorCode;

#define E_MES_ENDOFCOMMENT "End of comment"
#define E_MES_IDENTTOOLONG "Identifier too long"
#define E_MES_INVALIDCHAR "Invalid character"
#define E_MES_INVALIDSYMBOL "Invalid symbol"
#define E_MES_INVALIDTYPE "Invalid Type"
#define E_MES_INVALIDPARAM "Invalid Parameter"
#define E_MES_INVALIDSTATEMENT "Invalid Statement"
#define E_MES_INVALIDARGUMENT "Invalid Argument"
#define E_MES_INVALIDOPERATOR "Invalid Operator"
#define E_MES_INVALIDEXPRESSION "Invalid Expression"
#define E_MES_INVALIDTERM "Invalid Term"
#define E_MES_INVALIDFACTOR "Invalid Factor"

void throw_error(ErrorCode code, int lineNo, int columnNo);
void missing_token(TokenType type, int lineNo, int columnNo);
void assert(char *mesg);

#endif

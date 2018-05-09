//https://stackoverflow.com/questions/36836337/need-clarification-on-ifndef-define
//https://stackoverflow.com/questions/1653958/why-are-ifndef-and-define-used-in-c-header-files
#ifndef ERROR_H
#define ERROR_H

typedef enum {
    E_END_OF_COMMENT,
    E_IDENT_TOO_LONG,
    E_INVALID_CHAR,
    E_INVALID_SYMBOL
} ErrorCode;

#define E_MES_ENDOFCOMMENT "End of comment"
#define E_MES_IDENTTOOLONG "Identifier too long"
#define E_MES_INVALIDCHAR "Invalid character"
#define E_MES_INVALIDSYMBOL "Invalid symbol"

void throw_error(ErrorCode code, int lineNo, int columnNo);

#endif

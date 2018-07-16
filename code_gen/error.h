#ifndef ERROR_H
#define ERROR_H

#include "token.h"

// TODO: Like GCC, the compiler will report error and move on. But the max errors is 20
#define MAX_ERRORS 20

typedef enum {
    E_END_OF_COMMENT,
    E_IDENT_TOO_LONG,
    E_STRING_TOO_LONG,
    E_INVALID_CHAR,
    E_INVALID_STRING,
    E_INVALID_SYMBOL,
    E_INVALID_IDENT,
    E_INVALID_CONSTANT,
    E_INVALID_TYPE,
    E_INVALID_PARAM_TYPE,
    E_INVALID_VARIABLE,
    E_INVALID_PROCEDURE,
    E_INVALID_PARAM,
    E_INVALID_STATEMENT,
    E_INVALID_ARGUMENT,
    E_INVALID_ARITH_OPERATOR, // like + and -
    E_INVALID_RELATION, // like <, > and ==
    E_INVALID_EXPRESSION,
    E_INVALID_TERM,
    E_INVALID_FACTOR,
    E_INVALID_LVALUE,
    E_UNDECLARED_IDENT,
    E_UNDECLARED_CONST,
    E_UNDECLARED_TYPE,
    E_UNDECLARED_VARIABLE,
    E_UNDECLARED_PROCEDURE,
    E_DUPLICATE_IDENT,
    E_INCOSISTENT_TYPE,
    E_INCONSISTENT_PARAM_ARGS
} ErrorCode;

#define E_MES_ENDOFCOMMENT "End of comment expected"
#define E_MES_STRINGTOOLONG "String too long"
#define E_MES_IDENTTOOLONG "Identifier too long"
#define E_MES_INVALIDCHAR "Invalid character"
#define E_MES_INVALIDSTRING "Invalid string"
#define E_MES_INVALIDSYMBOL "Invalid symbol"
#define E_MES_INVALIDIDENT "An Identifier Expected"
#define E_MES_INVALIDCONSTANT "A Constant Expected"
#define E_MES_INVALIDTYPE "A Type Expected"
#define E_MES_INVALIDPARAMTYPE "Invalid Parameter Type"
#define E_MES_INVALIDVARIABLE "A Variable Expected"
#define E_MES_INVALIDPROCEDURE "A Procedure Expected"
#define E_MES_INVALIDPARAM "A Param Expected"
#define E_MES_INVALIDSTATEMENT "Invalid Statement"
#define E_MES_INVALIDARGUMENT "Invalid Argument"
#define E_MES_INVALIDARITHOPERATOR "Invalid Arithmetic Operator."
#define E_MES_INVALIDRELATION "Invalid Relation."
#define E_MES_INVALIDEXPRESSION "Invalid Expression"
#define E_MES_INVALIDTERM "Invalid Term"
#define E_MES_INVALIDFACTOR "Invalid Factor"
#define E_MES_INVALIDLVALUE "Invalid Left Value"
#define E_MES_UNDECLAREDIDENT "Undeclared Identifier"
#define E_MES_UNDECLAREDCONST "Undeclared Constant"
#define E_MES_UNDECLAREDTYPE "Undeclared Type"
#define E_MES_UNDECLAREDVARIABLE "Undeclared Variable"
#define E_MES_UNDECLAREDPROCEDURE "Undeclared Procedure"
#define E_MES_DUPLICATEIDENT "Duplicated Identifier"
#define E_MES_INCONSISENTTYPE "Inconsistent Type"
#define E_MES_INCONSISENTPARAMARGS "Inconsistent Parameters Arguments"

void throw_error(ErrorCode code, int lineNo, int columnNo);
void missing_token(TokenType type, int lineNo, int columnNo);
void assert_scanner(const char *mesg);
void assert_parser(const char *mesg);
void assert_symbol_table(const char *mesg);
void assert_ast(const char *mesg);
void assert_semantics(const char *mesg);
void assert_codegen(const char *mesg);

#endif

#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>
#define MAX_STRING_LENGTH 50
#define MAX_KEYWORD_LENGTH 30
// enum type: https://www.geeksforgeeks.org/enumeration-enum-c/
typedef enum {
    T_UNKNOWN,
    T_ERROR,
    T_END_OF_FILE,
    T_ASSIGNMENT,
    T_RELATION,
    T_NUMBER_INT,
    T_NUMBER_FLOAT,
    T_PLUS,
    T_MINUS,
    T_MULTIPLY,
    T_DIVIDE,
    T_CHAR,
    T_STRING,
    T_IDENTIFIER,
    T_AND,
    T_OR,
    T_SINGLE_QUOTE,
    T_DOUBLE_QUOTE,
    T_COLON,
    T_SEMI_COLON,
    T_COMMA,
    T_LPAREN,
    T_RPAREN,
    T_LBRACKET,
    T_RBRACKET,
    T_EQ,
    T_NEQ, // not equal
    T_LT, // less than
    T_LTEQ, // less than or equal
    T_GT, // greater than
    T_GTEQ, // greater than or equal
    K_PROGRAM,
    K_IS,
    K_GLOBAL,
    K_IN,
    K_OUT,
    K_INOUT,
    K_INT,
    K_FLOAT,
    K_BOOL,
    K_CHAR,
    K_STRING,
    K_PROCEDURE,
    K_RETURN,
    K_NOT,
    K_BEGIN,
    K_END,
    K_IF,
    K_ELSE,
    K_THEN,
    K_TRUE,
    K_FALSE,
    K_FOR,
    K_WHILE,
} TokenType;

typedef struct Token {
    TokenType type;
    int lineNo, columnNo;
    union {
        char stringVal[MAX_STRING_LENGTH+1];
        int intVal;
        float floatVal;
        bool boolVal;
        char charVal;
    } val;
} Token;

int strcicmp(char const *a, char const *b);
TokenType check_reserved_word(char *string);
Token *make_token(TokenType type, int lineNo, int columnNo);
char *print_token_type(TokenType type);

#endif

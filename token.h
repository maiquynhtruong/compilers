#include <stdbool.h>
#define MAX_STRING_LENGTH 50
#define MAX_KEYWORD 30
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
    T_NEQ,
    T_LT,
    T_LTEQ,
    T_GT,
    T_GTEQ,
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

typedef struct {
    TokenType type;
    union {
        char stringVal[MAX_STRING_LENGTH];
        int intVal;
        float floatVal;
        bool boolVal;
        char charVal;
    } val;
} Token;

TokenType check_reserved_word(char *string);
void insert_keyword(char *string, TokenType type);
void insert_all_keywords();

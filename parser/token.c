#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "token.h"
// map structure: https://github.com/soywod/c-map/blob/master/map.c
struct {
    char keyword[MAX_STRING_LENGTH];
    TokenType type;
} keywords[MAX_KEYWORD_LENGTH] = {
    {"PROGRAM", K_PROGRAM},
    {"IS", K_IS},
    {"global", K_GLOBAL},
    {"integer", K_INT},
    {"float", K_FLOAT},
    {"string", K_STRING},
    {"bool", K_BOOL},
    {"char", K_CHAR},
    {"procedure", K_PROCEDURE},
    {"in", K_IN},
    {"out", K_OUT},
    {"inout", K_INOUT},
    {"return", K_RETURN},
    {"not", K_NOT},
    {"begin", K_BEGIN},
    {"end", K_END},
    {"if", K_IF},
    {"else", K_ELSE},
    {"then", K_THEN},
    {"true", K_TRUE},
    {"false", K_FALSE},
    {"while", K_WHILE},
    {"for", K_FOR}
};

// case insensitive string comparision
int strcicmp(char const *a, char const *b){
    for (;; a++, b++) {
        int d = tolower(*a) - tolower(*b);
        if (d != 0 || !*a)
            return d;
    }
}

TokenType check_reserved_word(char *str) {
    printf("In check_reserved_word\n");
    int i, count = sizeof(keywords)/sizeof(keywords[0]);

    for (i = 0; i < count; i++) {
        if (strcicmp(keywords[i].keyword, str) == 0) {
            printf("A keyword: %s\n", keywords[i].keyword);
            return keywords[i].type;
        }
    }
//    printf("ident: %s\n", str);
    return T_IDENTIFIER;
}

Token *make_token(TokenType type, int lineNo, int columnNo) {
    Token *token = (Token *) malloc(sizeof(Token));
    token->type = type;
    token->lineNo = lineNo;
    token->columnNo = columnNo;
    return token;
}

char *print_token_type(TokenType type) {
    switch (type) {
        case T_UNKNOWN: return "";
        case T_ERROR: return "";
        case T_END_OF_FILE: return "";
        case T_ASSIGNMENT: return ":=";
        case T_RELATION: return "";
        case T_NUMBER_INT: return "";
        case T_NUMBER_FLOAT: return "";
        case T_PLUS: return "+";
        case T_MINUS: return "-";
        case T_MULTIPLY: return "*";
        case T_DIVIDE: return "/";
        case T_CHAR: return "a char";
        case T_STRING: return "a string";
        case T_IDENTIFIER: return "an identifier";
        case T_AND: return "&&";
        case T_OR: return "||";
        case T_SINGLE_QUOTE: return "'";
        case T_DOUBLE_QUOTE: return "\"";
        case T_COLON: return ":";
        case T_SEMI_COLON: return ";";
        case T_COMMA: return ",";
        case T_LPAREN: return "(";
        case T_RPAREN: return ")";
        case T_LBRACKET: return "[";
        case T_RBRACKET: return "]";
        case T_EQ: return "=";
        case T_NEQ: return "!="; // not equal
        case T_LT: return "<"; // less than
        case T_LTEQ: return "<="; // less than or equal
        case T_GT: return ">"; // greater than
        case T_GTEQ: return ">="; // greater than or equal
        case K_PROGRAM: return "keyword PROGRAM";
        case K_IS: return "keyword IS";
        case K_GLOBAL: return "keyword GLOBAL";
        case K_IN: return "keyword IN";
        case K_OUT: return "keyword OUT";
        case K_INOUT: return "keyword INOUT";
        case K_INT: return "keyword integer";
        case K_FLOAT: return "keyword float";
        case K_BOOL: return "keyword BOOL";
        case K_CHAR: return "keyword CHAR";
        case K_STRING: return "keyword STRING";
        case K_PROCEDURE: return "keyword PROCEDURE";
        case K_RETURN: return "keyword RETURN";
        case K_NOT: return "keyword NOT";
        case K_BEGIN: return "keyword BEGIN";
        case K_END: return "keyword END";
        case K_IF: return "keyword IF";
        case K_ELSE: return "keyword ELSE";
        case K_THEN: return "keyword THEN";
        case K_TRUE: return "keyword TRUE";
        case K_FALSE: return "keyword FALSE";
        case K_FOR: return "keyword FOR";
        case K_WHILE: return "keyword WHILE";
        default: return "";
    }
}

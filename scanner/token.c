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
    {"inout", K_INOUT},
    {"integer", K_INT},
    {"float", K_FLOAT},
    {"bool", K_BOOL},
    {"char", K_CHAR},
    {"procedure", K_PROCEDURE},
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

TokenType check_reserved_word(char *str) {
    int i, count = sizeof(keywords)/sizeof(keywords[0]);
//    printf("str: %s\n", str);
    for (i = 0; i < count; i++) {
        if (strcmp(keywords[i].keyword, str) == 0) {
//            printf("keyword: %s\n", keywords[i].keyword);
            return keywords[i].type;
        }
    }
//    printf("ident: %s\n", str);
    // build the symbol table so that new entries are created with the token type
    // field initialized to IDENTIFIER
    return T_IDENTIFIER;
}

Token *make_token(TokenType type, int lineNo, int columnNo) {
    Token *token = (Token *) malloc(sizeof(Token));
    token->type = type;
    token->lineNo = lineNo;
    token->columnNo = columnNo;
    return token;
}

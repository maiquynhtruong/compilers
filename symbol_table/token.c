#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "token.h"
// map structure: https://github.com/soywod/c-map/blob/master/map.c
typedef struct {
    char keyword[MAX_STRING_LENGTH];
    TokenType type;
} Keyword;

Keyword keywords[MAX_KEYWORD];
int count = 0;

void insert_keyword(char *keyword, TokenType type) {
    strcpy(keywords[count].keyword, keyword);
    keywords[count].type = type;
    count++;
}

void insert_all_keywords() {
    insert_keyword("PROGRAM", K_PROGRAM);
    insert_keyword("IS", K_IS);
    insert_keyword("global", K_GLOBAL);
    insert_keyword("in", K_IN);
    insert_keyword("out", K_OUT);
    insert_keyword("inout", K_INOUT);
    insert_keyword("integer", K_INT);
    insert_keyword("float", K_FLOAT);
    insert_keyword("bool", K_BOOL);
    insert_keyword("char", K_CHAR);
    insert_keyword("procedure", K_PROCEDURE);
    insert_keyword("return", K_RETURN);
    insert_keyword("not", K_NOT);
    insert_keyword("begin", K_BEGIN);
    insert_keyword("end", K_END);
    insert_keyword("if", K_IF);
    insert_keyword("else", K_ELSE);
    insert_keyword("then", K_THEN);
    insert_keyword("true", K_TRUE);
    insert_keyword("false", K_FALSE);
    insert_keyword("while", K_WHILE);
    insert_keyword("for", K_FOR);
}

TokenType check_reserved_word(char *str) {
    int i;
//    printf("str: %s\n", str);
    for (i = 0; i < count; i++) {
        if (strcmp(keywords[i].keyword, str) == 0) {
//            printf("keyword: %s\n", keywords[i].keyword);
            return keywords[i].type;
        }
    }
//    printf("ident: %s\n", str);
    return T_IDENTIFIER;
}

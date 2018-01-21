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

void insert_keyword(char *keyword, TokenType type) {
    static int count;
    strcpy(keywords[0].keyword, keyword);
    keywords[count].type = type;
}

void insert_all_keywords() {
    insert_keyword("PROGRAM", K_PROGRAM);
    insert_keyword("IS", K_IS);
    insert_keyword("global", K_GLOBAL);
    insert_keyword("in", K_IN);
    insert_keyword("out", K_OUT);
    insert_keyword("inout", K_INOUT);
    insert_keyword("int", K_INT);
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
    insert_keyword("for", K_FOR);
    insert_keyword("while", K_WHILE);
}

int check_reserved_word(char *str) {
    int i, n = sizeof(keywords)/sizeof(keywords[0]);
    for (i = 0; i < n; i++) {
        if (strcmp(keywords[i].keyword, str)) 
            return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    insert_all_keywords();
    printf("%s%d\n", keywords[0].keyword, keywords[0].type);
}

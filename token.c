#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "token.h"

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
}

int main(int argc, char *argv[]) {
    insert_all_keywords();
    printf("%s%d\n", keywords[0].keyword, keywords[0].type);

}

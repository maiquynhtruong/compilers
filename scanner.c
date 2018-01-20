#include "scanner.h"
#include <stdio.h>
#include <string.h>

FILE *inp;
Token token;
int main(int argc, char *argv[]) {
    if (argc < 1) 
        printf("Error! No input file");
    else 
        init_scanner(argv); 
    while (next_token(&token) != T_END_OF_FILE)
        ; // process each token
    return 0;
}

int next_token(Token *token) {
    int cur, ch, nextChar;
    ch = getc(inp);

    switch(ch) {
        case '/':
            nextChar = getc(inp);
            if (nextChar == '/' || nextChar = '*') 
                ; // skip over comments
            else 
                ungetc(nextChar, inp);
        case ':':
            nextChar = getc(inp);
            if (nextChar == '=') 
                token->type = T_ASSIGNMENT;
            else
                ungetc(nextChar, inp);
        case ';': case ',':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            token->val.strVal[0] = ch;
            for (i = 1; isalnum(ch = getc(inp)) || ch == '_'; i++) // check if is digit or a letter
                token->val.strVal[i] = ch;
            ungetc(ch, inp);
            token->val.strVal[i] = '\0';
            token->type = check_reserved_word(token->val.strVal);
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '9':
            token->type = T_INTEGER;
            token->val.intVal = ch - '0';
            while (isdigit(ch = getc(inp))) {
                token->val.intVal = token->val.intVal*10 + ch - '0';
            }
            ungetc(ch, inp);
        case '+': case '-': case '*': case '/':
            token->type = T_OPERATOR;
        case '<': case '>': case '=': case '!':
            nextChar = getc(inp);
            if (nextChar == '=') 
                ; // just read in the equal and probably do nothing
            else 
                ungetc(nextChar, inp);
            token->type = T_RELATION;
        case EOF: case '.':
            token
        default:
            token->val.intVal = ch;
            token->type = T_UNKNOWN;
    }
}
void init_scanner(char *file_name) {
    inp = fopen(file_name, "r");
}
static TokenType check_reserved_word(char *tokenStr) {
    TokenType type = T_IDENTIFIER;
    int i;
    for (i = 0; i < reserved_words.size(); i++) 
        if (reserved_words[tokenStr]) {
            type = reserved_words[tokenStr];
            break;
        }
    return type;
}
static char** create_reserved_table() {
    char **reserved_words[100][MAX_STRING_LENGTH];
}
static void insert_reserved_words(char **table) {
}

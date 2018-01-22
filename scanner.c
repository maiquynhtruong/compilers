#include "scanner.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

FILE *inp;
Token token;
int main(int argc, char *argv[]) {
    if (argc < 2) 
        printf("Error! No input file...");
    else 
        init_scanner(argv[1]); 
    while (next_token(&token) != T_END_OF_FILE)
        print_token(&token); // process each token
    return 0;
}

TokenType next_token(Token *token) {
    int ch, nextChar, i;
    ch = getc(inp);
    while (isspace(ch))
        ch = getc(inp);
    switch(ch) {
        case '/':
            nextChar = getc(inp);
            if (nextChar == '/') {
                while ((ch = getc(inp)) != '\n')
                    ; // skip over one line comment
            } else if (nextChar == '*') {
                skip_star_comment();
            } else {
                token->type = T_ARITHOP;
                ungetc(nextChar, inp);
                return token->type;
            }
        // If the current character is any letter in the alphabet
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': 
        case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': 
        case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': 
        case 'Y': case 'Z': 
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': 
        case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': 
        case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x': 
        case 'y': case 'z': 
            token->val.stringVal[0] = ch;
            //printf("str in scanner.c: %s\n", token->val.stringVal);
            // digit or a letter
            for (i = 1; isalnum(ch = getc(inp)) || ch == '_'; i++) {
                token->val.stringVal[i] = ch;
            }
            ungetc(ch, inp);
            token->val.stringVal[i] = '\0';
            token->type = check_reserved_word(token->val.stringVal);
            return token->type;
        // Read number
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '9':
            token->type = T_NUMBER;
            token->val.intVal = ch - '0';
            while (isdigit(ch = getc(inp)) || ch == '.') {
                if (ch == '.') {
                    token->val.floatVal = token->val.intVal * 1.0;
                    break;
                } else {
                    token->val.intVal = token->val.intVal*10 + ch - '0';
                }
            }
            // reading floating number
            //if (ch == '.') {
            //    while (isdigit(ch = getc(inp))) {
            //        token->val.floatVal = token->val.floatVal*10 + (ch - '0')
            //    }
            //}
            ungetc(ch, inp);
            return token->type;
        // single quote characters
        case '\'':
            token->type = T_SINGLE_QUOTE;
            token->val.charVal = ch;
            getc(inp); // if correct, only one character inside a pair of single quotes
            return token->type; 
        // double quote strings
        case '"':
            token->type = T_DOUBLE_QUOTE;
            for (i = 1; (ch = getc(inp)) != '"'; i++) // read anything until a double quote
                token->val.stringVal[i] = ch;
            token->val.stringVal[i] = '\0';
            return token->type;
        // check if this is assignment token
        case ':':
            nextChar = getc(inp);
            if (nextChar == '=') 
                token->type = T_ASSIGNMENT;
            else
                token->type = T_COLON; // some random colon?
            return token->type;
        case ';': // for end of statement
            token->type = T_SEMI_COLON; // separate cases for colon, comma and semi colon to not 
                                        // mix up with single quote characters
            return token->type;
        case ',': // for separating argument list
            token->type = T_COMMA;
            return token->type;
        // Arithmetic operations. Division has been handled by the comment case
        case '+': case '-': case '*': 
            token->type = T_ARITHOP;
            return token->type;
        // Relations 
        case '<': case '>': case '=': case '!':
            nextChar = getc(inp);
            if (nextChar == '=') 
                ; // just read in the relation and probably do nothing
            else 
                ungetc(nextChar, inp);
            token->type = T_RELATION;
            return token->type;
        case '(':
            token->type = T_LPAREN;
            return token->type;
        case ')':
            token->type = T_RPAREN;
            return token->type;
        case EOF: case '.':
            token->type = T_END_OF_FILE;
            return token->type;
        default: // anything else is not recognized
            token->val.intVal = ch;
            token->type = T_UNKNOWN;
            return token->type;
    }
}

void init_scanner(char *file_name) {
    inp = fopen(file_name, "r");
    printf("Reading from file %s\n", file_name);
    insert_all_keywords();
}

// skips comments in /*...*/ blocks
void skip_star_comment() {

}

void print_token(Token *token) {
//    printf("token type: %d\n", token->type);
    switch(token->type) {
        case T_END_OF_FILE:
            printf("T_END_OF_FILE\n"); break;
        case T_ASSIGNMENT:
            printf("T_ASSIGNMENT\n"); break;
        case T_RELATION:
            printf("T_RELATION\n"); break;
        case T_NUMBER:
            printf("T_NUMBER\n"); break;
        case T_ARITHOP:
            printf("T_ARITHOP\n"); break;
        case T_CHAR:
            printf("T_CHAR\n"); break;
        case T_STRING:
            printf("T_STRING\n"); break;
        case T_IDENTIFIER:
            printf("T_IDENTIFIER\n"); break;
        case T_AND:
            printf("T_AND\n"); break;
        case T_OR:
            printf("T_OR\n"); break;
        case T_SINGLE_QUOTE:
            printf("T_SINGLE_QUOTE\n"); break;
        case T_DOUBLE_QUOTE:
            printf("T_DOUBLE_QUOTE\n"); break;
        case T_COLON:
            printf("T_COLON\n"); break;
        case T_SEMI_COLON:
            printf("T_SEMI_COLON\n"); break;
        case T_COMMA:
            printf("T_COMMA\n"); break;
        case T_LPAREN:
            printf("T_LPAREN\n"); break;
        case T_RPAREN:
            printf("T_RPAREN\n"); break;
        case T_LBRACKET:
            printf("T_LBRACKET\n"); break;
        case T_RBRACKET:
            printf("T_RBRACKET\n"); break;
        case K_PROGRAM:
            printf("K_PROGRAM\n"); break;
        case K_IS:
            printf("K_IS\n"); break;
        case K_GLOBAL:
            printf("K_GLOBAL\n"); break;
        case K_IN:
            printf("K_IN\n"); break;
        case K_OUT:
            printf("K_OUT\n"); break;
        case K_INOUT:
            printf("K_INOUT\n"); break;
        case K_INT:
            printf("K_INT\n"); break;
        case K_FLOAT:
           printf("K_FLOAT \n"); break;
        case K_BOOL:
            printf("K_BOOL\n"); break;
        case K_CHAR:
            printf("K_CHAR\n"); break;
        case K_PROCEDURE:
            printf("K_PROCEDURE\n"); break;
        case K_RETURN:
            printf("K_RETURN\n"); break;
        case K_NOT:
            printf("K_NOT\n"); break;
        case K_BEGIN:
            printf("K_BEGIN\n"); break;
        case K_END:
            printf("K_END\n"); break;
        case K_IF:
            printf("K_IF\n"); break;
        case K_THEN:
            printf("K_THEN\n"); break;
        case K_ELSE:
            printf("K_ELSE\n"); break;
        case K_TRUE:
            printf("K_TRUE\n"); break;
        case K_FALSE:
            printf("K_FALSE\n"); break;
        case K_FOR:
            printf("K_FOR\n"); break;
        case K_WHILE:
            printf("K_WHILE\n"); break;
        default:
            printf("T_UNKNOWN\n"); break;
    }
}


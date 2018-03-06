#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "scanner.h"
#include "error.h"
FILE *inp;
Token token;
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error! No input file...");
        exit(1);
    } else {
        init_scanner(argv[1]); 
    }
    while (next_token(&token) != T_END_OF_FILE) {
        print_token(&token); // process each token
//        reset_token(&token);
    }
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
        /*******LOOK******/                skip_star_comment();
            } else {
                ungetc(nextChar, inp);
                return token->type = T_DIVIDE;
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
            for (i = 1; isalnum(ch = getc(inp)) || ch == '_'; i++) {
                token->val.stringVal[i] = ch;
            }
            ungetc(ch, inp);
            token->val.stringVal[i] = '\0';
            token->type = check_reserved_word(token->val.stringVal);
            return token->type;
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '9':
            token->val.intVal = ch - '0';
            while (isdigit(ch = getc(inp)) || ch == '.') {
                if (ch == '.') {
                    token->val.floatVal = 1.0 * token->val.intVal;
                    break; // go to the loop that reads the decimal part
                } 
                token->val.intVal = token->val.intVal*10 + ch - '0';
            }
            if (ch == '.') {
                int exponent = 1;
                while (isdigit(ch = getc(inp))) {
                    exponent = exponent*10;
                    token->val.floatVal = token->val.floatVal * 10 + ch - '0';
                }
                token->val.floatVal = token->val.floatVal / exponent; 
                ungetc(ch, inp);
                return token->type = T_NUMBER_FLOAT; // assuming there is only one '.'
            }
            ungetc(ch, inp);
            return token->type = T_NUMBER_INT;
        case '\'':// single quote characters
            token->val.charVal = ch;
            getc(inp); // if correct program, only one character inside a pair of single quotes
            return token->type = T_CHAR;
        case '"': // double quote strings
            for (i = 0; (ch = getc(inp)) != '"'; i++) // read anything until a double quote
                token->val.stringVal[i] = ch;
            token->val.stringVal[i] = '\0';
            return token->type = T_STRING; 
        case ':': // check if this is assignment token
            nextChar = getc(inp);
            if (nextChar == '=') 
                token->type = T_ASSIGNMENT;
            else
                token->type = T_COLON; // some random colon?
            return token->type;
        case ';': // for end of statement
            // separate cases for colon, comma and semi colon to not mix up with single quote characters
            return token->type = T_SEMI_COLON;;
        case ',': // for separating argument list
            return token->type = T_COMMA;
        case '+': 
            return token->type = T_PLUS;
        case '-': 
            return token->type = T_MINUS;
        case '*': 
            return token->type = T_MINUS;
        case '<': 
            nextChar = getc(inp);
            if (nextChar == '=') return token->type = T_LTEQ; 
            else {
                ungetc(nextChar, inp);
                return token->type = T_LT;
            }
        case '>': 
            nextChar = getc(inp);
            if (nextChar == '=') return token->type = T_GTEQ;
            else {
                ungetc(nextChar, inp);
                return token->type = T_GT;
            }
        case '=': 
            nextChar = getc(inp);
            if (nextChar == '=') return token->type = T_EQ; else ungetc(nextChar, inp);
        case '!':
            nextChar = getc(inp);
            if (nextChar == '=') return token->type = T_NEQ; else ungetc(nextChar, inp);
        case '(':
            return token->type = T_LPAREN;
        case ')':
            return token->type = T_RPAREN;
        case '[':
            return token->type = T_LBRACKET;
        case ']':
            return token->type = T_RBRACKET;
        case EOF: case '.':
            return token->type = T_END_OF_FILE;
        default: // anything else is not recognized
            token->val.charVal = ch;
            throw_error(E_INVALID_CHAR);
            return token->type = T_UNKNOWN;
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

void reset_token(Token *token) {
    token->type = T_UNKNOWN;
    token->val.intVal = 0;
    token->val.floatVal = 0.0;
    token->val.boolVal = false;
    token->val.charVal = 0;
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
        case T_NUMBER_FLOAT:
            printf("T_NUMBER_FLOAT, %f\n", token->val.floatVal); break;
        case T_NUMBER_INT:
            printf("T_NUMBER_INT, %d\n", token->val.intVal); break;
        case T_ARITHOP:
            printf("T_ARITHOP\n"); break;
        case T_CHAR:
            printf("T_CHAR, '%c'\n", token->val.charVal); break;
        case T_STRING:
            printf("T_STRING, \"%s\"\n", token->val.stringVal); break;
        case T_IDENTIFIER:
            printf("T_IDENTIFIER, %s\n", token->val.stringVal); break;
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


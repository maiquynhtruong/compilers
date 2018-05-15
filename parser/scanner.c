#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "error.h"
#include "token.h"
#include "reader.h"
#include "scanner.h"

extern int lineNo;
extern int columnNo;
extern int cur_char;
int cur_line, cur_col;

// skips comments in /*...*/ blocks
void skip_block_comment() {
    int state = 0;
    while (cur_char != EOF && state < 2) {
        switch (cur_char) {
            case '*':
                state = 1;
                break;
            case '/':
                if (state == 1) state = 2;
                else state = 0;
                break;
            default: state = 0;
        }
        read_char();
    }
    if (state != 2) throw_error(E_END_OF_COMMENT, lineNo, columnNo);
}

void skip_line_comment() {
    while (cur_char != '\n' && cur_char != EOF) read_char(); // skip over one line comment
}

void skip_blank() { while (isspace(cur_char) && cur_char != -1) read_char(); }

Token *read_ident() {
  int i = 0;
  Token *token = make_token(T_IDENTIFIER, lineNo, columnNo);

  for (i = 0; isalnum(cur_char) || cur_char == '_'; i++) {

      if (i == MAX_STRING_LENGTH) {
          throw_error(E_IDENT_TOO_LONG, lineNo, columnNo);
          return token;
      }

      token->val.stringVal[i] = cur_char;
      read_char();
  }
  token->val.stringVal[i] = '\0';

  token->type = check_reserved_word(token->val.stringVal);
  printf("in read_ident: stringVal = %s\n", token->val.stringVal);
  return token;
}

Token *read_number() {
    Token *token = make_token(T_NUMBER_INT, lineNo, columnNo);
    token->val.intVal = cur_char - '0';

    while (isdigit(cur_char = read_char())) { //|| cur_char == '.') {
        if (cur_char == '.') {
            token->val.floatVal = 1.0 * token->val.intVal;
            break; // go to the loop that reads the decimal part
        }
        token->val.intVal = token->val.intVal*10 + cur_char - '0';
    }
    // TODO: Float number
    // if (cur_char == '.') {
    //     int exponent = 1;
    //     while (isdigit(cur_char = read_char())) {
    //         exponent = exponent*10;
    //         token->val.floatVal = token->val.floatVal * 10 + cur_char - '0';
    //     }
    //     token->val.floatVal = token->val.floatVal / exponent;
    //     ungetc(cur_char, inp);
    //     return token->type = T_NUMBER_FLOAT; // assuming there is only one '.'
    // }
    // ungetc(cur_char, inp);
    // token->type = T_NUMBER_INT;
    return token;
}

Token *read_string() {
    read_char(); // eat the double quote
    cur_line = lineNo; cur_col = columnNo;
    Token *token = make_token(T_STRING, lineNo, columnNo);
    int cnt = 0;

    // <string> :: = “[a-zA-Z0-9 _,;:.']*”
    while (cur_char != EOF && (isalnum(cur_char)
    || isspace(cur_char) || cur_char == '_' || cur_char == ';'
    || cur_char == ':' || cur_char == '.' || cur_char == '\'')) {
        if (cnt <= MAX_STRING_LENGTH) token->val.stringVal[cnt++] = (char) cur_char;
        read_char();
    }

    if (cnt > MAX_STRING_LENGTH) {
        throw_error(E_IDENT_TOO_LONG, cur_line, cur_col);
        return token;
    }

    token->val.stringVal[cnt] = '\0';

    if (cur_char == '\"') read_char();
    else {
        token->type = T_UNKNOWN;
        throw_error(E_INVALID_STRING, cur_line, cur_col);
    }
    return token;
}

Token *read_single_char() {
    read_char(); // read the single quote

    Token *token = make_token(T_CHAR, lineNo, columnNo);
    read_char();
    // TODO: check for EOF

    //
    token->val.charVal = cur_char;
    if (cur_char == '\'') {
        read_char();
    } else {
        token->type = T_UNKNOWN;
        throw_error(E_INVALID_CHAR, lineNo, columnNo);
    }
    return token;
}

Token* next_token() {
    Token *token;

    skip_blank();

    switch(cur_char) {
        case '/':
            cur_line = lineNo; cur_col = columnNo;
            read_char();

            switch (cur_char) {
              case '/':
                  read_char();
                  skip_line_comment();
                  return next_token();
              case '*':
                  read_char();
                  skip_block_comment();
                  return next_token();
              default: return make_token(T_DIVIDE, cur_line, cur_col);
            }
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H':
        case 'I': case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P':
        case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
        case 'Y': case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h':
        case 'i': case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p':
        case 'q': case 'r': case 's': case 't': case 'u': case 'v': case 'w': case 'x':
        case 'y': case 'z':
            return read_ident();

        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '9':
            return read_number();

        case '\'':// single quote characters
            return read_single_char();
        case '"': // double quote strings
            return read_string();

        case ':': // check if this is assignment token
            cur_line = lineNo; cur_col = columnNo;
            read_char();
            if (cur_char != EOF && cur_char == '=') {
                token = make_token(T_ASSIGNMENT, cur_line, cur_col);
                read_char();
            } else token = make_token(T_COLON, cur_line, cur_col);
            return token;
        case ';': // for end of statement
            // separate cases for colon, comma and semi colon to not mix up with single quote characters
            token = make_token(T_SEMI_COLON, lineNo, columnNo);
            read_char(); return token;
        case ',': // for separating argument list
            token = make_token(T_COMMA, lineNo, columnNo);
            read_char(); return token;
        case '+':
            token = make_token(T_PLUS, lineNo, columnNo);
            read_char(); return token;
        case '-':
            token = make_token(T_MINUS, lineNo, columnNo);
            read_char(); return token;
        case '*':
            token = make_token(T_MULTIPLY, lineNo, columnNo);
            read_char(); return token;
        case '<':
            cur_line = lineNo; cur_col = columnNo;
            read_char();
            if (cur_char != EOF && cur_char == '=') {
                token = make_token(T_LTEQ, cur_line, cur_col);
                read_char();
            } else token = make_token(T_LT, cur_line, cur_col);
            return token;
        case '>':
            cur_line = lineNo; cur_col = columnNo;
            read_char();
            if (cur_col != EOF && cur_char == '=') {
                token = make_token(T_GTEQ, cur_line, cur_col);
                read_char();
            } else token = make_token(T_GT, cur_line, cur_col);
            return token;
        case '=':
            cur_line = lineNo; cur_col = columnNo;
            read_char();
            if (cur_char != EOF && cur_char == '=') {
                token = make_token(T_EQ, cur_line, cur_col);
                read_char(); return token;
            } else {
                token = make_token(T_UNKNOWN, cur_line, cur_col);
                throw_error(E_INVALID_SYMBOL, cur_line, cur_col);
                return token;
            }
        case '!':
            cur_line = lineNo; cur_col = columnNo;
            read_char();
            if (cur_char != EOF && cur_char == '=') {
                token = make_token(T_NEQ, cur_line, cur_col);
                read_char(); return token;
            } else {
                token = make_token(T_UNKNOWN, cur_line, cur_col);
                throw_error(E_INVALID_SYMBOL, cur_line, cur_col);
                return token;
            }
        case '(':
            token = make_token(T_LPAREN, lineNo, columnNo);
            read_char(); return token;
        case ')':
            token = make_token(T_RPAREN, lineNo, columnNo);
            read_char(); return token;
        case '[':
            token = make_token(T_LBRACKET, lineNo, columnNo);
            read_char(); return token;
        case ']':
            token = make_token(T_RBRACKET, lineNo, columnNo);
            read_char(); return token;
        case EOF: case '.':
            return make_token(T_END_OF_FILE, lineNo, columnNo);
        default: // anything else is not recognized
            token = make_token(T_UNKNOWN, lineNo, columnNo);
            throw_error(E_INVALID_CHAR, lineNo, columnNo);
            read_char(); return token;
    }
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
        case T_PLUS:
            printf("T_PLUS\n"); break;
        case T_MULTIPLY:
            printf("T_MULTIPLY\n"); break;
        case T_DIVIDE:
            printf("T_DIVIDE\n"); break;
        case T_MINUS:
            printf("T_MINUS\n"); break;
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
        case T_EQ:
            printf("T_EQ\n"); break;
        case T_NEQ:
            printf("T_NEQ\n"); break;
        case T_LT:
            printf("T_LT\n"); break;
        case T_LTEQ:
            printf("T_LTEQ\n"); break;
        case T_GT:
            printf("T_GT\n"); break;
        case T_GTEQ:
            printf("T_GTEQ\n"); break;
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

// filter out the bad tokens
Token *next_valid_token() {
    Token *token = next_token();
    while (token->type == T_UNKNOWN) {
        free(token);
        token = next_token();
    }
    return token;
}

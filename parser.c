#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

// recursive descent parser
Token* look_ahead;
int main() {

}
void match_token(TokenType type) {
    // call print_token with token of this type

    if (look_ahead->type != type) {
        printf("Syntax error!");
        exit(1);
    } else {
        // consume next token
        next_token();
        // move on to next token
    }
}

void next_token() {

}

void parse_program() {
    match_token(K_PROGRAM);
    parse_identifier();
    match_token(K_IS);
    parse_statement();
    match_token(K_END);
    match_token(K_PROGRAM);
}
void parse_procedure() {
    match_token(K_PROCEDURE);
    parse_identifier();
    match_token(T_LPAREN);
    parse_params();
    match_token(T_RPAREN);
    parse_statement();
    match_token(K_END);
    match_token(K_PROCEDURE);
}

void parse_if() {
    match_token(K_IF);
    match_token(T_LPAREN);
    parse_expression();
    match_token(T_RPAREN);
    match_token(K_THEN);
    parse_statement();
}
// a separate end if because after else can come another if, so 
// we may have to call parse if recursively
void parse_end_if() {
    if (look_ahead->type == K_END) { // end if
        next_token();
        match_token(K_IF);
    } else if (look_ahead->type == K_ELSE) {
        next_token();
        if (look_ahead->type == K_IF) {
            parse_if(); // another if            
        } else {
            parse_statement();
        }
    } else { // nothing matches, throw error
        printf("Syntax error!\n");
    }
}

void parse_for() {
    match_token(K_FOR);
    match_token(T_LPAREN);
    parse_identifier();
    match_token(T_COLON);
    match_token(T_RPAREN);
    parse_statement();
    match_token(K_END);
    match_token(K_FOR);
}

void parse_identifier() {

}

void parse_procedure_statement() {
    match_token(K_BEGIN);
    parse_statement();
    match_token(K_END);
}

void parse_statement() {

}
void parse_expression() {

}

void parse_params() {

}

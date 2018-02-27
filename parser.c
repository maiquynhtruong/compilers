/* Some references: 
https://ryanflannery.net/teaching/common/recursive-descent-parsing/
http://www.craftinginterpreters.com/parsing-expressions.html
https://news.ycombinator.com/item?id=13914218
https://www.reddit.com/r/learnprogramming/comments/14aa4h/writing_a_parser_in_c/
https://github.com/herophuong/kpl-compiler/blob/master/parser/parser.c
*/
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"

// recursive descent parser
Token* look_ahead;
Token* current_token;

// a table to keep track of variable names

void match_token(TokenType type) {
    // call print_token with token of this type

    if (look_ahead->type != type) {
        printf("Syntax error!");
        exit(1);
    } else {
        print_token(current_token);
        
        Token *temp = current_token;
        current_token = look_ahead;
        next_token(look_ahead);
    }
}

void start_parsing() {
    current_token = NULL;
    next_token(look_ahead);

    parse_program();

}
void parse_program() {
    match_token(K_PROGRAM);
    match_token(T_IDENTIFIER);
    match_token(K_IS);
    parse_proc_declarations();
    parse_var_declarations();
    match_token(K_BEGIN);
    parse_statements();
    match_token(K_END);
    match_token(K_PROGRAM);
}

void parse_proc_declaration() {
    match_token(K_PROCEDURE);
    match_token(T_IDENTIFIER);
    match_token(T_LPAREN);
    parse_params();
    match_token(T_RPAREN);
    parse_statement();
    match_token(K_END);
    match_token(K_PROCEDURE);
}

void parse_proc_declarations() {

}

void parse_var_declaration() {
    if (look_ahead->type == K_GLOBAL) {
        match_token(K_GLOBAL);
        // put this in a global symbol table
    }
    parse_type();
    match_token(T_IDENTIFIER);
    if (look_ahead->type == T_RBRACKET) { // an array
        match_token(T_RBRACKET);
        match_token(T_NUMBER_INT);
        match_token(T_LBRACKET);
    }
    match_token(T_SEMI_COLON);
}

void parse_var_declarations() {
    TokenType next_type = look_ahead->type;
    while (next_type == K_GLOBAL || next_type == K_INT || next_type == K_FLOAT || next_type == K_BOOL || next_type == K_CHAR) {
        parse_var_declaration();
    }
}

void parse_statements() {
    if (look_ahead->type == K_PROCEDURE) {
        parse_procedure_call();

    }
}
void parse_assignment() {
    match_token(T_IDENTIFIER); //destination
    match_token(T_ASSIGNMENT);
    parse_expression();
    match_token(T_SEMI_COLON);

}

void parse_procedure_call() {
  
}

void parse_param() {
    parse_var_declaration();
    switch (look_ahead->type) {
        case K_IN:
            match_token(K_IN);
        case K_OUT:
            match_token(K_OUT);
        case K_INOUT:
            match_token(K_INOUT);
        default:
            printf("Syntax error\n"); break;
    }
}
void parse_params() {
    TokenType next_type = look_ahead->type;
    while (next_type == K_INT || next_type == K_FLOAT || next_type == K_BOOL || next_type == K_CHAR) {
        parse_param();
    }
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
    match_token(T_IDENTIFIER);
    match_token(T_COLON);
    match_token(T_RPAREN);
    parse_statement();
    match_token(K_END);
    match_token(K_FOR);
}



void parse_procedure_statement() {
    match_token(K_BEGIN);
    parse_statement();
    match_token(K_END);
}

void parse_type() {
    switch(look_ahead->type) {
        case K_INT:
            match_token(K_INT); break;
        case K_FLOAT:
            match_token(K_FLOAT); break;
        case K_BOOL:
            match_token(K_BOOL); break;
        case K_CHAR:
            match_token(K_CHAR); break;
        case K_STRING:
            match_token(K_STRING); break;
        default:
            printf("Syntax error!\n"); break;
    }
}

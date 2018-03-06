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
    // procedure header
    match_token(K_PROCEDURE);
    match_token(T_IDENTIFIER);
    match_token(T_LPAREN);
    parse_params();
    match_token(T_RPAREN);

    // procedure body
    parse_var_declarations();
    // parse_proc_declarations();
    match_token(K_BEGIN);
    parse_statements(); 
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
    switch (look_ahead->type) {
        case K_PROCEDURE: parse_procedure_call(); break;
        case K_IF: parse_if(); break;
        case K_FOR: parse_loop(); break;
        case K_RETURN: parse_return(); break;
        default: parse_assignment(); break;
    }
}
void parse_assignment() {
    match_token(T_IDENTIFIER); //destination
    match_token(T_ASSIGNMENT);
    parse_expression();
    match_token(T_SEMI_COLON);
}

void parse_param() {
    parse_var_declaration();
    switch (look_ahead->type) {
        case K_IN:
            match_token(K_IN); break;
        case K_OUT:
            match_token(K_OUT); break;
        case K_INOUT:
            match_token(K_INOUT); break;
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
    parse_statements();
    parse_end_if();
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

void parse_loop() {
    match_token(K_FOR);
    match_token(T_LPAREN);
    parse_assignment();
    parse_expression();
    match_token(T_RPAREN);
    parse_statements();
    match_token(K_END);
    match_token(K_FOR);
}

void parse_return() {
    match_token(K_RETURN);
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

void parse_procedure_call() {
    match_token(T_IDENTIFIER);
    match_token(T_LPAREN);
    parse_arguments();
    match_token(T_RPAREN);
}

void parse_expression() {
    if (look_ahead->type == K_NOT) {
        match_token(K_NOT);
    } 
    parse_arith_op();
    parse_expression_arith_op();
}

void parse_expression_arith_op() {
    switch(look_ahead->type) {
        case T_AND:
            match_token(T_AND);
            parse_arith_op();
            parse_expression_arith_op();
            break;
        case T_OR:
            match_token(T_OR);
            parse_arith_op();
            parse_expression_arith_op();
            break;
        // default:
    }
}

void parse_arith_op() {
    parse_relation();
    parse_arith_op_relation();
}
    
void parse_arith_op_relation() {
    switch(look_ahead->type) {
        case T_PLUS:
            match_token(T_PLUS);
            parse_relation();
            parse_arith_op_relation(); // recurse
            break;
        case T_MINUS:
            match_token(T_MINUS);
            parse_relation();
            parse_arith_op_relation();
            break;
        default:
            // throws error
            break;
    }
}

void parse_relation() {
    parse_term();
    parse_relation_term();
}
void parse_relation_term() {
    switch(look_ahead->type) {
        case T_LT:
            match_token(T_LT);
            parse_term();
            parse_relation_term();
            break;
        case T_GTEQ:
            match_token(T_GTEQ);
            break;
        case T_LTEQ:
            match_token(T_LTEQ);
            break;
        case T_GT:
            match_token(T_GT);
            break;
        case T_EQ:
            match_token(T_EQ);
            break;
        case T_NEQ:
            match_token(T_NEQ);
            break;
    }
}

void parse_term() {
    parse_factor();
    parse_term_factor();
}

void parse_term_factor() {
    switch(look_ahead->type) {
        case '*':
            match_token(T_MULTIPLY);
            parse_factor();
            parse_term_factor(); // recurse
            break;
        case '/':
            match_token(T_DIVIDE);
            parse_factor();
            parse_term_factor();
            break;
    }
}
void parse_factor() {
    switch (look_ahead->type) {
        case T_STRING:
            match_token(T_STRING);
        case T_CHAR:
            match_token(T_CHAR);
        case T_LPAREN: // ( <expression> )
            match_token(T_LPAREN);
            parse_expression();
            match_token(T_RPAREN);
        case T_IDENTIFIER: // <name> ::= <identifier> [ [ <expression> ] ]
            match_token(T_IDENTIFIER);
            match_token(T_LBRACKET);  // does it mean it has to be here?
            parse_expression();
            match_token(T_RBRACKET);
        case KW_TRUE:
            match_token(KW_TRUE);
        case KW_FALSE:
            match_token(KW_FALSE);
    }
}
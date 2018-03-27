/* Some references: 
https://ryanflannery.net/teaching/common/recursive-descent-parsing/
http://www.craftinginterpreters.com/parsing-expressions.html
https://news.ycombinator.com/item?id=13914218
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

        free(temp);
    }
}

// holy entry point of entire program
void start_parsing(char *filename) {
    current_token = NULL;
    next_token(look_ahead);

    parse_program();

    free(current_token);
    free(look_ahead);
}

void parse_program() {
    Entry *entry = NULL;

    // program header
    match_token(K_PROGRAM);
    match_token(T_IDENTIFIER);
    match_token(K_IS);

    entry = createProgramEntry();
    enter_scope(entry->programAttr->scope);

    // program body
    if (look_ahead->type != K_BEGIN) {
        parse_declarations();    
    }
    match_token(K_BEGIN);
    if (look_ahead->type != K_END) {
        parse_statements();
    }

    exit_scope();

    match_token(K_END);
    match_token(K_PROGRAM);
}

void parse_declarations() {
    TokenType type = look_ahead->type;
    while (type == K_PROCEDURE || type == K_INT || type == K_FLOAT || type == K_STRING || type == K_BOOL || type == K_CHAR) {
        parse_declaration();
        match_token(T_SEMI_COLON);
    }
}

void parse_declaration() {
    if (look_ahead->type == K_GLOBAL) {
        match_token(K_GLOBAL);
        // put this in a global symbol table
    }
    if (look_ahead->type == K_PROCEDURE) {
        parse_proc_declaration();
    } else {
        TokenType type = look_ahead->type;
        if (type == K_INT || type == K_FLOAT || type == K_STRING || type == K_BOOL || type == K_CHAR) {
            parse_var_declaration();    
        }
    }
}

void parse_proc_declaration() {
    // procedure header
    match_token(K_PROCEDURE);
    match_token(T_IDENTIFIER);
    match_token(T_LPAREN);
    if (look_ahead->type != T_RPAREN) {
        parse_param_list();    
    }
    match_token(T_RPAREN);

    // procedure body
    if (look_ahead->type != K_BEGIN) {
        parse_declarations();    
    }
    match_token(K_BEGIN);
    if (look_ahead->type != K_END) {
        parse_statements();
    }
    match_token(K_END);
    match_token(K_PROCEDURE);
}

void parse_var_declaration() {
    parse_type_mark();
    match_token(T_IDENTIFIER);

    if (look_ahead->type == T_LBRACKET) { // an array
        match_token(T_LBRACKET);
        if (look_ahead->type == T_MINUS) {
            match_token(T_MINUS);
        }
        if (look_ahead->type == T_NUMBER_INT) match_token(T_NUMBER_INT); // lower bound
        else if (look_ahead->type == T_NUMBER_FLOAT) match_token(T_NUMBER_FLOAT);
        
        match_token(T_COLON);
        
        if (look_ahead->type == T_MINUS) {
            match_token(T_MINUS);
        }
        if (look_ahead->type == T_NUMBER_INT) match_token(T_NUMBER_INT); // uppper bound
        else if (look_ahead->type == T_NUMBER_FLOAT) match_token(T_NUMBER_FLOAT);
        match_token(T_RBRACKET);
    }
}

void parse_type_mark() {
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

void parse_statements() {
    TokenType type = look_ahead->type;
    while (type == T_IDENTIFIER || type == K_PROCEDURE || type == K_IF || type == K_FOR || type == K_RETURN) {
        parse_statement();
        match_token(T_SEMI_COLON);    
    }
}

void parse_statement() {
    switch (look_ahead->type) {
        case K_PROCEDURE: parse_procedure_call(); break;
        case K_IF: parse_if(); break;
        case K_FOR: parse_loop(); break;
        case K_RETURN: parse_return(); break;
        case T_IDENTIFIER: parse_assignment(); break;
        default: break;
    }
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

void parse_param_list() {
    parse_param();
    parse_param_list_param();
}

void parse_param_list_param() {
    if (look_ahead->type == T_COMMA) {
        match_token(T_COMMA);
        parse_param();
        parse_param_list_param();
    }
}

void parse_assignment() {
    parse_destination();
    match_token(T_ASSIGNMENT);
    parse_expression();
}

void parse_if() {
    match_token(K_IF);
    match_token(T_LPAREN);
    parse_expression();
    match_token(T_RPAREN);
    match_token(K_THEN);
    parse_statements();
    if (look_ahead->type == K_ELSE) {
        match_token(K_ELSE);
        parse_statements();
    }
    match_token(K_END);
    match_token(K_IF);

}

void parse_loop() {
    match_token(K_FOR);
    match_token(T_LPAREN);
    parse_assignment();
    match_token(T_SEMI_COLON);
    parse_expression();
    match_token(T_RPAREN);
    if (look_ahead->type != K_END) {
        parse_statements();
    }
    match_token(K_END);
    match_token(K_FOR);
}

void parse_return() {
    match_token(K_RETURN);
}

void parse_procedure_call() {
    match_token(T_IDENTIFIER);
    match_token(T_LPAREN);
    if (look_ahead->type != T_RPAREN) {
        parse_argument_list();    
    }
    match_token(T_RPAREN);
}

void parse_destination() {
    match_token(T_IDENTIFIER);
    if (look_ahead->type == T_LBRACKET) {
        match_token(T_LBRACKET);
        parse_expression();
        match_token(T_RBRACKET);
    }
}

void parse_argument_list() {
    parse_expression();
    parse_argument_list_expression();
}

void parse_argument_list_expression() {
    if (look_ahead->type == T_COMMA) 
        parse_argument_list();
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
        // TODO: default
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
            // TODO: throws error
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
            match_token(T_STRING); break;
        case T_CHAR:
            match_token(T_CHAR); break;
        case T_LPAREN: // ( <expression> )
            match_token(T_LPAREN);
            parse_expression();
            match_token(T_RPAREN);
            break;
        case T_MINUS:
            match_token(T_MINUS);
        case T_IDENTIFIER: // <name> ::= <identifier> [ [ <expression> ] ]
            match_token(T_IDENTIFIER);
            if (look_ahead->type == T_LBRACKET) {
                match_token(T_LBRACKET); 
                parse_expression();
                match_token(T_RBRACKET);    
            }
            break;
        case T_NUMBER_INT: 
            match_token(T_NUMBER_INT); break;
        case T_NUMBER_FLOAT:
            match_token(T_NUMBER_FLOAT); break;
        case K_TRUE:
            match_token(K_TRUE); break;
        case K_FALSE:
            match_token(K_FALSE); break;
    }
}

/* Some references:
https://ryanflannery.net/teaching/common/recursive-descent-parsing/
http://www.craftinginterpreters.com/parsing-expressions.html
https://news.ycombinator.com/item?id=13914218
*/
#include <stdlib.h>
#include <stdio.h>
#include "error.h"
#include "reader.h"
#include "parser.h"
#include "scanner.h"

// recursive descent parser
Token* look_ahead;
Token* current_token;

void match_token(TokenType type) {
    printf("In match_token. Expected type: ");
    printf("%s\n", print_token_type(type));

    if (look_ahead->type != type) {
        printf("Not Matched. look_ahead is: ");
        print_token(look_ahead);
        missing_token(type, look_ahead->lineNo, look_ahead->columnNo);
    } else {
        printf("Matched. look_ahead is: ");
        print_token(look_ahead);

        Token *temp = current_token;
        current_token = look_ahead;
        look_ahead = next_valid_token();

        free(temp);

        printf("Next parsed token : ");
        print_token(look_ahead);
        printf("\n");
    }
}

// holy entry point
int parse(char *file_name) {
    if (open_input_stream(file_name) == IO_ERROR)
        return IO_ERROR;

    current_token = NULL;
    look_ahead = next_token();

    parse_program();

    free(current_token);
    free(look_ahead);
    close_input_stream();
    return IO_SUCCESS;
}

void parse_program() {
    // program header
    match_token(K_PROGRAM);
    match_token(T_IDENTIFIER);
    match_token(K_IS);

    parse_program_body();

    if (look_ahead->type == T_END_OF_FILE) match_token(T_END_OF_FILE);
}

void parse_program_body() {
    // program body
    if (look_ahead->type != K_BEGIN) {
        parse_declarations();
    }
    match_token(K_BEGIN);

    if (look_ahead->type != K_END) {
        parse_statements();
    }
    match_token(K_END);
    match_token(K_PROGRAM);
}

void parse_declarations() {
    assert("Parsing declarations");
    switch (look_ahead->type) {
        case K_GLOBAL:
            match_token(K_GLOBAL);
            parse_declarations();
            break;
        case K_PROCEDURE:
            parse_proc_declaration();
            match_token(T_SEMI_COLON);
            parse_declarations();
            break;
        // FOLLOW set
        case K_BEGIN: // from program_body, procedure_body
            break;
        default:
            parse_var_declaration();
            match_token(T_SEMI_COLON);
            parse_declarations();
            break;
    }
    assert("Done parsing declarations");
}

void parse_proc_declaration() {
    assert("Parsing a procedure declaration");
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
    assert("Done parsing a procedure declaration");
}

void parse_var_declaration() {
    assert("Parsing a variable declaration");
    parse_type_mark();
    match_token(T_IDENTIFIER);

    if (look_ahead->type == T_LBRACKET) { // an array
        match_token(T_LBRACKET);
        //TODO: Is there upper and lower bound or just a number for array size?
        // if (look_ahead->type == T_MINUS) match_token(T_MINUS);
        match_token(T_NUMBER_INT); // lower bound

        // match_token(T_COLON);

        // if (look_ahead->type == T_MINUS) match_token(T_MINUS);
        // match_token(T_NUMBER_INT); // uppper bound
        match_token(T_RBRACKET);
    }
    assert("Done parsing a variable declaration");
}

void parse_type_mark() {
  assert("Parsing a type mark");
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
            throw_error(E_INVALID_TYPE, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    assert("Done parsing a type mark");
}

void parse_statements() {
    parse_statement();
    parse_statement_chain();
}

void parse_statement_chain() {
    switch (look_ahead->type) {
        case T_SEMI_COLON:
            match_token(T_SEMI_COLON);
            parse_statement();
            parse_statement_chain();
            break;
        case K_END: case K_ELSE: break;
        default:
            throw_error(E_INVALID_STATEMENT, look_ahead->lineNo, look_ahead->columnNo); break;
    }
}

void parse_statement() {
    assert("Parsing a statement");
    switch (look_ahead->type) {
        case K_IF: parse_if_statement(); break;
        case K_FOR: parse_loop_statement(); break;
        case K_RETURN: parse_return_statement(); break;
        case T_IDENTIFIER:
            // match_token(T_IDENTIFIER);
            parse_assignment_statement();
            if (look_ahead->type == T_LPAREN) parse_procedure_call();
            break;
        case K_END: case K_ELSE: case T_SEMI_COLON: break;
        default: throw_error(E_INVALID_STATEMENT, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    assert("Done parsing a statement");
}

void parse_assignment_statement() {
    assert("Parsing an assignment statement");
    parse_destination();
    if (look_ahead->type == T_LPAREN) return;
    match_token(T_ASSIGNMENT);
    parse_expression();
    assert("Done parsing an assignment statement");
}

void parse_if_statement() {
    assert("Parsing an if statement");
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
    assert("Done parsing an if statement");
}

void parse_loop_statement() {
    assert("Parsing a for loop");
    match_token(K_FOR);
    match_token(T_LPAREN);
    parse_assignment_statement();
    match_token(T_SEMI_COLON);
    parse_expression();
    match_token(T_RPAREN);
    if (look_ahead->type != K_END) {
        parse_statements();
    }
    match_token(K_END);
    match_token(K_FOR);
    assert("Done parsing a for loop");
}

void parse_return_statement() {
    match_token(K_RETURN);
}

void parse_procedure_call() {
    assert("Parsing a procedure call");
    // if the identifier had not be parsed by parse_assignment_statement()
    if (look_ahead->type == T_IDENTIFIER) match_token(T_IDENTIFIER);

    match_token(T_LPAREN);
    if (look_ahead->type != T_RPAREN) {
        parse_argument_list();
    }
    match_token(T_RPAREN);
    assert("Done parsing a procedure call");
}

void parse_param() {
    assert("Parsing a parameter");
    parse_var_declaration();
    switch (look_ahead->type) {
        case K_IN:
            match_token(K_IN); break;
        case K_OUT:
            match_token(K_OUT); break;
        case K_INOUT:
            match_token(K_INOUT); break;
        default:
            throw_error(E_INVALID_PARAM_TYPE, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    assert("Done parsing a parameter");
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

void parse_destination() {
    assert("Parsing a destination");
    match_token(T_IDENTIFIER);
    if (look_ahead == T_LPAREN) {
        assert("Not a destination. Backtracking to parse a procedure call");
        return; // back track to parse_statement
    }

    if (look_ahead->type == T_LBRACKET) {
        match_token(T_LBRACKET);
        parse_expression();
        match_token(T_RBRACKET);
    }
    assert("Done parsing an destination");
}

void parse_argument_list() {
    assert("Parsing an argument list");
    parse_expression();
    parse_argument_list_expression();
    assert("Done parsing an argument list");
}
//
void parse_argument_list_expression() {
    switch (look_ahead->type) {
        case T_COMMA:
            match_token(T_COMMA);
            parse_expression();
            parse_argument_list_expression();
            break;
        // FOLLOW set
        case T_RPAREN:
            break;
        default: throw_error(E_INVALID_ARGUMENT, look_ahead->lineNo, look_ahead->columnNo);
    }
}

void parse_expression() {
    assert("Parsing an expression");
    if (look_ahead->type == K_NOT) {
        match_token(K_NOT);
    }
    parse_arith_op();
    parse_expression_arith_op();
    assert("Done parsing an expression");
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
        default: break;
    }
}

void parse_arith_op() {
    assert("Parsing an arithmetic operation");
    parse_relation();
    parse_arith_op_relation();
    assert("Done parsing an arithmetic operation");
}

void parse_arith_op_relation() {
    switch(look_ahead->type) {
        case T_PLUS:
            match_token(T_PLUS);
            parse_relation();
            parse_arith_op_relation();
            break;
        case T_MINUS:
            match_token(T_MINUS);
            parse_relation();
            parse_arith_op_relation();
            break;
        // FOLLOW set
        case T_AND: case T_OR: case T_COMMA: // expression
        case T_RPAREN: // for loop
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            break;
        default: throw_error(E_INVALID_ARITH_OPERATOR, look_ahead->lineNo, look_ahead->columnNo); break;
    }
}

void parse_relation() {
    assert("Parsing a relation");
    parse_term();
    parse_relation_term();
    assert("Done parsing a relation");
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
            parse_term();
            parse_relation_term();
            break;
        case T_LTEQ:
            match_token(T_LTEQ);
            parse_term();
            parse_relation_term();
            break;
        case T_GT:
            match_token(T_GT);
            parse_term();
            parse_relation_term();
            break;
        case T_EQ:
            match_token(T_EQ);
            parse_term();
            parse_relation_term();
            break;
        case T_NEQ:
            match_token(T_NEQ);
            parse_term();
            parse_relation_term();
            break;
        // FOLLOW set
        case T_PLUS: case T_MINUS: // arith op
        case T_AND: case T_OR: case T_COMMA: // expression
        case T_RPAREN: // for loop
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            break;
        default: throw_error(E_INVALID_RELATION, look_ahead->lineNo, look_ahead->columnNo); break;
        // default: break;
    }
}

void parse_term() {
    assert("Parsing a term");
    parse_factor();
    parse_term_factor();
    assert("Done parsing a term");
}

void parse_term_factor() {
    switch(look_ahead->type) {
        case '*':
            match_token(T_MULTIPLY);
//             parse_factor();
//             parse_term_factor(); // recurse
            break;
        case '/':
            match_token(T_DIVIDE);
//             parse_factor();
//             parse_term_factor();
            break;
    }
}

void parse_factor() {
    assert("Parsing a factor");
    switch (look_ahead->type) {
//         case T_STRING:
//             match_token(T_STRING); break;
//         case T_CHAR:
//             match_token(T_CHAR); break;
//         case T_LPAREN: // ( <expression> )
//             match_token(T_LPAREN);
//             parse_expression();
//             match_token(T_RPAREN);
//             break;
        case T_MINUS: // [-] <name> | [-] <number>
            match_token(T_MINUS);
            assert("A negative number of a negative name");
            parse_factor();
            break;
        case T_IDENTIFIER: // <name> ::= <identifier> [ [ <expression> ] ]
            match_token(T_IDENTIFIER);
            if (look_ahead->type == T_LBRACKET) { // array
                match_token(T_LBRACKET);
                parse_expression();
                match_token(T_RBRACKET);
            }
            break;
        case T_NUMBER_INT:
            match_token(T_NUMBER_INT); break;
//         case T_NUMBER_FLOAT:
//             match_token(T_NUMBER_FLOAT); break;
        case K_TRUE:
            match_token(K_TRUE); break;
        case K_FALSE:
            match_token(K_FALSE); break;
    }
    assert("Done parsing a factor");
}

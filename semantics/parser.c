#include <stdlib.h>
#include <stdio.h>
#include "debug.h"
#include "error.h"
#include "reader.h"
#include "parser.h"
#include "scanner.h"

Token* look_ahead;
Token* current_token;

// from symbol_table.c
extern SymbolTable *symbolTable;
extern Type *intType;
extern Type *charType;
extern Type *floatType;
extern Type *stringType;
extern Type *boolType;

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

    init_symbol_table();

    parse_program();
    print_entry(symbolTable->program);

    clear_symbol_table();

    free(current_token);
    free(look_ahead);
    close_input_stream();
    return IO_SUCCESS;
}

void parse_program() {
    assert("Parsing the program");

    // create, enter and exit a program scope
    Entry *entry = NULL;
    // program header
    match_token(K_PROGRAM);
    match_token(T_IDENTIFIER);
    match_token(K_IS);
    entry = create_program_entry(current_token->val.stringVal);

    enter_scope(entry->progAttrs->scope);
    parse_program_body();
    if (look_ahead->type == T_END_OF_FILE) match_token(T_END_OF_FILE);
    exit_scope();

    assert("Done parsing the program");
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

    Entry *entry = NULL;
    int global = 0;

    if (look_ahead->type == K_GLOBAL) {
        match_token(K_GLOBAL);
        global = 1;
    }

    switch (look_ahead->type) {
        case K_PROCEDURE:
            parse_proc_declaration(&entry, global);
            match_token(T_SEMI_COLON);
            declare_entry(entry);
            parse_declarations();
            break;
        // FOLLOW set
        case K_BEGIN: // from program_body, procedure_body
            break;
        default:
            parse_var_declaration(&entry, global);
            match_token(T_SEMI_COLON);
            declare_entry(entry);
            parse_declarations();
            break;
    }
    assert("Done parsing declarations");
}

void parse_proc_declaration(Entry **entry, int global) {
    assert("Parsing a procedure declaration");

    // procedure header
    match_token(K_PROCEDURE);
    match_token(T_IDENTIFIER);

    *entry = create_procedure_entry(current_token->val.stringVal, global);
    enter_scope((*entry)->procAttrs->scope);

    match_token(T_LPAREN);
    if (look_ahead->type != T_RPAREN) parse_param_list();
    match_token(T_RPAREN);

    // procedure body
    if (look_ahead->type != K_BEGIN) parse_declarations();
    match_token(K_BEGIN);

    if (look_ahead->type != K_END) parse_statements();
    match_token(K_END);
    match_token(K_PROCEDURE);
    exit_scope();
    // declare_entry(entry);

    assert("Done parsing a procedure declaration");
}

void parse_var_declaration(Entry **entry, int global) {
    assert("Parsing a variable declaration");

    Type *typeMark = parse_type_mark();
    match_token(T_IDENTIFIER);

    *entry = create_variable_entry(current_token->val.stringVal, global);
    (*entry)->varAttrs->type = typeMark;

    if (look_ahead->type == T_LBRACKET) { // an array
        match_token(T_LBRACKET);
//         //TODO: Is there upper and lower bound or just a number for array size?
//         // if (look_ahead->type == T_MINUS) match_token(T_MINUS);
        match_token(T_NUMBER_INT); // lower bound
//
//         // match_token(T_COLON);
//
        // if (look_ahead->type == T_MINUS) match_token(T_MINUS);
        // match_token(T_NUMBER_INT); // uppper bound
        (*entry)->varAttrs->type->elementType = (*entry)->varAttrs->type;
        (*entry)->varAttrs->type->typeClass = TC_ARRAY;
        (*entry)->varAttrs->type->arraySize = current_token->val.intVal;
        match_token(T_RBRACKET);
    }
    // declare_entry(entry); // parse_declarations() should do this

    assert("Done parsing a variable declaration");
}

Type* parse_type_mark() {
    assert("Parsing a type mark");

    Type *typeMark = (Type *) malloc(sizeof(Type));
    switch(look_ahead->type) {
        case K_INT:
            match_token(K_INT);
            typeMark->typeClass = TC_INT; break;
        case K_FLOAT:
            match_token(K_FLOAT);
            typeMark->typeClass = TC_FLOAT; break;
        case K_BOOL:
            match_token(K_BOOL);
            typeMark->typeClass = TC_BOOL; break;
        case K_CHAR:
            match_token(K_CHAR);
            typeMark->typeClass = TC_CHAR; break;
        case K_STRING:
            match_token(K_STRING);
            typeMark->typeClass = TC_STRING; break;
        default:
            throw_error(E_INVALID_TYPE, look_ahead->lineNo, look_ahead->columnNo); break;
    }

    assert("Done parsing a type mark");
    return typeMark;
}
//
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
        // FOLLOW set
        case K_END: case K_ELSE: // if statement, for loop
            break;
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
            parse_assignment_statement();
            if (look_ahead->type == T_LPAREN) parse_procedure_call();
            break;
        // FOLLOW set
        case K_END: case K_ELSE: case T_SEMI_COLON: // if statement, for loop
            break;
        default: throw_error(E_INVALID_STATEMENT, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    assert("Done parsing a statement");
}

void parse_indexes() {
    if (look_ahead->type == T_LBRACKET) {
        match_token(T_LBRACKET);
        parse_expression();
        match_token(T_RBRACKET);
        parse_indexes(); // for 2D array or more
    }
}

void parse_destination() {
    assert("Parsing a destination");
    match_token(T_IDENTIFIER);
    if (look_ahead->type == T_LPAREN) {
        assert("Not a destination. Backtracking to parse a procedure call");
        return; // back track to parse_statement
    }

    parse_indexes();

    assert("Done parsing a destination");
}

void parse_assignment_statement() {
    assert("Parsing an assignment statement");
    parse_destination();
    if (look_ahead->type == T_LPAREN) return; // backtrack to parse procedure
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
    parse_argument_list();
    match_token(T_RPAREN);
    assert("Done parsing a procedure call");
}

// TODO: There's another way of parsing params
void parse_param_list() {
    parse_param();
    parse_param_list_param();
}

void parse_param_list_param() {
    switch (look_ahead->type) {
        case T_COMMA:
            match_token(T_COMMA);
            parse_param();
            parse_param_list_param();
            break;
        case T_RPAREN: break;
        default: throw_error(E_INVALID_PARAM_TYPE, look_ahead->lineNo, look_ahead->columnNo); break;
    }
}

void parse_param() {
    assert("Parsing a parameter");
    Entry *entry = NULL;
    parse_var_declaration(&entry, 0);
    // TODO: delete the declaration of the variable in the outerscope
    entry = create_parameter_entry(entry->name, symbolTable->currentScope->parent);
    switch (look_ahead->type) {
        case K_IN:
            entry->paramAttrs->paramType = PT_IN;
            match_token(K_IN); break;
        case K_OUT:
            entry->paramAttrs->paramType = PT_OUT;
            match_token(K_OUT); break;
        case K_INOUT:
            entry->paramAttrs->paramType = PT_INOUT;
            match_token(K_INOUT); break;
        default:
            throw_error(E_INVALID_PARAM_TYPE, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    declare_entry(entry);
    assert("Done parsing a parameter");
}

void parse_argument_list() {
    assert("Parsing an argument list");
    parse_expression();
    parse_argument_list_expression();
    assert("Done parsing an argument list");
}

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
        // FOLLOW set
        case T_COMMA: // argument list
        case T_RPAREN: // for loop, if statement
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            break;
        default: throw_error(E_INVALID_EXPRESSION, look_ahead->lineNo, look_ahead->columnNo);
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
        case T_RPAREN: // for loop, if statement
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
        case T_RPAREN: // for loop, if statement
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
        case T_MULTIPLY:
            match_token(T_MULTIPLY);
            parse_factor();
            parse_term_factor();
            break;
        case T_DIVIDE:
            match_token(T_DIVIDE);
            parse_factor();
            parse_term_factor();
            break;
        // FOLLOW set
        case T_LT: case T_LTEQ: case T_GT: case T_GTEQ: case T_EQ: case T_NEQ: // relation
        case T_PLUS: case T_MINUS: // arith op
        case T_AND: case T_OR: case T_COMMA: // expression
        case T_RPAREN: // for loop, if statement
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            break;
        default: throw_error(E_INVALID_TERM, look_ahead->lineNo, look_ahead->columnNo); break;
    }
}

void parse_factor() {
    assert("Parsing a factor");
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
        case T_MINUS: // [-] <name> | [-] <number>
            match_token(T_MINUS);
            assert("A negative number of a negative name");
            parse_factor();
            break;
        case T_IDENTIFIER: // <name> ::= <identifier> [ [ <expression> ] ]
            match_token(T_IDENTIFIER);
            parse_indexes();
            break;
        case T_NUMBER_INT:
            match_token(T_NUMBER_INT); break;
//         case T_NUMBER_FLOAT:
//             match_token(T_NUMBER_FLOAT); break;
        case K_TRUE:
            match_token(K_TRUE); break;
        case K_FALSE:
            match_token(K_FALSE); break;
        // FOLLOW set
        case T_AND: case T_OR: case T_COMMA: // expression
        case T_RPAREN: // for loop, if statement
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            break;
        default: throw_error(E_INVALID_FACTOR, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    assert("Done parsing a factor");
}

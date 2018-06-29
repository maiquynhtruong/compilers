#include <stdlib.h>
#include <stdio.h>

#include "error.h"
#include "reader.h"
#include "parser.h"
#include "semantics.h"
#include "scanner.h"

Token* look_ahead;
Token* current_token;

// from symbol_table.c
extern SymbolTable *symbolTable;

void match_token(TokenType type) {
    // printf("In match_token. Expected type: ");
    // printf("%s\n", print_token_type(type));

    if (look_ahead->type != type) {
        // printf("Not Matched. look_ahead is: ");
        print_token(look_ahead);
        missing_token(type, look_ahead->lineNo, look_ahead->columnNo);
    } else {
        // printf("Matched. look_ahead is: ");
        print_token(look_ahead);

        Token *temp = current_token;
        current_token = look_ahead;
        look_ahead = next_valid_token();

        free(temp);

        // printf("Next parsed token : ");
        print_token(look_ahead);
    }
}

int parse(char *file_name) {
    if (open_input_stream(file_name) == IO_ERROR)
        return IO_ERROR;

    current_token = NULL;
    look_ahead = next_token();

    init_symbol_table();

    symbolTable->root = parse_program();
    print_entry(symbolTable->root);

    clear_symbol_table();

    free(current_token);
    free(look_ahead);
    close_input_stream();
    return IO_SUCCESS;
}

EntryAST *parse_body_block() {
    EntryAST *bodyAST = NULL;
    EntryNodeAST *statementList = NULL, *declAST = NULL;
    if (look_ahead->type != K_BEGIN) declAST = parse_declaration_list();
    match_token(K_BEGIN);

    if (look_ahead->type != K_END) statementList = parse_statement_list();
    match_token(K_END);

    bodyAST = create_body_block(declAST, statementList);
    return bodyAST;
}

EntryAST *parse_program() {
    assert_parser("Parsing the program");

    EntryAST *programAST = NULL, *bodyAST; // create, enter and exit a program scope

    // program header
    match_token(K_PROGRAM);
    match_token(T_IDENTIFIER);
    match_token(K_IS);

    enter_scope(new_scope());
    bodyAST = parse_body_block(); // program body
    match_token(K_PROGRAM);
    programAST = create_program(current_token->val.stringVal, bodyAST);

    if (look_ahead->type == T_END_OF_FILE) match_token(T_END_OF_FILE);
    exit_scope();

    assert_parser("Done parsing the program");
    return programAST;
}

EntryNodeAST *parse_declaration_list(EntryNodeAST *node) {
    assert_parser("Parsing declarations");
    int isGlobal = 0;

    if (look_ahead->type == K_GLOBAL) {
        match_token(K_GLOBAL);
        isGlobal = 1;
    }
    EntryAST *declAST; // https://stackoverflow.com/questions/22419790/c-error-expected-expression-before-int

    switch (look_ahead->type) {
        case K_PROCEDURE:
            declAST = parse_proc_declaration(isGlobal);
            match_token(T_SEMI_COLON);
            node = create_entry_node(declAST, NULL);
            node->next = parse_declaration_list(node);
            node = node->next;
            break;
        // FOLLOW set
        case K_BEGIN: // from program_body, procedure_body
            break;
        default:
            declAST = parse_var_declaration(isGlobal);
            match_token(T_SEMI_COLON);
            // declare_entry(declAST, isGlobal);
            node = create_entry_node(declAST, NULL);
            node->next = parse_declaration_list(node);
            node = node->next;
            break;
    }
    assert_parser("Done parsing declarations");
    return node;
}

EntryAST *parse_proc_declaration(int isGlobal) {
    assert_parser("Parsing a procedure declaration");
    EntryAST *procAST = NULL;
    // procedure header
    match_token(K_PROCEDURE);
    match_token(T_IDENTIFIER);
    check_new_identifier(current_token->val.stringVal);

    EntryNodeAST *params = parse_param_list();
    EntryNodeAST *stmts = parse_body_block(); // procedure body
    match_token(K_PROCEDURE);

    procAST = create_procedure(current_token->val.stringVal, isGlobal, 0, params, stmts);

    assert_parser("Done parsing a procedure declaration");
    return procAST;
}

EntryAST *parse_var_declaration(int isGlobal) {
    assert_parser("Parsing a variable declaration");

    EntryAST *typeMark = parse_type_mark();
    check_builtin_type(typeMark->typeAST);

    match_token(T_IDENTIFIER);
    check_new_identifier(current_token->val.stringVal);

    EntryAST *varAST = create_variable(current_token->val.stringVal, typeMark, NULL);

    if (look_ahead->type == T_LBRACKET) { // an array
        match_token(T_LBRACKET);
        //TODO: Is there upper and lower bound or just a number for array size?
        // if (look_ahead->type == T_MINUS) match_token(T_MINUS);
        match_token(T_NUMBER_INT); // lower bound

        // match_token(T_COLON);
        // if (look_ahead->type == T_MINUS) match_token(T_MINUS);
        // match_token(T_NUMBER_INT); // uppper bound

        varAST->varAST->size = current_token->val.intVal;
        match_token(T_RBRACKET);
    }

    declare_entry(varAST, isGlobal); // in parse_declaration_list() and parse_param()
    assert_parser("Done parsing a variable declaration");
    return varAST;
}

EntryAST* parse_type_mark() {
    assert_parser("Parsing a type mark");

    EntryAST *typeMark = NULL;
    switch(look_ahead->type) {
        case K_INT:
            match_token(K_INT);
            typeMark = create_type(TC_INT); break;
        case K_FLOAT:
            match_token(K_FLOAT);
            typeMark = create_type(TC_FLOAT); break;
        case K_BOOL:
            match_token(K_BOOL);
            typeMark = create_type(TC_BOOL); break;
        case K_CHAR:
            match_token(K_CHAR);
            typeMark = create_type(TC_CHAR); break;
        case K_STRING:
            match_token(K_STRING);
            typeMark = create_type(TC_STRING); break;
        default:
            throw_error(E_INVALID_TYPE, look_ahead->lineNo, look_ahead->columnNo); break;
    }

    assert_parser("Done parsing a type mark");
    return typeMark;
}

EntryNodeAST *parse_statement_list() {
    EntryAST *statementAST = parse_statement();
    EntryNodeAST *node = NULL;

    while (look_ahead->type == T_SEMI_COLON) {
        match_token(T_SEMI_COLON);
        node = create_entry_node(statementAST, NULL);
        node->next = parse_statement_list();
        node = node->next;

        statementAST = parse_statement();
    }
    if (current_token->type == T_IDENTIFIER)
        throw_error(E_INVALID_STATEMENT, look_ahead->lineNo, look_ahead->columnNo);
    return node;
}

EntryAST *parse_statement() {
    assert_parser("Parsing a statement");
    EntryAST *statementAST = NULL;
    switch (look_ahead->type) {
        case K_IF: parse_if_statement(); break;
        case K_FOR: parse_loop_statement(); break;
        case K_RETURN: parse_return_statement(); break;
        case T_IDENTIFIER:
            match_token(T_IDENTIFIER);
            if (look_ahead->type == T_LPAREN) parse_procedure_call();
            else statementAST = parse_assignment_statement();
        // FOLLOW set
        case K_END: case K_ELSE: case T_SEMI_COLON: // if statement, for loop
            break;
        default: throw_error(E_INVALID_STATEMENT, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    assert_parser("Done parsing a statement");
    return statementAST;
}

// TODO: Fix this!
// EntryAST *parse_indexes(TypeAST *arrayType) {
EntryAST *parse_indexes() {
    // parse a sequence of indexes, check the consistency to the arrayType, and return the element type
    EntryAST *elemType = NULL;

    while (look_ahead->type == T_LBRACKET) {
        match_token(T_LBRACKET);

        elemType = parse_expression();
        check_int_type(elemType->typeAST); // Array indexes must be of type integer.
        // TODO: Bounds checking to insure that the index is within the upper and lower bound is required for all indexed array references
        // check_array_type(arrayType->typeAST); // if current element is not of array type, then the access to the next dimension is invalid
        // arrayType = arrayType->elementType;

        match_token(T_RBRACKET);
    }

    // return arrayType;
    return elemType;
}

EntryAST *parse_destination() {
    assert_parser("Parsing a destination");
    EntryAST *dest;
    dest = check_declared_destination(current_token->val.stringVal);

    if (dest->entryType == ET_VARIABLE) dest = parse_indexes();

    assert_parser("Done parsing a destination");
    return dest;
}

EntryAST *parse_assignment_statement() {
    assert_parser("Parsing an assignment statement");

    EntryAST *destAST = NULL, *expAST = NULL;
    TypeAST *destType, *expType;

    destAST = parse_destination();
    // if (look_ahead->type == T_LPAREN) return; // backtrack to parse procedure call

    match_token(T_ASSIGNMENT);

    expAST = parse_expression();
    destType = destAST->varAST->varType->typeAST;
    expType = expAST->varAST->varType->typeAST;

    if (destType->typeClass == TC_INT) {
        expType->typeClass = TC_BOOL;
        // TODO: call int_to_bool() here
    } else if (destType->typeClass == TC_BOOL){
        expType->typeClass = TC_INT;
        // TODO: call bool_to_int() here
    }
    check_type_equality(destType, expType);

    EntryAST *assigmentAST = create_binary_op(BO_EQ, destAST, expAST);

    assert_parser("Done parsing an assignment statement");
    return assigmentAST;
}

EntryAST *parse_if_statement() {
    assert_parser("Parsing an if statement");
    EntryAST *ifAST = NULL, *condition;
    EntryNodeAST *trueBlock = NULL, *falseBlock = NULL;

    match_token(K_IF);
    match_token(T_LPAREN);

    condition = parse_expression();
    convert_to_bool(condition->typeAST);
    check_bool_type(condition->typeAST);

    match_token(T_RPAREN);
    match_token(K_THEN);
    trueBlock = parse_statement_list();
    if (look_ahead->type == K_ELSE) {
        match_token(K_ELSE);
        falseBlock = parse_statement_list();
    }
    match_token(K_END);
    match_token(K_IF);

    ifAST = create_if(condition, trueBlock, falseBlock);
    assert_parser("Done parsing an if statement");
    return ifAST;
}

EntryAST *parse_loop_statement() {
    assert_parser("Parsing a for loop");
    EntryAST *loop = NULL, *assignment = NULL, *expr = NULL;
    EntryNodeAST *statements = NULL;

    match_token(K_FOR);
    match_token(T_LPAREN);
    assignment = parse_assignment_statement(); // TODO: What to do with this?
    match_token(T_SEMI_COLON);

    expr = parse_expression();
    // if (expType->typeClass == TC_INT) expType->typeClass = TC_BOOL;
    // check_bool_type(expType);

    match_token(T_RPAREN);
    if (look_ahead->type != K_END) {
        statements = parse_statement_list();
    }

    match_token(K_END);
    match_token(K_FOR);
    loop = create_loop(assignment, expr, statements);
    assert_parser("Done parsing a for loop");
    return loop;
}

// TODO: I don't know what to do with thissss
EntryAST *parse_return_statement() {
    match_token(K_RETURN);
    return create_return();
}

EntryAST *parse_procedure_call() {
    assert_parser("Parsing a procedure call");
    EntryAST *procCall = NULL;
    EntryAST *callee = check_declared_procedure(current_token->val.stringVal);
    unsigned int argc = callee->stmtAST->procCallAST->argc;

    match_token(T_LPAREN);
    EntryNodeAST *args = parse_argument_list(callee->procAST->params);
    match_token(T_RPAREN);
    procCall = create_procedure_call(current_token->val.stringVal, args, argc);

    assert_parser("Done parsing a procedure call");
    return procCall;
}

EntryNodeAST *parse_param_list() {
    EntryAST *param = NULL;
    EntryNodeAST *node = NULL;

    if (look_ahead->type == T_LPAREN) {
        match_token(T_LPAREN);
        param = parse_param();
        while (look_ahead->type == T_COMMA) {
            match_token(T_COMMA);
            node = create_entry_node(param, NULL);
            node = node->next;
            param = parse_param();
        }
        match_token(T_RPAREN);
    }
    return node;
}

EntryAST *parse_param() {
    assert_parser("Parsing a parameter");
    EntryAST *paramAST = NULL;
    EntryAST *varAST = parse_var_declaration(0);

    switch (look_ahead->type) {
        case K_IN:
            paramAST = create_param(PT_IN, varAST);
            match_token(K_IN); break;
        case K_OUT:
            paramAST = create_param(PT_OUT, varAST);
            match_token(K_OUT); break;
        case K_INOUT:
            paramAST = create_param(PT_INOUT, varAST);
            match_token(K_INOUT); break;
        default:
            throw_error(E_INVALID_PARAM_TYPE, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    // declare_entry(paramAST, 0);

    assert_parser("Done parsing a parameter");
    return paramAST;
}

EntryNodeAST *parse_argument_list(EntryNodeAST *paramList) {
    assert_parser("Parsing an argument list");

    EntryNodeAST *argNode = NULL, *paramNode = paramList, *dummy = create_entry_node(NULL, NULL); // holds the head of argument list
    dummy->next = argNode;
    if (argNode != NULL) {
        parse_argument(paramNode->entryAST);
        argNode = argNode->next;
    }

    while (look_ahead->type == T_COMMA) {
        match_token(T_COMMA);
        argNode = argNode->next;
        if (argNode != NULL) {
            parse_argument(paramNode->entryAST);
            argNode = argNode->next;
        } else throw_error(E_INCONSISTENT_PARAM_ARGS, look_ahead->lineNo, look_ahead->columnNo);
    }

    // paramList still has another argument but we've done parsing
    // -> number of args doesn't match number of params
    if (argNode != NULL)
        throw_error(E_INCONSISTENT_PARAM_ARGS, look_ahead->lineNo, look_ahead->columnNo);
    assert_parser("Done parsing an argument list");

    return dummy->next;
}

void parse_argument(EntryAST *paramType) {
    EntryAST *argType = parse_expression();
    check_type_equality(paramType->typeAST, argType->typeAST);
}

EntryAST *parse_expression() {
    assert_parser("Parsing an expression");
    if (look_ahead->type == K_NOT) match_token(K_NOT);
    EntryAST *expAST = parse_arith_op();
    expAST = parse_expression_arith_op(expAST);
    assert_parser("Done parsing an expression");
    return expAST;
}

EntryAST *parse_expression_arith_op(EntryAST *expAST) {
    EntryAST *arithOpAST = NULL;
    switch(look_ahead->type) {
        case T_AND:
            match_token(T_AND);
            arithOpAST = parse_arith_op();
            check_int_type(arithOpAST->typeAST);
            expAST = create_binary_op(BO_AND, expAST, arithOpAST);
            expAST = parse_expression_arith_op(expAST);
            break;
        case T_OR:
            match_token(T_OR);
            arithOpAST = parse_arith_op();
            check_int_type(arithOpAST->typeAST);
            expAST = create_binary_op(BO_OR, expAST, arithOpAST);
            expAST = parse_expression_arith_op(expAST);
            break;
        // FOLLOW set
        case T_COMMA: // argument list
        case T_RPAREN: // for loop, if statement
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            break;
        default: throw_error(E_INVALID_EXPRESSION, look_ahead->lineNo, look_ahead->columnNo);
    }
    return expAST;
}

EntryAST *parse_arith_op() {
    assert_parser("Parsing an arithmetic operation");
    EntryAST *arithOpAST = parse_relation();
    check_int_float_type(arithOpAST->typeAST);
    arithOpAST = parse_arith_op_relation(arithOpAST);
    assert_parser("Done parsing an arithmetic operation");
    return arithOpAST;
}

EntryAST *parse_arith_op_relation(EntryAST *arithOpAST) {
    EntryAST *relationAST = NULL;
    switch(look_ahead->type) {
        case T_PLUS:
            match_token(T_PLUS);
            relationAST = parse_relation();
            check_int_float_type(relationAST->typeAST);
            arithOpAST = create_binary_op(BO_PLUS, arithOpAST, relationAST);
            arithOpAST = parse_arith_op_relation(arithOpAST);
            break;
        case T_MINUS:
            match_token(T_MINUS);
            relationAST = parse_relation();
            check_int_float_type(relationAST->typeAST);
            arithOpAST = create_binary_op(BO_MINUS, arithOpAST, relationAST);
            arithOpAST = parse_arith_op_relation(arithOpAST);
            break;
        // FOLLOW set
        case T_AND: case T_OR: case T_COMMA: // expression
        case T_RPAREN: // for loop, if statement
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            break;
        default: throw_error(E_INVALID_ARITH_OPERATOR, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    return arithOpAST;
}

EntryAST *parse_relation() {
    assert_parser("Parsing a relation");
    EntryAST *relationAST = parse_term();
    check_basic_type(relationAST->typeAST);
    relationAST = parse_relation_term(relationAST);
    assert_parser("Done parsing a relation");
    return relationAST;
}

EntryAST *parse_relation_term(EntryAST *relationAST) {
    EntryAST *termAST = NULL;
    switch(look_ahead->type) {
        case T_LT:
            match_token(T_LT);
            termAST = parse_term();
            check_basic_type(termAST->typeAST);
            check_type_equality(relationAST->typeAST, termAST->typeAST);
            relationAST = create_binary_op(BO_LT, relationAST, termAST);
            relationAST = parse_relation_term(relationAST);
            break;
        case T_LTEQ:
            match_token(T_LTEQ);
            termAST = parse_term();
            check_basic_type(termAST->typeAST);
            check_type_equality(relationAST->typeAST, termAST->typeAST);
            relationAST = create_binary_op(BO_LTEQ, relationAST, termAST);
            relationAST = parse_relation_term(relationAST);
            break;
        case T_GT:
            match_token(T_GT);
            termAST = parse_term();
            check_basic_type(termAST->typeAST);
            check_type_equality(relationAST->typeAST, termAST->typeAST);
            relationAST = create_binary_op(BO_GT, relationAST, termAST);
            relationAST = parse_relation_term(relationAST);
            break;
        case T_GTEQ:
            match_token(T_GTEQ);
            termAST = parse_term();
            check_basic_type(termAST->typeAST);
            check_type_equality(relationAST->typeAST, termAST->typeAST);
            relationAST = create_binary_op(BO_GTEQ, relationAST, termAST);
            relationAST = parse_relation_term(relationAST);
            break;
        case T_EQ:
            match_token(T_EQ);
            termAST = parse_term();
            check_basic_type(termAST->typeAST);
            check_type_equality(relationAST->typeAST, termAST->typeAST);
            relationAST = create_binary_op(BO_EQ, relationAST, termAST);
            relationAST = parse_relation_term(relationAST);
            break;
        case T_NEQ:
            match_token(T_NEQ);
            termAST = parse_term();
            check_basic_type(termAST->typeAST);
            check_type_equality(relationAST->typeAST, termAST->typeAST);
            relationAST = create_binary_op(BO_NEQ, relationAST, termAST);
            relationAST = parse_relation_term(relationAST);
            break;
        // FOLLOW set
        case T_PLUS: case T_MINUS: // arith op
        case T_AND: case T_OR: case T_COMMA: // expression
        case T_RPAREN: // for loop, if statement
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            break;
        default: throw_error(E_INVALID_RELATION, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    return relationAST;
}

EntryAST *parse_term() {
    assert_parser("Parsing a term");
    EntryAST* termAST = parse_factor();
    check_int_float_type(termAST->typeAST);
    termAST = parse_term_factor(termAST);
    assert_parser("Done parsing a term");
    return termAST;
}

EntryAST *parse_term_factor(EntryAST *termAST) {
    EntryAST *factorAST = NULL;
    switch(look_ahead->type) {
        case T_MULTIPLY:
            match_token(T_MULTIPLY);
            factorAST = parse_factor();
            check_int_float_type(factorAST->typeAST);
            termAST = create_binary_op(BO_MULTIPLY, termAST, factorAST);
            termAST = parse_term_factor(termAST);
            break;
        case T_DIVIDE:
            match_token(T_DIVIDE);
            factorAST = parse_factor();
            check_int_float_type(factorAST->typeAST);
            termAST = create_binary_op(BO_DIVIDE, termAST, factorAST);
            termAST = parse_term_factor(termAST);
            break;
        // FOLLOW set
        case T_LT: case T_LTEQ: case T_GT: case T_GTEQ: case T_EQ: case T_NEQ: // relation
        case T_PLUS: case T_MINUS: // arith op
        case T_AND: case T_OR: case T_COMMA: // expression
        case T_RPAREN: // for loop, if statement
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            // termAST = factorAST;
            break;
        default: throw_error(E_INVALID_TERM, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    return termAST;
}

EntryAST *parse_factor() {
    assert_parser("Parsing a factor");
    EntryAST *factorAST = NULL;
    switch (look_ahead->type) {
        case T_STRING:
            match_token(T_STRING);
            factorAST = create_factor(TC_STRING, current_token);
            break;
        case T_CHAR:
            match_token(T_CHAR);
            factorAST = create_factor(TC_CHAR, current_token);
            break;
        case T_NUMBER_INT:
            match_token(T_NUMBER_INT);
            factorAST = create_factor(TC_INT, current_token);
            break;
        case T_NUMBER_FLOAT:
            match_token(T_NUMBER_FLOAT);
            factorAST = create_factor(TC_FLOAT, current_token);
            break;
        case T_LPAREN: // ( <expression> )
            match_token(T_LPAREN);
            factorAST = parse_expression(); //
            match_token(T_RPAREN);
            break;
        case T_MINUS: // [-] <name> | [-] <number>
            match_token(T_MINUS);
            assert_parser("A negative number or a negative name");
            factorAST = parse_factor();
            factorAST = create_unary_op(UN_MINUS, factorAST);
            break;
        case T_IDENTIFIER: // <name> ::= <identifier> [ [ <expression> ] ]. Same as <destination> ::= <identifier> [ [ <expression> ] ]?
            match_token(T_IDENTIFIER);
            factorAST = check_declared_identifier(current_token->val.stringVal);
            TypeAST *varType = factorAST->varAST->varType->typeAST;
            if (factorAST->varAST->size > 0) { // an array
                EntryAST *indexExpr = parse_indexes(); // TODO: How do I even represent an array in factor?
            }
            factorAST = create_factor(varType->typeClass, current_token);
            break;
        case K_TRUE:
            match_token(K_TRUE);
            factorAST = create_factor(TC_BOOL, current_token);
            break;
        case K_FALSE:
            match_token(K_FALSE);
            factorAST = create_factor(TC_BOOL, current_token);
            break;
        // FOLLOW set
        case T_AND: case T_OR: case T_COMMA: // expression
        case T_RPAREN: // for loop, if statement
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            break;
        default: throw_error(E_INVALID_FACTOR, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    assert_parser("Done parsing a factor");
    return factorAST;
}

int bool_to_int(bool boolean) {
    if (boolean == true) return 1;
    else return 0;
}

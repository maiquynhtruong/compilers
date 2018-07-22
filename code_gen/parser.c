#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Transforms/Scalar.h>
#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>

#include "error.h"
#include "reader.h"
#include "parser.h"
#include "semantics.h"
#include "scanner.h"
#include "code_gen.h"

Token* look_ahead;
Token* current_token;

extern LLVMModuleRef module;
extern LLVMBuilderRef builder;
extern LLVMExecutionEngineRef engine;
extern LLVMValueRef llvm_printf;
// from symbol_table.c
extern SymbolTable *symbolTable;

void match_token(TokenType type) {

    print_token(look_ahead);
    if (look_ahead->type != type) {
        missing_token(type, look_ahead->lineNo, look_ahead->columnNo);
    } else {
        Token *temp = current_token;
        current_token = look_ahead;
        look_ahead = next_valid_token();

        free(temp);
    }
}

int parse(char *file_name) {
    if (open_input_stream(file_name) == IO_ERROR)
        return IO_ERROR;

    current_token = NULL;
    look_ahead = next_token();

    init_symbol_table();

    parse_program();

    clear_symbol_table();

    free(current_token);
    free(look_ahead);
    close_input_stream();
    return IO_SUCCESS;
}

void parse_body_block() {

    if (look_ahead->type != K_BEGIN) parse_declaration_list();
    match_token(K_BEGIN);

    if (look_ahead->type != K_END) parse_statement_list();
    match_token(K_END);

    LLVMBuildRetVoid(builder);
}

void parse_program() {
    assert_parser("Parsing the program\n");

    EntryAST *program;

    // program header
    match_token(K_PROGRAM);
    match_token(T_IDENTIFIER);

    // program = create_program(current_token->val.stringVal);
    program = create_program("main"); // top level function has to be "main"
    enter_scope(program->progAST->scope);

    match_token(K_IS);

    LLVMTypeRef programType = LLVMFunctionType(LLVMVoidType(), NULL, 0, false);
    LLVMValueRef programValue = LLVMAddFunction(module, program->name, programType);
    LLVMBasicBlockRef entry = LLVMAppendBasicBlock(programValue, strcat(program->name, "_entry"));
    LLVMPositionBuilderAtEnd(builder, entry);

    parse_body_block(); // program body

    match_token(K_PROGRAM);

    if (look_ahead->type == T_END_OF_FILE) match_token(T_END_OF_FILE);
    exit_scope();

    printf("After parsing program: %s\n", LLVMPrintValueToString(programValue));

    assert_parser("Done parsing the program\n");
}

void parse_declaration_list() {
    assert_parser("Parsing declaration list\n");
    int isGlobal = 0;

    if (look_ahead->type == K_GLOBAL) {
        match_token(K_GLOBAL);
        isGlobal = 1;
    }

    switch (look_ahead->type) {
        case K_PROCEDURE:
            parse_proc_declaration(isGlobal);
            match_token(T_SEMI_COLON);
            parse_declaration_list();
            break;
        // FOLLOW set
        case K_BEGIN: // from program_body, procedure_body
            break;
        default:
            parse_var_declaration(isGlobal);
            match_token(T_SEMI_COLON);
            parse_declaration_list();
            // node = node->next;
            break;
    }
    assert_parser("Done parsing declaration list\n");
    // return node;
}

void parse_proc_declaration(int isGlobal) {
    assert_parser("Parsing a procedure declaration\n");

    // procedure header
    match_token(K_PROCEDURE);
    match_token(T_IDENTIFIER);
    check_new_identifier(current_token->val.stringVal);

    parse_param_list();

    parse_body_block(); // procedure body
    match_token(K_PROCEDURE);

    assert_parser("Done parsing a procedure declaration\n");
}

void parse_var_declaration(int isGlobal) {
    assert_parser("Parsing a variable declaration\n");

    int size = 0;
    TypeClass varType = parse_type_mark();
    check_builtin_type(varType);

    match_token(T_IDENTIFIER);
    check_new_identifier(current_token->val.stringVal);
    EntryAST *entry = create_variable(current_token->val.stringVal);
    entry->typeAST = create_type(varType);

    if (look_ahead->type == T_LBRACKET) { // an array
        match_token(T_LBRACKET);

        match_token(T_NUMBER_INT); // lower bound

        size = current_token->val.intVal;
        match_token(T_RBRACKET);
    }

    declare_entry(entry, isGlobal); // in parse_declaration_list() and parse_param()

    assert_parser("Done parsing a variable declaration\n");
}

TypeClass parse_type_mark() {
    assert_parser("Parsing a type mark\n");

    TypeClass typeMark = TC_INVALID;
    switch(look_ahead->type) {
        case K_INT:
            match_token(K_INT);
            typeMark = TC_INT;
            break;
        case K_FLOAT:
            match_token(K_FLOAT);
            typeMark = TC_FLOAT;
            break;
        case K_BOOL:
            match_token(K_BOOL);
            typeMark = TC_BOOL;
            break;
        case K_CHAR:
            match_token(K_CHAR);
            typeMark = TC_CHAR;
            break;
        case K_STRING:
            match_token(K_STRING);
            typeMark = TC_STRING;
            break;
        default:
            throw_error(E_INVALID_TYPE, look_ahead->lineNo, look_ahead->columnNo); break;
    }

    assert_parser("Done parsing a type mark\n");
    assert_parser("TypeMark type is: "); print_type(typeMark); assert_parser("\n");
    return typeMark;
}

void parse_param_list() {
    if (look_ahead->type == T_LPAREN) {
        match_token(T_LPAREN);

        if (look_ahead->type != T_RPAREN) {
            parse_param();
            while (look_ahead->type == T_COMMA) {
                match_token(T_COMMA);
                parse_param();
            }
        }

        match_token(T_RPAREN);
    }
}

void parse_param() {
    assert_parser("Parsing a parameter\n");

    parse_var_declaration(0);
    ParamType paramType = PT_IN;
    switch (look_ahead->type) {
        case K_IN:
            // paramAST = create_param(PT_IN, varAST);
            match_token(K_IN);
            paramType = PT_IN;
            break;
        case K_OUT:
            // paramAST = create_param(PT_OUT, varAST);
            match_token(K_OUT);
            paramType = PT_OUT;
            break;
        case K_INOUT:
            // paramAST = create_param(PT_INOUT, varAST);
            match_token(K_INOUT);
            paramType = PT_INOUT;
            break;
        default:
            throw_error(E_INVALID_PARAM_TYPE, look_ahead->lineNo, look_ahead->columnNo); break;
    }

    assert_parser("Done parsing a parameter\n");
}

void parse_statement_list() {
    parse_statement();

    while (look_ahead->type == T_SEMI_COLON) {
        match_token(T_SEMI_COLON);
        parse_statement_list();
    }
}

void parse_statement() {
    assert_parser("Parsing a statement\n");

    switch (look_ahead->type) {
        case K_IF: parse_if_statement(); break;
        case K_FOR: parse_loop_statement(); break;
        case K_RETURN: parse_return_statement(); break;
        case T_IDENTIFIER:
            match_token(T_IDENTIFIER);
            if (look_ahead->type == T_LPAREN) parse_procedure_call();
            else parse_assignment_statement();
        // FOLLOW set
        case K_END: case K_ELSE: case T_SEMI_COLON: // if statement, for loop
            break;
        default: throw_error(E_INVALID_STATEMENT, look_ahead->lineNo, look_ahead->columnNo); break;
    }

    assert_parser("Done parsing a statement\n");
}

void parse_indexes() {
    TypeAST *elemType = NULL;

    while (look_ahead->type == T_LBRACKET) {
        match_token(T_LBRACKET);

        elemType = parse_expression();
        check_int_type(elemType->typeClass); // Array indexes must be of type integer.

        match_token(T_RBRACKET);
    }
}

TypeAST *parse_destination() {
    assert_parser("Parsing a destination\n");
    EntryAST *dest;
    dest = check_declared_identifier(current_token->val.stringVal);

    parse_indexes();

    TypeAST *destType = dest->typeAST;

    assert_parser("Done parsing a destination\n");
    assert_parser("Destination type is: "); print_type(destType->typeClass); assert_parser("\n");
    return destType;
}

void parse_assignment_statement() {
    assert_parser("Parsing an assignment statement\n");

    TypeAST *destType = NULL;
    TypeAST *expType = NULL;

    destType = parse_destination();

    match_token(T_ASSIGNMENT);
    expType = parse_expression();
    check_type_equality(destType->typeClass, expType->typeClass);

    printf("Dest value is: %s, Exp type is: %s\n", LLVMPrintValueToString(destType->valueRef), LLVMPrintValueToString(expType->valueRef));
    LLVMBuildStore(builder, expType->valueRef, destType->valueRef);

    assert_parser("Done parsing an assignment statement\n");
}

void parse_if_statement() {
    assert_parser("Parsing an if statement\n");
    TypeAST *condition = NULL;
    LLVMBasicBlockRef thenBlock = NULL, elseBlock = NULL, mergeBlock = NULL;
    LLVMValueRef conditionValue = NULL;

    match_token(K_IF);
    match_token(T_LPAREN);

    // LLVMValueRef scope = LLVMGetNamedFunction(module, symbolTable->currentScope->name);
    LLVMValueRef scope = LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder));

    thenBlock = LLVMAppendBasicBlock(scope, "then");
    elseBlock = LLVMAppendBasicBlock(scope, "else");
    mergeBlock = LLVMAppendBasicBlock(scope, "merge");

    condition = parse_expression();
    convert_to_bool(&condition);
    conditionValue = condition->valueRef;

    match_token(T_RPAREN);
    LLVMBuildCondBr(builder, conditionValue, thenBlock, elseBlock);

    match_token(K_THEN);
    LLVMPositionBuilderAtEnd(builder, thenBlock);
    parse_statement_list();
    LLVMBuildBr(builder, mergeBlock);

    LLVMPositionBuilderAtEnd(builder, elseBlock);
    if (look_ahead->type == K_ELSE) {
        match_token(K_ELSE);
        parse_statement_list();
    }
    LLVMBuildBr(builder, mergeBlock);

    match_token(K_END);
    match_token(K_IF);
    LLVMPositionBuilderAtEnd(builder, mergeBlock);

    assert_parser("Done parsing an if statement\n");
}

void parse_loop_statement() {
    assert_parser("Parsing a for loop\n");
    TypeAST *expType = NULL;
    LLVMBasicBlockRef startBlock = NULL, loopBlock = NULL, endBlock = NULL;
    LLVMValueRef conditionValue = NULL;

    match_token(K_FOR);
    match_token(T_LPAREN);
    match_token(T_IDENTIFIER);

    LLVMValueRef scope = LLVMGetNamedFunction(module, symbolTable->currentScope->name);
    startBlock = LLVMAppendBasicBlock(scope, "start_loop");
    loopBlock = LLVMAppendBasicBlock(scope, "loop");
    endBlock = LLVMAppendBasicBlock(scope, "end_loop");

    LLVMBuildBr(builder, startBlock);
    LLVMPositionBuilderAtEnd(builder, startBlock);

    parse_assignment_statement();
    match_token(T_SEMI_COLON);

    expType = parse_expression();
    convert_to_bool(&expType);
    conditionValue = expType->valueRef;

    match_token(T_RPAREN);
    LLVMBuildCondBr(builder, conditionValue, loopBlock, endBlock);

    LLVMPositionBuilderAtEnd(builder, loopBlock);
    if (look_ahead->type != K_END) {
        parse_statement_list();
        LLVMBuildBr(builder, startBlock);
    }

    match_token(K_END);
    match_token(K_FOR);

    LLVMPositionBuilderAtEnd(builder, endBlock);

    assert_parser("Done parsing a for loop\n");
}

void parse_return_statement() {
    match_token(K_RETURN);
    LLVMBuildRetVoid(builder);
}

void parse_procedure_call() {
    assert_parser("Parsing a procedure call\n");

    char *name = current_token->val.stringVal;
    EntryAST *entry = check_declared_procedure(name);

    match_token(T_LPAREN);
    LLVMValueRef *args = parse_argument_list(entry);
    match_token(T_RPAREN);

    assert_parser("Done parsing a procedure call\n");
    codegen_proc_call(name, args, entry->procAST->paramCnt);
}

LLVMValueRef *parse_argument_list(EntryAST *proc) {
    assert_parser("Parsing an argument list\n");

    EntryNodeAST *node = proc->procAST->params;
    if (node == NULL) throw_error(E_INCONSISTENT_PARAM_ARGS, look_ahead->lineNo, look_ahead->columnNo);

    int argi = 0, argc = proc->procAST->paramCnt;
    LLVMValueRef *args = (LLVMValueRef *) malloc(sizeof(LLVMValueRef) * argc);
    // LLVMValueRef args[] = LLVMValueRef[argc];
    LLVMValueRef arg = parse_argument(node->entryAST);
    args[argi++] = arg;
    node = node->next;

    while (look_ahead->type == T_COMMA) {
        match_token(T_COMMA);
        if (node == NULL) throw_error(E_INCONSISTENT_PARAM_ARGS, look_ahead->lineNo, look_ahead->columnNo);

        arg = parse_argument(node->entryAST);
        node = node->next;

        if (argi < argc) args[argi++] = arg;
        else throw_error(E_INCONSISTENT_PARAM_ARGS, look_ahead->lineNo, look_ahead->columnNo);
    }

    // paramList still has another argument but we've done parsing
    // or number of args doesn't match number of params
    if (node != NULL)
        throw_error(E_INCONSISTENT_PARAM_ARGS, look_ahead->lineNo, look_ahead->columnNo);

    assert_parser("Done parsing an argument list\n");

    return args;
}

LLVMValueRef parse_argument(EntryAST *param) {
    TypeAST *argType = parse_expression();
    check_type_equality(param->typeAST->typeClass, argType->typeClass);
    return argType->valueRef;
}

TypeAST *parse_expression() {
    assert_parser("Parsing an expression\n");
    if (look_ahead->type == K_NOT) match_token(K_NOT);

    TypeAST *expType = parse_arith_op();
    expType = parse_expression_arith_op(expType);
    assert_parser("Done parsing an expression\n");
    assert_parser("Expression type is: "); print_type(expType->typeClass); assert_parser("\n");
    return expType;
}

TypeAST *parse_expression_arith_op(TypeAST *arithOpType1) {
    TypeAST *arithOpType2 = NULL;
    TypeAST *exprType = NULL;

    switch(look_ahead->type) {
        case T_AND:
            match_token(T_AND);
            check_int_type(arithOpType1->typeClass);
            arithOpType2 = parse_arith_op();
            check_int_type(arithOpType2->typeClass);

            // generate expression
            // arithOpType1 = arithOp result
            // convert_to_bool(&arithOpType1);

            exprType = parse_expression_arith_op(arithOpType1);
            break;
        case T_OR:
            match_token(T_OR);
            check_int_type(arithOpType1->typeClass);
            arithOpType2 = parse_arith_op();
            check_int_type(arithOpType2->typeClass);

            // generate expression
            // arithOpType1 = arithOp result
            // convert_to_bool(&arithOpType1);

            exprType = parse_expression_arith_op(arithOpType1);
            break;
        // FOLLOW set
        case T_COMMA: // argument list
        case T_RPAREN: // for loop, if statement
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            exprType = arithOpType1;
            break;
        default: throw_error(E_INVALID_EXPRESSION, look_ahead->lineNo, look_ahead->columnNo);
    }
    return exprType;
}

TypeAST *parse_arith_op() {
    assert_parser("Parsing an arithmetic operation\n");
    TypeAST *arithOpType = parse_relation();
    // check_int_float_type(arithOpAST->typeAST);
    arithOpType = parse_arith_op_relation(arithOpType);
    assert_parser("Done parsing an arithmetic operation\n");
    assert_parser("Arithmetic operation type is: "); print_type(arithOpType->typeClass); assert_parser("\n");
    return arithOpType;
}

TypeAST *parse_arith_op_relation(TypeAST *relationType1) {
    // EntryAST *relationAST = NULL;
    TypeAST *relationType2 = NULL;
    TypeAST *arithOpType = NULL;
    switch(look_ahead->type) {
        case T_PLUS:
            match_token(T_PLUS);
            check_int_float_type(relationType1->typeClass);
            relationType2 = parse_relation();
            check_int_float_type(relationType2->typeClass);

            // generate arithOp
            // relationType1 = arithOp result
            // convert_to_bool(&relationType1);

            arithOpType = parse_arith_op_relation(relationType1);
            break;
        case T_MINUS:
            match_token(T_MINUS);
            check_int_float_type(relationType1->typeClass);
            relationType1 = parse_relation();
            check_int_float_type(relationType2->typeClass);

            // generate arithOp
            // relationType1 = arithOp result
            // convert_to_bool(&relationType1);

            arithOpType = parse_arith_op_relation(relationType1);
            break;
        // FOLLOW set
        case T_AND: case T_OR: case T_COMMA: // expression
        case T_RPAREN: // for loop, if statement
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            arithOpType = relationType1;
            break;
        default: throw_error(E_INVALID_ARITH_OPERATOR, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    return arithOpType;
}

TypeAST *parse_relation() {
    assert_parser("Parsing a relation\n");
    TypeAST *relationType = parse_term();
    check_basic_type(relationType->typeClass);
    relationType = parse_relation_term(relationType);
    assert_parser("Done parsing a relation\n");
    assert_parser("Relation type is: "); print_type(relationType->typeClass); assert_parser("\n");
    return relationType;
}

TypeAST *parse_relation_term(TypeAST *termType1) {
    TypeAST *termType2 = NULL;
    TypeAST *relationType = NULL;

    TokenType binOp = look_ahead->type;
    switch(binOp) {
        case T_LT:
            match_token(T_LT);
            check_int_float_type(termType1->typeClass);
            termType2 = parse_term();
            check_type_equality(termType1->typeClass, termType2->typeClass);

            // generate binOp
            // termType1 = binOp result
            convert_to_bool(&termType1);

            relationType = parse_relation_term(termType1);
            break;
        case T_LTEQ:
            match_token(T_LTEQ);
            check_int_float_type(termType1->typeClass);
            termType2 = parse_term();
            check_type_equality(termType1->typeClass, termType2->typeClass);

            // generate binOp
            // termType1 = binOp result
            convert_to_bool(&termType1);

            relationType = parse_relation_term(termType1);
            break;
        case T_GT:
            match_token(T_GT);
            check_int_float_type(termType1->typeClass);
            termType2 = parse_term();
            check_type_equality(termType1->typeClass, termType2->typeClass);

            // generate binOp
            // termType1 = binOp result
            convert_to_bool(&termType1);

            relationType = parse_relation_term(termType1);
            break;
        case T_GTEQ:
            match_token(T_GTEQ);
            check_int_float_type(termType1->typeClass);
            termType2 = parse_term();
            check_type_equality(termType1->typeClass, termType2->typeClass);

            // generate binOp
            // termType1 = binOp result
            convert_to_bool(&termType1);

            relationType = parse_relation_term(termType1);
            break;
        case T_EQ:
            match_token(T_EQ);
            check_int_float_type(termType1->typeClass);
            termType2 = parse_term();
            check_type_equality(termType1->typeClass, termType2->typeClass);

            // generate binOp
            // termType1 = binOp result
            convert_to_bool(&termType1);

            relationType = parse_relation_term(termType1);
            break;
        case T_NEQ:
            match_token(T_NEQ);
            check_int_float_type(termType1->typeClass);
            termType2 = parse_term();
            check_type_equality(termType1->typeClass, termType2->typeClass);

            // generate binOp
            // termType1 = binOp result
            convert_to_bool(&termType1);

            relationType = parse_relation_term(termType1);
            break;
        // FOLLOW set
        case T_PLUS: case T_MINUS: // arith op
        case T_AND: case T_OR: case T_COMMA: // expression
        case T_RPAREN: // for loop, if statement
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            relationType = termType1;
            break;
        default: throw_error(E_INVALID_RELATION, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    return termType1;
}

TypeAST *parse_term() {
    assert_parser("Parsing a term\n");
    TypeAST *termType = parse_factor();
    termType = parse_term_factor(termType);
    assert_parser("Done parsing a term\n");
    assert_parser("Term type is: "); print_type(termType->typeClass); assert_parser("\n");
    return termType;
}

TypeAST *parse_term_factor(TypeAST *factorType1) {
    TypeAST *termType = NULL;
    TypeAST *factorType2 = NULL;
    switch(look_ahead->type) {
        case T_MULTIPLY:
            match_token(T_MULTIPLY);
            convert_to_int(&factorType1);
            check_int_float_type(factorType1->typeClass);

            factorType2 = parse_factor();
            convert_to_int(&factorType2);
            check_int_float_type(factorType2->typeClass);

            // generate multiply result

            termType = parse_term_factor(factorType1);
            break;
        case T_DIVIDE:
            match_token(T_DIVIDE);
            convert_to_int(&factorType1);
            check_int_float_type(factorType1->typeClass);

            factorType2 = parse_factor();
            convert_to_int(&factorType2);
            check_int_float_type(factorType2->typeClass);

            // generate divide result

            termType = parse_term_factor(factorType1);
            break;
        // FOLLOW set
        case T_LT: case T_LTEQ: case T_GT: case T_GTEQ: case T_EQ: case T_NEQ: // relation
        case T_PLUS: case T_MINUS: // arith op
        case T_AND: case T_OR: case T_COMMA: // expression
        case T_RPAREN: // for loop, if statement
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            // termAST = factorAST;
            termType = factorType1;
            break;
        default: throw_error(E_INVALID_TERM, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    return termType;
}

TypeAST *parse_factor() {
    assert_parser("Parsing a factor\n");
    EntryAST *factorAST = NULL;
    TypeAST *typeAST = NULL;
    TypeClass factorType = TC_INVALID;
    LLVMValueRef value = NULL;
    switch (look_ahead->type) {
        case T_STRING:
            match_token(T_STRING);
            factorType = TC_STRING;
            value = LLVMBuildGlobalStringPtr(builder, current_token->val.stringVal, "string");
            break;
        case T_CHAR:
            match_token(T_CHAR);
            factorType = TC_CHAR;
            value = LLVMConstInt(LLVMInt8Type(), current_token->val.charVal, 1);
            break;
        case T_NUMBER_INT:
            match_token(T_NUMBER_INT);
            factorType = TC_INT;
            value = LLVMConstInt(LLVMInt32Type(), current_token->val.intVal, 1);
            break;
        case T_NUMBER_FLOAT:
            match_token(T_NUMBER_FLOAT);
            factorType = TC_FLOAT;
            value = LLVMConstReal(LLVMDoubleType(), current_token->val.floatVal);
            break;
        case K_TRUE:
            match_token(K_TRUE);
            value = LLVMConstInt(LLVMInt32Type(), 1, 1); // a bool is an 8 byte integer, but I see no point keeping the difference
            factorType = TC_BOOL;
            break;
        case K_FALSE:
            match_token(K_FALSE);
            value = LLVMConstInt(LLVMInt32Type(), 0, 1);
            factorType = TC_BOOL;
            break;
        case T_LPAREN: // ( <expression> )
            match_token(T_LPAREN);
            typeAST = parse_expression();
            factorType = typeAST->typeClass;
            match_token(T_RPAREN);
            break;
        case T_MINUS: // [-] <name> | [-] <number>
            match_token(T_MINUS);
            // factorAST = parse_factor();
            // if (factorType == TC_INT) current_token->val.intVal = -look_ahead->val.intVal;
            // else if (factorType == TC_FLOAT) current_token->val.floatVal = -look_ahead->val.floatVal;
            break;
        case T_IDENTIFIER: // <name> ::= <identifier> [ [ <expression> ] ]
            match_token(T_IDENTIFIER);
            factorAST = check_declared_identifier(current_token->val.stringVal);
            factorType = factorAST->typeAST->typeClass;
            if (factorAST->varAST->size > 0) { // an array
                parse_indexes(); // TODO: How do I even represent an array in factor?
            }
            value = LLVMBuildLoad(builder, factorAST->typeAST->valueRef, factorAST->name);
            break;
        // FOLLOW set
        case T_AND: case T_OR: case T_COMMA: // expression
        case T_RPAREN: // for loop, if statement
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            break;
        default: throw_error(E_INVALID_FACTOR, look_ahead->lineNo, look_ahead->columnNo); break;
    }
    assert_parser("Done parsing a factor\n");
    assert_parser("Factor type is: "); print_type(factorType); assert_parser("\n");

    typeAST = create_type(factorType);
    typeAST->valueRef = value;

    printf("Factor type: %s, value: %s\n", LLVMPrintTypeToString(typeAST->typeRef), LLVMPrintValueToString(typeAST->valueRef));
    return typeAST;
}

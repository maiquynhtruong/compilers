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
LLVMValueRef mainFunc;

// from symbol_table.c
extern SymbolTable *symbolTable;

void match_token(TokenType type) {
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

    LLVMTypeRef mainType = LLVMFunctionType(LLVMVoidType(), NULL, 0, false);
    mainFunc = LLVMAddFunction(module, "main", mainType);
    LLVMBasicBlockRef mainEntry = LLVMAppendBasicBlock(mainFunc, "main_entry");
    LLVMPositionBuilderAtEnd(builder, mainEntry);

    init_symbol_table();

    parse_program();

    assert_parser("After parsing program: "); assert_parser(LLVMPrintValueToString(mainFunc)); assert_parser("\n");

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
}

void parse_program() {
    assert_parser("Parsing the program\n");

    EntryAST *program;

    // program header
    match_token(K_PROGRAM);
    match_token(T_IDENTIFIER);

    program = create_program("main"); // top level function has to be "main"
    // program = create_program(current_token->val.stringVal); // this won't run

    enter_scope(program->progAST->scope);

        match_token(K_IS);

        parse_body_block(); // program body

        match_token(K_PROGRAM);

        if (look_ahead->type == T_END_OF_FILE) match_token(T_END_OF_FILE);

    exit_scope();
    LLVMBuildRetVoid(builder);

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
            break;
    }
    assert_parser("Done parsing declaration list\n");
}

void parse_proc_declaration(int isGlobal) {
    assert_parser("Parsing a procedure declaration\n");

    // procedure header
    match_token(K_PROCEDURE);
    match_token(T_IDENTIFIER);
    check_new_identifier(current_token->val.stringVal);

    EntryAST *proc = create_procedure(current_token->val.stringVal);
    declare_entry(proc, isGlobal);
    enter_scope(proc->procAST->scope);

        match_token(T_LPAREN);

        if (look_ahead->type != T_RPAREN) parse_param_list(&proc);
        match_token(T_RPAREN);

        LLVMTypeRef procType = LLVMFunctionType(LLVMVoidType(), proc->procAST->paramTypes, proc->procAST->paramc, false);
        LLVMValueRef procValue = LLVMAddFunction(module, proc->name, procType);
        LLVMBasicBlockRef entry = LLVMAppendBasicBlock(procValue, proc->name);
        LLVMPositionBuilderAtEnd(builder, entry);

        EntryNodeAST *node = proc->procAST->params;
        LLVMValueRef curParam = LLVMGetFirstParam(procValue);
        while (node != NULL) {
            EntryAST *entry = node->entryAST;

            if (entry->typeAST->typeClass == TC_STRING) {
                // entry->typeAST->address = LLVMBuildAlloca(builder, LLVMPointerType(LLVMInt8Type(), 0), entry->name);
                entry->typeAST->address = LLVMBuildAlloca(builder, LLVMInt8Type(), entry->name);
			} else if (entry->typeAST->sizeRef != NULL) {
                entry->typeAST->address = LLVMBuildAlloca(builder, entry->typeAST->typeRef, entry->name);
                entry->typeAST->typeRef = LLVMPointerType(entry->typeAST->typeRef, 0);
			} else {
				entry->typeAST->address = LLVMBuildAlloca(builder, entry->typeAST->typeRef, entry->name);
			}
            LLVMBuildStore(builder, curParam, entry->typeAST->address);
            curParam = LLVMGetNextParam(curParam);
            node = node->next;
        }

        parse_body_block(); // procedure body
        match_token(K_PROCEDURE);
        LLVMBuildRetVoid(builder);

    exit_scope();
    proc->typeAST->valueRef = procValue;

    assert_parser("After parsing procedure: "); assert_parser(LLVMPrintValueToString(procValue)); assert_parser("\n");

    LLVMPositionBuilderAtEnd(builder, LLVMGetLastBasicBlock(mainFunc));

    assert_parser("Done parsing a procedure declaration\n");
}

void parse_var_declaration(int isGlobal) {
    assert_parser("Parsing a variable declaration\n");

    TypeClass varType = parse_type_mark();
    check_builtin_type(varType);
    match_token(T_IDENTIFIER);
    check_new_identifier(current_token->val.stringVal);

    EntryAST *entry = create_variable(current_token->val.stringVal);
    entry->typeAST = create_type(varType);

    LLVMValueRef arraySize = parse_indexes();
    if (arraySize != NULL) entry->typeAST->sizeRef = arraySize;
    declare_entry(entry, isGlobal);

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
    assert_parser("TypeMark type is: "); assert_parser(print_type(typeMark)); assert_parser("\n");
    return typeMark;
}

void parse_param_list(EntryAST **proc) {
    assert_parser("Parsing a parameter list\n");
    EntryNodeAST *paramList = (EntryNodeAST *) malloc(sizeof(EntryNodeAST));

    EntryNodeAST *param = paramList;
    param->entryAST = parse_param();
    param->next = NULL;
    (*proc)->procAST->paramc++;

    while (look_ahead->type == T_COMMA) {
        match_token(T_COMMA);
        param->next = (EntryNodeAST *) malloc(sizeof(EntryNodeAST));
        param = param->next;
        param->entryAST = parse_param();
        param->next = NULL;
        (*proc)->procAST->paramc++;
    }

    int parami = 0;
    LLVMTypeRef *paramTypes = (LLVMTypeRef *) malloc(sizeof(LLVMTypeRef) * (*proc)->procAST->paramc);
    param = paramList;
    while (param != NULL) {
        paramTypes[parami++] = param->entryAST->typeAST->typeRef;
        assert_parser("Current param parsed: "); assert_parser(param->entryAST->name); assert_parser("\n");
        param = param->next;
    }
    (*proc)->procAST->params = paramList;
    (*proc)->procAST->paramTypes = paramTypes;

    assert_parser("Done parsing a parameter list\n");

}

EntryAST *parse_param() {
    assert_parser("Parsing a parameter\n");

    TypeClass typeClass = parse_type_mark();
    check_builtin_type(typeClass);

    match_token(T_IDENTIFIER);
    char *name = current_token->val.stringVal;
    check_new_identifier(name);

    LLVMValueRef sizeRef = parse_indexes();
    ParamType paramType = PT_IN;
    switch (look_ahead->type) {
        case K_IN:
            match_token(K_IN);
            paramType = PT_IN;
            break;
        case K_OUT:
            match_token(K_OUT);
            paramType = PT_OUT;
            break;
        case K_INOUT:
            match_token(K_INOUT);
            paramType = PT_INOUT;
            break;
        default:
            throw_error(E_INVALID_PARAM_TYPE, look_ahead->lineNo, look_ahead->columnNo); break;
    }

    EntryAST *param = create_param(name);
    param->typeAST = create_type(typeClass);
    param->typeAST->paramType = paramType;

    if (param->typeAST->typeClass == TC_STRING) {
        param->typeAST->typeRef = LLVMPointerType(LLVMInt8Type(), 0);
    } else if (sizeRef != NULL) {
        param->typeAST->sizeRef = sizeRef;
        param->typeAST->typeRef = LLVMPointerType(param->typeAST->typeRef, 0);
    }

    declare_entry(param, 0);

    assert_parser("Done parsing a parameter\n");
    return param;
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

LLVMValueRef parse_indexes() {
    LLVMValueRef indexRef = NULL;
    if (look_ahead->type == T_LBRACKET) {
        match_token(T_LBRACKET);

        TypeAST *elemType = parse_expression();
        check_int_type(elemType->typeClass); // Array indexes must be of type integer.
        indexRef = elemType->valueRef;

        match_token(T_RBRACKET);
    }

    return indexRef;
}

TypeAST *parse_destination() {
    assert_parser("Parsing a destination\n");
    EntryAST *dest = check_declared_identifier(current_token->val.stringVal);

    LLVMValueRef indexValue = parse_indexes();
    TypeAST *destType = create_type(dest->typeAST->typeClass);
    destType->typeRef = dest->typeAST->typeRef;
    destType->address = dest->typeAST->address;

    if (dest->typeAST->sizeRef != NULL) { // an array
        if (indexValue == NULL) indexValue = LLVMConstInt(LLVMInt32Type(), 0, false);
        if (LLVMGetTypeKind(dest->typeAST->typeRef) == LLVMArrayTypeKind) {
            LLVMValueRef indices[] = { LLVMConstInt(LLVMInt32Type(), 0, false), indexValue };
            destType->address = LLVMBuildInBoundsGEP(builder, dest->typeAST->address, indices, 2, "dest_array_GEP");
        } else {
            destType->address = LLVMBuildLoad(builder, destType->address, "dest_pointer_load");
            LLVMValueRef indices[] = { indexValue };
            destType->address = LLVMBuildInBoundsGEP(builder, destType->address, indices, 1, "dest_pointer_GEP");
        }
        destType->typeRef = LLVMGetElementType(destType->typeRef);
    }
    destType->valueRef = destType->address; // we don't really care about value for destination

    assert_parser("Done parsing a destination\n");
    assert_parser("Destination type is: "); assert_parser(print_type(destType->typeClass)); assert_parser("\n");
    return destType;
}

void parse_assignment_statement() {
    assert_parser("Parsing an assignment statement\n");

    TypeAST *destType = NULL;
    TypeAST *expType = NULL;

    destType = parse_destination();

    match_token(T_ASSIGNMENT);
    expType = parse_expression();

    // casting
    if (destType->typeClass == TC_INT) {
        expType->typeClass = TC_INT; // cast bool to int
        expType->valueRef = LLVMBuildZExtOrBitCast(builder, expType->valueRef, LLVMInt32Type(), "to-int");
    }
    if (destType->typeClass == TC_BOOL) {
        expType->typeClass = TC_BOOL; // cast int to bool
        expType->valueRef = LLVMBuildZExtOrBitCast(builder, expType->valueRef, LLVMInt8Type(), "to-bool");
    }
    if (destType->typeClass == TC_FLOAT) {
        expType->typeClass = TC_FLOAT;
        expType->valueRef = LLVMBuildZExtOrBitCast(builder, expType->valueRef, LLVMFloatType(), "to-float");
    }
    check_type_equality(destType->typeClass, expType->typeClass);
    printf("exp type is: %s, exp type is: %s, exp address is: %s\n", LLVMPrintTypeToString(expType->typeRef), LLVMPrintValueToString(expType->valueRef), LLVMPrintValueToString(expType->address));
    printf("Dest value is: %s, dest value is: %s, dest address is: %s\n", LLVMPrintTypeToString(destType->typeRef), LLVMPrintValueToString(destType->valueRef), LLVMPrintValueToString(destType->address));
    LLVMBuildStore(builder, expType->valueRef, destType->address);

    assert_parser("Done parsing an assignment statement\n");
}

void parse_if_statement() {
    assert_parser("Parsing an if statement\n");
    TypeAST *condition = NULL;
    LLVMBasicBlockRef thenBlock = NULL, elseBlock = NULL, mergeBlock = NULL;
    LLVMValueRef conditionValue = NULL;

    match_token(K_IF);
    match_token(T_LPAREN);

    LLVMValueRef scope = LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder));

    thenBlock = LLVMAppendBasicBlock(scope, "then");
    elseBlock = LLVMAppendBasicBlock(scope, "else");
    mergeBlock = LLVMAppendBasicBlock(scope, "merge");

    condition = parse_expression();
    // cast expression to bool for evaluation
    if (condition->typeClass == TC_INT) condition->typeClass = TC_BOOL;
    condition->valueRef = LLVMBuildICmp(builder, LLVMIntNE, condition->valueRef, LLVMConstInt(LLVMInt32Type(), 0, 0), "exp != 0?");
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

    // cast expression to bool for evaluation
    if (LLVMIsConstant(expType->valueRef)) {
        if (expType->typeClass == TC_INT) expType->typeClass = TC_BOOL;
        expType->valueRef = LLVMBuildICmp(builder, LLVMIntNE, expType->valueRef, LLVMConstInt(LLVMInt32Type(), 0, 0), "exp != 0?");
    }
    conditionValue = expType->valueRef;

    match_token(T_RPAREN);
    LLVMBuildCondBr(builder, conditionValue, loopBlock, endBlock);

    LLVMPositionBuilderAtEnd(builder, loopBlock);
    if (look_ahead->type != K_END) {
        parse_statement_list();
        LLVMBuildBr(builder, startBlock);
    }

    LLVMPositionBuilderAtEnd(builder, endBlock);

    match_token(K_END);
    match_token(K_FOR);


    assert_parser("Done parsing a for loop\n");
}

void parse_return_statement() {
    match_token(K_RETURN);
    LLVMBuildRetVoid(builder);
}

void parse_procedure_call() {
    assert_parser("Parsing a procedure call\n");

    EntryAST *entry = check_declared_procedure(current_token->val.stringVal);

    match_token(T_LPAREN);
    LLVMValueRef *args = parse_argument_list(entry);
    match_token(T_RPAREN);

    assert_parser("Done parsing a procedure call\n");
    LLVMValueRef proc = LLVMGetNamedFunction(module, entry->name);
    printf("Param passed to build call is %s\n", LLVMPrintValueToString(args[0]));
    LLVMBuildCall(builder, proc, args, entry->procAST->paramc, "");
}

LLVMValueRef *parse_argument_list(EntryAST *proc) {
    assert_parser("Parsing an argument list\n");

    EntryNodeAST *params = proc->procAST->params;
    if (params == NULL) throw_error(E_INCONSISTENT_PARAM_ARGS, look_ahead->lineNo, look_ahead->columnNo);

    EntryNodeAST *node = params;
    int argi = 0, argc = proc->procAST->paramc;
    LLVMValueRef *args = (LLVMValueRef *) malloc(sizeof(LLVMValueRef) * argc);
    args[argi++] = parse_argument(node->entryAST->typeAST);
    node = node->next;

    while (look_ahead->type == T_COMMA) {
        match_token(T_COMMA);
        if (node == NULL) throw_error(E_INCONSISTENT_PARAM_ARGS, look_ahead->lineNo, look_ahead->columnNo);

        args[argi++] = parse_argument(node->entryAST->typeAST);
        node = node->next;

        if (argi > argc) throw_error(E_INCONSISTENT_PARAM_ARGS, look_ahead->lineNo, look_ahead->columnNo);
    }

    if (argi < argc) throw_error(E_INCONSISTENT_PARAM_ARGS, look_ahead->lineNo, look_ahead->columnNo);

    assert_parser("Done parsing an argument list\n");

    return args;
}

LLVMValueRef parse_argument(TypeAST *paramType) {
    TypeAST *argType = parse_expression();
    assert_parser("Checking argument type equality\n");
    check_type_equality(paramType->typeClass, argType->typeClass);

    if (paramType->paramType == PT_OUT) {
        if (argType->typeClass != TC_STRING) {
            argType->valueRef = argType->address; // the pointer because what the pointer points to isn't initialized, yet.
        }
    }
    return argType->valueRef;
}

TypeAST *parse_expression() {
    assert_parser("Parsing an expression\n");
    bool invertNot = false;
    if (look_ahead->type == K_NOT) {
        match_token(K_NOT);
        invertNot = true;

    }

    TypeAST *expType = parse_arith_op();
    expType = parse_expression_arith_op(expType);
    if (invertNot) expType->valueRef = LLVMBuildNot(builder, expType->valueRef, "not");

    assert_parser("Done parsing an expression\n");
    assert_parser("Expression type is: "); assert_parser(print_type(expType->typeClass)); assert_parser("\n");
    assert_parser("Code gen exp: "); assert_parser(LLVMPrintValueToString(expType->valueRef)); assert_parser("\n");

    return expType;
}

TypeAST *parse_expression_arith_op(TypeAST *arithOpType1) {
    TypeAST *arithOpType2 = NULL;
    TypeAST *expType = NULL;
    LLVMValueRef valueRef = arithOpType1->valueRef;

    switch(look_ahead->type) {
        case T_AND:
            match_token(T_AND);
            check_int_type(arithOpType1->typeClass);

            arithOpType2 = parse_arith_op();
            check_int_type(arithOpType2->typeClass);

            valueRef = LLVMBuildAnd(builder, arithOpType1->valueRef, arithOpType2->valueRef, "and");
            assert_parser("Code gen and: "); assert_parser(LLVMPrintValueToString(valueRef)); assert_parser("\n");

            expType = parse_expression_arith_op(arithOpType1);
            break;
        case T_OR:
            match_token(T_OR);
            check_int_type(arithOpType1->typeClass);

            arithOpType2 = parse_arith_op();
            check_int_type(arithOpType2->typeClass);

            valueRef = LLVMBuildOr(builder, arithOpType1->valueRef, arithOpType2->valueRef, "or");
            assert_parser("Code gen or: "); assert_parser(LLVMPrintValueToString(valueRef)); assert_parser("\n");

            expType = parse_expression_arith_op(arithOpType1);
            break;
        // FOLLOW set
        case T_COMMA: // argument list
        case T_RPAREN: // for loop, if statement
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            expType = arithOpType1;
            break;
        default: throw_error(E_INVALID_EXPRESSION, look_ahead->lineNo, look_ahead->columnNo);
    }

    expType->typeRef = arithOpType1->typeRef;
    expType->valueRef = valueRef;

    return expType;
}

TypeAST *parse_arith_op() {
    assert_parser("Parsing an arithmetic operation\n");
    TypeAST *arithOpType = parse_relation();
    arithOpType = parse_arith_op_relation(arithOpType);
    assert_parser("Done parsing an arithmetic operation\n");
    return arithOpType;
}

TypeAST *parse_arith_op_relation(TypeAST *relationType1) {
    TypeAST *relationType2 = NULL;
    TypeAST *arithOpType = NULL;
    LLVMValueRef valueRef = relationType1->valueRef;

    switch(look_ahead->type) {
        case T_PLUS:
            match_token(T_PLUS);
            check_int_float_type(relationType1->typeClass);

            relationType2 = parse_relation();
            check_int_float_type(relationType2->typeClass);

            valueRef = LLVMBuildAdd(builder, relationType1->valueRef, relationType2->valueRef, "add");
            assert_parser("Code gen add: "); assert_parser(LLVMPrintValueToString(valueRef));

            arithOpType = parse_arith_op_relation(relationType1);
            break;
        case T_MINUS:
            match_token(T_MINUS);
            check_int_float_type(relationType1->typeClass);

            relationType1 = parse_relation();
            check_int_float_type(relationType2->typeClass);

            valueRef = LLVMBuildSub(builder, relationType1->valueRef, relationType2->valueRef, "sub");
            assert_parser("Code gen sub: "); assert_parser(LLVMPrintValueToString(valueRef));

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

    arithOpType->typeRef = relationType1->typeRef;
    arithOpType->valueRef = valueRef;
    return arithOpType;
}

TypeAST *parse_relation() {
    assert_parser("Parsing a relation\n");
    TypeAST *relationType = parse_term();
    relationType = parse_relation_term(relationType);
    assert_parser("Done parsing a relation\n");
    assert_parser("Relation type is: "); assert_parser(print_type(relationType->typeClass)); assert_parser("\n");
    return relationType;
}

TypeAST *parse_relation_term(TypeAST *termType1) {
    TypeAST *termType2 = NULL;
    TypeAST *relationType = NULL;
    LLVMValueRef valueRef = termType1->valueRef;
    assert_parser("Relation term before: "); assert_parser(LLVMPrintValueToString(valueRef));

    TokenType binOp = look_ahead->type;
    switch(binOp) {
        case T_LT:
            match_token(T_LT);
            check_basic_type(termType1->typeClass);

            termType2 = parse_term();
            check_basic_type(termType2->typeClass);

            if (termType1->typeClass == TC_INT && termType2->typeClass == TC_BOOL) termType2->typeClass = TC_INT;
            if (termType1->typeClass == TC_BOOL && termType2->typeClass == TC_INT) termType2->typeClass = TC_INT;
            check_type_equality(termType1->typeClass, termType2->typeClass);

            valueRef = LLVMBuildICmp(builder, LLVMIntSLT, termType1->valueRef, termType2->valueRef, "lt");

            relationType = parse_relation_term(termType1);
            break;
        case T_LTEQ:
            match_token(T_LTEQ);
            check_basic_type(termType1->typeClass);

            termType2 = parse_term();
            check_basic_type(termType2->typeClass);

            if (termType1->typeClass == TC_INT && termType2->typeClass == TC_BOOL) termType2->typeClass = TC_INT;
            if (termType1->typeClass == TC_BOOL && termType2->typeClass == TC_INT) termType2->typeClass = TC_INT;
            check_type_equality(termType1->typeClass, termType2->typeClass);

            valueRef = LLVMBuildICmp(builder, LLVMIntSLE, termType1->valueRef, termType2->valueRef, "lteq");

            relationType = parse_relation_term(termType1);
            break;
        case T_GT:
            match_token(T_GT);
            check_basic_type(termType1->typeClass);

            termType2 = parse_term();
            check_basic_type(termType2->typeClass);

            if (termType1->typeClass == TC_INT && termType2->typeClass == TC_BOOL) termType2->typeClass = TC_INT;
            if (termType1->typeClass == TC_BOOL && termType2->typeClass == TC_INT) termType2->typeClass = TC_INT;
            check_type_equality(termType1->typeClass, termType2->typeClass);

            valueRef = LLVMBuildICmp(builder, LLVMIntSGT, termType1->valueRef, termType2->valueRef, "gt");

            relationType = parse_relation_term(termType1);
            break;
        case T_GTEQ:
            match_token(T_GTEQ);
            check_basic_type(termType1->typeClass);

            termType2 = parse_term();
            check_basic_type(termType2->typeClass);

            if (termType1->typeClass == TC_INT && termType2->typeClass == TC_BOOL) termType2->typeClass = TC_INT;
            if (termType1->typeClass == TC_BOOL && termType2->typeClass == TC_INT) termType2->typeClass = TC_INT;
            check_type_equality(termType1->typeClass, termType2->typeClass);

            valueRef = LLVMBuildICmp(builder, LLVMIntSGE, termType1->valueRef, termType2->valueRef, "gteq");

            relationType = parse_relation_term(termType1);
            break;
        case T_EQ:
            match_token(T_EQ);
            check_basic_type(termType1->typeClass);

            termType2 = parse_term();
            check_basic_type(termType2->typeClass);

            if (termType1->typeClass == TC_INT && termType2->typeClass == TC_BOOL) termType2->typeClass = TC_INT;
            if (termType1->typeClass == TC_BOOL && termType2->typeClass == TC_INT) termType2->typeClass = TC_INT;
            check_type_equality(termType1->typeClass, termType2->typeClass);

            valueRef = LLVMBuildICmp(builder, LLVMIntEQ, termType1->valueRef, termType2->valueRef, "eq");

            relationType = parse_relation_term(termType1);
            break;
        case T_NEQ:
            match_token(T_NEQ);
            check_basic_type(termType1->typeClass);

            termType2 = parse_term();
            check_basic_type(termType2->typeClass);

            if (termType1->typeClass == TC_INT && termType2->typeClass == TC_BOOL) termType2->typeClass = TC_INT;
            if (termType1->typeClass == TC_BOOL && termType2->typeClass == TC_INT) termType2->typeClass = TC_INT;
            check_type_equality(termType1->typeClass, termType2->typeClass);

            valueRef = LLVMBuildICmp(builder, LLVMIntNE, termType1->valueRef, termType2->valueRef, "neq");

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

    relationType->typeRef = termType1->typeRef;
    relationType->valueRef = valueRef;
    assert_parser("Relation term after: "); assert_parser(LLVMPrintValueToString(valueRef));

    return relationType;
}

TypeAST *parse_term() {
    assert_parser("Parsing a term\n");
    TypeAST *termType = parse_factor();
    termType = parse_term_factor(termType);
    assert_parser("Done parsing a term\n");
    assert_parser("Term type is: "); assert_parser(print_type(termType->typeClass)); assert_parser("\n");
    return termType;
}

TypeAST *parse_term_factor(TypeAST *factorType1) {
    TypeAST *termType = NULL;
    TypeAST *factorType2 = NULL;
    LLVMValueRef valueRef = factorType1->valueRef;

    switch(look_ahead->type) {
        case T_MULTIPLY:
            match_token(T_MULTIPLY);
            check_int_float_type(factorType1->typeClass);

            factorType2 = parse_factor();
            check_int_float_type(factorType2->typeClass);

            valueRef = LLVMBuildMul(builder, factorType1->valueRef, factorType2->valueRef, "mul");
            assert_parser("Code gen mul: "); assert_parser(LLVMPrintValueToString(valueRef)); assert_parser("\n");

            termType = parse_term_factor(factorType1);
            break;
        case T_DIVIDE:
            match_token(T_DIVIDE);
            check_int_float_type(factorType1->typeClass);

            factorType2 = parse_factor();
            check_int_float_type(factorType2->typeClass);

            valueRef = LLVMBuildSDiv(builder, factorType1->valueRef, factorType2->valueRef, "div");
            assert_parser("Code gen div: "); assert_parser(LLVMPrintValueToString(valueRef)); assert_parser("\n");

            termType = parse_term_factor(factorType1);
            break;
        // FOLLOW set
        case T_LT: case T_LTEQ: case T_GT: case T_GTEQ: case T_EQ: case T_NEQ: // relation
        case T_PLUS: case T_MINUS: // arith op
        case T_AND: case T_OR: case T_COMMA: // expression
        case T_RPAREN: // for loop, if statement
        case T_RBRACKET: // assignment statement
        case T_SEMI_COLON: // statements
            termType = factorType1;
            break;
        default: throw_error(E_INVALID_TERM, look_ahead->lineNo, look_ahead->columnNo); break;
    }

    termType->typeRef = factorType1->typeRef;
    termType->valueRef = valueRef;
    return termType;
}

TypeAST *parse_factor() {
    assert_parser("Parsing a factor\n");
    EntryAST *factorAST = NULL;
    TypeAST *typeAST = NULL;
    TypeClass factorType = TC_INVALID;
    LLVMValueRef value = NULL, address = NULL;
    LLVMTypeRef subType = NULL;
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
            factorType = TC_BOOL;
            value = LLVMConstInt(LLVMInt32Type(), 1, 1); // a bool is an 8 byte integer, but I see no point keeping the difference
            break;
        case K_FALSE:
            match_token(K_FALSE);
            factorType = TC_BOOL;
            value = LLVMConstInt(LLVMInt32Type(), 0, 1);
            break;
        case T_LPAREN: // ( <expression> )
            match_token(T_LPAREN);
            typeAST = parse_expression();
            factorType = typeAST->typeClass;
            value = typeAST->valueRef;
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
            address = factorAST->typeAST->address;

            if (factorAST->typeAST->sizeRef != NULL) { // an array so parse the index
                LLVMValueRef indexValue = parse_indexes(), arrayIndex = NULL;
                if (indexValue == NULL) {
                    arrayIndex = LLVMConstInt(LLVMInt32Type(), 0, false);
                } else {
                    arrayIndex = indexValue;
                }

                if (LLVMGetTypeKind(factorAST->typeAST->typeRef) == LLVMArrayTypeKind) {
                    LLVMValueRef indices[] = { LLVMConstInt(LLVMInt32Type(), 0, false), arrayIndex };
                    address = LLVMBuildInBoundsGEP(builder, address, indices, 2, "factor_array_GEP");
                } else {
                    address = LLVMBuildLoad(builder, address, "factor_pointer_load");
                    LLVMValueRef indices[] = { arrayIndex };
                    address = LLVMBuildInBoundsGEP(builder, address, indices, 1, "factor_pointer_GEP");
                }

                if (indexValue == NULL) {
                    value = address; // we want address, not the first element itself
                    subType = LLVMGetElementType(factorAST->typeAST->typeRef);
                    subType = LLVMPointerType(subType, 0); // just a pointer to the sub type of the sequential type
                } else {
                    value = LLVMBuildLoad(builder, address, "factor_array_load"); //there was an index so we should load it
                    subType = LLVMGetElementType(factorAST->typeAST->typeRef);
                }

            } else if (factorType == TC_STRING) {
                LLVMValueRef indices[] = { LLVMConstInt(LLVMInt32Type(), 0, false), LLVMConstInt(LLVMInt32Type(), 0, false) };
                address = LLVMBuildInBoundsGEP(builder, address, indices, 2, "factor_string_GEP");
                value = address;
            } else {
                value = LLVMBuildLoad(builder, address, factorAST->name);
            }
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

    typeAST = create_type(factorType);
    if (subType != NULL) typeAST->typeRef = subType;
    typeAST->valueRef = value;
    typeAST->address = address;

    assert_parser("Factor type: ");
    assert_parser(LLVMPrintTypeToString(typeAST->typeRef));
    assert_parser(", value ");
    assert_parser(LLVMPrintValueToString(typeAST->valueRef));
    return typeAST;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "error.h"

// from symbol_table.c
extern Token *current_token;

/******************************* Make Types ********************************/
// TypeAST *make_int_type() {
// 	TypeAST *type = (TypeAST *) malloc(sizeof(TypeAST));
// 	type->typeClass = TC_INT;
// 	return type;
// }
//
// TypeAST *make_char_type() {
// 	TypeAST *type = (TypeAST *) malloc(sizeof(TypeAST));
// 	type->typeClass = TC_CHAR;
// 	return type;
// }
//
// TypeAST *make_float_type() {
// 	TypeAST *type = (TypeAST *) malloc(sizeof(TypeAST));
// 	type->typeClass = TC_FLOAT;
// 	return type;
// }
//
// TypeAST *make_string_type() {
// 	TypeAST *type = (TypeAST *) malloc(sizeof(TypeAST));
// 	type->typeClass = TC_STRING;
// 	return type;
// }
//
// TypeAST *make_bool_type() {
	// TypeAST *type = (TypeAST *) malloc(sizeof(TypeAST));
	// type->typeClass = TC_BOOL;
	// return type;
// }

/*
The language has the following built I/O functions:
getBool(bool val out)
getInteger(integer val out)
getFloat(float val out)
getString(string val out)
getChar(char val out)
putBool(bool val in)
putInteger(integer val in)
putFloat(float val in)
putString(string val in)
putChar(char val in)
The put operations do not return a value.
These functions read/write input/output to standard in and standard out.
If you prefer, these routines can read/write to named files such as “input” and “output”.
*/
EntryAST *create_builtin_function(char *name, TypeClass varType, ParamType paramType) {
	assert_ast("Create builtin function");
	assert_ast(name);

	// EntryAST *type = create_type(varType);
	EntryAST *var = create_variable("val", 1, varType, 0, NULL);

	EntryAST *param = create_param(paramType, var);
	EntryNodeAST *params = create_entry_node(param, NULL);
	// EntryNodeAST *decls = create_entry_node(var, NULL);

	// EntryAST *body = create_body_block(decls, NULL);
	EntryAST *func = create_procedure(name, 1, 1, params, NULL);

	return func;
}

// int compare_type(TypeAST *type1, TypeAST *type2) {
// 	if (type1->typeClass == type2->typeClass) return 1; else return 0;
// }

int compare_type(TypeClass type1, TypeClass type2) {
	if (type1 == type2) return 1; else return 0;
}

void free_type(TypeAST *type) {
	free(type);
}

EntryAST *create_entry(EntryType entryType) {
	EntryAST *entryAST = (EntryAST *) malloc(sizeof(EntryAST));
	entryAST->entryType = entryType;
	return entryAST;
}

EntryNodeAST *create_entry_node(EntryAST *entryAST, EntryNodeAST *next) {
	EntryNodeAST *node = (EntryNodeAST *) malloc(sizeof(EntryNodeAST));
	node->entryAST = entryAST;
	node->next = next;
	return node;
}

EntryListAST *create_list() {
	EntryListAST *list = (EntryListAST *) malloc(sizeof(EntryListAST));
	list->head = NULL;
	list->tail = NULL;
	return list;
}

// EntryAST *create_type(TypeClass typeClass) {
// 	assert_ast("Create a type");
//     EntryAST *entryAST = create_entry(ET_TYPE_MARK);
// 	TypeAST *type = NULL;
// 	switch (typeClass) {
// 		case TC_INT: type = make_int_type(); break;
// 		case TC_FLOAT: type = make_float_type(); break;
// 		case TC_STRING: type = make_string_type(); break;
// 		case TC_BOOL: type = make_bool_type(); break;
// 		case TC_CHAR: type = make_char_type(); break;
// 	}
// 	entryAST->typeAST = type;
// 	return entryAST;
// }

EntryAST *create_binary_op(BinaryOpType type, EntryAST *lhs, EntryAST *rhs) {
	assert_ast("Create binary operation");
	EntryAST *entryAST = create_entry(ET_BIN_OP);
	BinaryOpAST *binOpAST = (BinaryOpAST *) malloc(sizeof(BinaryOpAST));
    binOpAST->binOpType = type;
    binOpAST->lhs = lhs;
    binOpAST->rhs = rhs;

    entryAST->binOpAST = binOpAST;
    return entryAST;
}

EntryAST *create_variable(char *name, int isGlobal, TypeClass type, int size, EntryAST *value) {
	assert_ast("Create variable");
	EntryAST *entryAST = create_entry(ET_VARIABLE);
	VariableAST *var = (VariableAST *) malloc(sizeof(VariableAST));

	var->name = name;
	var->varType = type;
	var->value = value;
	var->size = (unsigned) size;

	entryAST->varAST = var;
    return entryAST;
}

EntryAST *create_array(char *name, int isGlobal, TypeClass type, int size, EntryAST *value) {
	EntryAST *entryAST = create_variable(name, isGlobal, type, 0, value);
	entryAST->varAST->size = (unsigned) size;
	return entryAST;
}

EntryAST *create_body_block(EntryNodeAST *decls, EntryNodeAST *statements) {
	assert_ast("Create body block");
	EntryAST *entryAST = create_entry(ET_BODY);
	return entryAST;
}

EntryAST *create_program(char *name, EntryAST *body) {
	assert_ast("Create program");
	EntryAST *entryAST = create_entry(ET_PROGRAM);
	return entryAST;
}

EntryAST *create_procedure_call(char *name, EntryNodeAST *args, int argc) {
	assert_ast("Create procedure call");
	EntryAST *entryAST = create_entry(ET_STATEMENT);
	return entryAST;
}

EntryAST *create_param(ParamType paramType, EntryAST *var) {
	assert_ast("Create param");
	EntryAST *entryAST = create_entry(ET_PARAMTER);
	ParamAST *param = (ParamAST *) malloc(sizeof(ParamAST));
	param->var = var;
	entryAST->paramAST = param;
	return entryAST;
}

EntryAST *create_procedure(char *name, int isGlobal, int argc, EntryNodeAST *params, EntryAST *body) {
	assert_ast("Create procedure");
	EntryAST *entryAST = create_entry(ET_PROCEDURE);
	ProcedureAST *proc = (ProcedureAST *) malloc(sizeof(EntryAST));

	proc->name = name;
	proc->params = params;
	proc->argc = (unsigned) argc;

	entryAST->procAST = proc;
	return entryAST;
}

EntryAST *create_factor(TypeClass typeClass, Token *value) {
	assert_ast("Create factor");
	EntryAST *entryAST = create_entry(ET_FACTOR);
	return entryAST;
}

EntryAST *create_loop(EntryAST *assignment, EntryAST *expr, EntryNodeAST *statements) {
	assert_ast("Create loop");
	EntryAST *entryAST = create_entry(ET_STATEMENT);
	return entryAST;
}

EntryAST *create_return() {
	assert_ast("Create return");
	EntryAST *entryAST = create_entry(ET_STATEMENT);
	return entryAST;
}

EntryAST *create_unary_op(UnaryOpType unaOp, EntryAST *factor) {
	assert_ast("Create unary operator");
	EntryAST *entryAST = create_entry(ET_UNA_OP);
	return entryAST;
}

EntryAST *create_if(EntryAST *condition, EntryNodeAST *trueBlock, EntryNodeAST *falseBlock) {
	assert_ast("Create if");
	EntryAST *entryAST = create_entry(ET_STATEMENT);
	IfAST *ifAST = entryAST->stmtAST->ifAST;
	ifAST->condition = condition;
	ifAST->trueBlock = trueBlock;
	ifAST->falseBlock = falseBlock;
	return entryAST;
}

void print_type(TypeClass type) {
    assert_ast("A type node");
    printf("TypeClass: ");
    switch (type) {
        case TC_INT:
            printf("Int"); break;
    	case TC_FLOAT:
            printf("Float"); break;
    	case TC_STRING:
            printf("String"); break;
    	case TC_BOOL:
            printf("Bool"); break;
    	case TC_CHAR:
            printf("Char"); break;
            break;
        default:
            printf("Unknown");break;
    }
    printf("\n");
}

void print_variable(VariableAST *varAST) {
    assert_ast("A variable node");
    printf("Name: %s\n", varAST->name);

    print_type(varAST->varType);

    printf("Value: ");
    FactorAST *value = varAST->value->factorAST;
    switch (varAST->varType) {
        case TC_INT:
            printf("%d", value->intVal); break;
    	case TC_FLOAT:
            printf("%f", value->floatVal); break;
    	case TC_STRING:
            printf("\"%s\"", value->stringVal); break;
    	case TC_BOOL:
            printf("%d", value->boolVal); break;
    	case TC_CHAR:
            printf("'%c'", value->charVal); break;
        default:
            printf("Unknown value"); break;
    }
    printf("\n");
}

void print_program(ProgramAST *progAST) {
    assert_ast("A program node");
}

void print_procedure(ProcedureAST *procAST) {
    assert_ast("A procedure node");
}

void print_bin_op(BinaryOpAST *binOpAST) {
    assert_ast("A Binary Operation node");
}

void print_statement(StatementAST *statAST) {
    assert_ast("A statment node");
}

void print_param(ParamAST *paramAST) {
    assert_ast("A parameter node");
}

void print_entry(EntryAST *entry) {
    switch (entry->entryType) {
    	case ET_VARIABLE:
            print_variable(entry->varAST); break;
    	// case ET_TYPE_MARK:
        //     print_type(entry->typeAST); break;
        case ET_PARAMTER:
            print_param(entry->paramAST); break;
    	case ET_PROCEDURE:
            print_procedure(entry->procAST); break;
    	case ET_PROGRAM:
            print_program(entry->progAST); break;
        case ET_BIN_OP:
            print_bin_op(entry->binOpAST); break;
        case ET_STATEMENT:
            print_statement(entry->stmtAST); break;
        default: break;
    }
}

void print_entry_list(EntryNodeAST *entryList) {
    EntryNodeAST *node = entryList;
    while (node != NULL) {
        print_entry(node->entryAST);
        node = node->next;
    }
}

#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

// from symbol_table.c
extern Token *current_token;

/******************************* Make Types ********************************/
TypeAST *make_int_type() {
	TypeAST *type = (TypeAST *) malloc(sizeof(TypeAST));
	type->typeClass = TC_INT;
	return type;
}

TypeAST *make_char_type() {
	TypeAST *type = (TypeAST *) malloc(sizeof(TypeAST));
	type->typeClass = TC_CHAR;
	return type;
}

TypeAST *make_float_type() {
	TypeAST *type = (TypeAST *) malloc(sizeof(TypeAST));
	type->typeClass = TC_FLOAT;
	return type;
}

TypeAST *make_string_type() {
	TypeAST *type = (TypeAST *) malloc(sizeof(TypeAST));
	type->typeClass = TC_STRING;
	return type;
}

TypeAST *make_bool_type() {
	TypeAST *type = (TypeAST *) malloc(sizeof(TypeAST));
	type->typeClass = TC_BOOL;
	return type;
}

TypeAST *make_array_type(int size, EntryAST *elementType) {
	TypeAST *type = (TypeAST *) malloc(sizeof(TypeAST));
	type->typeClass = TC_ARRAY;
	type->arraySize = size;
	type->elementType = elementType;
	return type;
}

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
	EntryAST *type = create_type(varType);
	EntryAST *var = create_variable("val", type, NULL);

	EntryAST *param = create_param(paramType, var, type);
	EntryNodeAST *params = create_entry_node(param, NULL);
	// EntryNodeAST *decls = create_entry_node(var, NULL);

	// EntryAST *body = create_body_block(decls, NULL);
	EntryAST *func = create_procedure(name, params, NULL);

	return func;
}

int compare_type(TypeAST *type1, TypeAST *type2) {
	if (type1->typeClass == type2->typeClass) {
		if (type1->typeClass == TC_ARRAY) {
			if (type1->arraySize == type2->arraySize) {
				return compare_type(type1->elementType->typeAST, type2->elementType->typeAST);
			} else return 0;
		} else return 1;
	} else return 0;
}

void free_type(TypeAST *type) {
	free(type);
}

EntryNodeAST *create_entry_node(EntryAST *entryAST, EntryNodeAST *next) {
	EntryNodeAST *node = (EntryNodeAST *) malloc(sizeof(EntryNodeAST));
	node->entryAST = entryAST;
	node->next = next;
	return node;
}

EntryAST *create_type(TypeClass typeClass) {
    EntryAST *entryAST = (EntryAST *) malloc(sizeof(EntryAST));
	TypeAST *type = NULL;
	switch (typeClass) {
		case TC_INT: type = make_int_type(); break;
		case TC_FLOAT: type = make_float_type(); break;
		case TC_STRING: type = make_string_type(); break;
		case TC_BOOL: type = make_bool_type(); break;
		case TC_CHAR: type = make_char_type(); break;
		// case TC_ARRAY: type = make_array_type(); break; //TODO: Probably need to include elementType and size
	}
	entryAST->typeAST = type;
	return entryAST;
}

EntryAST *create_binary_op(BinaryOpType type, EntryAST *lhs, EntryAST *rhs) {
    EntryAST *entryAST = (EntryAST *) malloc(sizeof(EntryAST));
    BinaryOpAST *binOpAST = (BinaryOpAST *) malloc(sizeof(BinaryOpAST));
    entryAST->entryType = ET_BIN_OP;
    binOpAST->binOpType = type;
    binOpAST->lhs = lhs;
    binOpAST->rhs = rhs;
    entryAST->binOpAST = binOpAST;
    return entryAST;
}

EntryAST *create_variable(char *name, EntryAST *type, EntryAST *value) {
    EntryAST *entryAST = (EntryAST *) malloc(sizeof(EntryAST));
    if (type->typeAST->typeClass != TC_ARRAY) {
        if (value != NULL) {

        } // else throw some error
    } // else do something for variable AST
    return entryAST;
}

EntryAST *create_body_block(EntryNodeAST *decls, EntryNodeAST *statements) {
	assert_ast("Create body block");
	EntryAST *entryAST = (EntryAST *) malloc(sizeof(EntryAST));
	return entryAST;
}

EntryAST *create_program(char *name, EntryAST *body) {
	assert_ast("Create program");
	EntryAST *entryAST = (EntryAST *) malloc(sizeof(EntryAST));
	return entryAST;
}

EntryAST *create_procedure_call(char *name, EntryNodeAST *args, int argc) {
	assert_ast("Create procedure call");
	EntryAST *entryAST = (EntryAST *) malloc(sizeof(EntryAST));
	return entryAST;
}

EntryAST *create_param(ParamType paramType, EntryAST *var, EntryAST *type) {
	assert_ast("Create param");
	EntryAST *entryAST = (EntryAST *) malloc(sizeof(EntryAST));
	return entryAST;
}

EntryAST *create_procedure(char *name, EntryNodeAST *params, EntryAST *body) {
	assert_ast("Create procedure");
	EntryAST *entryAST = (EntryAST *) malloc(sizeof(EntryAST));
	return entryAST;
}

EntryAST *create_factor(TypeClass typeClass, Token *value) {
	assert_ast("Create factor");
	EntryAST *entryAST = (EntryAST *) malloc(sizeof(EntryAST));
	return entryAST;
}

EntryAST *create_loop(EntryAST *assignment, EntryAST *expr, EntryNodeAST *statements) {
	assert_ast("Create loop");
	EntryAST *entryAST = (EntryAST *) malloc(sizeof(EntryAST));
	return entryAST;
}

EntryAST *create_return() {
	assert_ast("Create return");
	EntryAST *entryAST = (EntryAST *) malloc(sizeof(EntryAST));
	return entryAST;
}

EntryAST *create_unary_op(UnaryOpType unaOp, EntryAST *factor) {
	assert_ast("Create unary operator");
	EntryAST *entryAST = (EntryAST *) malloc(sizeof(EntryAST));
	return entryAST;
}

EntryAST *create_if(EntryAST *condition, EntryNodeAST *trueBlock, EntryNodeAST *falseBlock) {
	assert_ast("Create if");
	EntryAST *entryAST = (EntryAST *) malloc(sizeof(EntryAST));
	IfAST *ifAST = entryAST->stmtAST->ifAST;
	ifAST->condition = condition;
	ifAST->trueBlock = trueBlock;
	ifAST->falseBlock = falseBlock;
	return entryAST;
}

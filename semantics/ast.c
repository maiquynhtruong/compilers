#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

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

TypeAST *make_array_type(int size, TypeAST *elementType) {
	TypeAST *type = (TypeAST *) malloc(sizeof(TypeAST));
	type->typeClass = TC_ARRAY;
	type->arraySize = size;
	type->elementType = elementType;
	return type;
}

int compare_type(TypeAST *type1, TypeAST *type2) {
	if (type1->typeClass == type2->typeClass) {
		if (type1->typeClass == TC_ARRAY) {
			if (type1->arraySize == type2->arraySize) {
				return compare_type(type1->elementType, type2->elementType);
			} else return 0;
		} else return 1;
	} else return 0;
}

void free_type(TypeAST *type) {
	free(type);
}

Type *create_type() {
    Type *type = (Type *) malloc(sizeof(Type *));
    return type;
}

EntryAST *create_unary_op(UnaryOpAST unaOp, EntryAST *factor);

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

EntryAST *create_variable(char *name, Type *type, ConstantAST *value) {
    EntryAST *varAST = (EntryAST *) malloc(sizeof(EntryAST));
    if (type->typeClass != TC_ARRAY) {
        if (value != NULL) {

        } // else throw some error
    } // else do something for variable AST
    return varAST;
}

EntryAST *create_constant(char *name, TypeAST *type);
EntryAST *create_body_block(EntryAST *decls, EntryAST *statements);
EntryAST *create_program(char *name, EntryAST *body);
EntryAST *create_procedure_call(char *name, EntryAST **args, int argc);
EntryAST *create_param(char *name, EntryAST *procedure);
EntryAST *create_procedure(char *name, EntryAST *body);
EntryAST *create_if_statement(EntryAST *condition, EntryAST *trueBlock, EntryAST *falseBlock);

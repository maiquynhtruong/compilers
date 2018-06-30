#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

// from symbol_table.c
extern SymbolTable *symbolTable;
extern Token *current_token;

// Check if an entry has been inserted in table before throws an error if that happens
void check_new_identifier(char *name) {
	EntryAST *entry = find_entry(symbolTable->currentScope->entryList, name);

	if (entry != NULL) throw_error(E_DUPLICATE_IDENT, current_token->lineNo, current_token->columnNo);
}

EntryAST *check_declared_identifier(char *name) {
	EntryAST *entry = lookup(name);

	if (entry == NULL) throw_error(E_UNDECLARED_IDENT, current_token->lineNo, current_token->columnNo);
	return entry;
}

EntryAST *check_declared_variable(char *name) {
	EntryAST *entry = lookup(name);

	if (entry == NULL) throw_error(E_UNDECLARED_VARIABLE, current_token->lineNo, current_token->columnNo);
	if (entry->entryType != ET_VARIABLE)
		throw_error(E_INVALID_VARIABLE, current_token->lineNo, current_token->columnNo);

	return entry;
}

EntryAST *check_declared_procedure(char *name) {
	EntryAST *entry = lookup(name);

	if (entry == NULL) throw_error(E_UNDECLARED_PROCEDURE, current_token->lineNo, current_token->columnNo);
	if (entry->entryType != ET_PROCEDURE)
		throw_error(E_INVALID_PROCEDURE, current_token->lineNo, current_token->columnNo);

	return entry;
}

EntryAST *check_declared_destination(char *name) {
	EntryAST *entry = lookup(name);

	if (entry == NULL) throw_error(E_UNDECLARED_IDENT, current_token->lineNo, current_token->columnNo);

	switch (entry->entryType) {
		case ET_PARAMTER: case ET_VARIABLE: break;
		// case ET_PROCEDURE:
		// 	if (entry != symbol_table->currentScope->parent) // procedure name
		// 		throw_error(E_UNDECLARED_IDENT, current_token->lineNo, current_token->columnNo);
		default: throw_error(E_UNDECLARED_IDENT, current_token->lineNo, current_token->columnNo);
	};

	return entry;
}

void check_int_float_type(TypeAST *type) {
	if (type != NULL && (type->typeClass == TC_INT || type->typeClass == TC_FLOAT)) return;
	else throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_basic_type(TypeAST *type) {
	if (type != NULL && (type->typeClass == TC_INT || type->typeClass == TC_FLOAT ||
		 type->typeClass == TC_BOOL || type->typeClass == TC_CHAR)) return;
	else throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_builtin_type(TypeClass type) {
	if (type == TC_INT || type == TC_FLOAT || type == TC_BOOL || type == TC_BOOL || type == TC_CHAR || type == TC_STRING) return;
	else throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

// void check_builtin_type(TypeAST *type) {
// 	if (type != NULL && (type->typeClass == TC_INT || type->typeClass == TC_FLOAT ||
// 		 type->typeClass == TC_BOOL || type->typeClass == TC_CHAR || type->typeClass == TC_STRING)) return;
// 	else throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
// }

void check_int_type(TypeAST *type) {
	if (type != NULL && type->typeClass == TC_INT) return;
	else throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_char_type(TypeAST *type) {
	if (type != NULL && type->typeClass == TC_CHAR) return;
	else throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_string_type(TypeAST *type) {
	if (type != NULL && type->typeClass == TC_STRING) return;
	else throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_float_type(TypeAST *type) {
	if (type != NULL && type->typeClass == TC_FLOAT) return;
	else throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_bool_type(TypeAST *type) {
	if (type != NULL && type->typeClass == TC_BOOL) return;
	else throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_type_equality(TypeAST *type1, TypeAST *type2) {
	if (compare_type(type1, type2) == 0)
		throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void convert_to_bool(TypeAST *type) {
	if (type->typeClass == TC_INT) type->typeClass = TC_BOOL;
}
void convert_to_int(TypeAST *type) {
	if (type->typeClass == TC_BOOL) type->typeClass = TC_INT;
}

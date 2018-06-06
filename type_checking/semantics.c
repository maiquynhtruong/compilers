#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

// from symbol_table.c
extern SymbolTable *symbolTable;
extern Token *current_token;

Entry *lookup(char *name) {
	Entry *entry = NULL;
	Scope *current_scope = symbolTable->currentScope;

	while (current_token != NULL) {
		entry = find_entry(current_scope->entryList, name);
		if (entry != NULL) return entry;
		current_scope = current_scope->outerScope;
	}

	// search in global scope as the last resort
	entry = find_entry(symbolTable->globalEntryList, name);
	if (entry != NULL) return entry;
	else return NULL;
}

// Check if an entry has been inserted in table before throws an error if that happens
void check_new_identifier(char *name) {
	Entry *entry = find_entry(symbolTable->currentScope->entryList, name);

	if (entry != NULL) throw_error(E_DUPLICATE_IDENT, current_token->lineNo, current_token->columnNo);
}

Entry *check_declared_identifier(char *name) {
	Entry *entry = lookup(name);

	if (entry == NULL) throw_error(E_UNDECLARED_IDENT, current_token->lineNo, current_token->columnNo);
	return entry;
}

Entry *check_declared_variable(char *name) {
	Entry *entry = lookup(name);

	if (entry == NULL) throw_error(E_UNDECLARED_VARIABLE, current_token->lineNo, current_token->columnNo);
	if (entry->entryType != ET_VARIABLE)
		throw_error(E_INVALID_VARIABLE, current_token->lineNo, current_token->columnNo);

	return entry;
}

Entry *check_declared_procedure(char *name) {
	Entry *entry = lookup(name);

	if (entry == NULL) throw_error(E_UNDECLARED_PROCEDURE, current_token->lineNo, current_token->columnNo);
	if (entry->entryType != ET_PROCEDURE)
		throw_error(E_INVALID_PROCEDURE, current_token->lineNo, current_token->columnNo);

	return entry;
}

Entry *check_declared_destination(char *name) {
	Entry *entry = lookup(name);

	if (entry == NULL) throw_error(E_UNDECLARED_IDENT, current_token->lineNo, current_token->columnNo);

	switch (entry->entryType) {
		case ET_PARAMTER: case ET_VARIABLE: break;
		// case ET_PROCEDURE:
		// 	if (entry != symbolTable->currentScope->parent) // procedure name
		// 		throw_error(E_UNDECLARED_IDENT, current_token->lineNo, current_token->columnNo);
		default: throw_error(E_UNDECLARED_IDENT, current_token->lineNo, current_token->columnNo);
	};

	return entry;
}

void check_int_float_type(Type *type) {
	if (type->typeClass != TC_INT && type->typeClass != TC_FLOAT)
		throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_int_float_bool_char_type(Type *type) {
	if (type->typeClass != TC_INT && type->typeClass != TC_FLOAT && type->typeClass != TC_BOOL && type->typeClass != TC_CHAR)
		throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_int_type(Type *type) {
	if (type->typeClass != TC_INT)
		throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_char_type(Type *type) {
	if (type->typeClass != TC_CHAR)
		throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_string_type(Type *type) {
	if (type->typeClass != TC_STRING)
		throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_float_type(Type *type) {
	if (type->typeClass != TC_FLOAT)
		throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_bool_type(Type *type) {
	if (type->typeClass != TC_BOOL)
		throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_array_type(Type *type) {
	if (type->typeClass != TC_ARRAY)
		throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_type_equality(Type *type1, Type *type2) {
	if (compare_type(type1, type2) == 0)
		throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

// from symbol_table.c
extern SymTab *symbol_table;
extern Token *current_token;

Entry *lookup(char *name) {
	Entry *entry = NULL;
	static Scope *current_scope = symbol_table->currentScope;

	while (current_token != NULL) {
		entry = find_entry(current_scope->entryList, name);
		if (entry != NULL) return entry;
		current_scope = current_scope->outerScope;
	}

	// search in global scope as the last resort
	entry = find_entry(symbol_table->globalEntryList, name);
	if (entry != NULL) return entry;
	else return NULL;
}

// Check if an entry has been inserted in table before throws an error if that happens
void check_new_identifier(char *name) {
	Entry *entry = find_entry(symbol_table->currentScope->entryList, name);

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

Entry *check_declared_LValue_identifier(char *name) {
	Entry *entry = lookup(name);

	if (entry == NULL) throw_error(E_UNDECLARED_IDENT, current_token->lineNo, current_token->columnNo);

	switch (entry->entryType) {
		case ET_PARAMTER: case ET_VARIABLE: break;
		case ET_PROCEDURE:
			if (entry != symbol_table->currentScope->parent) // procedure name
				throw_error(E_UNDECLARED_IDENT, current_token->lineNo, current_token->columnNo);
		default: throw_error(E_UNDECLARED_IDENT, current_token->lineNo, current_token->columnNo);
	};

	return entry;
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
	if (type1->typeClass != type2->typeClass)
		throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
	else if (type1->typeClass == TC_ARRAY) {
		if (type2->typeClass != TC_ARRAY)
			throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);

		check_type_equality(type1->elementType. type2->elementType);
		if (type1->arraySize != type2->arraySize)
			throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
	}
}

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

SymTab *symbolTable;
Token *current_token;

static int searching = 0;

Entry *lookup_object(char *name) {
	Entry *entry = NULL;
	static Scope *currentScope = NULL;

	// search current scope if we haven't search at all
	// or a global search has been conducted
	if (searching == 0) currentScope = symbolTable->currentScope;

	// start from the scope of previous search
	while (currentScope != NULL) {
		entry = find_entry(currentScope->entryList, name);
		currentScope = currentScope->outerScope;
		searching = 1; // searching so other functions can't search at the same time
		if (entry != NULL) return entry;
	}

	// search in global scope as the last resort
	entry = find_entry(symbolTable->globalEntryList, name);
	searching = 0;
	return entry;
}

// Check if an entry has been inserted in table before throws an error if that happens
void check_new_identifier(char *name) {
	Entry *entry = find_entry(symbolTable->currentScope->entryList, name);
	if (entry != NULL) throw_error(E_DUPLICATE_IDENT, current_token->lineNo, current_token->columnNo);
}

Entry *checkDeclaredIdentifier(char *name) {
	searching = 0; // start searching
	Entry *entry = lookup_object(name);
	searching = 0; // stop searching

	if (entry == NULL) throw_error("Undeclared Identifier\n");
	return entry;
}

Entry *checkDeclaredVariable(char *name) {
	Entry *entry = NULL;

	searching = 0;

	do {
		entry = lookup_object(name);
		if (entry != NULL && entry->entryType == ET_VARIABLE) break;
	} while (entry != NULL);

	searching = 0;

	if (entry == NULL) throw_error("Undeclared variable\n");

	return entry;
}

Entry *checkDeclaredProcedure(char *name) {
	Entry *entry = NULL;

	searching = 0;

	do {
		entry = lookup_object_object(name);
		if (entry != NULL && entry->entryType == ET_PROCEDURE) break;
	} while (entry != NULL);

	searching = 0;

	if (entry == NULL) throw_error("Undeclared Procedure\n");

	return entry;
}

Entry *checkDeclaredLValueIdentifier(char *name) {

}

void check_int_type(Type *type) {

}

void check_char_type(Type *type) {

}

void check_string_type(Type *type) {

}

void check_float_type(Type *type) {

}

void check_bool_type(Type *type) {

}

void check_type_equality(Type *type1, Type *type2) {

}

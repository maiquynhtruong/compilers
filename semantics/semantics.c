#include <stdlib.h>
#include <string.h>
#include "semantics.h"

SymTab *symbolTable;
Token *currentToken;

static int searching = 0;

Entry *lookup(char *name) {
	Entry *entry = NULL;
	static Scope *currentScope = NULL;

	// search current scope if no search has been carried out
	if (searching == 0) currentScope = symbolTable->currentScope;

	// start from the scope of previous search
	while (currentScope != NULL) {
		entry = findEntry(currentScope->entryList, name);

		currentScope = currentScope->outerScope;

		searching = 1; // mark as searching

		if (entry != NULL) return entry;
	}

	// search in global scope as the last resort
	entry = findEntry(symbolTable->globalEntryList, name);

	searching = 0; // done searching

	return entry;
}

// Check if an entry has been inserted in table before throws an error if that happens
void checkNewIdentifier(char *name) {
	Entry *entry = findEntry(symbolTable->currentScope->entryList, name);
	if (entry != NULL) perror("Duplicate Identifier\n");
}

Entry *checkDeclaredIdentifier(char *name) {
	searching = 0; // start searching
	Entry *entry = lookup(name);
	searching = 0; // stop searching

	if (entry == NULL) perror("Undeclared Identifier\n");
	return entry;
}

Entry *checkDeclaredVariable(char *name) {
	Entry *entry = NULL;

	searching = 0;

	do {
		entry = lookup(name);
		if (entry != NULL && entry->entryType == ET_VARIABLE) break;
	} while (entry != NULL);

	searching = 0;

	if (entry == NULL) perror("Undeclared variable\n");

	return entry;
}

Entry *checkDeclaredProcedure(char *name) {
	Entry *entry = NULL;

	searching = 0;

	do {
		entry = lookup(name);
		if (entry != NULL && entry->entryType == ET_PROCEDURE) break;
	} while (entry != NULL);

	searching = 0;

	if (entry == NULL) perror("Undeclared Procedure\n");

	return entry;
}

Entry *checkDeclaredLValueIdentifier(char *name) {

}

void checkIntType(Type *type) {

}

void checkCharType(Type *type) {

}

void checkStringType(Type *type) {

}

void checkFloatType(Type *type) {

}

void checkBoolType(Type *type) {

}

void checkTypeEquality(Type *type1, Type *type2) {

}

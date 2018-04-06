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

/* Check if an entry has been inserted in table before
	throws an error if that happens
*/
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
	if (type->typeClass != TC_INT)
		perror("Type Inconsistency: Not an integer");
}

void checkFloatType(Type *type) {
	if (type->typeClass != TC_FLOAT)
		perror("Type Inconsistency: Not a float");
}

void checkStringType(Type *type) {
	if (type->typeClass != TC_STRING)
		perror("Type Inconsistency: Not a string");
}

void checkBooltType(Type *type) {
	if (type->typeClass != TC_BOOL)
		perror("Type Inconsistency: Not a boolean");
}

void checkCharType(Type *type) {
	if (type->typeClass != TC_CHAR)
		perror("Type Inconsistency: Not a character");
}

void checkArrayType(Type *type) {
	if (type->typeClass != TC_ARRAY)
		perror("Type Inconsistency: Not an array");
}

void checkEqualType(Type *type1, Type *type2) {
	if (type1->typeClass == type2->typeClass) {
		if (type1->typeClass == TC_ARRAY) {
			checkEqualType(type1->elementType, type2->elementType);
			if (type1->arraySize != type2->arraySize) 
				perror("Type Inconsistency: Different array sizes");
		}
	} else perror("Type Inconsistency: Different types")
}

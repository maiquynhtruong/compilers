#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "error.h"

SymbolTable *symbolTable;

void declare_entry(EntryAST *entry, int isGlobal) {
	assert_symbol_table("Declaring an entry");
	print_entry(entry);

	if (entry == NULL) return;

	EntryNodeAST *curList = NULL;

	if (symbolTable->currentScope == NULL || isGlobal) curList = symbolTable->globalEntryList;
	else curList = symbolTable->currentScope->entryList;

	EntryNodeAST *entryNode = (EntryNodeAST *) malloc(sizeof(EntryNodeAST));
	entryNode->entryAST = entry;

	if (curList == NULL) {
		curList = entryNode;
	} else {
		while (curList->next != NULL) curList = curList->next;
		curList->next = entryNode;
	}
}

EntryAST *lookup(char *name) {
	EntryAST *entry = NULL;
	Scope *current_scope = symbolTable->currentScope;

	while (current_scope != NULL) {
		entry = find_entry(current_scope->entryList, name);
		if (entry != NULL) return entry;

		current_scope = current_scope->outerScope;
	}

	// search in global scope as the last resort
	// entry = find_entry(symbol_table->globalEntryList, name); // globalEntryList is the outerScope of the second to last scope so no need for this
	return entry;
}

// find entry by name in a EntryNodeAST list
EntryAST *find_entry(EntryNodeAST *list, char *name) {
	assert_symbol_table("Finding entry: ");
	assert_symbol_table(name);

	EntryNodeAST *curNode = list;
	EntryAST *entryAST = NULL;

	while (curNode != NULL) {
		entryAST = curNode->entryAST;
		char *entryName;
		switch (entryAST->entryType) {
			case ET_PROGRAM: entryName = entryAST->progAST->name; break;
			case ET_VARIABLE: entryName = entryAST->varAST->name; break;
			case ET_PROCEDURE: entryName = entryAST->procAST->name; break;
			case ET_STATEMENT:
				switch (entryAST->stmtAST->statementType) {
					case ST_CALL: entryName = entryAST->stmtAST->procCallAST->callee; break;
					default: break;
				}
				break;
			case ET_PARAMTER: entryName = entryAST->paramAST->var->varAST->name; break;
			default: break;
		}
		if (strcmp(entryName, name) == 0)
			return entryAST;
		else curNode = curNode->next;
	}

	return entryAST;
}

void init_symbol_table() {
	assert_symbol_table("Initialize a symbol table");

	symbolTable = (SymbolTable *) malloc(sizeof(SymbolTable));
	symbolTable->currentScope = NULL;
	symbolTable->root = NULL;
	symbolTable->globalEntryList = NULL;

	enter_scope(symbolTable->currentScope);

	// built-in functions e.g. getInteger(integer val out)
	EntryAST *getBool = create_builtin_function("getBool", TC_BOOL, PT_OUT); // getBool(bool val out)
	declare_entry(getBool, 1); // or should it be add_entry(&(symbolTable->globalEntryList), func)?
	EntryAST *getInteger = create_builtin_function("getInteger", TC_INT, PT_OUT); // getInteger(integer val out)
	declare_entry(getInteger, 1);
	EntryAST *getFloat = create_builtin_function("getFloat", TC_FLOAT, PT_OUT); // getFloat(float val out)
	declare_entry(getFloat, 1);
	EntryAST *getString = create_builtin_function("getString", TC_STRING, PT_OUT); // getString(string val out)
	declare_entry(getString, 1);
	EntryAST *getChar = create_builtin_function("getChar", TC_CHAR, PT_OUT); // getChar(char val out)
	declare_entry(getChar, 1);

	EntryAST *putBool = create_builtin_function("putBool", TC_BOOL, PT_IN); // putBool(bool val in)
	declare_entry(putBool, 1);
	EntryAST *putInteger = create_builtin_function("putInteger", TC_INT, PT_IN); // putInteger(integer val in)
	declare_entry(putInteger, 1);
	EntryAST *putFloat = create_builtin_function("putFloat", TC_FLOAT, PT_IN); // putFloat(float val in)
	declare_entry(putFloat, 1);
	EntryAST *putString = create_builtin_function("putString", TC_STRING, PT_IN); // putString(string val in)
	declare_entry(putString, 1);
	EntryAST *putChar = create_builtin_function("putChar", TC_CHAR, PT_IN); // putChar(char val in)
	declare_entry(putChar, 1);

	// TODO: Are these needed?
	// TypeAST *intType = make_int_type();
	// TypeAST *charType = make_char_type();
	// TypeAST *floatType = make_float_type();
	// TypeAST *stringType = make_string_type();
	// TypeAST *boolType = make_bool_type();

	assert_symbol_table("Finish initializing a symbol table");
}

void clear_symbol_table() {
	free_entry(symbolTable->root);
	free_entry_list(symbolTable->globalEntryList);
	// free_scope(symbolTable->currentScope);
	free(symbolTable);
	//TODO: Are these needed?
	// free_type(intType);
	// free_type(charType);
	// free_type(floatType);
	// free_type(stringType);
	// free_type(boolType);
}

Scope *create_scope() {
	assert_symbol_table("New scope");
	Scope *scope = (Scope *) malloc(sizeof(Scope));
	scope->entryList = NULL;
	scope->outerScope = symbolTable->currentScope;
	return scope;
}

void enter_scope(Scope *scope) {
	assert_symbol_table("Enter a scope");

	if (scope == NULL) scope = create_scope();
	symbolTable->currentScope = scope;
}

void exit_scope() {
	assert_symbol_table("Exit a scope");

	symbolTable->currentScope = symbolTable->currentScope->outerScope;
}

void free_scope(Scope *scope) {
	if (scope != NULL) {
		free_entry_list(scope->entryList);
		free(scope);
		scope = NULL;
	}
}

void free_entry(EntryAST *entry) {
	if (entry != NULL) {
		switch(entry->entryType) {
			case ET_VARIABLE:
				if (entry->varAST != NULL) free(entry->varAST);
				break;
			case ET_PROCEDURE:
				if (entry->procAST != NULL) free(entry->procAST);
				break;
			case ET_PARAMTER:
				if (entry->paramAST != NULL) free(entry->paramAST);
				break;
			case ET_PROGRAM:
				if (entry->progAST != NULL) free(entry->progAST);
				break;
			default: break;
		}
		free(entry);
		entry = NULL;
	}
}

void free_entry_list(EntryNodeAST *node) {
	while (node != NULL) {
		free_entry(node->entryAST);
		node = node->next;
	}
}


void print_scope(Scope *scope) {
    print_entry_list(scope->entryList);
}

void print_symbol_table() {

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "error.h"

SymbolTable *symbolTable;
TypeAST *intType;
TypeAST *charType;
TypeAST *floatType;
TypeAST *stringType;
TypeAST *boolType;


EntryAST *getInteger;
EntryAST *getFloat;
EntryAST *getString;
EntryAST *getChar;
EntryAST *putBool;
EntryAST *putInteger;
EntryAST *putFloat;
EntryAST *putString;
EntryAST *putChar;

/******************************* Symbol table functions ********************************/

void declare_entry(EntryAST *entryAST, int isGlobal) {
	assert("Declaring an entry");

	if (entryAST == NULL) return;

	EntryNodeAST *curList = NULL;

	if (symbolTable->currentScope == NULL || isGlobal) curList = symbolTable->globalEntryList;
	else curList = symbolTable->currentScope->entryList;

	add_entry(&curList, entry);
}

void add_entry(EntryNodeAST **list, EntryAST *entry) {
	EntryNodeAST *entryNode = (EntryNode *) malloc(sizeof(EntryNodeAST));
	entryNode->entryAST = entry;
	entryNode->next = NULL;

	if ((*list) == NULL) *list = entryNode;
	else {
		EntryNodeAST *curNode = *list;
		while (curNode->next != NULL) curNode = curNode->next;
		curNode->next = entryNode;
	}
}

EntryAST *lookup(char *name) {
	EntryAST *entry = NULL;
	Scope *current_scope = symbol_table->currentScope;

	while (current_token != NULL) {
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
	EntryNode *curNode = list;
	while (curNode != NULL) {
		if (strcmp(curNode->entry->name, name) == 0)
			return curNode->entry;
		else curNode = curNode->next;
	}
	return NULL;
}

void init_symbol_table() {
	symbolTable = (SymbolTable *) malloc(sizeof(SymbolTable));
	symbolTable->currentScope = NULL;
	symbolTable->root = NULL;
	symbolTable->globalEntryList = NULL;

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

	intType = make_int_type();
	charType = make_char_type();
	floatType = make_float_type();
	stringType = make_string_type();
	boolType = make_bool_type();
}

void clear_symbol_table() {
	free_entry(symbolTable->program);
	free_entry_list(symbolTable->globalEntryList);
	// free_scope(symbolTable->currentScope);
	free(symbolTable);
	free_type(intType);
	free_type(charType);
	free_type(floatType);
	free_type(stringType);
	free_type(boolType);
}

Scope *new_scope() {
	Scope *scope = (Scope *) malloc(sizeof(Scope));
	scope->entryList = NULL;
	scope->outerScope = symbolTable->currentScope;
	return scope;
}

void enter_scope(Scope *scope) {
	symbolTable->currentScope = scope;
}

void exit_scope() {
	symbolTable->currentScope = symbolTable->currentScope->outerScope;
}

void free_scope(Scope *scope) {
	if (scope != NULL) {
		free_entry_list(scope->entryList);
		free(scope);
		scope = NULL;
	}
}

/******************************* Create Constants ********************************/

ConstantValue *make_string_constant(char *str) {
	ConstantValue *value = (ConstantValue *) malloc(sizeof(ConstantValue));
	value->typeClass = TC_STRING;
	strcpy(value->stringVal, str);
	return value;
}

//TODO: Fix this one
void free_entry(EntryAST *entry) {
	if (entry != NULL) {
		switch(entry->entryType) {
			case ET_CONSTANT:
				if (entry->constAttrs->constantValue != NULL) {
					free(entry->constAttrs->constantValue);
					entry->constAttrs->constantValue = NULL;
				} break;
			case ET_VARIABLE:
				if (entry->varAttrs->type != NULL) {
					free(entry->varAttrs->type);
					entry->varAttrs->type = NULL;
				} break;
			case ET_TYPE_MARK:
				if (entry->typeAttrs->type != NULL) {
					free(entry->typeAttrs->type);
					entry->typeAttrs->type = NULL;
				} break;
			case ET_PROCEDURE:
				if (entry->procAttrs->scope != NULL) {
					free_scope(entry->procAttrs->scope);
					entry->procAttrs->scope = NULL;
				} break;
			case ET_PARAMTER:
				if (entry->paramAttrs->type != NULL) {
					free(entry->paramAttrs->type);
					entry->paramAttrs->type = NULL;
				} break;
			case ET_PROGRAM:
				if (entry->progAttrs->scope != NULL) {
					free_scope(entry->progAttrs->scope);
					entry->progAttrs->scope = NULL;
				} break;
			default: break;
		}
		free(entry);
		entry = NULL;
	}
}

void free_entry_list(EntryNode *node) {
	if (node != NULL) {
		free_entry(node->entry);
		free_entry_list(node->next);
		node = NULL;
	}
}

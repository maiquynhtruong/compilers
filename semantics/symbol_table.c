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

EntryAST *getBool;
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

// TODO: Should global entry added to current scope?
void declare_entry(EntryAST *entryAST) {
	assert("Declaring an entry");

	if (entryAST == NULL) return;

	if (entry->currentScope = NULL)
		add_entry(&(symbolTable->globalEntryList), entry);
	else if (entry->entryType == ET_PARAMTER) {
		EntryAST *procAST = symbolTable->currentScope->parent;
		ParamAST **paramAST = entryAST->procAST->prototype->params;
		add_entry((paramAST), entry);
	} else add_entry(&(symbolTable->currentScope->entryList), entry);
}

void add_entry(EntryNodeAST **list, EntryAST *entry) {
	EntryNodeAST *entryNode = (EntryNode *) malloc(sizeof(EntryNodeAST));
	entryNode->entryAST = entry;
	entryNode->next = NULL;

	if ((*list) == NULL) *list = node;
	else {
		EntryNodeAST *curNode = *list;
		while (curNode->next != NULL) curNode = curNode->next;
		curNode->next = entryNode;
	}
}

// find entry by name
EntryAST *find_entry(EntryNode *list, char *name) {
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
	symbolTable->program = NULL;
	symbolTable->globalEntryList = NULL;

	Entry* param;

	// built-in functions e.g. getInteger(integer val out)
	getBool = create_procedure_entry("getBool", 1);
	declare_entry(getBool);
	enter_scope(getBool->procAttrs->scope);
		param = create_parameter_entry("val");
		param->paramAttrs->type = make_bool_type();
		declare_entry(param);
	exit_scope();

	getInteger = create_procedure_entry("getInteger", 1);
	declare_entry(getInteger);
	enter_scope(getInteger->procAttrs->scope);
		param = create_parameter_entry("val");
		param->paramAttrs->type = make_int_type();
		declare_entry(param);
	exit_scope();

	getFloat = create_procedure_entry("getFloat", 1);
	declare_entry(getFloat);
	enter_scope(getFloat->procAttrs->scope);
		param = create_parameter_entry("val");
		param->paramAttrs->type = make_float_type();
		declare_entry(param);
	exit_scope();

	getString = create_procedure_entry("getString", 1);
	declare_entry(getString);
	enter_scope(getString->procAttrs->scope);
		param = create_parameter_entry("val");
		param->paramAttrs->type = make_string_type();
		declare_entry(param);
	exit_scope();

	getChar = create_procedure_entry("getChar", 1);
	declare_entry(getChar);
	enter_scope(getChar->procAttrs->scope);
		param = create_parameter_entry("val");
		param->paramAttrs->type = make_char_type();
		declare_entry(param);
	exit_scope();

	putBool = create_procedure_entry("putBool", 1);
	declare_entry(putBool);
	enter_scope(putBool->procAttrs->scope);
		param = create_parameter_entry("val", entry);
		param->paramAttrs->type = make_bool_type();
		declare_entry(param);
	exit_scope();

	putInteger = create_procedure_entry("putInteger", 1);
	declare_entry(putInteger);
	enter_scope(putInteger->procAttrs->scope);
		param = create_parameter_entry("val", entry);
		param->paramAttrs->type = make_int_type();
		declare_entry(param);
	exit_scope();

	putFloat = create_procedure_entry("putFloat", 1);
	declare_entry(putFloat);
	enter_scope(putFloat->procAttrs->scope);
		param = create_parameter_entry("val", entry);
		param->paramAttrs->type = make_float_type();
		declare_entry(param);
	exit_scope();

	putString = create_procedure_entry("putString", 1);
	declare_entry(putString);
	enter_scope(putString->procAttrs->scope);
		param = create_parameter_entry("val", entry);
		param->paramAttrs->type = make_string_type();
		declare_entry(param);
	exit_scope();

	putChar = create_procedure_entry("putChar", 1);
	declare_entry(putChar);
	enter_scope(putChar->procAttrs->scope);
		param = create_parameter_entry("val", entry);
		param->paramAttrs->type = make_char_type();
		declare_entry(param);
	exit_scope();

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

Scope *new_scope(Scope *outerScope, EntryAST *parent) {
	Scope *scope = (Scope* ) malloc(sizeof(Scope));
	scope->entryList = NULL;
	scope->outerScope = outerScope;
	scope->parent = parent;
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

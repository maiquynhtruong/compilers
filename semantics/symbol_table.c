#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "error.h"

SymbolTable *symbolTable;

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

void declare_entry(EntryAST *entry, int isGlobal) {
	assert_symbol_table("Declaring an entry: ");
	assert_symbol_table(entry->name);
	EntryAST *parent = NULL;

	if (symbolTable->currentScope == NULL || isGlobal) {
		add_entry(&(symbolTable->globalEntryList), entry);
		assert_symbol_table(" in Global scope");
	} else {
		assert_symbol_table(" in Scope ");
		assert_symbol_table(symbolTable->currentScope->parent->name);
		switch (entry->entryType) {
			case ET_VARIABLE:
				entry->varAST->scope = symbolTable->currentScope;
				break;
			case ET_PARAMTER:
				entry->paramAST->scope = symbolTable->currentScope;
				parent = symbolTable->currentScope->parent;
				add_entry(&(parent->procAST->params), entry);
				parent->procAST->paramCnt++;
				break;
			case ET_PROCEDURE:
				entry->procAST->scope->outerScope = symbolTable->currentScope;
				break;
			default: break;
		}

		add_entry(&(symbolTable->currentScope->entryList), entry);
	}
	assert_symbol_table("\n");
}

void add_entry(EntryNodeAST **list, EntryAST *entry) {
	EntryNodeAST *entryNode = (EntryNodeAST *) malloc(sizeof(EntryNodeAST));
	entryNode->entryAST = entry;
	entryNode->next = NULL;
	if ((*list) == NULL) {
		(*list) = entryNode;
	} else {
		EntryNodeAST *cur = *list;
		while (cur->next != NULL) cur = cur->next;
		cur->next = entryNode;
	}
}

EntryAST *lookup(char *name) {
	EntryAST *entry = NULL;
	Scope *curScope = symbolTable->currentScope;

	while (curScope != NULL) {

		entry = find_entry(curScope->entryList, name);
		if (entry != NULL) return entry;

		curScope = curScope->outerScope;
	}

	// search in global scope as the last resort
	entry = find_entry(symbolTable->globalEntryList, name); // globalEntryList is the outerScope of the second to last scope so no need for this

	if (entry == NULL) { assert_symbol_table("Entry "); assert_symbol_table(name); assert_symbol_table(" not found\n"); }
	else  { assert_symbol_table("Entry "); assert_symbol_table(name); assert_symbol_table(" found\n"); }

	return entry;
}

// find entry by name in a EntryNodeAST list
EntryAST *find_entry(EntryNodeAST *list, char *name) {
	assert_symbol_table("Finding entry: ");
	assert_symbol_table(name);
	assert_symbol_table("\n");

	print_current_scope();

	EntryNodeAST *curNode = list;

	while (curNode != NULL) {
		if (strcmp(curNode->entryAST->name, name) == 0)
			return curNode->entryAST;
		else curNode = curNode->next;
	}

	return NULL;
}

void init_symbol_table() {
	assert_symbol_table("Initialize a symbol table"); assert_symbol_table("\n");

	symbolTable = (SymbolTable *) malloc(sizeof(SymbolTable));
	symbolTable->currentScope = NULL;
	symbolTable->root = NULL;
	symbolTable->globalEntryList = NULL;

	// enter_scope(symbolTable->currentScope);

	// built-in functions e.g. getInteger(integer val out)
	getBool = create_builtin_function("getBool", TC_BOOL, PT_OUT); // getBool(bool val out)
	getInteger = create_builtin_function("getInteger", TC_INT, PT_OUT); // getInteger(integer val out)
	getFloat = create_builtin_function("getFloat", TC_FLOAT, PT_OUT); // getFloat(float val out)
	getString = create_builtin_function("getString", TC_STRING, PT_OUT); // getString(string val out)
	getChar = create_builtin_function("getChar", TC_CHAR, PT_OUT); // getChar(char val out)

	putBool = create_builtin_function("putBool", TC_BOOL, PT_IN); // putBool(bool val in)
	putInteger = create_builtin_function("putInteger", TC_INT, PT_IN); // putInteger(integer val in)
	putFloat = create_builtin_function("putFloat", TC_FLOAT, PT_IN); // putFloat(float val in)
	putString = create_builtin_function("putString", TC_STRING, PT_IN); // putString(string val in)
	putChar = create_builtin_function("putChar", TC_CHAR, PT_IN); // putChar(char val in)

	assert_symbol_table("Finish initializing a symbol table"); assert_symbol_table("\n");
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

Scope *create_scope(EntryAST *parent) {
	assert_symbol_table("New scope: ");
	assert_symbol_table(parent->name);
	assert_symbol_table("\n");

	Scope *scope = (Scope *) malloc(sizeof(Scope));
	scope->entryList = NULL;
	scope->parent = parent;
	scope->outerScope = NULL;
	return scope;
}

void enter_scope(Scope *scope) {
	assert_symbol_table("Enter a scope: ");
	assert_symbol_table(scope->parent->name);
	assert_symbol_table("\n");

	symbolTable->currentScope = scope;

	print_current_scope();
}

void exit_scope() {
	assert_symbol_table("Exit a scope\n");

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

void print_current_scope() {
	assert_symbol_table("Current scope is ");
	assert_symbol_table(symbolTable->currentScope->parent->name);
	assert_symbol_table("\n");
}

void print_entry_type(EntryAST *entry) {
	TypeClass type = TC_INVALID;
	switch (entry->entryType) {
		case ET_VARIABLE:
			type = entry->varAST->varType; break;
		case ET_FACTOR:
			type = entry->factorAST->typeClass; break;
		case ET_PARAMTER:
			type = entry->paramAST->type; break;
		case ET_PROCEDURE:
			type = TC_INVALID;
	}
	print_type(type);
}

void print_type(TypeClass type) {
    switch (type) {
        case TC_INT:
            printf("Integer"); break;
    	case TC_FLOAT:
            printf("Float"); break;
    	case TC_STRING:
            printf("String"); break;
    	case TC_BOOL:
            printf("Bool"); break;
    	case TC_CHAR:
            printf("Char"); break;
		case TC_INVALID:
			printf("Invalid"); break;
        default:
            printf("Unknown"); break;
    }
}

EntryAST *create_builtin_function(char *name, TypeClass varType, ParamType paramType) {
	assert_symbol_table("Create builtin function: ");
	assert_symbol_table(name); assert_symbol_table("\n");

	EntryAST *func = create_procedure(name);
	declare_entry(func, 1);
	enter_scope(func->procAST->scope);
		EntryAST *paramEntry = create_param("val", paramType);
		paramEntry->paramAST->type = varType;
		declare_entry(paramEntry, 0);
	exit_scope();

	return func;
}

EntryAST *create_program(char *name) {
	assert_symbol_table("Creating a program entry\n");

	EntryAST *progEntry = (EntryAST *) malloc(sizeof(EntryAST));
	strcpy(progEntry->name, name);
	progEntry->entryType = ET_PROGRAM;
	ProgramAST *program = (ProgramAST *) malloc(sizeof(ProgramAST));
	program->scope = create_scope(progEntry);
	progEntry->progAST = program;
	symbolTable->root = progEntry;
	return progEntry;
}

EntryAST *create_variable(char *name) {
	assert_symbol_table("Creating a variable entry: ");
	assert_symbol_table(name); assert_symbol_table("\n");

	EntryAST *varEntry = (EntryAST *) malloc(sizeof(EntryAST));
	varEntry->entryType = ET_VARIABLE;
	VariableAST *var = (VariableAST *) malloc(sizeof(VariableAST));
	strcpy(varEntry->name, name);
	var->varType = TC_INVALID;
	var->scope = NULL;
	var->size = 0;
	// var->value = NULL;
	varEntry->varAST = var;
	return varEntry;
}

EntryAST *create_param(char *name, ParamType paramType) {
	assert_symbol_table("Creating a param entry: ");
	assert_symbol_table(name); assert_symbol_table("\n");

	EntryAST *paramEntry = (EntryAST *) malloc(sizeof(EntryAST));
	strcpy(paramEntry->name, name);
	paramEntry->entryType = ET_PARAMTER;
	ParamAST *param = (ParamAST *) malloc(sizeof(ParamAST));
	param->paramType = paramType;
	paramEntry->paramAST = param;
	return paramEntry;
}

EntryAST *create_procedure(char *name) {
	assert_symbol_table("Creating a procedure entry: ");
	assert_symbol_table(name); assert_symbol_table("\n");

	EntryAST *procEntry = (EntryAST *) malloc(sizeof(EntryAST));
	strcpy(procEntry->name, name);
	procEntry->entryType = ET_PROCEDURE;
	ProcedureAST *proc = (ProcedureAST *) malloc(sizeof(ProcedureAST));
	proc->params = NULL;
	proc->paramCnt = 0;
	proc->scope = create_scope(procEntry);
	procEntry->procAST = proc;
	return procEntry;
}

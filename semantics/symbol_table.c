#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "error.h"

SymbolTable *symbolTable;
Type *intType;
Type *charType;
Type *floatType;
Type *stringType;
Type *boolType;

/******************************* Make Types ********************************/
Type *make_int_type() {
	Type *type = (Type *) malloc(sizeof(Type));
	type->typeClass = TC_INT;
	return type;
}

Type *make_char_type() {
	Type *type = (Type *) malloc(sizeof(Type));
	type->typeClass = TC_CHAR;
	return type;
}

Type *make_float_type() {
	Type *type = (Type *) malloc(sizeof(Type));
	type->typeClass = TC_FLOAT;
	return type;
}

Type *make_string_type() {
	Type *type = (Type *) malloc(sizeof(Type));
	type->typeClass = TC_STRING;
	return type;
}

Type *make_bool_type() {
	Type *type = (Type *) malloc(sizeof(Type));
	type->typeClass = TC_BOOL;
	return type;
}

Type *make_array_type(int size, Type *elementType) {
	Type *type = (Type *) malloc(sizeof(Type));
	type->typeClass = TC_ARRAY;
	type->arraySize = size;
	type->elementType = elementType;
	return type;
}

int compare_type(Type *type1, Type *type2) {
	if (type1->typeClass == type2->typeClass) {
		if (type1->typeClass == TC_ARRAY) {
			if (type1->arraySize == type2->arraySize) {
				return compare_type(type1->elementType, type2->elementType);
			} else return -1;
		} else return 1;
	} else return -1;
}

void free_type(Type *type) {
	free(type);
}

/******************************* Create Constants ********************************/

ConstantValue *make_int_constant(int i) {
	ConstantValue *value = (ConstantValue *) malloc(sizeof(ConstantValue));
	value->typeClass = TC_INT;
	value->intVal = i;
	return value;
}

ConstantValue *make_char_constant(char c) {
	ConstantValue *value = (ConstantValue *) malloc(sizeof(ConstantValue));
	value->typeClass = TC_CHAR;
	value->charVal = c;
	return value;
}

ConstantValue *make_float_constant(float f) {
	ConstantValue *value = (ConstantValue *) malloc(sizeof(ConstantValue));
	value->typeClass = TC_FLOAT;
	value->floatVal = f;
	return value;
}

ConstantValue *make_bool_constant(bool b) {
	ConstantValue *value = (ConstantValue *) malloc(sizeof(ConstantValue));
	value->typeClass = TC_BOOL;
	value->boolVal = b;
	return value;
}

ConstantValue *make_string_constant(char *str) {
	ConstantValue *value = (ConstantValue *) malloc(sizeof(ConstantValue));
	value->typeClass = TC_STRING;
	strcpy(value->stringVal, str);
	return value;
}

/******************************* Create Entries ********************************/

Entry *create_program_entry(char *name) {
	assert("Create program entry");
	assert(name);

	Entry *programEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(programEntry->name, name);
	programEntry->entryType = ET_PROGRAM;
	programEntry->global = 1; // TODO: Not sure
	programEntry->progAttrs = (ProgramAttributes *) malloc(sizeof(ProgramAttributes));
	programEntry->progAttrs->scope = new_scope(NULL, programEntry);
	symbolTable->program = programEntry;
	return programEntry;
}

Entry *create_type_entry(char *name) {
	assert("Create type entry");
	assert(name);

	Entry *typeEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(typeEntry->name, name);
	typeEntry->entryType = ET_TYPE_MARK;
	typeEntry->global = 0;
	typeEntry->typeAttrs = (TypeAttributes *) malloc(sizeof(TypeAttributes));
	typeEntry->typeAttrs->type = NULL;
	return typeEntry;
}

Entry *create_constant_entry(char *name) {
	assert("Create constant entry");
	assert(name);

	Entry *constEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(constEntry->name, name);
	constEntry->entryType = ET_CONSTANT;
	constEntry->global = 0;
	constEntry->constAttrs = (ConstantValueAttributes *) malloc(sizeof(ConstantValueAttributes));
	constEntry->constAttrs->constantValue = NULL;
	return constEntry;
}

Entry *create_variable_entry(char *name, int global) {
	assert("Create variable entry");
	assert(name);

	Entry *variableEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(variableEntry->name, name);
	variableEntry->entryType = ET_VARIABLE;
	variableEntry->global = global;
	variableEntry->varAttrs = (VariableAttributes *) malloc(sizeof(VariableAttributes));
	variableEntry->varAttrs->type = NULL;
	variableEntry->varAttrs->scope = symbolTable->currentScope;
	return variableEntry;
}

Entry *create_procedure_entry(char *name, int global) {
	assert("Create procedure entry");
	assert(name);

	Entry *procedureEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(procedureEntry->name, name);
	procedureEntry->entryType = ET_PROCEDURE;
	procedureEntry->global = global;
	procedureEntry->procAttrs = (ProcedureAttributes *) malloc(sizeof(ProcedureAttributes));
	procedureEntry->procAttrs->paramList = NULL;
	procedureEntry->procAttrs->scope = new_scope(symbolTable->currentScope, procedureEntry);
	return procedureEntry;
}

Entry *create_parameter_entry(char *name, Entry *procedure) {
	assert("Create parameter entry");
	assert(name);

	Entry *parameterEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(parameterEntry->name, name);
	parameterEntry->entryType = ET_PARAMTER;
	parameterEntry->global = 0;
	parameterEntry->paramAttrs = (ParameterAttributes *) malloc(sizeof(ParameterAttributes));
	parameterEntry->paramAttrs->type = NULL;
	parameterEntry->paramAttrs->procedure = procedure;
	return parameterEntry;
}

//TODO: Fix this one
void free_entry(Entry *entry) {
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

/* list: current scope */
// https://www.tutorialspoint.com/compiler_design/compiler_design_symbol_table.htm
void add_entry(EntryNode **list, Entry *entry) {
	EntryNode *node = (EntryNode *) malloc(sizeof(EntryNode));
	node->entry = entry;
	node->next = NULL;
	if ((*list) == NULL) *list = node;
	else {
		EntryNode *curNode = *list;
		while (curNode->next != NULL) curNode = curNode->next;
		curNode->next = node;
	}
}

// find entry by name
Entry *find_entry(EntryNode *list, char *name) {
	EntryNode *curNode = list;
	while (curNode != NULL) {
		if (strcmp(curNode->entry->name, name) == 0)
			return curNode->entry;
		else curNode = curNode->next;
	}
	return NULL;
}

/******************************* Symbol table functions ********************************/

void init_symbol_table() {
	symbolTable = (SymbolTable *) malloc(sizeof(SymbolTable));
	// symbolTable->currentScope = new_scope(NULL, NULL);
	// symbolTable->program = NULL;
	symbolTable->globalEntryList = NULL;

	Entry *entry;
	Entry* param;

	// built-in functions e.g. getInteger(integer val out)
	entry = create_procedure_entry("getBool", 1);
	param = create_parameter_entry("val", entry);
	param->paramAttrs->type = make_bool_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("getInteger", 1);
	param = create_parameter_entry("val", entry);
	param->paramAttrs->type = make_int_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("getFloat", 1);
	param = create_parameter_entry("val", entry);
	param->paramAttrs->type = make_float_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("getString", 1);
	param = create_parameter_entry("val", entry);
	param->paramAttrs->type = make_string_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("getChar", 1);
	param = create_parameter_entry("val", entry);
	param->paramAttrs->type = make_char_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("putBool", 1);
	param = create_parameter_entry("val", entry);
	param->paramAttrs->type = make_bool_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("putInteger", 1);
	param->paramAttrs->type = make_int_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("putFloat", 1);
	param->paramAttrs->type = make_float_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("putString", 1);
	param->paramAttrs->type = make_string_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("putChar", 1);
	param->paramAttrs->type = make_char_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

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

Scope *new_scope(Scope *outerScope, Entry *parent) {
	Scope *scope = (Scope* ) malloc(sizeof(Scope));
	scope->entryList = NULL;
	scope->outerScope = outerScope;
	scope->parent = parent;
	return scope;
}

void enter_scope(Scope *scope) {
	symbolTable->currentScope = scope;
}

Entry* lookup(char *name) {
	// look up through the parent nodes
	Scope *curScope = symbolTable->currentScope;
	Entry *entry = NULL;
	while (curScope != NULL) {
		entry = find_entry(curScope->entryList, name);
		if (entry != NULL) return entry;
		else curScope = curScope->outerScope;
	}
	// if still couldn't find, search in global scope
	entry = find_entry(symbolTable->globalEntryList, name);
	return entry;
}

void exit_scope() {
	symbolTable->currentScope = symbolTable->currentScope->outerScope;
}

void dump() {

}

void free_scope(Scope *scope) {
	if (scope != NULL) {
		free_entry_list(scope->entryList);
		free(scope);
		scope = NULL;
	}
}
// TODO: Should global entry added to current scope?
void declare_entry(Entry *entry) {
	assert("Declaring an entry");
	if (entry->global) add_entry(&(symbolTable->globalEntryList), entry);
	else if (entry->entryType == ET_PARAMTER) {
		Entry *procedureEntry = symbolTable->currentScope->parent;
		add_entry(&(procedureEntry->procAttrs->paramList), entry);
	} else add_entry(&(symbolTable->currentScope->entryList), entry);
}

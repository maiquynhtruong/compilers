// https://www.tutorialspoint.com/compiler_design/compiler_design_symbol_table.htm

#include "symbol_table.h"

SymbolTable *symbolTable;
Type *intType;
Type *charType;
Type *floatType;
Type *stringType;
Type *boolType;

/* Type functions */
Type *makeIntType() {
	Type *type = (Type *) malloc(sizeof(Type));
	type->typeClass = TC_INT;
	return type;
}

Type *makeCharType() {
	Type *type = (Type *) malloc(sizeof(Type));
	type->typeClass = TC_CHAR;
	return type;
}

Type *makeFloatType() {
	Type *type = (Type *) malloc(sizeof(Type));
	type->typeClass = TC_FLOAT;
	return type;
}

Type *makeStringType() {
	Type *type = (Type *) malloc(sizeof(Type));
	type->typeClass = TC_STRING;
	return type;
}

Type *makeBoolType() {
	Type *type = (Type *) malloc(sizeof(Type));
	type->typeClass = TC_BOOL;
	return type;
}

Type *makeArrayType() {

}

int compareType(Type *type1, Type *type2) {
	if (type1->typeClass == type2->typeClass) {
		if (type1->typeClass == TC_ARRAY) {
			if (type1->arraySize == type2->arraySize) {
				return compareType(type1->elementType, type2->elementType);
			} else return -1;
		} else return 1;
	} else return -1;
}

void freeType(Type *type) {

}
/* Entry functions */

Entry *createProgramEntry(char *name) {
	Entry *programEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(programEntry->name, name);
	programEntry->entryType = ET_PROGRAM;
	programEntry->progAttrs = (progAttrs *) malloc(sizeof(ProgramAttributes));
	// program->progAttrs->scope = new_scope()
	return programEntry;
}

Entry *createTypeEntry(char *name) {
	Entry *typeEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(typeEntry->name, name);
	typeEntry->typeAttrs = (TypeAttributes *) malloc(sizeof(TypeAttributes));
	// typeEntry->typeAttrs->type = ;
	return typeEntry;
}

Entry *createVariableEntry(char *name) {
	Entry *variableEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(variableEntry->name, name);
	variableEntry->varAttrs = (VariableAttributes *) malloc(sizeof(VariableAttributes));
	// varEntry->varAttrs->type = ;
	variableEntry->varAttrs->scope = symbolTable->currentScope;
	return variableEntry;
}

Entry *createProcedureEntry(char *name) {
	Entry *procedureEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(procedureEntry->name, name);
	procedureEntry->procAttrs = (ProcedureAttributes *) malloc(sizeof(ProcedureAttributes));
	procedureEntry->procAttrs->paramList = NULL;
	// procedureEntry->procAttrs->scope = symbolTable->currentScope;
	return procedureEntry;
}

Entry *createParameterEntry(char *name, Entry *procedure) {
	Entry *parameterEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(parameterEntry->name, name);
	parameterEntry->paramAttrs = (ParameterAttributes *) malloc(sizeof(ParameterAttributes));	
	// parameterEntry->paramAttrs->type = ;
	parameterEntry->paramAttrs->procedure = procedure; 
	return parameterEntry;
}

void freeEntry(Entry *entry) {

}

void freeEntryList(EntryNode *node) {

}

void addEntry(EntryNode **list, Entry *entry) {
	EntryNode *node = (EntryNode *) malloc(sizeof(EntryNode));
	node->entry = entry;
	node->next = NULL;
	if ((*list) == NULL) list = node;
	else {
		EntryNode *curNode = *list;
		while (curNode->next != NULL) curNode = curNode->next;
		curNode->next = node;
	}
}

// find entry by name
Entry *findEntry(EntryNode *list, char *name) {
	while (list != NULL) {
		if (strcmp(list->entry->name, name) == 0) 
			return list->entry;
		else list = list->next;
	}
	return NULL;
}

/* Symbol table functions */

void init_symbol_table() {
	symbolTable = (SymbolTable *) malloc(sizeof(SymbolTable));
	symbolTable->currentScope = new_scope(NULL); // shoudl first layer scope point to global scope?
	symbolTable->program = NULL;
	symbolTable->globalScope = new_scope(NULL);
}

void clear_symbol_table() {
	freeEntry(symbolTable->program);
	freeEntryList(symbolTable->globalScope);
	freeScope(currentScope);
	free(symbolTable);
	freeType(intType);
	freeType(charType);
	freeType(floatType);
	freeType(stringType);
	freeType(boolType);
}

Scope *new_scope(Scope *outerScope) {
	Scope *scope = (Scope* ) malloc(sizeof(Scope));
	scope->entryList = NULL;
	scope->outerScope = outerScope;
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
		entry = findEntry(curScope->entryList, name);
		if (entry != NULL) return entry;
		else curScope = curScope->outerScope;
	}
	// if still couldn't find, search in global scope
	entry = findEntry(symbolTable->globalScope->entryList, name);
	return entry;
}

void exit_scope() {
	symbolTable->currentScope = symbolTable->currentScope->outerScope;
}

void dump()

void freeScope(Scope *scope) {

}
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
	Type *type = (Type *) malloc(sizeof(Type));
	type->typeClass = TC_ARRAY;
	return type;
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
	if (entry != NULL) {
		switch(entry->entryType) {
			case ET_VARIABLE:
				if (entry->varAttrs->type != NULL) {
					free(entry->varAttrs->type);
					entry->varAttrs->type = NULL;
				}
				break;
			case ET_TYPE_MARK:
				if (entry->typeAttrs->type != NULL) {
					free(entry->typeAttrs->type);
					entry->typeAttrs->type = NULL;
				}
				break;
			case ET_PROCEDURE:
				if (entry->procAttrs->scope != NULL) {
					freeScope(entry->procAttrs->scope);
					entry->procAttrs->scope = NULL;
				}
				break;
			case ET_PARAMTER:
				if (entry->paramAttrs->type != NULL) {
					free(entry->paramAttrs->type);
					entry->paramAttrs->type = NULL;
				}
				break;
			case ET_PROGRAM:
				if (entry->progAttrs->scope != NULL) {
					freeScope(entry->progAttrs->scope);
					entry->progAttrs->scope = NULL;
				}
				break;
			default:
				break;
		}
		free(entry);
		entry = NULL;
	}
}

void freeEntryList(EntryNode *node) {
	if (node != NULL) {
		freeEntry(node->entry);
		freeEntryList(node->next);
		node = NULL;
	}
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
	symbolTable->globalEntryList = NULL;

	Entry *entry;
	Entry* param;

	// built-in functions
	// e.g. getInteger(integer val out)
	entry = createProcedureEntry("getBool");
	param = createParameterEntry("val");
	param->paramAttrs->type = makeBoolType();
	addEntry(&(entry->procAttrs->paramList), param);
	addEntry(&(symbolTable->globalEntryList), entry);

	entry = createProcedureEntry("getInteger");
	param = createParameterEntry("val");
	param->paramAttrs->type = makeIntType();
	addEntry(&(entry->procAttrs->paramList), param);
	addEntry(&(symbolTable->globalEntryList), entry);

	entry = createProcedureEntry("getFloat");
	param = createParameterEntry("val");
	param->paramAttrs->type = makeFloatType();
	addEntry(&(entry->procAttrs->paramList), param);
	addEntry(&(symbolTable->globalEntryList), entry);

	entry = createProcedureEntry("getString");
	param = createParameterEntry("val");
	param->paramAttrs->type = makeStringType();
	addEntry(&(entry->procAttrs->paramList), param);
	addEntry(&(symbolTable->globalEntryList), entry);

	entry = createProcedureEntry("getChar");
	param = createParameterEntry("val");
	param->paramAttrs->type = makeCharType();
	addEntry(&(entry->procAttrs->paramList), param);
	addEntry(&(symbolTable->globalEntryList), entry);

	entry = createProcedureEntry("putBool");
	param = createParameterEntry("val");
	param->paramAttrs->type = makeBoolType();
	addEntry(&(entry->procAttrs->paramList), param);
	addEntry(&(symbolTable->globalEntryList), entry);

	entry = createProcedureEntry("putInteger");
	param->paramAttrs->type = makeIntType();
	addEntry(&(entry->procAttrs->paramList), param);
	addEntry(&(symbolTable->globalEntryList), entry);

	entry = createProcedureEntry("putFloat");
	param->paramAttrs->type = makeFloatType();
	addEntry(&(entry->procAttrs->paramList), param);
	addEntry(&(symbolTable->globalEntryList), entry);

	entry = createProcedureEntry("putString");
	param->paramAttrs->type = makeStringType();
	addEntry(&(entry->procAttrs->paramList), param);
	addEntry(&(symbolTable->globalEntryList), entry);

	entry = createProcedureEntry("putChar");
	param->paramAttrs->type = makeCharType();
	addEntry(&(entry->procAttrs->paramList), param);
	addEntry(&(symbolTable->globalEntryList), entry);

	symbolTable->globalEntryList
}

void clear_symbol_table() {
	freeEntry(symbolTable->program);
	freeEntryList(symbolTable->globalEntryList);
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
	entry = findEntry(symbolTable->globalEntryList, name);
	
	return entry;
}

void exit_scope() {
	symbolTable->currentScope = symbolTable->currentScope->outerScope;
}

void dump() {

}

void freeScope(Scope *scope) {
	if (scope != NULL) {
		freeEntryList(scope->entryList);
		free(scope);
		scope = NULL;
	}
}
// for declarations
void declareEntry(Entry *entry) {
	addEntry(&(symbolTable->currentScope->entryList), entry);
}

void declareGlobalEntry(Entry *entry) {
	addEntry(&(symbolTable->globalEntryList), entry);
}
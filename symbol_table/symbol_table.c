// https://www.tutorialspoint.com/compiler_design/compiler_design_symbol_table.htm

#include "symbol_table.h"

SymbolTable *symbolTable;
Type *intType;
Type *charType;
Type *floatType;
Type *stringType;
Type *boolType;

/* Type functions */
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

Type *make_array_type() {

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
/* Entry functions */

Entry *create_program_entry(char *name) {
	Entry *programEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(programEntry->name, name);
	programEntry->entryType = ET_PROGRAM;
	programEntry->progAttrs = (progAttrs *) malloc(sizeof(ProgramAttributes));
	// program->progAttrs->scope = new_scope()
	return programEntry;
}

Entry *create_type_entry(char *name) {
	Entry *typeEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(typeEntry->name, name);
	typeEntry->typeAttrs = (TypeAttributes *) malloc(sizeof(TypeAttributes));
	// typeEntry->typeAttrs->type = ;
	return typeEntry;
}

Entry *create_variable_entry(char *name) {
	Entry *variableEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(variableEntry->name, name);
	variableEntry->varAttrs = (VariableAttributes *) malloc(sizeof(VariableAttributes));
	// varEntry->varAttrs->type = ;
	variableEntry->varAttrs->scope = symbolTable->currentScope;
	return variableEntry;
}

Entry *create_procedure_entry(char *name) {
	Entry *procedureEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(procedureEntry->name, name);
	procedureEntry->procAttrs = (ProcedureAttributes *) malloc(sizeof(ProcedureAttributes));
	procedureEntry->procAttrs->paramList = NULL;
	// procedureEntry->procAttrs->scope = symbolTable->currentScope;
	return procedureEntry;
}

Entry *create_parameter_entry(char *name, Entry *procedure) {
	Entry *parameterEntry = (Entry *) malloc(sizeof(Entry));
	strcpy(parameterEntry->name, name);
	parameterEntry->paramAttrs = (ParameterAttributes *) malloc(sizeof(ParameterAttributes));
	// parameterEntry->paramAttrs->type = ;
	parameterEntry->paramAttrs->procedure = procedure;
	return parameterEntry;
}

void free_entry(Entry *entry) {
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
					free_scope(entry->procAttrs->scope);
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
					free_scope(entry->progAttrs->scope);
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

void free_entry_list(EntryNode *node) {
	if (node != NULL) {
		freeEntry(node->entry);
		free_entry_list(node->next);
		node = NULL;
	}
}

void add_entry(EntryNode **list, Entry *entry) {
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
Entry *find_entry(EntryNode *list, char *name) {
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
	entry = create_procedure_entry("getBool");
	param = create_parameter_entry("val");
	param->paramAttrs->type = make_bool_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("getInteger");
	param = create_parameter_entry("val");
	param->paramAttrs->type = make_int_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("getFloat");
	param = create_parameter_entry("val");
	param->paramAttrs->type = make_float_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("getString");
	param = create_parameter_entry("val");
	param->paramAttrs->type = make_string_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("getChar");
	param = create_parameter_entry("val");
	param->paramAttrs->type = make_char_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("putBool");
	param = create_parameter_entry("val");
	param->paramAttrs->type = make_bool_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("putInteger");
	param->paramAttrs->type = makeIntType();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("putFloat");
	param->paramAttrs->type = make_float_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("putString");
	param->paramAttrs->type = make_string_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	entry = create_procedure_entry("putChar");
	param->paramAttrs->type = make_char_type();
	add_entry(&(entry->procAttrs->paramList), param);
	add_entry(&(symbolTable->globalEntryList), entry);

	symbolTable->globalEntryList
}

void clear_symbol_table() {
	free_entry(symbolTable->program);
	free_entry_list(symbolTable->globalEntryList);
	free_scope(currentScope);
	free(symbolTable);
	free_type(intType);
	free_type(charType);
	free_type(floatType);
	free_type(stringType);
	free_type(boolType);
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
// for declarations
void declare_entry(Entry *entry) {
	add_entry(&)
}

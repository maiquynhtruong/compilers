// https://www.tutorialspoint.com/compiler_design/compiler_design_symbol_table.htm

#include "symbol_table.h"

SymbolTable *symbolTable;

/* Symbol table functions */
void init_symbol_table() {
	symbolTable = (SymbolTable *) malloc(sizeof(SymbolTable));
	symbolTable->currentScope = new_scope(NULL); // shoudl first layer scope point to global scope?
	symbolTable->program = NULL;
	symbolTable->globalScope = new_scope(NULL);
}

void clear_symbol_table() {

	}

Scope *new_scope(Scope *outerScope);

void enter_scope(Scope *scope) {
	symbolTable->currentScope = scope;
}

Entry* lookup(char *name) {
	// look up through the parent nodes
	EntryNode *currentNode = symbolTable->currentScope->entryList;
	while (currentNode != NULL) {
		if (strcmp(currentNode->entry->name, name) == 0) return currentNode->entry;
		currentNode = currentNode->next;
	}
	// if still couldn't find, search in global scope

	// unable to find, return nothing
}

void exit_scope() {

}
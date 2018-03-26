// https://www.tutorialspoint.com/compiler_design/compiler_design_symbol_table.htm

#include "symbol_table.h"

SymbolTable *symbolTable;

/* Symbol table functions */

Scope *new_scope()
void enter_scope(Scope *scope) {
	symbolTable->currentScope = scope;
}

Entry* lookup(EntryNodeList* headNode, char *name) {
	// look up "name" starting the first node in the list
}

void exit_scope() {

}
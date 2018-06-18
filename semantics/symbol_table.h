#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_STRING_LENGTH 50
#define MAX_IDENT_LENGTH 15

#include <stdbool.h>
#include "ast.h"

/* outerScope:  each scope is a list of entries a scope also keeps a pointer to
its parent scope so a variable can be searched upward
parent: the Entry that upon creating it we also need to create a new scope
(e.g. procedure and program)
*/
typedef struct Scope {
	EntryNodeAST* entryList;
	// struct EntryAST *parent;
	struct Scope *outerScope;
} Scope;

typedef struct SymbolTable {
	struct Scope *currentScope;
	struct EntryAST *root; // root is program
	struct EntryNodeAST* globalEntryList;
} SymbolTable;

void declare_entry(EntryAST *entry);
EntryAST *find_entryAST(char *name);
void add_entryAST(EntryNodeAST **list, EntryAST *entry);

void init_symbol_table();
void clear_symbol_table();
// Scope *new_scope(Scope *outerScope, Entry *parent);
Scope *new_scope();
void enter_scope(Scope *scope);
void exit_scope();
void free_scope(Scope *scope);

void free_entryAST(EntryAST *entry);
void free_entryAST_list(EntryNodeAST *node);
#endif

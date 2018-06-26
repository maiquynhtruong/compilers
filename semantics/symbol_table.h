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
	// struct EntryAST *currentBlock; // the block that started this scope
	struct Scope *outerScope;
} Scope;

typedef struct SymbolTable {
	struct Scope *currentScope;
	struct EntryAST *root; // root is program
	struct EntryNodeAST* globalEntryList;
} SymbolTable;

EntryAST *lookup(char *name);
EntryAST *find_entry(EntryNodeAST *list, char *name);
void declare_entry(EntryAST *entryAST, int isGlobal);
void add_entry(EntryNodeAST **list, EntryAST *entry);

void init_symbol_table();
void clear_symbol_table();
// Scope *new_scope(Scope *outerScope, Entry *parent);
Scope *new_scope();
void enter_scope(Scope *scope);
void exit_scope();
void free_scope(Scope *scope);

void free_entry(EntryAST *entry);
void free_entry_list(EntryNodeAST *node);

void print_scope(Scope *scope);
void print_symbol_table();
#endif

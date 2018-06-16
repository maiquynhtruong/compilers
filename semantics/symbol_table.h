#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_STRING_LENGTH 50
#define MAX_IDENT_LENGTH 15

#include <stdbool.h>
#include "ast.h"

struct SymbolTable {
	struct Scope *currentScope;
	struct EntryAST *root; // root is program
	struct EntryNodeAST* globalEntryList;
};

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

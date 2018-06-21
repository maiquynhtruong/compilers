#ifndef DEBUG_H
#define DEBUG_H

#include "symbol_table.h"

void print_type(TypeAST *type);
void print_entry(EntryAST *entry);
void print_entry_list(EntryNodeAST *entryList);
void print_scope(Scope *scope);

#endif

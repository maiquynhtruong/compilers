#ifndef DEBUG_H
#define DEBUG_H

#include "symbol_table.h"

void print_type_class(Type *type);
void print_constant_value(ConstantValue *value);
void print_entry(Entry *entry);
void print_entry_list(EntryNode *entryList);
void print_scope(Scope *scope);

#endif

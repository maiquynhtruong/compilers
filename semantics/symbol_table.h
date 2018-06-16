#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_STRING_LENGTH 50
#define MAX_IDENT_LENGTH 15

#include <stdbool.h>
#include "ast.h"



typedef enum {
	PT_IN,
	PT_OUT,
	PT_INOUT
} ParamType;

/* outerScope:  each scope is a list of entries a scope also keeps a pointer to
its parent scope so a variable can be searched upward
parent: the Entry that upon creating it we also need to create a new scope
(e.g. procedure and program)
*/

// instead of one global symbol table and one scope symbol table
// we have one global scope and a list of scopes

struct SymbolTable {
	struct Scope *currentScope;
	struct Entry *program; // root is program
	struct EntryNode* globalEntryList;
};

typedef struct ConstantValue ConstantValue;
typedef struct Type Type;
typedef struct Entry Entry;
typedef struct EntryNode EntryNode;
typedef struct Scope Scope;
typedef struct SymbolTable SymbolTable;

Type *make_int_type();
Type *make_char_type();
Type *make_float_type();
Type *make_string_type();
Type *make_bool_type();
Type *make_array_type(int size, Type *type);
int compare_type(Type *type1, Type *type2);
void free_type(Type *type);

void init_symbol_table();
void clear_symbol_table();
Scope *new_scope(Scope *outerScope, Entry *parent);
void enter_scope(Scope *scope);
void exit_scope();
void dump();
void free_scope(Scope *scope);
void add_entry(EntryNode **list, Entry *entry);
void declare_entry(Entry *entry);

#endif

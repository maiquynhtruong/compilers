#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_STRING_LENGTH 50
#define MAX_IDENT_LENGTH 15

#include <stdbool.h>

typedef enum {
	TC_INT,
	TC_FLOAT,
	TC_STRING,
	TC_BOOL,
	TC_CHAR,
	TC_ARRAY
} TypeClass;

typedef enum {
	ET_CONSTANT,
	ET_VARIABLE,
	ET_TYPE_MARK,
	ET_PROCEDURE,
	ET_PARAMTER,
	ET_PROGRAM
} EntryType;

struct ConstantValue;
struct Type;
struct Entry;
struct EntryNode;
struct Scope;

typedef struct ConstantValueAttributes {
	struct ConstantValue *constantValue;
} ConstantValueAttributes;

typedef struct VariableAttributes {
	struct Type *type;
	struct Scope *scope;
} VariableAttributes;

typedef struct TypeAttributes {
	struct Type *type;
} TypeAttributes;

typedef struct ProcedureAttributes {
	struct EntryNode* paramList;
	struct Scope *scope;
} ProcedureAttributes;

typedef struct ProgramAttributes {
	struct Scope *scope;
} ProgramAttributes;

typedef struct ParameterAttributes {
	struct Type *type;
	struct Entry *procedure;
} ParameterAttributes;

struct ConstantValue {
	TypeClass typeClass;
	union {
		char stringVal[MAX_STRING_LENGTH+1];
        int intVal;
        float floatVal;
        bool boolVal;
        char charVal;
	};
};

struct Type {
	TypeClass typeClass;
	int arraySize;
	struct Type *elementType;
};

// an entry: <name, type, attribute>
struct Entry {
	char name[MAX_IDENT_LENGTH];
	EntryType entryType;
	union {
		ConstantValueAttributes *constAttrs;
		VariableAttributes *varAttrs;
		TypeAttributes *typeAttrs;
		ProcedureAttributes *procAttrs;
		ProgramAttributes *progAttrs;
		ParameterAttributes *paramAttrs;
	};
};

// to make life easier when defining a chain of nodes
struct EntryNode {
	struct Entry *entry;
	struct EntryNode *next;
};

// each scope is a list of entries
// a scope also keeps a pointer to its parent scope so
// a variable can be search upward
struct Scope {
	struct EntryNode* entryList;
	struct Scope *outerScope;
};

typedef struct ConstantValue ConstantValue;
typedef struct Type Type;
typedef struct Entry Entry;
typedef struct EntryNode EntryNode;
typedef struct Scope Scope;

// instead of one global symbol table and one scope symbol table
// we have one global scope and a list of scopes
// https://www.tutorialspoint.com/compiler_design/compiler_design_symbol_table.htm
typedef struct SymbolTable {
	Scope *currentScope;
	Entry *program; // root is program
	EntryNode* globalEntryList;
} SymbolTable;

Type *make_int_type();
Type *make_char_type();
Type *make_float_type();
Type *make_string_type();
Type *make_bool_type();
Type *make_array_type();
int compare_type(Type *type1, Type *type2);
void free_type(Type *type);

Entry *create_program_entry(char *name);
Entry *create_type_entry(char *name);
Entry *create_variable_entry(char *name);
Entry *create_procedure_entry(char *name);
Entry *create_parameter_entry(char *name, Entry* procedure);
Entry *find_entry(EntryNode *list, char *name);
void free_entry(Entry *entry);
void free_entry_list(EntryNode *node);
void add_entry(EntryNode **list, Entry *entry);

void init_symbol_table();
void clear_symbol_table();
Scope *new_scope(Scope *outerScope);
void enter_scope(Scope *scope);
void exit_scope();
Entry* lookup(char *name);
void dump();
void free_scope(Scope *scope);
void add_entry(EntryNode **list, Entry *entry);
void declare_entry(Entry *entry);

#endif

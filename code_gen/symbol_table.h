#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_STRING_LENGTH 50
#define MAX_IDENT_LENGTH 15

#include <stdbool.h>
#include <llvm-c/Core.h>

typedef enum ParamType {
	PT_IN,
	PT_OUT,
	PT_INOUT
} ParamType;

typedef enum EntryType {
	ET_VARIABLE, ET_PROCEDURE, ET_PARAMTER, ET_PROGRAM,
	ET_FACTOR, ET_TYPE_MARK, ET_ARRAY,
} EntryType;

typedef enum TypeClass {
	TC_INT,
	TC_FLOAT,
	TC_STRING,
	TC_BOOL,
	TC_CHAR,
	TC_VOID,
	TC_INVALID
} TypeClass;

struct Scope;
struct EntryNodeAST;
struct EntryAST;

typedef struct FactorAST {
	TypeClass typeClass;
	union {
		char stringVal[MAX_STRING_LENGTH+1];
        int intVal;
        float floatVal;
        bool boolVal;
        char charVal;
	};
} FactorAST;

/*
size: 0 if variable, positive if array
This is the same as removing arraySize and elementType out of Type. It makes sense to not have to
a separate elementType to represent type of elements in the array (they should both be the same)
It also makes more sense to let the variable holds the size info
*/
typedef struct VariableAST {
	struct Scope *scope;
	TypeClass varType;
	unsigned int size;
} VariableAST;

typedef struct ProcedureAST {
	struct Scope *scope;
	struct EntryNodeAST *params;
	unsigned int paramCnt;
	struct EntryAST *body;
} ProcedureAST;

typedef struct ProgramAST {
	struct Scope *scope;
} ProgramAST;

typedef struct ParamAST {
	struct Scope *scope;
    ParamType paramType;
    struct EntryAST *var;
	TypeClass type;
} ParamAST;

typedef struct TypeAST {
	TypeClass typeClass;
	LLVMTypeRef typeRef;
} TypeAST;

typedef struct EntryAST {
	EntryType entryType;
	char name[MAX_IDENT_LENGTH];
	LLVMValueRef value; // might not be here but in individual structs
	TypeAST *typeAST; // https://overiq.com/c-programming/101/union-basics-in-c/
	union {
		ProgramAST *progAST;
		FactorAST *factorAST;
		VariableAST *varAST;
		ProcedureAST *procAST;
		ParamAST *paramAST;
	}; // value
} EntryAST;

/* LinkedList of EntryAST to store variable number of statements and declarations in program and procedure. Also list of entries in a scope */
typedef struct EntryNodeAST {
	char *name;
	struct EntryAST *entryAST;
	struct EntryNodeAST *next;
	struct EntryNodeAST *prev;
} EntryNodeAST;

/* outerScope:  each scope is a list of entries a scope also keeps a pointer to
its parent scope so a variable can be searched upward
parent: the Entry that upon creating it we also need to create a new scope
(e.g. procedure and program)
*/
typedef struct Scope {
	char name[MAX_IDENT_LENGTH]; // for printing purposes
	EntryNodeAST *entryList;
	struct EntryAST *parent; // the block that started this scope
	struct Scope *outerScope;
} Scope;

typedef struct SymbolTable {
	struct Scope *currentScope;
	struct EntryAST *root; // root is program
	struct EntryNodeAST* globalEntryList;
} SymbolTable;

void declare_entry(EntryAST *entryAST, int isGlobal);
void add_entry(EntryNodeAST **list, EntryAST *entry);
EntryAST *lookup(char *name);
EntryAST *find_entry(EntryNodeAST *list, char *name);

void init_symbol_table();
void clear_symbol_table();
// Scope *create_scope(Scope *outerScope, Entry *parent);
Scope *create_scope(EntryAST *parent);
void enter_scope(Scope *scope);
void exit_scope();
void free_scope(Scope *scope);

void free_entry(EntryAST *entry);
void free_entry_list(EntryNodeAST *node);

void print_current_scope();
void print_entry_type(EntryAST *entry);
void print_entry_type_class(EntryAST *entry);
void print_type(TypeClass type);

TypeAST *create_type(TypeClass typeClass);
EntryAST *create_builtin_function(const char *name, TypeClass varType, ParamType paramType);
EntryAST *create_program(const char *name);
EntryAST *create_variable(const char *name);
EntryAST *create_param(const char *name, ParamType paramType);
EntryAST *create_procedure(const char *name);
#endif

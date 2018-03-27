// 
#include "token.h"

enum TypeClass {
	TC_INT,
	TC_FLOAT,
	TC_STRING,
	TC_BOOL,
	TC_CHAR,
	TC_ARRAY
};

enum EntryType {
	ET_VARIABLE,
	ET_TYPE_MARK,
	ET_PROCEDURE,
	ET_PARAMTER,
	ET_PROGRAM
};

typedef struct struct_Type {
	enum TypeClass typeClass;
	int arraySize;
	struct struct_Type *elementType;
} Type;

typedef struct struct_Entry {
	char name[MAX_STRING_LENGTH];
	enum EntryType entryType;
	union {
		VariableAttributes *varAttrs;
		TypeAttributes *typeAttrs;
		ProcedureAttributes *procAttrs;
		ProgramAttributes *progAttrs;
		ParameterAttributes *paramAttrs;
	};
} Entry;

typedef struct struct_EntryNode {
	Entry *entry;
	struct struct_EntryNode *next;
} EntryNode;

typedef struct struct_Scope {
	EntryNode* EntryNodeList;
	Entry* parent;
	struct struct_Scope *outerScope;
} Scope;

typedef struct struct_VariableAttributes {
	Type *type;
	struct Scope *scope;
} VariableAttributes;

typedef struct struct_TypeAttributes {
	Type *type;
} TypeAttributes;

typedef struct struct_ProcedureAttributes {
	EntryNode* paramList;
	Scope *scope;
} ProcedureAttributes;

typedef struct struct_ProgramAttributes {
	Scope *scope;
} ProgramAttributes;

typedef struct struct_ParameterAttributes {
	Type *type;
	Entry *procedure;
} ParameterAttributes;

// instead of one global symbol table and one scope symbol table
// we have one global scope and a list of scopes
// https://www.tutorialspoint.com/compiler_design/compiler_design_symbol_table.htm
typedef struct struct_SymbolTable {
	Scope *currentScope;
	Entry *program; // root is program
	EntryNode* globalScope;
} SymbolTable;

Scope *new_scope();

void enter_scope(Scope *scope);

void exit_scope();

Entry* lookup(EntryNodeList* list, char *name);

void dump(SymbolTable *symbolTable);
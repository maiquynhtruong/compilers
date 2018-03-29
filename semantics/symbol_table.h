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

struct Type;
struct Entry;
struct EntryNode;
struct Scope;

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

struct Type {
	enum TypeClass typeClass;
	int arraySize;
	struct Type *elementType;
} Type;

// an entry: <name, type, attribute>
struct Entry {
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

// to make life easier when defining a chain of nodes
struct EntryNode {
	Entry *entry;
	struct EntryNode *next;
} EntryNode;

// each scope is a list of entries
// a scope also keeps a pointer to its parent scope so
// a variable can be search upward
struct Scope {
	struct EntryNode* entryList;
	struct Scope *outerScope;
};

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

Type *makeIntType();
Type *makeCharType();
Type *makeFloatType();
Type *makeStringType();
Type *makeBoolType();
Type *makeArrayType();
int compareType(Type *type1, Type *type2);
void freeType(Type *type);

Entry *createProgramEntry(char *name);
Entry *createTypeEntry(char *name);
Entry *createVariableEntry(char *name);
Entry *createProcedureEntry(char *name);
Entry *createParameterEntry(char *name, Entry* procedure);
void freeEntry(Entry *entry);
void freeEntryList(EntryNode *node);
void addEntry(EntryNode **list, Entry *entry);

void init_symbol_table();
void clear_symbol_table();
Scope *new_scope(Scope *outerScope);
void enter_scope(Scope *scope);
void exit_scope();
Entry* lookup(char *name);
void dump();
void freeScope(Scope *scope);


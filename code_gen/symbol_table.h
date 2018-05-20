#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_STRING_LENGTH 50
#define MAX_IDENT_LENGTH 15

#include <stdbool.h>

/********************************* LLVM headers **********************************/
#include "llvm/ADT/APFloat.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"


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

typedef struct ProgramAttributes {
	struct Scope *scope;
} ProgramAttributes;

typedef struct TypeAttributes {
	struct Type *type;
} TypeAttributes;

typedef struct ConstantValueAttributes {
	struct ConstantValue *constantValue;
} ConstantValueAttributes;

typedef struct VariableAttributes {
	struct Type *type;
	struct Scope *scope;
} VariableAttributes;

typedef struct ProcedureAttributes {
	struct EntryNode* paramList;
	struct Scope *scope;
} ProcedureAttributes;

typedef struct ParameterAttributes {
	struct Type *type;
	struct Entry *procedure;
} ParameterAttributes;

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
		ParameterAttributes *paramAttrs;
		ProgramAttributes *progAttrs;
	};
};

// to make life easier when defining a chain of nodes
struct EntryNode {
	struct Entry *entry;
	struct EntryNode *next;
};

/* outerScope:  each scope is a list of entries a scope also keeps a pointer to
its parent scope so a variable can be searched upward
parent: the Entry that upon creating it we also need to create a new scope
(e.g. procedure)
*/

struct Scope {
	struct EntryNode* entryList;
	struct Entry *parent;
	struct Scope *outerScope;
};

// instead of one global symbol table and one scope symbol table
// we have one global scope and a list of scopes
// https://www.tutorialspoint.com/compiler_design/compiler_design_symbol_table.htm

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

ConstantValue *make_int_constant(int i);
ConstantValue *make_char_constant(char c);
ConstantValue *make_float_constant(float f);
ConstantValue *make_bool_constant(bool b);
ConstantValue *make_string_constant(char *str);

Entry *create_constant_entry(char *name);
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
Scope *new_scope(Scope *outerScope, Entry *parent);
void enter_scope(Scope *scope);
void exit_scope();
Entry* lookup(char *name);
void dump();
void free_scope(Scope *scope);
void add_entry(EntryNode **list, Entry *entry);
void declare_entry(Entry *entry);

#endif

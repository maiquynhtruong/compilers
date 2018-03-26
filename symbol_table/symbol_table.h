// symbol table is list of lists of scope. 
typedef struct struct_Entry {
	char *name;
	TokenType type;
	union {
		int intVal;
		double doubleVal;
		char *stringVal;
	} value;
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

typedef struct struct_SymbolTable {
	Scope *currentScope;
	Entry *program; // root is program
	EntryNode* topScope;
} SymbolTable;

Scope *new_scope();

void enter_scope(Scope *scope);

void exit_scope();

Entry* lookup(EntryNodeList* list, char *name);

void dump(SymbolTable *symbolTable);
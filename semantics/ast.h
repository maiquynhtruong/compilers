#ifndef AST_H
#define AST_H

typedef enum EntryType {
	ET_CONSTANT,
	ET_VARIABLE,
	ET_TYPE_MARK,
	ET_PROCEDURE,
	ET_PARAMTER,
	ET_PROGRAM,
	ET_BIN_OP,
	ET_CALL,
	ET_IF
} EntryType;

typedef enum TypeClass {
	TC_INT,
	TC_FLOAT,
	TC_STRING,
	TC_BOOL,
	TC_CHAR,
	TC_ARRAY
} TypeClass;

typedef enum BinaryOpType {
	BO_PLUS,
	BO_MINUS,
	BO_DIVIDE,
	BO_MULTIPLY,
	BO_LE,
	BO_LTEQ,
	BO_GE,
	BO_GTEQ,
	BO_EQ,
	BO_NEQ,
	BO_AND,
	BO_OR
} BinaryOpType;

struct EntryNodeAST {
	struct EntryAST *entryAST;
	struct EntryNodeAST *next;
};

struct Scope {
	struct EntryNodeAST* entryList;
	struct EntryAST *parent;
	struct Scope *outerScope;
};

struct Type {
	TypeClass typeClass;
	int arraySize;
	struct Type *elementType;
};

typedef struct ConstantAST {
	TypeClass typeClass;
	union {
		char stringVal[MAX_STRING_LENGTH+1];
        int intVal;
        float floatVal;
        bool boolVal;
        char charVal;
	};
} ConstantAST;

typedef struct ProgramAST {
	// struct Scope *scope;
} ProgramAST;

typedef struct TypeAttributes {
	struct Type *type;
} TypeAttributes;

typedef struct VariableAST {
	Type *varType;
    char *name;
	Scope *scope;
	ConstantAST *value;
} VariableAST;

typedef struct BinaryOpAST {
    BinaryOpType binOpType;
	struct EntryAST *rhs;
    struct EntryAST *lhs;
} BinaryOpAST;

typedef struct ProcedureCallAST {
    char *name;
    struct EntryAST **args;
    unsigned int argc;
} ProcedureCallAST;

typedef struct ParamAST {
    Type *type;
    ParamType paramType;
    char *name;
    struct EntryAST *procedure;
} ParamAST;

typedef struct PrototypeAST {
    char *name;
    struct ParamAST **args;
    unsigned int argc;
} PrototypeAST;

typedef struct ProcedureAST {
    Scope *scope;
    struct EntryAST *prototype;
} ProcedureAST;

// typedef struct ProcedureAttributes {
// 	struct EntryNode* paramList;
// 	struct Scope *scope;
// } ProcedureAttributes;

typedef struct IfStatementAST {
    struct EntryAST *condition;
    struct EntryAST *trueBlock;
    struct EntryAST *falseBlock;
} IfStatementAST;

typedef struct EntryAST {
	EntryType entryType; // type
	union {
		ConstantAST *constAST;
		TypeAST *typeAST;
		VariableAST *varAST;
        BinaryOpAST *binOpAST;
        ProcedureCallAST *procCallAST;
		PrototypeAST *protoAST;
		ProcedureAST *procAST;
		ParamAST *paramAST;
		IfStatementAST *ifAST;
	}; // value
} EntryAST;

Type *create_type();

EntryAST *create_constant(char *name, Type *type);
EntryAST *create_program(char *name, EntryType entryType);
EntryAST *create_variable(char *name, Type *type, ConstantAST *value);
EntryAST *create_binary_op(BinaryOpType type, EntryAST *lhs, EntryAST *rhs);
EntryAST *create_procedure_call(char *name, EntryAST **args, int argc);
EntryAST *create_param(char *name, EntryAST *procedure);
EntryAST *create_prototype(char *name, char **args, int argc);
EntryAST *create_procedure_declaration(EntryAST *prototype, EntryAST *body);
EntryAST *create_if_statement(EntryAST *condition, EntryAST *trueBlock, EntryAST *falseBlock);
void free_entry_AST(EntryAST *entry);

EntryAST *create_type_entry(char *name);
EntryAST *create_variable_entry(char *name, int global);
EntryAST *create_procedure_entry(char *name, int global);
EntryAST *create_parameter_entry(char *name);
EntryAST *find_entry(EntryNode *list, char *name);
void free_entry(Entry *entry);
void free_entry_list(EntryNode *node);
void add_entry(EntryNode **list, Entry *entry);

#endif

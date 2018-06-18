#ifndef AST_H
#define AST_H

/*
There are two main types of entry: EntryAST, EntryNodeAST
*/

typedef enum StatementType {
	ST_ASSIGNMENT,
	ST_IF,
	ST_LOOP,
	ST_RETURN,
	ST_CALL
} StatementType;

typedef enum ParamType {
	PT_IN,
	PT_OUT,
	PT_INOUT
} ParamType;

typedef enum EntryType {
	ET_CONSTANT,
	ET_VARIABLE,
	ET_TYPE_MARK,
	ET_PROCEDURE,
	ET_PARAMTER,
	ET_PROGRAM,
	ET_BIN_OP,
	ET_STATEMENT
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

typedef enum UnaryOpAST {
	UN_PLUS,
	UN_MINUS
} UnaryOpAST;

/* LinkedList of EntryAST to store variable number of statements and declarations in program and procedure. Also list of entries in a scope */
typedef struct EntryNodeAST {
	struct EntryAST *entryAST;
	struct EntryNodeAST *next;
} EntryNodeAST;

/* outerScope:  each scope is a list of entries a scope also keeps a pointer to
its parent scope so a variable can be searched upward
parent: the Entry that upon creating it we also need to create a new scope
(e.g. procedure and program)
*/
typedef struct Scope {
	EntryNodeAST* entryList;
	// struct EntryAST *parent;
	struct Scope *outerScope;
} Scope;

struct TypeAST {
	TypeClass typeClass;
	int arraySize;
	struct EntryAST *elementType;
};

typedef struct UnaryOpAST {
	UnaryOpAST unaOp;
	struct EntryAST *factor;
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

typedef struct BodyAST {
	struct EntryNodeAST *declarationList;
	struct EntryNodeAST *statementList;
} BodyAST;

typedef struct ProgramAST {
	// struct Scope *scope;
	char *name;
	struct EntryAST *body;
} ProgramAST;

typedef struct VariableAST {
	struct EntryAST *varType;
    char *name;
	Scope *scope;
	struct EntryAST *value;
} VariableAST;

typedef struct BinaryOpAST {
    BinaryOpType binOpType;
	struct EntryAST *rhs;
    struct EntryAST *lhs;
} BinaryOpAST;

typedef struct ParamAST {
    // TypeAST *type;
    ParamType paramType;
    struct EntryAST *var;
    // struct ProcedureAST *procedure;
} ParamAST;

typedef struct ProcedureAST {
    Scope *scope;
	char *name;
	struct EntryNodeAST *params;
	unsigned int argc;
	struct EntryAST *body;
} ProcedureAST;

// typedef struct ProcedureAttributes {
// 	struct EntryNode* paramList;
// 	struct Scope *scope;
// } ProcedureAttributes;

typedef struct IfAST {
    struct EntryAST *condition;
    struct EntryAST *trueBlock;
    struct EntryAST *falseBlock;
} IfStatementAST;

typedef struct AssignmentAST {
	struct EntryAST *destAST;
	struct EntryAST *expAST;
} AssignmentAST;

typedef struct LoopAST {

} LoopAST;

typedef struct ReturnAST {} ReturnAST;

typedef struct ProcedureCallAST {
    char *name;
    struct EntryNodeAST *args;
    unsigned int argc;
} ProcedureCallAST;

typedef struct StatementAST {
	StatementType statementType;
	union {
		AssignmentAST *assignment;
		IfAST *ifAST;
		LoopAST *loop;
		ReturnAST *returnAST;
		ProcedureCallAST *procCall;
	}
} StatementAST;

typedef struct EntryAST {
	EntryType entryType; // type
	union {
		UnaryOpAST *unaOpAST;
		ConstantAST *constAST;
		TypeAST *typeAST;
		VariableAST *varAST;
        BinaryOpAST *binOpAST;
		PrototypeAST *protoAST;
		ProcedureAST *procAST;
		ParamAST *paramAST;
		StatementAST *statementAST;
	}; // value
} EntryAST;

TypeAST *create_type();
TypeAST *make_int_type();
TypeAST *make_char_type();
TypeAST *make_float_type();
TypeAST *make_string_type();
TypeAST *make_bool_type();
TypeAST *make_array_type(int size, TypeAST *type);
int compare_type(TypeAST *type1, TypeAST *type2);
void free_type(TypeAST *type);

EntryNodeAST *create_entryAST_node(EntryAST *entryAST, EntryNodeAST *next);

EntryAST *create_unary_op(UnaryOpAST unaOp, EntryAST *factor);
EntryAST *create_constant(char *name, TypeAST *type);
EntryAST *create_body_block(EntryAST *decls, EntryAST *statements);
EntryAST *create_program(char *name, EntryAST *body);
EntryAST *create_variable(char *name, TypeAST *type);
EntryAST *create_binary_op(BinaryOpType type, EntryAST *lhs, EntryAST *rhs);
EntryAST *create_procedure_call(char *name, EntryAST **args, int argc);
EntryAST *create_param(char *name, TypeAST *type,);
EntryAST *create_procedure(char *name, EntryAST *body);
EntryAST *create_if_statement(EntryAST *condition, EntryAST *trueBlock, EntryAST *falseBlock);

#endif

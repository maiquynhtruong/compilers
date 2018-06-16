#ifndef AST_H
#define AST_H

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

typedef enum UnaryOpAST {
	UN_PLUS,
	UN_MINUS
} UnaryOpAST;

struct EntryNodeAST {
	struct EntryAST *entryAST;
	struct EntryNodeAST *next;
};

/* outerScope:  each scope is a list of entries a scope also keeps a pointer to
its parent scope so a variable can be searched upward
parent: the Entry that upon creating it we also need to create a new scope
(e.g. procedure and program)
*/
struct Scope {
	struct EntryNodeAST* entryList;
	struct EntryAST *parent;
	struct Scope *outerScope;
};

struct TypeAST {
	TypeClass typeClass;
	int arraySize;
	struct TypeAST *elementType;
};

typedef struct UnaryOpAST {
	UnaryOpAST unaOp;
	EntryAST *factor;
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
	EntryNodeAST *declarationList;
	EntryNodeAST *statementList;
} BodyAST;

typedef struct ProgramAST {
	// struct Scope *scope;
	char *name;
	struct BodyAST *body;
} ProgramAST;

typedef struct VariableAST {
	TypeAST *varType;
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
    TypeAST *type;
    ParamType paramType;
    VariableAST *var;
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
	struct ParamAST *params;
	struct BodyAST *body;
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
		UnaryOpAST *unaOpAST;
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

TypeAST *create_type();
TypeAST *make_int_type();
TypeAST *make_char_type();
TypeAST *make_float_type();
TypeAST *make_string_type();
TypeAST *make_bool_type();
TypeAST *make_array_type(int size, TypeAST *type);
int compare_type(TypeAST *type1, TypeAST *type2);
void free_type(TypeAST *type);

EntryAST *create_unary_op(UnaryOpAST unaOp, EntryAST *factor);
EntryAST *create_constant(char *name, TypeAST *type);
EntryAST *create_body_block(EntryAST *decls, EntryAST *statements);
EntryAST *create_program(char *name, EntryAST *body);
EntryAST *create_variable(char *name, TypeAST *type);
EntryAST *create_binary_op(BinaryOpType type, EntryAST *lhs, EntryAST *rhs);
EntryAST *create_procedure_call(char *name, EntryAST **args, int argc);
EntryAST *create_param(char *name, EntryAST *procedure);
EntryAST *create_prototype(char *name, char **args, int argc);
EntryAST *create_procedure_declaration(EntryAST *prototype, EntryAST *body);
EntryAST *create_if_statement(EntryAST *condition, EntryAST *trueBlock, EntryAST *falseBlock);

#endif

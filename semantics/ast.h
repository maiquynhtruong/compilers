#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include "token.h"

/*
There are two main types of entry:
	EntryAST, EntryNodeAST
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
	BO_LT,
	BO_LTEQ,
	BO_GT,
	BO_GTEQ,
	BO_EQ,
	BO_NEQ,
	BO_AND,
	BO_OR
} BinaryOpType;

typedef enum UnaryOpType {
	UN_PLUS,
	UN_MINUS
} UnaryOpType;

/* LinkedList of EntryAST to store variable number of statements and declarations in program and procedure. Also list of entries in a scope */
typedef struct EntryNodeAST {
	struct EntryAST *entryAST;
	struct EntryNodeAST *next;
} EntryNodeAST;

typedef struct TypeAST {
	TypeClass typeClass;
	int arraySize;
	struct EntryAST *elementType;
} TypeAST;

typedef struct UnaryOpAST {
	UnaryOpType unaOp;
	struct EntryAST *factor;
} UnaryOpAST;

// Might be Replaced by a struct Token from token.h
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

typedef struct BodyAST {
	struct EntryNodeAST *declList;
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
	struct EntryAST *value; // should it be replaced by a token?
	// Token *value; // for value
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
    // Scope *scope;
	char *name;
	struct EntryNodeAST *params;
	unsigned int argc;
	struct EntryAST *body;
} ProcedureAST;

typedef struct IfAST {
    struct EntryAST *condition;
    struct EntryNodeAST *trueBlock;
    struct EntryNodeAST *falseBlock;
} IfAST;

typedef struct AssignmentAST {
	struct EntryAST *destAST;
	struct EntryAST *expAST;
} AssignmentAST;

typedef struct LoopAST {
	AssignmentAST *assignment;
	struct EntryAST *expr;
	EntryNodeAST *statements;
} LoopAST;

typedef struct ReturnAST {} ReturnAST; // TODO: what to fill in?

typedef struct ProcCallAST {
    char *callee;
    struct EntryNodeAST *args;
    unsigned int argc;
} ProcCallAST;

typedef struct StatementAST {
	StatementType statementType;
	union {
		AssignmentAST *asgmtAST;
		IfAST *ifAST;
		LoopAST *loopAST;
		ReturnAST *returnAST;
		ProcCallAST *procCallAST;
	};
} StatementAST;

typedef struct EntryAST {
	EntryType entryType; // type
	union {
		UnaryOpAST *unaOpAST; //TODO: this should belong to FactorAST
		BodyAST *bodyAST;
		ProgramAST *progAST;
		TypeAST *typeAST;
		FactorAST *factorAST;
		VariableAST *varAST;
        BinaryOpAST *binOpAST;
		ProcedureAST *procAST;
		ParamAST *paramAST;
		StatementAST *stmtAST;
	}; // value
} EntryAST;

TypeAST *make_int_type();
TypeAST *make_char_type();
TypeAST *make_float_type();
TypeAST *make_string_type();
TypeAST *make_bool_type();
TypeAST *make_array_type(int size, EntryAST *type);
int compare_type(TypeAST *type1, TypeAST *type2);
void free_type(TypeAST *type);

EntryNodeAST *create_entry_node(EntryAST *entryAST, EntryNodeAST *next);

EntryAST *create_builtin_function(char *name, TypeClass varType, ParamType paramType);
EntryAST *create_type(TypeClass typeClass);
EntryAST *create_body_block(EntryNodeAST *decls, EntryNodeAST *statements);
EntryAST *create_program(char *name, EntryAST *body);
EntryAST *create_factor(TypeClass typeClass, Token *value);
EntryAST *create_variable(char *name, EntryAST *type, EntryAST *value);
EntryAST *create_binary_op(BinaryOpType binOp, EntryAST *lhs, EntryAST *rhs);
EntryAST *create_unary_op(UnaryOpType unaOp, EntryAST *factor);
EntryAST *create_procedure_call(char *callee, EntryNodeAST *args, int argc);
EntryAST *create_param(ParamType paramType, EntryAST *var);
EntryAST *create_procedure(char *name, EntryNodeAST *params, EntryAST *body);
EntryAST *create_if(EntryAST *condition, EntryNodeAST *trueBlock, EntryNodeAST *falseBlock);
EntryAST *create_loop(EntryAST *assignment, EntryAST *expr, EntryNodeAST *statements);
EntryAST *create_return();

/*********** Debug functions ****************/
void print_type(TypeAST *type);
void print_variable(VariableAST *varAST);
void print_program(ProgramAST *progAST);
void print_procedure(ProcedureAST *procAST);
void print_bin_op(BinaryOpAST *binOpAST);
void print_statement(StatementAST *statAST);
void print_param(ParamAST *paramAST);
void print_entry(EntryAST *entry);
void print_entry_list(EntryNodeAST *entryList);

#endif

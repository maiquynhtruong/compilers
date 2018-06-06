#ifndef AST_H
#define AST_H

#include "symbol_table.h"

struct EntryAST;

typedef struct ConstantAST {
	struct ConstantValue *constVal;
} ConstantAST;

typedef struct VariableAST {
	Type *type;
    char *name;
	Scope *scope;
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
    struct EntryAST *body;
} ProcedureAST;

typedef struct IfStatementAST {
    struct EntryAST *condition;
    struct EntryAST *trueBlock;
    struct EntryAST *falseBlock;
} IfStatementAST;

// an entry: <name, type, attribute>
typedef struct EntryAST {
	// char name[MAX_IDENT_LENGTH];
	EntryType entryType;
	int global;
	union {
		ConstantAST *constAST;
		VariableAST *varAST;
        BinaryOpAST *binOpAST;
        ProcedureCallAST *procCallAST;
		PrototypeAST *protoAST;
		ProcedureAST *procAST;
		ParamAST *paramAST;
		IfStatementAST *ifAST;
	};
} EntryAST;

EntryAST *create_constant(ConstantValue *value);
EntryAST *create_variable(char *name);
EntryAST *create_binary_op(BinaryOpType type, EntryAST *lhs, EntryAST *rhs);
EntryAST *create_procedure_call(char *name, EntryAST **args, int argc);
EntryAST *create_param(char *name, EntryAST *procedure);
EntryAST *create_prototype(char *name, char **args, int argc);
EntryAST *create_procedure_declaration(EntryAST *prototype, EntryAST *body);
EntryAST *create_if_statement(EntryAST *condition, EntryAST *trueBlock, EntryAST *falseBlock);
void free_entry_AST(EntryAST *entry);

#endif

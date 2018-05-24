#ifndef AST_H
#define AST_H

#include "symbol_table.h"

typedef struct ConstantAST {
	struct ConstantValue *constVal;
} ConstantAST;

typedef struct VariableAST {
	struct Type *type;
	struct *scope;
} VariableAST;

// Binary Operators
typedef struct BinaryOpAST {
	char operation;
	// Entry *rhs, *lhs;
} BinaryOpAST;

#endif

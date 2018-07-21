#include <stdlib.h>
#include <string.h>

#include "semantics.h"
#include "error.h"
#include "symbol_table.h"

// from symbol_table.c
extern SymbolTable *symbolTable;
extern Token *current_token;
extern LLVMValueRef llvm_printf;
extern LLVMBuilderRef builder;

LLVMValueRef check_builtin_proc(char *name) {
	if (strcmp(name, "putbool") == 0 || strcmp(name, "putinteger") == 0 || strcmp(name, "putfloat") == 0 ||
		strcmp(name, "putstring") == 0 || strcmp(name, "putchar") == 0) {
			return llvm_printf;
		}
	return NULL;
}

// Check if an entry has been inserted in table before throws an error if that happens
void check_new_identifier(char *name) {
	// EntryAST *entry = find_entry(symbolTable->currentScope->entryList, name);
	assert_semantics("Verify a new ident for "); assert_semantics(name); assert_semantics("\n");

	EntryAST *entry = lookup(name);
	if (entry == NULL) {
		assert_semantics(name); assert_semantics(" is a new ident \n");
	} else {
		throw_error(E_DUPLICATE_IDENT, current_token->lineNo, current_token->columnNo);
	}
}

EntryAST *check_declared_identifier(char *name) {
	assert_semantics("Verify declared ident for "); assert_semantics(name); assert_semantics("\n");
	EntryAST *entry = lookup(name);

	if (entry == NULL) {
		throw_error(E_UNDECLARED_IDENT, current_token->lineNo, current_token->columnNo);
	} else {
		assert_semantics(name); assert_semantics(" is "); print_entry_type_class(entry); assert_semantics("\n");
	}
	return entry;
}

EntryAST *check_declared_variable(char *name) {
	assert_semantics("Verify declared variable for "); assert_semantics(name); assert_semantics("\n");
	EntryAST *entry = lookup(name);

	if (entry == NULL) throw_error(E_UNDECLARED_VARIABLE, current_token->lineNo, current_token->columnNo);
	if (entry->entryType != ET_VARIABLE) {
		throw_error(E_INVALID_VARIABLE, current_token->lineNo, current_token->columnNo);
	} else {
		assert_semantics(name); assert_semantics(" is declared. It has type ");
		print_entry_type(entry); assert_semantics("\n");
	}

	return entry;
}

EntryAST *check_declared_procedure(char *name) {
	assert_semantics("Verify declared procedure for "); assert_semantics(name); assert_semantics("\n");
	EntryAST *entry = lookup(name);

	if (entry == NULL) throw_error(E_UNDECLARED_PROCEDURE, current_token->lineNo, current_token->columnNo);
	if (entry->entryType != ET_PROCEDURE) {
		throw_error(E_INVALID_PROCEDURE, current_token->lineNo, current_token->columnNo);
	} else {
		assert_semantics(name); assert_semantics(" is declared"); assert_semantics("\n");
	}

	return entry;
}

EntryAST *check_declared_destination(char *name) {
	assert_semantics("Verify declared destination for "); assert_semantics(name); assert_semantics("\n");
	EntryAST *entry = lookup(name);
	Scope *scope;

	if (entry == NULL) throw_error(E_UNDECLARED_IDENT, current_token->lineNo, current_token->columnNo);

	switch (entry->entryType) {
		case ET_PARAMTER:
		case ET_VARIABLE:
			break;
		case ET_PROCEDURE:
			scope = symbolTable->currentScope;
			while ((scope != NULL) && (scope != entry->procAST->scope)) // looking upwards for the procedure
				scope = scope->outerScope;

			if (scope == NULL) //TODO: should I check in global scope too?
				throw_error(E_UNDECLARED_IDENT, current_token->lineNo, current_token->columnNo);
			break;
		default: throw_error(E_UNDECLARED_IDENT, current_token->lineNo, current_token->columnNo);
	};

	assert_semantics(name); assert_semantics(" is declared. It has type ");
	print_entry_type(entry); assert_semantics("\n");
	return entry;
}

void check_int_float_type(TypeClass type) {
	if (type == TC_INT || type == TC_FLOAT) return;
	else throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_basic_type(TypeClass type) {
	if (type == TC_INT || type == TC_FLOAT || type == TC_BOOL || type == TC_CHAR) return;
	else throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_builtin_type(TypeClass type) {
	if (type == TC_INT || type == TC_FLOAT || type == TC_BOOL || type == TC_BOOL || type == TC_CHAR || type == TC_STRING) return;
	else throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_int_type(TypeClass type) {
	if (type == TC_INT) return; else throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void check_type_equality(TypeClass type1, TypeClass type2) {
	assert_semantics("Checking type equality: Type 1 is "); print_type(type1); assert_semantics(", type 2 is "); print_type(type2);
	assert_semantics("\n");
	if (type1 != type2)
		throw_error(E_INCOSISTENT_TYPE, current_token->lineNo, current_token->columnNo);
}

void convert_to_int(TypeAST **type) {
	(*type)->typeClass = TC_INT;
}

void convert_to_bool(TypeAST **type) {
	// if ((*type)->typeClass == TC_INT) {
		(*type)->valueRef = LLVMBuildICmp(builder, LLVMIntNE, (*type)->valueRef, LLVMConstInt(LLVMInt32Type(), 0, 0), "num != 0?");
		// printf("%s\n", LLVMPrintValueToString((*type)->valueRef));
		(*type)->typeClass = TC_BOOL;
	// }
}

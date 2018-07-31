#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "error.h"

#define MAX_STRING_LENGTH 50

SymbolTable *symbolTable;

EntryAST *getBool;
EntryAST *getInteger;
EntryAST *getFloat;
EntryAST *getString;
EntryAST *getChar;
EntryAST *putBool;
EntryAST *putInteger;
EntryAST *putFloat;
EntryAST *putString;
EntryAST *putChar;

extern LLVMValueRef llvm_printf;
extern LLVMValueRef llvm_scanf;
extern LLVMBuilderRef builder;
extern LLVMModuleRef module;
extern LLVMValueRef mainFunc;

void declare_entry(EntryAST *entry, int isGlobal) {
	assert_symbol_table("Declaring an entry: ");
	assert_symbol_table(entry->name);
	EntryNodeAST **list = NULL;

	switch (entry->entryType) {
		case ET_VARIABLE:
			if (entry->typeAST->typeClass == TC_STRING) {
				LLVMValueRef stringArray = LLVMBuildArrayAlloca(builder, LLVMArrayType(LLVMInt8Type(), MAX_STRING_LENGTH), NULL, entry->name);
				entry->typeAST->address = stringArray;
			} else {
				entry->typeAST->address = LLVMBuildAlloca(builder, entry->typeAST->typeRef, entry->name);
			}
			entry->typeAST->valueRef = entry->typeAST->address;
			entry->varAST->scope = symbolTable->currentScope;
			list = &(symbolTable->currentScope->entryList);
			break;
		case ET_PARAMTER:
			entry->varAST->scope = symbolTable->currentScope;
			EntryAST *parent = symbolTable->currentScope->parentEntry;
			add_entry(&(parent->procAST->params), entry);
			list = &(symbolTable->currentScope->entryList);
			break;
		case ET_PROCEDURE:
			entry->procAST->scope->outerScope = symbolTable->currentScope;
			list = &(symbolTable->globalEntryList);
			break;
		default: break;
	}

	if (symbolTable->currentScope == NULL || isGlobal) {
		list = &(symbolTable->globalEntryList);
	}

	add_entry(list, entry);
}

void add_entry(EntryNodeAST **list, EntryAST *entry) {
	EntryNodeAST *entryNode = (EntryNodeAST *) malloc(sizeof(EntryNodeAST));
	entryNode->entryAST = entry;
	entryNode->next = NULL;

	if (*list == NULL) {
		*list = entryNode;
	} else {
		EntryNodeAST *cur = *list;
		while (cur->next != NULL) cur = cur->next;
		cur->next = entryNode;
	}
}

EntryAST *lookup(char *name) {
	EntryAST *entry = NULL;
	Scope *curScope = symbolTable->currentScope;

	while (curScope != NULL) {
		entry = find_entry(curScope->entryList, name);
		if (entry != NULL) break;
		curScope = curScope->outerScope;
	}

	// search in global scope as the last resort
	if (entry == NULL) {
		entry = find_entry(symbolTable->globalEntryList, name);
	}

	return entry;
}

// find entry by name in a EntryNodeAST list
EntryAST *find_entry(EntryNodeAST *list, char *name) {
	EntryNodeAST *curNode = list;

	while (curNode != NULL) {
		if (strcmp(curNode->entryAST->name, name) == 0) {
			return curNode->entryAST;
		} else curNode = curNode->next;
	}
	return NULL;
}

void init_symbol_table() {
	assert_symbol_table("Initialize a symbol table"); assert_symbol_table("\n");

	symbolTable = (SymbolTable *) malloc(sizeof(SymbolTable));
	symbolTable->currentScope = NULL;
	symbolTable->root = NULL;
	symbolTable->globalEntryList = NULL;

	// built-in functions e.g. getInteger(integer val out)
	// getBool = create_builtin_function("getbool", TC_BOOL, PT_OUT); // getBool(bool val out)
	getString = create_builtin_function("getstring", TC_STRING, PT_OUT); // getString(string val out)
	getInteger = create_builtin_function("getinteger", TC_INT, PT_OUT); // getInteger(integer val out)
	// getFloat = create_builtin_function("getfloat", TC_FLOAT, PT_OUT); // getFloat(float val out)
	// getChar = create_builtin_function("getchar", TC_CHAR, PT_OUT); // getChar(char val out)

	putInteger = create_builtin_function("putinteger", TC_INT, PT_IN); // putInteger(integer val in)
	// putBool = create_builtin_function("putbool", TC_BOOL, PT_IN); // putBool(bool val in)
	// putFloat = create_builtin_function("putfloat", TC_FLOAT, PT_IN); // putFloat(float val in)
	putString = create_builtin_function("putstring", TC_STRING, PT_IN); // putString(string val in)
	// putChar = create_builtin_function("putchar", TC_CHAR, PT_IN); // putChar(char val in)

	assert_symbol_table("Finish initializing a symbol table"); assert_symbol_table("\n");
}

void clear_symbol_table() {
	free_entry(symbolTable->root);
	free_entry_list(symbolTable->globalEntryList);
	// free_scope(symbolTable->currentScope);
	free(symbolTable);
	//TODO: Are these needed?
	// free_type(intType);
	// free_type(charType);
	// free_type(floatType);
	// free_type(stringType);
	// free_type(boolType);
}

Scope *create_scope(EntryAST *parentEntry) {
	assert_symbol_table("New scope: ");
	assert_symbol_table(parentEntry->name);
	assert_symbol_table("\n");

	Scope *scope = (Scope *) malloc(sizeof(Scope));
	scope->entryList = NULL;
	scope->parentEntry = parentEntry;
	scope->outerScope = symbolTable->currentScope;
	strcpy(scope->name, parentEntry->name);
	return scope;
}

void enter_scope(Scope *scope) {
	assert_symbol_table("Enter a scope: ");
	assert_symbol_table(scope->name);
	assert_symbol_table("\n");

	symbolTable->currentScope = scope;

	print_current_scope();
}

void exit_scope() {
	assert_symbol_table("Exit a scope\n");

	symbolTable->currentScope = symbolTable->currentScope->outerScope;
}

void free_scope(Scope *scope) {
	if (scope != NULL) {
		free_entry_list(scope->entryList);
		free(scope);
		scope = NULL;
	}
}

void free_entry(EntryAST *entry) {
	if (entry != NULL) {
		switch(entry->entryType) {
			case ET_VARIABLE: case ET_PARAMTER:
				if (entry->varAST != NULL) free(entry->varAST);
				break;
			case ET_PROCEDURE:
				if (entry->procAST != NULL) free(entry->procAST);
				break;
			case ET_PROGRAM:
				if (entry->progAST != NULL) free(entry->progAST);
				break;
			default: break;
		}
		free(entry);
		entry = NULL;
	}
}

void free_entry_list(EntryNodeAST *node) {
	while (node != NULL) {
		free_entry(node->entryAST);
		node = node->next;
	}
}

void print_current_scope() {
	assert_symbol_table("Current scope is ");
	if (symbolTable->currentScope != NULL) assert_symbol_table(symbolTable->currentScope->name);
	else assert_symbol_table("main");
	assert_symbol_table("\n");
}

void print_entry_type_class(EntryAST *entry) {
	print_type(entry->typeAST->typeClass);
}

void print_entry_type(EntryAST *entry) {
	switch (entry->entryType) {
		case ET_VARIABLE:
			printf("A variable"); break;
		case ET_FACTOR:
			printf("A factor"); break;
		case ET_PARAMTER:
			printf("A parameter"); break;
		case ET_PROCEDURE:
			printf("A procedure"); break;
		default:
			printf("Unknown entry type"); break;
	}
}

const char *print_type(TypeClass type) {
	const char *s;
    switch (type) {
        case TC_INT:
            s = "Integer"; break;
    	case TC_FLOAT:
            s = "Float"; break;
    	case TC_STRING:
            s = "String"; break;
    	case TC_BOOL:
            s = "Bool"; break;
    	case TC_CHAR:
            s = "Char"; break;
		case TC_VOID:
			s = "Void"; break;
		case TC_INVALID:
			s = "Invalid"; break;
        default:
            s = "Unknown type class"; break;
    }
	return s;
}

TypeAST *create_type(TypeClass typeClass) {
	assert_symbol_table("Create type "); print_type(typeClass); assert_symbol_table("\n");

	TypeAST *type = (TypeAST *) malloc(sizeof(TypeAST));
	type->typeClass = typeClass;
	switch (typeClass) {
		case TC_INT:
			type->typeRef = LLVMInt32Type(); break;
		case TC_FLOAT:
			type->typeRef = LLVMFloatType(); break;
		case TC_STRING:
			type->typeRef = LLVMPointerType(LLVMInt8Type(), 0); break;
		case TC_BOOL:
			type->typeRef = LLVMInt32Type(); break;
		case TC_CHAR:
			type->typeRef = LLVMInt8Type(); break;
		case TC_VOID:
			type->typeRef = LLVMVoidType(); break;
		default:
			type->typeRef = LLVMVoidType(); break;
	}
	return type;
}

EntryAST *create_builtin_function(const char *name, TypeClass varType, ParamType paramAttr) {
	assert_symbol_table("Create builtin function: ");
	assert_symbol_table(name); assert_symbol_table("\n");

	EntryAST *proc = create_procedure(name);
	declare_entry(proc, 1);

	enter_scope(proc->procAST->scope);
		EntryAST *param = create_param("val");
		param->typeAST = create_type(varType);
		param->typeAST->paramType = paramAttr;
		declare_entry(param, 0);
		proc->procAST->paramc = 1;

		LLVMTypeRef params[] = { param->typeAST->typeRef };
		if (name[0] == 'g') {
			if (varType != TC_STRING) { // string is such a special type. Exception everytime
				params[0] = LLVMPointerType(param->typeAST->typeRef, 0);
			}
		}

		LLVMTypeRef procType = LLVMFunctionType(LLVMVoidType(), params, 1, false);
		LLVMValueRef procValue = LLVMAddFunction(module, proc->name, procType);
		LLVMBasicBlockRef procEntry = LLVMAppendBasicBlock(procValue, proc->name);
		LLVMPositionBuilderAtEnd(builder, procEntry);

		LLVMValueRef value = LLVMGetParam(procValue, 0);
		LLVMSetValueName(value, "val");
		param->typeAST->valueRef = value;

		const char *format_str = "";
	    if (strcmp(name, "putbool") == 0 || strcmp(name, "putinteger") == 0 ||
			strcmp(name, "getbool") == 0 || strcmp(name, "getinteger") == 0) {
	        format_str = "%d";
	    } else if (strcmp(name, "putfloat") == 0 || strcmp(name, "getfloat") == 0) {
	        format_str = "%f";
	    } else if (strcmp(name, "putstring") == 0 || strcmp(name, "getstring") == 0) {
	        format_str = "%s";
	    } else if (strcmp(name, "putchar") == 0 || strcmp(name, "getchar") == 0) {
	        format_str = "%c";
	    }

	    LLVMValueRef format = LLVMBuildGlobalStringPtr(builder, format_str, "format_str");

		if (name[0] == 'p') { // put* functions
			LLVMValueRef args[] = { format, param->typeAST->valueRef };
			LLVMBuildCall(builder, llvm_printf, args, 2, name);
		} else { // get* functions
			LLVMValueRef args[] = { format, param->typeAST->valueRef };
			LLVMBuildCall(builder, llvm_scanf, args, 2, name);
		}

		LLVMBuildRetVoid(builder);

		LLVMPositionBuilderAtEnd(builder, LLVMGetLastBasicBlock(mainFunc));
	exit_scope();

	assert_symbol_table("Builtin proc "); assert_symbol_table(name);
	assert_symbol_table(" has value: "); assert_symbol_table(LLVMPrintValueToString(procValue));
	assert_symbol_table("\n");
	return proc;
}

EntryAST *create_program(const char *name) {
	assert_symbol_table("Creating a program entry named "); assert_symbol_table(name); assert_symbol_table("\n");

	EntryAST *progEntry = (EntryAST *) malloc(sizeof(EntryAST));
	strcpy(progEntry->name, name);
	progEntry->entryType = ET_PROGRAM;
	ProgramAST *program = (ProgramAST *) malloc(sizeof(ProgramAST));
	program->scope = create_scope(progEntry);
	progEntry->progAST = program;
	symbolTable->root = progEntry;
	return progEntry;
}

EntryAST *create_variable(const char *name) {
	assert_symbol_table("Creating a variable entry named "); assert_symbol_table(name); assert_symbol_table("\n");

	EntryAST *varEntry = (EntryAST *) malloc(sizeof(EntryAST));
	strcpy(varEntry->name, name);
	varEntry->entryType = ET_VARIABLE;
	VariableAST *var = (VariableAST *) malloc(sizeof(VariableAST));
	strcpy(varEntry->name, name);
	var->scope = NULL;
	var->size = 0;
	varEntry->varAST = var;
	return varEntry;
}

EntryAST *create_param(const char *name) {
	assert_symbol_table("Creating a param entry named "); assert_symbol_table(name); assert_symbol_table("\n");

	EntryAST *paramEntry = create_variable(name);
	paramEntry->entryType = ET_PARAMTER;
	return paramEntry;
}

EntryAST *create_procedure(const char *name) {
	assert_symbol_table("Creating a procedure entry named "); assert_symbol_table(name); assert_symbol_table("\n");

	EntryAST *procEntry = (EntryAST *) malloc(sizeof(EntryAST));
	strcpy(procEntry->name, name);
	procEntry->entryType = ET_PROCEDURE;
	ProcedureAST *proc = (ProcedureAST *) malloc(sizeof(ProcedureAST));
	proc->params = NULL;
	proc->paramc = 0;
	proc->scope = create_scope(procEntry);
	procEntry->procAST = proc;
	procEntry->typeAST = create_type(TC_VOID);
	return procEntry;
}

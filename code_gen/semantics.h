#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <llvm-c/Core.h>
#include "symbol_table.h"

LLVMValueRef check_builtin_proc(char *name);
void check_new_identifier(char *name);
EntryAST *check_declared_identifier(char *name);
EntryAST *check_declared_variable(char *name);
EntryAST *check_declared_procedure(char *name);
EntryAST *check_declared_destination(char *name);

void check_int_type(TypeClass type);
void check_int_float_type(TypeClass type);
void check_basic_type(TypeClass type);
void check_builtin_type(TypeClass type);
void check_type_equality(TypeClass type1, TypeClass type2);

void convert_to_int(TypeAST **type);
void convert_to_bool(TypeAST **type);

#endif

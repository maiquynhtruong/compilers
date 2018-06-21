#ifndef SEMANTICS_H
#define SEMANTICS_H

#include "symbol_table.h"

void check_new_identifier(char *name);
EntryAST *check_declared_identifier(char *name);
EntryAST *check_declared_variable(char *name);
EntryAST *check_declared_procedure(char *name);
EntryAST *check_declared_destination(char *name);

void check_int_type(TypeAST *type);
void check_char_type(TypeAST *type);
void check_string_type(TypeAST *type);
void check_float_type(TypeAST *type);
void check_int_float_type(TypeAST *type);
void check_basic_type(TypeAST *type);
void check_builtin_type(TypeAST *type);
void check_bool_type(TypeAST *type);
void check_type_equality(TypeAST *type1, TypeAST *type2);

#endif
